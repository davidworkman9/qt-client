/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "avalaraIntegration.h"

#include <QApplication>
#include <QCursor>
#include <QMessageBox>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QHostInfo>
#include <QJsonDocument>
#include <QJsonObject>

#include "errorReporter.h"

AvalaraIntegration::AvalaraIntegration(bool listen) : TaxIntegration(listen)
{
  restclient = new QNetworkAccessManager;
  connect(restclient, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleResponse(QNetworkReply*)));

  // Can't access metrics from here, but only queries once at startup and in setup
  XSqlQuery service("SELECT fetchMetricText('ServerVersion') AS ServerVersion, "
                    "       fetchMetricBool('NoAvaTaxCommit') AS NoAvaTaxCommit, "
                    "       fetchMetricBool('LogTaxService') AS LogTaxService, "
                    "       fetchMetricText('TaxServiceLogFile') AS TaxServiceLogFile;");
  if (service.first())
  {
    _ServerVersion = service.value("ServerVersion").toString();
    _NoAvaTaxCommit = service.value("NoAvaTaxCommit").toBool();
    _LogTaxService = service.value("LogTaxService").toBool();
    _TaxServiceLogFile = service.value("TaxServiceLogFile").toString();
  }
  else
  {
    _ServerVersion = "";
    _NoAvaTaxCommit = false;
    _LogTaxService = false;
    _TaxServiceLogFile = "";
  }
}

void AvalaraIntegration::sendRequest(QString type, QString orderType, int orderId, QString payload, QStringList config, QString orderNumber)
{
  if (_NoAvaTaxCommit &&
      (type == "committransaction" || type == "voidtransaction" || type == "refundtransaction"))
    return;

  XSqlQuery build;
  build.prepare("SELECT buildAvalaraUrl(:type, :orderType, :orderId, :url, :orderNumber) AS url, "
                "       buildAvalaraHeaders(:account, :key) AS headers;");
  build.bindValue(":type", type);
  build.bindValue(":orderType", orderType);
  build.bindValue(":orderId", orderId);
  if (config.size() >= 3)
  {
    build.bindValue(":account", config[0]);
    build.bindValue(":key", config[1]);
    build.bindValue(":url", config[2]);
  }
  if (!orderNumber.isEmpty())
    build.bindValue(":orderNumber", orderNumber);
  build.exec();

  if (build.first())
  {
    QNetworkRequest netrequest;
    QJsonDocument doc = QJsonDocument::fromJson(payload.toUtf8());

    netrequest.setUrl(build.value("url").toString());

    foreach(QString header, build.value("headers").toString().split(","))
    {
      if (header.split(": ").size() > 1)
        netrequest.setRawHeader(header.split(": ")[0].toUtf8(), header.split(": ")[1].toUtf8());
    }

    netrequest.setRawHeader("X-Avalara-UID", QByteArray("a0o0b000003PfVt"));
    netrequest.setRawHeader("X-Avalara-Client",
                            (QString("xTuple; %1; REST; V2; %2")
                             .arg(_ServerVersion)
                             .arg(QHostInfo::localHostName())).toUtf8());

    foreach (QNetworkReply* other, replies)
    {
      if (other->property("type") == type &&
          ((other->property("type") == "test" &&
            other->property("config") == config) ||
           (other->property("type") == "taxcodes") ||
           (other->property("type") == "taxexempt") ||
           ((other->property("type") == "createtransaction" ||
             other->property("type") == "committransaction" ||
             other->property("type") == "voidtransaction" ||
             other->property("type") == "refundtransaction") &&
            other->property("orderType") == orderType &&
            other->property("orderId") == orderId)))
      {
        other->abort();
        replies.removeOne(other);
      }
    }

    QNetworkReply* reply;
    QDateTime time;
    if (type == "test" || type == "taxcodes" || type == "taxexempt")
    {
      timer.start();
      time = QDateTime::currentDateTime();
      reply = restclient->get(netrequest);
    }
    else
    {
      timer.start();
      time = QDateTime::currentDateTime();
      reply = restclient->post(netrequest, doc.toJson(QJsonDocument::Compact));
    }
    reply->setProperty("type", type);
    reply->setProperty("orderType", orderType);
    reply->setProperty("orderId", orderId);
    reply->setProperty("config", config);
    reply->setProperty("request", QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    reply->setProperty("time", time);
    replies.append(reply);
  }

  ErrorReporter::error(QtCriticalMsg, 0, tr("Error building request"),
                       build, __FILE__, __LINE__);
}

void AvalaraIntegration::handleResponse(QNetworkReply* reply)
{
  int elapsed = timer.nsecsElapsed();
  QString orderType = reply->property("orderType").toString();
  int orderId = reply->property("orderId").toInt();
  QString type = reply->property("type").toString();
  QByteArray request = reply->property("request").toByteArray();
  QByteArray response = reply->readAll();

  if (_LogTaxService)
  {
    QFile log(_TaxServiceLogFile);
    if (log.open(QIODevice::Append))
    {
      QString txt = reply->property("time").toString() + ":\n";
      txt += "AvaTax ";
      if (type == "test")
        txt += "Ping";
      else if (type == "taxcodes")
        txt += "ListTaxCodes";
      else if (type == "taxexempt")
        txt += "ListEntityUseCodes";
      else if (type == "createtransaction")
        txt += "CreateOrAdjustTransaction";
      else if (type == "committransaction")
        txt += "CommitTransaction";
      else if (type == "voidtransaction")
        txt += "VoidTransaction";
      else if (type == "refundtransaction")
        txt += "RefundTransaction";
      else
        txt += "Error";
      txt += "\n";
      txt += QString::number(elapsed) + " ns elapsed\n";
      txt += "Request:\n";
      txt += QString::fromUtf8(QJsonDocument::fromJson(request).toJson(QJsonDocument::Indented));
      txt += "\nResponse:\n";
      txt += QString::fromUtf8(QJsonDocument::fromJson(response).toJson(QJsonDocument::Indented));
      txt += "\n";

      log.write(txt.toUtf8());
    }
    log.close();
  }

  if (reply->error() != QNetworkReply::OperationCanceledError)
  {
    QJsonObject responseJson = QJsonDocument::fromJson(response).object();
    replies.removeOne(reply);
    TaxIntegration::handleResponse(type, orderType, orderId, QString::fromUtf8(response), error(type, reply, responseJson));
  }

  reply->deleteLater();
}

QString AvalaraIntegration::error(QString type, QNetworkReply* reply, QJsonObject response)
{
  if (type == "test")
  {
    if (reply->error() != QNetworkReply::NoError)
      return reply->errorString();
    else if (!response["authenticated"].toBool())
      return tr("Invalid authentication details.");
  }
  else if (type == "createtransaction")
  {
    if (reply->error() != QNetworkReply::NoError)
    {
      QJsonObject err = response["error"].toObject();
      QString errcode = err["code"].toString();
      QString errmsg  = err["message"].toString();
      QJsonArray details = err["details"].toArray();
      QJsonObject ary = details.at(0).toObject();
      QString errhelp = ary["description"].toString();
      return tr("%1: %2<br><br>Caused by:<br>%3.").arg(errcode).arg(errmsg).arg(errhelp);
    }
  }
  else
  {
    if (reply->error() != QNetworkReply::NoError)
      return reply->errorString();
  }

  return "";
}

void AvalaraIntegration::wait()
{
  if (!eventLoop.isRunning() && replies.size())
  {
    qApp->setOverrideCursor(Qt::WaitCursor);
    eventLoop.exec();
  }
}

void AvalaraIntegration::done()
{
  if (replies.isEmpty())
  {
    eventLoop.quit();
    qApp->restoreOverrideCursor();
  }
}
