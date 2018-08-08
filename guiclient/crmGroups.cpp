/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "crmGroups.h"

#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include <parameter.h>
#include "crmGroup.h"
#include "errorReporter.h"
#include "guiclient.h"
#include "mqlutil.h"

QMap<int,  struct GroupMap*> crmGroups::_grpMap;

bool crmGroups::addToMap(int id, QString _title, QString _priv, QString _table, QString _itemTable,
                         QString _search, CRMAcctLineEdit::CRMAcctSubtype _subtype, QString _source,
                         XComboBox::XComboBoxTypes _groupcombo)
{
  if (_grpMap.contains(id)) {
    qDebug() << "crmGroups::addToMap(" << id << ") duplicate!";
    return false;
  }

  GroupMap *config = new GroupMap(id, _title, _priv, _table, _itemTable, _search, _subtype, _source, _groupcombo);
  _grpMap.insert(id,  config);
  return true;
}

QMap<int, struct GroupMap *> &crmGroups::groupMap() {
  if (_grpMap.isEmpty()) {
    addToMap(Account, tr("Account"), "MaintainAccountGroups", "crmacctgrp",  "crmacctgrpitem", "CRMAcctSearch", CRMAcctLineEdit::Crmacct, "CRMA",XComboBox::AccountGroups);
    addToMap(Customer, tr("Customer"), "MaintainCustomerGroups", "custgrp",  "custgrpitem",  "CRMAcctSearch", CRMAcctLineEdit::Cust,    "C",     XComboBox::CustomerGroups);
    addToMap(Prospect, tr("Prospect"), "MaintainProspectGroups", "pspctgrp", "pspctgrpitem", "CRMAcctSearch", CRMAcctLineEdit::Prospect,"PSPCT", XComboBox::ProspectGroups);
    addToMap(Employee, tr("Employee"), "MaintainEmployeeGroups", "empgrp",   "empgrpitem",   "CRMAcctSearch", CRMAcctLineEdit::Employee,"EMP",   XComboBox::EmployeeGroups);
    addToMap(Contact,  tr("Contact"),  "MaintainContactGroups",  "cntctgrp", "cntctgrpitem", "ContactSearch", CRMAcctLineEdit::Crmacct, "T",     XComboBox::ContactGroups);
    addToMap(Address,  tr("Address"),  "MaintainAddressGroups",  "addrgrp",  "addrgrpitem",  "AddressSearch", CRMAcctLineEdit::Crmacct, "ADDR",  XComboBox::AddressGroups);
  }
  return _grpMap;
}

crmGroups::crmGroups(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : XWidget(parent, name, fl)
{
  setupUi(this);

  if (_grpMap.isEmpty())
    (void)groupMap();

  connect(_new, SIGNAL(clicked()), this, SLOT(sNew()));
  connect(_edit, SIGNAL(clicked()), this, SLOT(sEdit()));
  connect(_delete, SIGNAL(clicked()), this, SLOT(sDelete()));
  connect(_view, SIGNAL(clicked()), this, SLOT(sView()));

  _grp->addColumn(tr("Name"), _itemColumn, Qt::AlignLeft, true, "groups_name");
  _grp->addColumn(tr("Description"),   -1, Qt::AlignLeft, true, "groups_descrip");
  
}

crmGroups::~crmGroups()
{
  // no need to delete child widgets, Qt does it all for us
}

void crmGroups::languageChange()
{
  retranslateUi(this);
}

enum SetResponse crmGroups::set(const ParameterList &pParams)
{
  XWidget::set(pParams);
  QVariant param;
  bool     valid;

  param = pParams.value("groupType", &valid);
  if (valid)
  {
    _elem = _grpMap.value(param.toInt());
    setWindowTitle(tr("List %1 Groups").arg(_elem->title));
    _crmGroupsLit->setText(tr("%1 Groups:").arg(_elem->title));
  }

  if (_privileges->check(_elem->priv))
  {
    connect(_grp, SIGNAL(valid(bool)), _edit, SLOT(setEnabled(bool)));
    connect(_grp, SIGNAL(valid(bool)), _delete, SLOT(setEnabled(bool)));
    connect(_grp, SIGNAL(itemSelected(int)), _edit, SLOT(animateClick()));
  }
  else
  {
    _new->setEnabled(false);
    connect(_grp, SIGNAL(itemSelected(int)), _view, SLOT(animateClick()));
  }

  sFillList();

  return NoError;
}

void crmGroups::sDelete()
{
  XSqlQuery groupDelete;
  ParameterList params;

  if (QMessageBox::question(this, tr("Delete?"),
                            tr("Are you sure you want to delete this %1 Group?").arg(_elem->title),
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
    return;

  QString sql = "DELETE FROM <? literal('groupsitem') ?> "
                "WHERE (groupsitem_groups_id=<? value('groups_id') ?>); "
                "DELETE FROM <? literal('groups') ?> "
                "WHERE (groups_id=<? value('groups_id') ?>);";
  params.append("groups",     _elem->table);
  params.append("groupsitem", _elem->itemTable);
  params.append("groups_id",  _grp->id());
  MetaSQLQuery delq(sql);
  groupDelete = delq.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting %1 Group").arg(_elem->title),
                                  groupDelete, __FILE__, __LINE__))
    return;
  else
    sFillList();
}

void crmGroups::sNew()
{
  ParameterList params;
  params.append("mode", "new");
  params.append("groupType", _elem->groupId);

  crmGroup newdlg(this, "", true);
  newdlg.set(params);
  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void crmGroups::sEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("groups_id", _grp->id());
  params.append("groupType", _elem->groupId);

  crmGroup newdlg(this, "", true);
  newdlg.set(params);
  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void crmGroups::sView()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("groups_id", _grp->id());
  params.append("groupType", _elem->groupId);

  crmGroup newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void crmGroups::sFillList()
{
  XSqlQuery populateList;
  ParameterList params;

  QString sql = "SELECT groups_id, groups_name, groups_descrip "
                      "FROM <? literal('groups') ?> "
                      "ORDER BY groups_name;";
  params.append("groups", _elem->table);
  MetaSQLQuery listq(sql);
  populateList = listq.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving %1 Groups").arg(_elem->title),
                                  populateList, __FILE__, __LINE__))
    return;
  else
    _grp->populate(populateList);
}
