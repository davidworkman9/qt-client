/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "crmRole.h"

#include <metasql.h>
#include "mqlutil.h"
#include "errorReporter.h"
#include "guiErrorCheck.h"

#include <QMessageBox>
#include <QVariant>

crmRole::crmRole(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
  : XDialog(parent, name, modal, fl)
{
  setupUi(this);

  connect(_save, SIGNAL(clicked()), this, SLOT(sSave()));
  connect(_close, SIGNAL(clicked()), this, SLOT(close()));

}

crmRole::~crmRole()
{
  // no need to delete child widgets, Qt does it all for us
}

void crmRole::languageChange()
{
  retranslateUi(this);
}

enum SetResponse crmRole::set(const ParameterList &pParams)
{
  XDialog::set(pParams);
  XSqlQuery role;
  QVariant param;
  bool     valid;

  emit saveBeforeBegin();

  param = pParams.value("crmrole_id", &valid);
  if (valid)
  {
    _crmroleid = param.toInt();
    populate();
  }

  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "new")
      _mode = cNew;
    else if (param.toString() == "edit")
      _mode = cEdit;
    else if (param.toString() == "view")
    {
      _mode = cView;
      _role->setEnabled(false);
      _appliesTo->setEnabled(false);
      _order->setEnabled(false);
      _close->setText(tr("&Close"));
      _save->hide();
    }
  }

  emit saveAfterCommit();

  return NoError;
}

void crmRole::sSave()
{
  XSqlQuery typeSave;

  QList<GuiErrorCheck> errors;
  errors << GuiErrorCheck(_role->text().trimmed().length() == 0, _role,
                          tr("You must enter a Role before you may save it.") )
         << GuiErrorCheck(!_addr->isChecked() && !_cntct->isChecked() && !_account->isChecked() && !_phone->isChecked(), _addr,
                          tr("You must make at least one assignment before you may save the role.") )
  ;

  if (GuiErrorCheck::reportErrors(this, tr("Cannot Save CRM Role"), errors))
      return;

  if (_mode == cNew)
    typeSave.prepare( "INSERT INTO crmrole "
               "(crmrole_name, crmrole_cntct, crmrole_addr, crmrole_crmacct, crmrole_phone, crmrole_sort) "
               "VALUES "
               "(:crmrole_name, :crmrole_cntct, :crmrole_addr, :crmrole_crmacct, :crmrole_phone, :crmrole_sort);" );
  else
    typeSave.prepare( "UPDATE crmrole "
               "SET crmrole_name=:crmrole_name, crmrole_cntct=:crmrole_cntct, crmrole_addr=:crmrole_addr, "
               "    crmrole_crmacct=:crmrole_crmacct, crmrole_phone=:crmrole_phone, crmrole_sort=:crmrole_sort "
               "WHERE (crmrole_id=:crmrole_id);" );

  typeSave.bindValue(":crmrole_id", _crmroleid);
  typeSave.bindValue(":crmrole_name", _role->text());
  typeSave.bindValue(":crmrole_cntct", QVariant(_cntct->isChecked()));
  typeSave.bindValue(":crmrole_addr", QVariant(_addr->isChecked()));
  typeSave.bindValue(":crmrole_crmacct", QVariant(_account->isChecked()));
  typeSave.bindValue(":crmrole_phone", QVariant(_phone->isChecked()));
  typeSave.bindValue(":crmrole_sort", QVariant(_order->value()));

  typeSave.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Saving CRM Role"),
                                typeSave, __FILE__, __LINE__))
  {
    return;
  }

  omfgThis->sItemGroupsUpdated(-1, true);

  close();
}

void crmRole::populate()
{
  XSqlQuery rolepopulate;
  rolepopulate.prepare( "SELECT * FROM crmrole "
             "WHERE (crmrole_id=:crmrole_id);" );
  rolepopulate.bindValue(":crmrole_id", _crmroleid);
  rolepopulate.exec();
  if (rolepopulate.first())
  {
    _role->setText(rolepopulate.value("crmrole_name").toString());
    _cntct->setChecked(rolepopulate.value("crmrole_cntct").toBool());
    _addr->setChecked(rolepopulate.value("crmrole_addr").toBool());
    _account->setChecked(rolepopulate.value("crmrole_crmacct").toBool());
    _phone->setChecked(rolepopulate.value("crmrole_phone").toBool());
    _order->setValue(rolepopulate.value("crmrole_sort").toInt());
  }
}
