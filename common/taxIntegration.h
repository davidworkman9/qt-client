/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef TAXINTEGRATION_H
#define TAXINTEGRATION_H

#include <QElapsedTimer>
#include <QObject>
#include <QDate>
#include <QSqlDriver>
#include <QJsonObject>

#include "xsqlquery.h"

class QScriptEngine;

class TaxIntegration : public QObject
{
  Q_OBJECT

  public:
    TaxIntegration(bool = false);

    Q_INVOKABLE static TaxIntegration* getTaxIntegration(bool = false);
    Q_INVOKABLE virtual void test(QStringList);
    Q_INVOKABLE virtual void getTaxCodes();
    Q_INVOKABLE virtual void getTaxExemptCategories(QStringList = QStringList());
    Q_INVOKABLE virtual bool calculateTax(QString, int, bool = false);
    Q_INVOKABLE virtual bool commit(QString, int);
    Q_INVOKABLE virtual bool cancel(QString, int, QString = QString());
    Q_INVOKABLE virtual void refund(int, QDate);
    Q_INVOKABLE virtual void wait();

    Q_INVOKABLE virtual QString error();

  signals:
    void connectionTested(QString);
    void taxCodesFetched(QJsonObject, QString);
    void taxExemptCategoriesFetched(QJsonObject, QString);
    void taxCalculated(double, QString);

  protected:
    virtual void sendRequest(QString, QString = QString(), int = 0, QString = QString(), QStringList = QStringList(), QString = QString()) = 0;
    virtual void done();

    QElapsedTimer timer;
    QString _error;

  protected slots:
    virtual void handleResponse(QString, QString, int, QString, QString);
    virtual void sNotified(const QString&, QSqlDriver::NotificationSource, const QVariant&);
};

void setupTaxIntegration(QScriptEngine *engine);
Q_DECLARE_METATYPE(TaxIntegration *)

#endif
