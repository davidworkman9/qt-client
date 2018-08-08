/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "crmGroup.h"

#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include "addresscluster.h"
#include "crmacctcluster.h"
#include "contactcluster.h"
#include "errorReporter.h"
#include "guiErrorCheck.h"
#include "mqlutil.h"

QMap<int,  struct GroupMap*> crmGroup::_grpMap;

crmGroup::crmGroup(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : XDialog(parent, name, modal, fl)
{
  setupUi(this);

  if (_grpMap.isEmpty())
    _grpMap = crmGroups::_grpMap;

  connect(_new, SIGNAL(clicked()), this, SLOT(sNew()));
  connect(_delete, SIGNAL(clicked()), this, SLOT(sDelete()));
  connect(_save, SIGNAL(clicked()), this, SLOT(sSaveClicked()));
  connect(_close, SIGNAL(clicked()), this, SLOT(sClose()));
  connect(_name, SIGNAL(editingFinished()), this, SLOT(sCheck()));

  _grpitem->addColumn(tr("Number"), _itemColumn, Qt::AlignLeft, true, "grp_number");
  _grpitem->addColumn(tr("Name"),   -1,          Qt::AlignLeft, true, "grp_name");

  _grpid = -1;
}

crmGroup::~crmGroup()
{
  // no need to delete child widgets, Qt does it all for us
}

void crmGroup::languageChange()
{
  retranslateUi(this);
}

enum SetResponse crmGroup::set(const ParameterList &pParams)
{
  XDialog::set(pParams);
  QVariant param;
  bool     valid;

  param = pParams.value("groupType", &valid);
  if (valid)
  {
    _elem = _grpMap.value(param.toInt());
    setWindowTitle(tr("%1 Group").arg(_elem->title));
    _memberLit->setText(tr("%1:").arg(_elem->title + "s"));
  }

  param = pParams.value("groups_id", &valid);
  if (valid)
  {
    _grpid = param.toInt();
    populate();
  }

  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "new")
    {
      _mode = cNew;
      _saved = false;

      connect(_grpitem, SIGNAL(valid(bool)), _delete, SLOT(setEnabled(bool)));
    }
    else if (param.toString() == "edit")
    {
      _mode = cEdit;

      sFillList();

      connect(_grpitem, SIGNAL(valid(bool)), _delete, SLOT(setEnabled(bool)));
    }
    else if (param.toString() == "view")
    {
      _mode = cView;

      _name->setEnabled(false);
      _descrip->setEnabled(false);
      _new->setEnabled(false);
      _close->setText(tr("&Close"));
      _save->hide();

      sFillList();
    }
  }

  return NoError;
}

void crmGroup::sCheck()
{
  XSqlQuery crmCheck;
  ParameterList params;
  _name->setText(_name->text().trimmed());
  if ((_mode == cNew) && (_name->text().length()))
  {
    QString sql = "SELECT groups_id "
                  "FROM <? literal('groups') ?> "
                  "WHERE (UPPER(groups_name)=UPPER(<? value('groups_name') ?>));";
    params.append("groups",     _elem->table);
    params.append("groups_name", _name->text());
    MetaSQLQuery chkq(sql);
    crmCheck = chkq.toQuery(params);
    if (crmCheck.first())
    {
      _grpid = crmCheck.value("groups_id").toInt();
      _mode = cEdit;
      populate();

      _name->setEnabled(false);
    }
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving %1 Group Information").arg(_elem->title),
                                  crmCheck, __FILE__, __LINE__))
    {
      return;
    }
  }
  _save->setEnabled(_name->text().length());
  _new->setEnabled(_name->text().length());
}

void crmGroup::sClose()
{
  XSqlQuery crmClose;
  ParameterList params;

  if (_mode == cNew)
  {
    QString sql = "DELETE FROM <? literal('groups') ?> "
                  "WHERE (groups_id=<? value('groups_id') ?>);";
    params.append("groups",     _elem->table);
    params.append("groups_id",  _grpid);
    MetaSQLQuery delq(sql);
    crmClose = delq.toQuery(params);
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error rejecting %1 Group").arg(_elem->title),
                                  crmClose, __FILE__, __LINE__))
      reject();
  }

  reject();
}

void crmGroup::sSaveClicked()
{
  sSave(false);
}

void crmGroup::sSave(bool pPartial)
{
  XSqlQuery crmet;
  XSqlQuery crmSave;
  ParameterList params;
  QString sql;

  QList<GuiErrorCheck>errors;
  errors<<GuiErrorCheck(_name->text().trimmed().isEmpty(), _name,
                        tr("You cannot have an empty name."));

  if(GuiErrorCheck::reportErrors(this,tr("Cannot Save %1 Group").arg(_elem->title),errors))
      return;

  if (_grpid < 1)
  {
    crmet.exec("SELECT NEXTVAL('groups_groups_id_seq') AS groups_id;");
    if (crmet.first())
      _grpid = crmet.value("groups_id").toInt();
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Group Information"),
                                  crmet, __FILE__, __LINE__))
      return;
  }

  sql = "SELECT groups_id "
        "FROM <? literal('groups') ?> "
        " WHERE((groups_name=<? value('groups_name') ?>)"
        "   AND (groups_id != <? value('groups_id') ?>))" ;
  params.append("groups",     _elem->table);
  params.append("groups_id",  _grpid);
  params.append("groups_name", _name->text());
  MetaSQLQuery dupchk(sql);
  crmSave = dupchk.toQuery(params);
  if(crmSave.first())
  {
    QMessageBox::warning(this, tr("Cannot Save %1 Group").arg(_elem->title),
      tr("You cannot have a duplicate group name."));
    _name->setFocus();
    return;
  }

  if (_mode == cNew && !_saved)
    sql = "INSERT INTO <? literal('groups') ?> (groups_id, groups_name, groups_descrip) "
          "VALUES "
          "(<? value('groups_id') ?>, <? value('groups_name') ?>, <? value('groups_descrip') ?>);" ;
  else if (_mode == cEdit)
    sql = "UPDATE <? literal('groups') ?> "
          "SET groups_name=<? value('groups_name') ?>, "
          "    groups_descrip=<? value('groups_descrip') ?> "
          "WHERE (groups_id=<? value('groups_id') ?>);" ;

  params.append("groups",     _elem->table);
  params.append("groups_id",  _grpid);
  params.append("groups_name", _name->text());
  params.append("groups_descrip", _descrip->text().trimmed());
  MetaSQLQuery savegrp(sql);
  crmSave = savegrp.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Saving %1 Group").arg(_elem->title),
                                crmSave, __FILE__, __LINE__))
  {
    return;
  }

  _saved = true;

  if (!pPartial)
    done(_grpid);
}

void crmGroup::sDelete()
{
  XSqlQuery crmDelete;
  ParameterList params;
  QString sql = "DELETE FROM <? literal('groupsitem') ?> "
             "WHERE (groupsitem_id=<? value('grpitem_id') ?> );" ;
  params.append("groupsitem", _elem->itemTable);
  params.append("grpitem_id", _grpitem->id());
  MetaSQLQuery delq(sql);
  crmDelete = delq.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting %1 From Group").arg(_elem->title),
                                crmDelete, __FILE__, __LINE__))
  {
    return;
  }

  sFillList();
}

void crmGroup::sNew()
{
  XSqlQuery crmNew;
  ParameterList params;
  QString sql;
  int refid = 0;

  if (!_saved)
    sSave(true);

  QString uiName = _elem->search;
  if (uiName == "CRMAcctSearch")
  {
    CRMAcctSearch *newdlg  = new CRMAcctSearch(this);
    newdlg->setSubtype(_elem->subtype);
    refid = newdlg->exec();
  }
  else if  (uiName == "ContactSearch")
  {
    ContactSearch *newdlg2 = new ContactSearch(this);
    refid = newdlg2->exec();
  }
  else if  (uiName == "AddressSearch")
  {
    AddressSearch *newdlg3 = new AddressSearch(this);
    refid = newdlg3->exec();
  }

  if (refid < 1)
    return;

  sql = "SELECT groupsitem_id "
        "FROM <? literal('groupsitem') ?> "
        "WHERE ( (groupsitem_groups_id=<? value('groupsitem_groups_id') ?>)"
        " AND (groupsitem_reference_id=<? value('groupsitem_reference_id') ?>) );" ;
  params.append("groupsitem", _elem->itemTable);
  params.append("groupsitem_groups_id", _grpid);
  params.append("groupsitem_reference_id", refid);
  MetaSQLQuery newq(sql);
  crmNew = newq.toQuery(params);
  if (crmNew.first())
    return;
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Adding %1 To Group").arg(_elem->title),
                                crmNew, __FILE__, __LINE__))
    return;

  sql = "INSERT INTO <? literal('groupsitem') ?> (groupsitem_groups_id, groupsitem_reference_id) "
        "VALUES "
        "(<? value('groupsitem_groups_id') ?>, <? value('groupsitem_reference_id') ?>);" ;
  MetaSQLQuery insq(sql);
  crmNew = insq.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Adding %1 To Group").arg(_elem->title),
                                crmNew, __FILE__, __LINE__))
    return;

  sFillList();

}

void crmGroup::sFillList()
{
  XSqlQuery crmSource;
  XSqlQuery crmFillList;
  ParameterList params;
  QString sql;

  sql = "SELECT source_table, source_key_field, source_number_field, source_name_field, source_joins "
        "FROM source "
        "WHERE source_name = <? value('source_name') ?>;" ;
  params.append("source_name", _elem->source);
  MetaSQLQuery srcq(sql);
  crmSource = srcq.toQuery(params);
  if (crmSource.first())
  {
    QString sql =  "SELECT groupsitem_id, <? literal('tbl_number') ?> AS grp_number, "
                   "                      <? literal('tbl_name') ?> AS grp_name "
                   "FROM <? literal('groupsitem') ?>, <? literal('srctable') ?> "
                   " <? literal('joins') ?> "
                   "WHERE ( (groupsitem_reference_id=<? literal('tbl_id') ?>) "
                   " AND (groupsitem_groups_id=<? value('groups_id') ?>) ) "
                   "ORDER BY <? literal('tbl_number') ?>;" ;
    params.append("groups_id", _grpid);
    params.append("tbl_number", crmSource.value("source_number_field").toString());
    params.append("tbl_name", crmSource.value("source_name_field").toString());
    params.append("tbl_id", crmSource.value("source_key_field").toString());
    params.append("srctable", crmSource.value("source_table").toString());
    if (!crmSource.value("source_table").toString().isEmpty())
      params.append("joins", crmSource.value("source_joins").toString());
    params.append("groupsitem", _elem->itemTable);
    MetaSQLQuery listq(sql);
    crmFillList = listq.toQuery(params);
    _grpitem->populate(crmFillList);
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving %1 Group Information").arg(_elem->title),
                                  crmFillList, __FILE__, __LINE__))
      return;
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Group Source Information"),
                                  crmSource, __FILE__, __LINE__))
    return;
}

void crmGroup::populate()
{
  XSqlQuery crmpopulate;
  ParameterList params;

  QString sql = "SELECT groups_name, groups_descrip "
                "FROM <? literal('groups') ?> "
                "WHERE (groups_id=<? value('groups_id') ?>);" ;
  params.append("groups",    _elem->table);
  params.append("groups_id", _grpid);
  MetaSQLQuery popq(sql);
  crmpopulate = popq.toQuery(params);
  if (crmpopulate.first())
  {
    _name->setText(crmpopulate.value("groups_name").toString());
    _descrip->setText(crmpopulate.value("groups_descrip").toString());
    _save->setEnabled(true);
    _new->setEnabled(true);
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving %1 Group Information").arg(_elem->title),
                                crmpopulate, __FILE__, __LINE__))
    return;
}
