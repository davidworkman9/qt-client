/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "configureCRM.h"

#include <QMessageBox>
#include <QVariant>

#include "guiclient.h"
#include "mqlutil.h"
#include "errorReporter.h"

configureCRM::configureCRM(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : XAbstractConfigure(parent, fl)
{
  Q_UNUSED(modal);
  XSqlQuery configureconfigureCRM;
  setupUi(this);

  if (name)
    setObjectName(name);

  _nextInNumber->setValidator(omfgThis->orderVal());
  _nextAcctNumber->setValidator(omfgThis->orderVal());
  _nextProjectNumber->setValidator(omfgThis->orderVal());
  _nextTaskNumber->setValidator(omfgThis->orderVal());

  configureconfigureCRM.exec( "SELECT orderseq_number AS innumber "
          "  FROM orderseq"
          " WHERE (orderseq_name='IncidentNumber');" );
  if (configureconfigureCRM.first())
    _nextInNumber->setText(configureconfigureCRM.value("innumber"));

  configureconfigureCRM.exec( "SELECT orderseq_number AS acnumber "
          "  FROM orderseq"
          " WHERE (orderseq_name='CRMAccountNumber');" );
  if (configureconfigureCRM.first())
    _nextAcctNumber->setText(configureconfigureCRM.value("acnumber"));

  configureconfigureCRM.exec( "SELECT orderseq_number AS prjnumber "
          "  FROM orderseq"
          " WHERE (orderseq_name='ProjectNumber');" );
  if (configureconfigureCRM.first())
    _nextProjectNumber->setText(configureconfigureCRM.value("prjnumber"));

  configureconfigureCRM.exec( "SELECT orderseq_number AS tsknumber "
          "  FROM orderseq"
          " WHERE (orderseq_name='TaskNumber');" );
  if (configureconfigureCRM.first())
    _nextTaskNumber->setText(configureconfigureCRM.value("tsknumber"));

  _acctGeneration->append(0, tr("Manual"), "M");
  _acctGeneration->append(1, tr("Automatic"), "A");
  _acctGeneration->append(2, tr("Automatic, Allow Override"), "O");
  _incidentGeneration->append(1, tr("Automatic"), "A");
  _projectGeneration->append(0, tr("Manual"), "M");
  _projectGeneration->append(1, tr("Automatic"), "A");
  _projectGeneration->append(2, tr("Automatic, Allow Override"), "O");
  _taskGeneration->append(0, tr("Manual"), "M");
  _taskGeneration->append(1, tr("Automatic"), "A");
  _taskGeneration->append(2, tr("Automatic, Allow Override"), "O");

  _acctGeneration->setCode(_metrics->value("CRMAccountNumberGeneration"));
  _projectGeneration->setCode(_metrics->value("ProjectNumberGeneration"));
  _taskGeneration->setCode(_metrics->value("TaskNumberGeneration"));
  _useProjects->setChecked(_metrics->boolean("UseProjects"));
  _autoCreate->setChecked(_metrics->boolean("AutoCreateProjectsForOrders"));
  _accountChangeLog->setChecked(_metrics->boolean("AccountChangeLog"));
  _addressChangeLog->setChecked(_metrics->boolean("AddressChangeLog"));
  _contactChangeLog->setChecked(_metrics->boolean("ContactChangeLog"));
  _opportunityChangeLog->setChecked(_metrics->boolean("OpportunityChangeLog"));
  _requireProjectAssignment->setChecked(_metrics->boolean("RequireProjectAssignment"));
  _projectWarningDays->setValue(_metrics->value("ProjectDueDateWarning").toInt());
  _requireUniqueEmails->setChecked(_metrics->boolean("EnforceUniqueContactEmails"));
  _defaultEmailopt->setChecked(_metrics->boolean("DefaultEmailOptIn"));
  _defaultAddropt->setChecked(_metrics->boolean("DefaultAddressOptIn"));
  _documentPrivileges->setChecked(_metrics->boolean("UnprivilegedViewDocInList"));

  if (! _metrics->value("DefaultAddressCountry").isEmpty())
    _country->setCode(_metrics->value("DefaultAddressCountry"));

  _incidentsPublicShow->setChecked(_metrics->boolean("IncidentsPublicPrivate"));
  _incidentsPublicDefault->setChecked(_metrics->boolean("IncidentPublicDefault"));

  if (_metrics->boolean("EnableBatchManager"))
  {
    _incdtEmailProfile->populate("SELECT ediprofile_id, ediprofile_name "
                                 "FROM xtbatch.ediprofile "
                                 "WHERE (ediprofile_type='email');");
    _incdtEmailProfile->setId(_metrics->value("CRMIncidentEmailProfile").toInt());
    _incdtCreated->setChecked(_metrics->boolean("CRMIncidentEmailCreated"));
    _incdtAssigned->setChecked(_metrics->boolean("CRMIncidentEmailAssigned"));
    _incdtStatus->setChecked(_metrics->boolean("CRMIncidentEmailStatus"));
    _incdtUpdated->setChecked(_metrics->boolean("CRMIncidentEmailUpdated"));
    _incdtComments->setChecked(_metrics->boolean("CRMIncidentEmailComments"));
  }
  else
  {
    _incdtEmailProfileLit->hide();
    _incdtEmailProfile->hide();
    _incdtDelGroup->hide();
  }

  connect(_validateEmail,  SIGNAL(clicked()), this, SLOT(sEmailValidation()));
  _validateEmail->setChecked(_metrics->boolean("ValidateEmail"));
  sEmailValidation();
  _defaultRegexp = "^(([^<>()\\[\\]\\\\.,;:\\s@\"]+(\\.[^<>()\\[\\]\\\\.,;:\\s@\"]+)*)|(\".+\"))@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}])|(([a-zA-Z\\-0-9]+\\.)+[a-zA-Z]{2,}))$";

  _emailValidatorMethod->append(0, tr("Basic"), "B");
  // _emailValidatorMethod->append(1, "External Service", "E");

  connect(_emailValidatorMethod,  SIGNAL(currentIndexChanged(int)), this, SLOT(sEmailValidator()));
  if (_metrics->value("EmailValidatorMethod").length() > 0)
    _emailValidatorMethod->setCode(_metrics->value("EmailValidatorMethod"));

  sEmailValidator();

  configureconfigureCRM.exec("SELECT * FROM status WHERE (status_type='INCDT') ORDER BY status_seq;");
  configureconfigureCRM.first();
  _new->setText(configureconfigureCRM.value("status_color"));
  configureconfigureCRM.next();
  _feedback->setText(configureconfigureCRM.value("status_color"));
  configureconfigureCRM.next();
  _confirmed->setText(configureconfigureCRM.value("status_color"));
  configureconfigureCRM.next();
  _assigned->setText(configureconfigureCRM.value("status_color"));
  configureconfigureCRM.next();
  _resolved->setText(configureconfigureCRM.value("status_color"));
  configureconfigureCRM.next();
  _closed->setText(configureconfigureCRM.value("status_color"));

  adjustSize();
}

configureCRM::~configureCRM()
{
    // no need to delete child widgets, Qt does it all for us
}

void configureCRM::languageChange()
{
    retranslateUi(this);
}

void configureCRM::sEmailValidation()
{
  _emailValidatorGroup->setVisible(_validateEmail->isChecked());
}

void configureCRM::sEmailValidator()
{
  int _method = _emailValidatorMethod->id();
// Use of Validation method and stacked widget allows for future
// External service plugins.
  _emailStack->setCurrentIndex(_method);
  switch(_method){
    case 0:  // Basic Validation
      if (_metrics->value("EmailValidatorRegexp").length() == 0)
        _emailRegexp->setText(_defaultRegexp);
      else
        _emailRegexp->setText(_metrics->value("EmailValidatorRegexp"));
      break;
    default:
      _emailValidatorGroup->setVisible(false);
  }
}

bool configureCRM::sSave()
{
  emit saving();

  XSqlQuery configq;
  configq.prepare( "SELECT setNextIncidentNumber(:innumber);" );
  configq.bindValue(":innumber", _nextInNumber->text().toInt());
  configq.exec();

  if (_acctGeneration->code() != "M" && _nextAcctNumber->text().toInt() < 1)
    _nextAcctNumber->setText("1");
  if (_projectGeneration->code() != "M" && _nextProjectNumber->text().toInt() < 1)
    _nextProjectNumber->setText("1");
  if (_taskGeneration->code() != "M" && _nextTaskNumber->text().toInt() < 1)
    _nextTaskNumber->setText("1");

  configq.prepare( "SELECT setNextCRMAccountNumber(:acnumber);" );
  configq.bindValue(":acnumber", _nextAcctNumber->text().toInt());
  configq.exec();

  configq.prepare( "SELECT setNextProjectNumber(:prjnumber);" );
  configq.bindValue(":prjnumber", _nextProjectNumber->text().toInt());
  configq.exec();

  configq.prepare( "SELECT setNextTaskNumber(:tsknumber);" );
  configq.bindValue(":tsknumber", _nextTaskNumber->text().toInt());
  configq.exec();

  _metrics->set("CRMAccountNumberGeneration", _acctGeneration->code());
  _metrics->set("ProjectNumberGeneration", _projectGeneration->code());
  _metrics->set("TaskNumberGeneration", _taskGeneration->code());

  _metrics->set("UseProjects", _useProjects->isChecked());
  _metrics->set("AutoCreateProjectsForOrders", (_autoCreate->isChecked() && _useProjects->isChecked()));
  _metrics->set("AccountChangeLog", _accountChangeLog->isChecked());
  _metrics->set("AddressChangeLog", _addressChangeLog->isChecked());
  _metrics->set("ContactChangeLog", _contactChangeLog->isChecked());
  _metrics->set("OpportunityChangeLog", _opportunityChangeLog->isChecked());
  _metrics->set("RequireProjectAssignment", _requireProjectAssignment->isChecked());
  _metrics->set("ProjectDueDateWarning", _projectWarningDays->value());
  _metrics->set("UnprivilegedViewDocInList", _documentPrivileges->isChecked());
  _metrics->set("ValidateEmail", _validateEmail->isChecked());
  _metrics->set("EmailValidatorMethod", _emailValidatorMethod->code());
  if (_validateEmail->isChecked())
  {
    if (_emailValidatorMethod->code() == "B")   //Basic Validation
      _metrics->set("EmailValidatorRegexp", _emailRegexp->toPlainText());
  }

  if (_country->isValid())
    _metrics->set("DefaultAddressCountry", _country->code());
  else
    _metrics->set("DefaultAddressCountry", QString(""));

  _metrics->set("IncidentsPublicPrivate", _incidentsPublicShow->isChecked());
  _metrics->set("IncidentPublicDefault", _incidentsPublicDefault->isChecked());

  if (!_metrics->boolean("EnforceUniqueContactEmails") && (_requireUniqueEmails->isChecked()))
  {
    // Check for existing duplicated Contact email addresses if switching on
    QString usql = "SELECT EXISTS(SELECT 1 FROM cntcteml "
                   "GROUP BY cntcteml_email "
                   "HAVING count(*) > 1) AS result;";
    configq.exec(usql);
    if (configq.first() && (configq.value("result").toBool()))
    {
      QMessageBox::warning(this, tr("Unique Emails"),
      tr("You cannot enable unique email addresses on contacts "
         "when duplicates exist.  Please first remove all "
         "duplicate email addresses on contacts, then enable this setting."));
      _requireUniqueEmails->setChecked(false);
      return false;
    }
  }
  _metrics->set("EnforceUniqueContactEmails", _requireUniqueEmails->isChecked());
  _metrics->set("DefaultEmailOptIn", _defaultEmailopt->isChecked());
  _metrics->set("DefaultAddressOptIn", _defaultAddropt->isChecked());

  if (_metrics->boolean("EnableBatchManager"))
  {
    _metrics->set("CRMIncidentEmailProfile", _incdtEmailProfile->id());
    _metrics->set("CRMIncidentEmailCreated"   , _incdtCreated->isChecked());
    _metrics->set("CRMIncidentEmailAssigned"  , _incdtAssigned->isChecked());
    _metrics->set("CRMIncidentEmailStatus"    , _incdtStatus->isChecked());
    _metrics->set("CRMIncidentEmailUpdated"   , _incdtUpdated->isChecked());
    _metrics->set("CRMIncidentEmailComments"  , _incdtComments->isChecked());
  }

  configq.prepare("UPDATE status SET status_color = :color "
            "WHERE ((status_type='INCDT') "
            " AND (status_code=:code));");
  configq.bindValue(":code", "N");
  configq.bindValue(":color", _new->text());
  configq.exec();
  configq.bindValue(":code", "F");
  configq.bindValue(":color", _feedback->text());
  configq.exec();
  configq.bindValue(":code", "C");
  configq.bindValue(":color", _confirmed->text());
  configq.exec();
  configq.bindValue(":code", "A");
  configq.bindValue(":color", _assigned->text());
  configq.exec();
  configq.bindValue(":code", "R");
  configq.bindValue(":color", _resolved->text());
  configq.exec();
  configq.bindValue(":code", "L");
  configq.bindValue(":color", _closed->text());
  configq.exec();

  return true;
}
