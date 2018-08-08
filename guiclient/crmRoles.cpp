/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to y6ou under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "crmRoles.h"

#include <parameter.h>
#include <QMessageBox>
#include "crmRole.h"
#include "errorReporter.h"
#include "guiclient.h"

crmRoles::crmRoles(QWidget* parent, const char* name, Qt::WindowFlags fl)
  : XWidget(parent, name, fl)
{
  setupUi(this);

  // signals and slots connections
  connect(_new, SIGNAL(clicked()), this, SLOT(sNew()));
  connect(_edit, SIGNAL(clicked()), this, SLOT(sEdit()));
  connect(_view, SIGNAL(clicked()), this, SLOT(sView()));
  connect(_delete, SIGNAL(clicked()), this, SLOT(sDelete()));
  connect(_close, SIGNAL(clicked()), this, SLOT(close()));
  connect(_crmroles, SIGNAL(valid(bool)), _view, SLOT(setEnabled(bool)));

  _crmroles->addColumn(tr("Role"),        -1, Qt::AlignRight, true, "crmrole_name" );
  _crmroles->addColumn(tr("Account"),     -1, Qt::AlignRight, true, "crmrole_crmacct" );
  _crmroles->addColumn(tr("Contact"),     -1, Qt::AlignRight, true, "crmrole_cntct" );
  _crmroles->addColumn(tr("Address"),     -1, Qt::AlignRight, true, "crmrole_addr" );
  _crmroles->addColumn(tr("Phone"),       -1, Qt::AlignRight, true, "crmrole_phone" );
  _crmroles->addColumn(tr("System Role"), -1, Qt::AlignRight, true, "crmrole_system" );
  
  if (_privileges->check("MaintainCRMRoles"))
  {
    connect(_crmroles, SIGNAL(valid(bool)), _edit, SLOT(setEnabled(bool)));
    connect(_crmroles, SIGNAL(valid(bool)), _delete, SLOT(setEnabled(bool)));
    connect(_crmroles, SIGNAL(itemSelected(int)), _edit, SLOT(animateClick()));
  }
  else
  {
    _new->setEnabled(false);
    connect(_crmroles, SIGNAL(itemSelected(int)), _view, SLOT(animateClick()));
  }

  connect(omfgThis, SIGNAL(itemGroupsUpdated(int, bool)), this, SLOT(sFillList()));

  sFillList();
}

crmRoles::~crmRoles()
{
  // no need to delete child widgets, Qt does it all for us
}

void crmRoles::languageChange()
{
  retranslateUi(this);
}

void crmRoles::sDelete()
{
  XSqlQuery roleDelete;
  XTreeWidgetItem * item = (XTreeWidgetItem*)_crmroles->currentItem();
  if(item->rawValue("crmrole_system").toBool())
  {
    QMessageBox::critical(this, tr("System Role"),
                                tr("<p>You cannot delete a system role."));
    return;
  }

  roleDelete.prepare("DELETE FROM crmrole WHERE crmrole_id=:crmrole_id AND NOT crmrole_system;");
  roleDelete.bindValue(":crmrole_id", _crmroles->id());
  roleDelete.exec();

  ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting CRM Role"),
                            roleDelete, __FILE__, __LINE__);

  sFillList();
}


void crmRoles::sNew()
{
  ParameterList params;
  params.append("mode", "new");

  crmRole newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void crmRoles::sEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("crmrole_id", _crmroles->id());

  crmRole newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void crmRoles::sView()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("crmrole_id", _crmroles->id());

  crmRole newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void crmRoles::sFillList()
{
  _crmroles->populate( "SELECT crmrole_id, * "
                      "FROM crmrole "
                      "ORDER BY crmrole_name;" );
}

