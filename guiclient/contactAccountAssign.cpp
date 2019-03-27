/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "contactAccountAssign.h"

#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include "errorReporter.h"
#include "guiErrorCheck.h"

contactAccountAssign::contactAccountAssign(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : XDialog(parent, name, modal, fl)
{
  setupUi(this);

  connect(_save, SIGNAL(clicked()), this, SLOT(sSave()));
}

contactAccountAssign::~contactAccountAssign()
{
  // no need to delete child widgets, Qt does it all for us
}

void contactAccountAssign::languageChange()
{
  retranslateUi(this);
}

enum SetResponse contactAccountAssign::set(const ParameterList &pParams)
{
  XDialog::set(pParams);
  QVariant param;
  bool     valid;

  _accnt->setFocus();

  param = pParams.value("assign_id", &valid);
  if (valid)
  {
    _assignid = param.toInt();
    populate();
    _accnt->setEnabled(false);
  }

  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "new")
    {
      _mode = cNew;
      _active->setChecked(true);
      _contact->setEnabled(true);
    }
    else if (param.toString() == "edit")
      _mode = cEdit;
  }

  param = pParams.value("crmacct", &valid);
  if (valid)
  {
    _accnt->setId(param.toInt());
    _accnt->setEnabled(false);
    _role->setFocus();
  }

  param = pParams.value("contact", &valid);
  if (valid)
  {
    _contact->setId(param.toInt());
    _contact->setEnabled(false);
    if (_mode == cNew)
      _accnt->setEnabled(true);
  }

  return NoError;
}

void contactAccountAssign::sSave()
{
  XSqlQuery saveq;

  QList<GuiErrorCheck> errors;
  errors << GuiErrorCheck(!_accnt->isValid(), _accnt,
                          tr("You must select a valid Account before continuing"))
         << GuiErrorCheck(!_contact->isValid(), _contact,
                          tr("You must enter a valid Contact before continuing"))
         << GuiErrorCheck(!_role->isValid(), _role,
                          tr("You must select a valid CRM Role before continuing"))
     ;


  if (GuiErrorCheck::reportErrors(this, tr("Cannot Save Assignment"), errors))
    return;

  if (_mode == cNew)
    saveq.prepare("INSERT INTO crmacctcntctass (crmacctcntctass_crmacct_id, crmacctcntctass_cntct_id,"
                  "      crmacctcntctass_crmrole_id, crmacctcntctass_default, crmacctcntctass_active)"
                  " VALUES (:accnt, :cntct, :role, :default, :active)"
                  " ON CONFLICT ON CONSTRAINT crmacctcntctass_unq "
                  " DO UPDATE SET crmacctcntctass_default=:default, crmacctcntctass_active=:active "
                  " RETURNING crmacctcntctass_id AS id;");
  else
  {
    saveq.prepare("UPDATE crmacctcntctass SET crmacctcntctass_crmrole_id=:role,"
                  "                           crmacctcntctass_default=:default,"
                  "                           crmacctcntctass_active=:active "
                  " WHERE crmacctcntctass_id=:assignid;");
    saveq.bindValue(":assignid", _assignid);
  }
  saveq.bindValue(":accnt",   _accnt->id());
  saveq.bindValue(":cntct",   _contact->id());
  saveq.bindValue(":role",    _role->id());
  saveq.bindValue(":default", _default->isChecked());
  saveq.bindValue(":active",  _active->isChecked());
  saveq.exec();
  if (saveq.first())
    _assignid = saveq.value("id").toInt();
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Saving Assignment"),
                                saveq, __FILE__, __LINE__))
    return;

  omfgThis->sContactsUpdated(-1);
  done(_assignid);
}

void contactAccountAssign::populate()
{
  XSqlQuery popq;
  popq.prepare("SELECT crmacctcntctass.* "
       	       "  FROM crmacctcntctass "
	       " WHERE crmacctcntctass_id=:assign_id;");
  popq.bindValue(":assign_id", _assignid);
  popq.exec();
  if (popq.first())
  {
    _accnt->setId(popq.value("crmacctcntctass_crmacct_id").toInt());
    _contact->setId(popq.value("crmacctcntctass_cntct_id").toInt());
    _default->setChecked(popq.value("crmacctcntctass_default").toBool());
    _active->setChecked(popq.value("crmacctcntctass_active").toBool());
    _role->setId(popq.value("crmacctcntctass_crmrole_id").toInt());
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Assignment Information"),
                                popq, __FILE__, __LINE__))
    return;
}
