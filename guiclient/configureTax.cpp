/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "configureTax.h"
#include "avalaraIntegration.h"
#include "errorReporter.h"
#include "guiErrorCheck.h"

#include <QJsonDocument>
#include <QMessageBox>
#include <QStandardPaths>

configureTax::configureTax(QWidget* parent, const char* name, bool /*modal*/, Qt::WindowFlags fl)
    : XAbstractConfigure(parent, fl)
{
  setupUi(this);

  _tax = new AvalaraIntegration();

  _service->append(0, "None",    "N");
  _service->append(1, "Avalara", "A");

  connect(_account, SIGNAL(editingFinished()), this, SLOT(sCheck()));
  connect(_key, SIGNAL(editingFinished()), this, SLOT(sCheck()));
  connect(_url, SIGNAL(editingFinished()), this, SLOT(sCheck()));
  connect(_company, SIGNAL(editingFinished()), this, SLOT(sCheck()));
  connect(_test, SIGNAL(clicked()), this, SLOT(sTest()));
  connect(_tax, SIGNAL(connectionTested(QString)), this, SLOT(sTest(QString)));
  connect(_console, SIGNAL(leftClickedURL(QString)), this, SLOT(sOpenUrl(QString)));

  if (_metrics->value("TaxService") != "")
    _service->setCode(_metrics->value("TaxService"));

  _account->setText(_metrics->value("AvalaraAccount"));
  _key->setText(_metrics->value("AvalaraKey"));
  _url->setText(_metrics->value("AvalaraUrl"));
  _company->setText(_metrics->value("AvalaraCompany"));
  _upc->setChecked(_metrics->boolean("AvalaraUPC"));
  _disableRecording->setChecked(_metrics->boolean("NoAvaTaxCommit"));
  _log->setChecked(_metrics->boolean("LogTaxService"));
  _logFile->setText(_metrics->value("TaxServiceLogFile"));

  if (_logFile->text().isEmpty())
    _logFile->setText(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/log.txt");

  _salesExempt->setCode(_metrics->value("AvalaraSalesExemptionCode"));
  _purchaseExempt->setCode(_metrics->value("AvalaraPurchaseExemptionCode"));

  _salesAccount->setId(_metrics->value("AvalaraSalesAccountId").toInt());
  _useAccount->setId(_metrics->value("AvalaraUseAccountId").toInt());

  sCheck();
}

configureTax::~configureTax()
{
  // no need to delete child widgets, Qt does it all for us

  delete _tax;
}

void configureTax::languageChange()
{
  retranslateUi(this);
}

bool configureTax::sSave()
{
  emit saving();

  if (_service->code() == "A")
  {
    QList<GuiErrorCheck> errors;
    errors << GuiErrorCheck(_account->text().isEmpty(), _account,
                            tr("You must enter an Account #."))
           << GuiErrorCheck(_key->text().isEmpty(), _key,
                            tr("You must enter a License Key."))
           << GuiErrorCheck(_url->text().isEmpty(), _url,
                            tr("You must enter a Service URL."))
           << GuiErrorCheck(_company->text().isEmpty(), _company,
                            tr("You must enter a Company Code."))
           << GuiErrorCheck(!_salesAccount->isValid(), _salesAccount,
                            tr("You must enter a Sales Tax Liability Account."))
           << GuiErrorCheck(!_useAccount->isValid(), _useAccount,
                            tr("You must enter a Use Tax Liability Account."));

    if (GuiErrorCheck::reportErrors(this, tr("Cannot Save Tax Configuration"), errors))
      return false;
  }

  _metrics->set("TaxService", _service->code());

  if (_service->code() == "A")
  {
    _metrics->set("AvalaraAccount", _account->text());
    _metrics->set("AvalaraKey", _key->text());
    _metrics->set("AvalaraUrl", _url->text());
    _metrics->set("AvalaraCompany", _company->text());
    _metrics->set("AvalaraUPC", _upc->isChecked());
    _metrics->set("NoAvaTaxCommit", _disableRecording->isChecked());
    _metrics->set("LogTaxService", _log->isChecked());
    _metrics->set("TaxServiceLogFile", _logFile->text());
    _metrics->set("AvalaraSalesAccountId", _salesAccount->id());
    _metrics->set("AvalaraUseAccountId", _useAccount->id());
    _metrics->set("AvalaraSalesExemptionCode", _salesExempt->code());
    _metrics->set("AvalaraPurchaseExemptionCode", _purchaseExempt->code());
  }

  _taxIntegration->wait();
  delete _taxIntegration;
  _taxIntegration = TaxIntegration::getTaxIntegration(true);
  _x_taxIntegration = _taxIntegration;

  return true;
}

bool configureTax::sCheck()
{
  bool valid = (!_account->isNull() && !_key->isNull() && !_url->isNull() && !_company->isNull());
  _test->setEnabled(valid);

  if (valid)
  {
    QStringList configuration;
    configuration << _account->text() << _key->text() << _url->text() << _company->text();
    _salesExempt->populate(configuration);
    _purchaseExempt->populate(configuration);
  }

  return valid;
}

void configureTax::sTest()
{
  QStringList configuration;
  configuration << _account->text() << _key->text() << _url->text() << _company->text();

  _tax->test(configuration);
  _tax->wait();

  return;
}

void configureTax::sTest(QString error)
{
  if (error.isEmpty())
    QMessageBox::information(this, tr("Avalara Integration Test"), tr("Connection Test Succeeded"));
  else
    QMessageBox::information(this, tr("Avalara Integration Test"), tr("Connection Test Failed:\n%1").arg(error));
}

void configureTax::sOpenUrl(QString url)
{
  omfgThis->launchBrowser(this, url);
}
