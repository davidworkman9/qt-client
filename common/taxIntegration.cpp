/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "taxIntegration.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QtScript>

#include "errorReporter.h"
#include "avalaraIntegration.h"
#include "noIntegration.h"

TaxIntegration* TaxIntegration::getTaxIntegration(bool listen)
{
  // Can't access _metrics from here, but only queries once at startup and in setup
  XSqlQuery service("SELECT fetchMetricText('TaxService') AS TaxService;");

  if (service.first() && service.value("TaxService") == "A")
    return new AvalaraIntegration(listen);
  else
    return new NoIntegration(listen);
}

TaxIntegration::TaxIntegration(bool listen)
{
  if (listen)
  {
    if (!QSqlDatabase::database().driver()->subscribedToNotifications().contains("calculatetax"))
      QSqlDatabase::database().driver()->subscribeToNotification("calculatetax");

    connect(QSqlDatabase::database().driver(),
            SIGNAL(notification(const QString&, QSqlDriver::NotificationSource, const QVariant&)),
            this,
            SLOT(sNotified(const QString&, QSqlDriver::NotificationSource, const QVariant&))
           );
  }
}

void TaxIntegration::getTaxCodes()
{
  sendRequest("taxcodes");
}

void TaxIntegration::getTaxExemptCategories(QStringList config)
{
  sendRequest("taxexempt", QString(), 0, QString(), config);
}

void TaxIntegration::test(QStringList config)
{
  sendRequest("test", QString(), 0, QString(), config);
}

bool TaxIntegration::calculateTax(QString orderType, int orderId, bool record)
{
  XSqlQuery qry;
  qry.prepare("SELECT calculateOrderTax(:orderType, :orderId, :record) AS request;");
  qry.bindValue(":orderType", orderType);
  qry.bindValue(":orderId", orderId);
  qry.bindValue(":record", record);
  qry.exec();
  if (qry.first())
    if (qry.value("request").isNull())
      emit taxCalculated(0.0, "");
    else
      sendRequest("createtransaction", orderType, orderId, qry.value("request").toString());
  else
  {
    _error = qry.lastError().text();
    return false;
  }

  wait();
  return _error.isEmpty();
}

bool TaxIntegration::commit(QString orderType, int orderId)
{
  calculateTax(orderType, orderId, true);
  wait();

  if (!_error.isEmpty())
    return false;

  XSqlQuery qry;
  qry.prepare("SELECT postTax(:orderType, :orderId) AS request;");
  qry.bindValue(":orderType", orderType);
  qry.bindValue(":orderId", orderId);
  qry.exec();
  if (qry.first() && !qry.value("request").isNull())
    sendRequest("committransaction", orderType, orderId, qry.value("request").toString());
  else if (qry.lastError().type() != QSqlError::NoError)
  {
    _error =  qry.lastError().text();
    return false;
  }

  wait();
  return _error.isEmpty();
}

bool TaxIntegration::cancel(QString orderType, int orderId, QString orderNumber)
{
  XSqlQuery qry;
  qry.prepare("SELECT voidTax(:orderType, :orderId) AS request;");
  qry.bindValue(":orderType", orderType);
  qry.bindValue(":orderId", orderId);
  qry.exec();
  if (qry.first() && !qry.value("request").isNull())
    sendRequest("voidtransaction", orderType, orderId, qry.value("request").toString(),
                QStringList(), orderNumber);
  else if (qry.lastError().type() != QSqlError::NoError)
  {
    _error = qry.lastError().text();
    return false;
  }

  wait();
  return _error.isEmpty();
}

void TaxIntegration::refund(int invcheadId, QDate refundDate)
{
  XSqlQuery qry;
  qry.prepare("SELECT refundTax(:invcheadId, :refundDate) AS request;");
  qry.bindValue(":invcheadId", invcheadId);
  qry.bindValue(":refundDate", refundDate);
  qry.exec();
  if (qry.first() && !qry.value("request").isNull())
    sendRequest("refundtransaction", "INV", invcheadId, qry.value("request").toString());
  else
    ErrorReporter::error(QtCriticalMsg, 0, tr("Error refunding tax"),
                         qry, __FILE__, __LINE__);
}

void TaxIntegration::handleResponse(QString type, QString orderType, int orderId, QString response, QString error)
{
  _error = error;

  if (type == "test")
  {
    done();
    emit connectionTested(error);
  }
  else if (type=="taxcodes")
  {
    done();
    emit taxCodesFetched(QJsonDocument::fromJson(response.toUtf8()).object(), error);
  }
  else if (type == "taxexempt")
  {
    done();
    emit taxExemptCategoriesFetched(QJsonDocument::fromJson(response.toUtf8()).object(), error);
  }
  else if (type=="createtransaction")
  {
    if (error.isEmpty())
    {
      XSqlQuery qry;
      qry.prepare("SELECT saveTax(:orderType, :orderId, :response) AS tax;");
      qry.bindValue(":orderType", orderType);
      qry.bindValue(":orderId", orderId);
      qry.bindValue(":response", response);
      qry.exec();
      if (qry.first())
      {
        done();
        emit taxCalculated(qry.value("tax").toDouble(), error);
      }
      else
      {
        done();
        ErrorReporter::error(QtCriticalMsg, 0, tr("Error calculating tax"),
                             qry, __FILE__, __LINE__);
      }
    }
    else
    {
      done();
      emit taxCalculated(0.0, error);
    }
  }
  else
    done();
}

void TaxIntegration::sNotified(const QString& name, QSqlDriver::NotificationSource source, const QVariant& payload)
{
  if (source != QSqlDriver::SelfSource)
    return;

  QStringList args = payload.toString().split(",");

  if (name == "calculatetax" && args.size() >= 2)
  {
    if (args.size() == 2)
      calculateTax(args[0], args[1].toInt());
    else
      calculateTax(args[0], args[1].toInt(), true);
  }
}

void TaxIntegration::wait()
{
}

void TaxIntegration::done()
{
}

QString TaxIntegration::error()
{
  return _error;
}

// script exposure /////////////////////////////////////////////////////////////

QScriptValue TaxIntegrationToScriptValue(QScriptEngine *engine, TaxIntegration *const &ti)
{
  return engine->newQObject(ti);
}

void TaxIntegrationFromScriptValue(const QScriptValue &obj, TaxIntegration * &ti)
{
  ti = qobject_cast<TaxIntegration *>(obj.toQObject());
}

// proxies for C++ methods with default arguments
QScriptValue getTaxIntegration(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);
  TaxIntegration *ti = 0;
  if (context->argumentCount() >= 1)
    ti = TaxIntegration::getTaxIntegration(context->argument(0).toBool());
  else
    ti = TaxIntegration::getTaxIntegration();

  QScriptValue result = TaxIntegrationToScriptValue(engine, ti);
  return result;
}

QScriptValue getTaxExemptCategories(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);
  TaxIntegration *ti = qscriptvalue_cast<TaxIntegration*>(context->thisObject());
  if (context->argumentCount() >= 1)
    ti->getTaxExemptCategories(context->argument(0).toVariant().toStringList());
  else
    ti->getTaxExemptCategories();
  return QScriptValue();
}

QScriptValue calculateTax(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);
  TaxIntegration *ti = qscriptvalue_cast<TaxIntegration*>(context->thisObject());
  if (! ti)
    context->throwError(QScriptContext::UnknownError, "calculateTax() called on an invalid object");
  else if (context->argumentCount() >= 3)
    ti->calculateTax(context->argument(0).toString(), context->argument(1).toInt32(), context->argument(2).toBool());
  else if (context->argumentCount() == 2)
    ti->calculateTax(context->argument(0).toString(), context->argument(1).toInt32());
  else
    context->throwError(QScriptContext::UnknownError, "calculateTax(string, integer, optional boolean) requires at least 2 arguments");
  return QScriptValue();
}

QScriptValue cancel(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);
  TaxIntegration *ti = qscriptvalue_cast<TaxIntegration*>(context->thisObject());
  if (! ti)
    context->throwError(QScriptContext::UnknownError, "cancel() called on an invalid object");
  else if (context->argumentCount() >= 3)
    ti->cancel(context->argument(0).toString(), context->argument(1).toInt32(), context->argument(2).toString());
  else if (context->argumentCount() == 2)
    ti->cancel(context->argument(0).toString(), context->argument(1).toInt32());
  else
    context->throwError(QScriptContext::UnknownError, "cancel(string, integer, optional string) requires at least 2 arguments");
  return QScriptValue();
}

void setupTaxIntegration(QScriptEngine *engine)
{
  const QScriptValue::PropertyFlags propflags = (QScriptValue::ReadOnly | QScriptValue::Undeletable);

  qScriptRegisterMetaType(engine, TaxIntegrationToScriptValue, TaxIntegrationFromScriptValue);

  // we can't instantiate a prototype TaxIntegration because it's an abstract class
  QScriptValue proto = engine->newQObject(new QObject());
  engine->setDefaultPrototype(qMetaTypeId<TaxIntegration*>(), proto);
  
  // no constructor here -- scripts should call TaxIntegration::getTaxIntegration()
  engine->globalObject().setProperty("TaxIntegration", proto);
  proto.setProperty("getTaxIntegration",      engine->newFunction(getTaxIntegration), propflags);
  proto.setProperty("getTaxExemptCategories", engine->newFunction(getTaxExemptCategories), propflags);
  proto.setProperty("calculateTax",           engine->newFunction(calculateTax), propflags);
  proto.setProperty("cancel",                 engine->newFunction(cancel), propflags);
}
