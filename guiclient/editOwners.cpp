/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "editOwners.h"

#include <QMessageBox>
#include <QSqlError>
#include <QValidator>
#include <QVariant>

#include <parameter.h>
#include <metasql.h>
#include "mqlutil.h"

#include "storedProcErrorLookup.h"
#include "errorReporter.h"

editOwners::editOwners(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : XDialog(parent, name, modal, fl)
{
  XSqlQuery editeditOwners;
  setupUi(this);

  connect(_close,	SIGNAL(clicked()),	this,	SLOT(sClose()));
  connect(_query,	SIGNAL(clicked()),	this,	SLOT(sFillList()));
  connect(_modify,  	SIGNAL(clicked()),	this,	SLOT(sModify()));
  connect(_modifyAll,   SIGNAL(clicked()),      this,   SLOT(sModifyAll()));
  connect(_list, 	SIGNAL(itemClicked(QTreeWidgetItem*, int)),	this, 	SLOT(sItemClicked()));

  _list->addColumn(tr("Type"),    _statusColumn,  Qt::AlignCenter, true, "type_name");
  _list->addColumn(tr("Name"),              100,  Qt::AlignLeft,   true, "name");
  _list->addColumn(tr("Description"),        -1,  Qt::AlignLeft,   true, "description");
  _list->addColumn(tr("Owner"),     _userColumn,  Qt::AlignLeft,   true, "owner_username");

  _modify->setEnabled(false);
  _modifyAll->setEnabled(false);

  _newOwner->setType(UsernameLineEdit::UsersActive);

  _first = true;

  editeditOwners.prepare("SELECT usr_id "
	    "FROM usr "
	    "WHERE (usr_username=getEffectiveXtUser());");
  editeditOwners.exec();
  if (editeditOwners.first())
  {
    _owner->setId(editeditOwners.value("usr_id").toInt());
  }  
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Owner Information"),
                                editeditOwners, __FILE__, __LINE__))
  {
    return;
  }
}

void editOwners::sFillList()
{
  XSqlQuery editFillList;
  ParameterList params;
  QString qry = "<? if exists('task') ?>"
                "SELECT task_id AS id, "
                "       task_owner_username AS owner_username, "
                "       task_name AS name, "
                "       task_descrip AS description, "
                "       <? value('task') ?> AS type_name, "
                "       'task' AS table "
                "FROM task "
                "WHERE task_owner_username = <? value('owner') ?> "
                "<? endif ?>"
                "<? if exists('project') ?>" 
                "  <? if exists('task') ?>"
                "  UNION ALL"
                "  <? endif ?>"  
                "SELECT prj_id AS id, "
                "       prj_owner_username AS owner_username, "
                "       prj_name AS name, "
                "       prj_descrip AS description, "
                "       <? value('project') ?> AS type_name, "
                "       'prj' AS table "
                "FROM prj "
                "WHERE prj_owner_username = <? value('owner') ?> "
                "<? endif ?>"
                "<? if exists('contact') ?>"
                "  <? if reExists('(task|project)') ?>"
                "  UNION ALL"
                "  <? endif ?>"  
                "SELECT cntct_id AS id, "
                "       cntct_owner_username AS owner_username, "
                "       cntct_first_name || ' ' || cntct_last_name AS name, "
                "       '' AS description, "
                "       <? value('contact') ?> AS type_name, "
                "       'cntct' AS table "
                "FROM cntct "
                "WHERE cntct_owner_username = <? value('owner') ?> "
                "<? endif ?>"
                "<? if exists('incident') ?>"
                "  <? if reExists('(task|project|contact)') ?>"
                "  UNION ALL"
                "  <? endif ?>"  
                "SELECT incdt_id AS id, "
                "       incdt_owner_username AS owner_username, "
                "       incdt_summary AS name, "
                "       incdt_descrip AS description, "
                "       <? value('incident') ?> AS type_name, "
                "       'incdt' AS table "
                "FROM incdt "
                "WHERE incdt_owner_username = <? value('owner') ?> "
                "<? endif ?>"
                "<? if exists('account') ?>"
                "  <? if reExists('(task|project|contact|incident)') ?>"
                "  UNION ALL"
                "  <? endif ?>"  
                "SELECT crmacct_id AS id, "
                "       crmacct_owner_username AS owner_username, "
                "       crmacct_name AS name, "
                "       crmacct_notes AS description, "
                "       <? value('account') ?> AS type_name, "
                "       'crmacct' AS table "
                "FROM crmacct "
                "WHERE crmacct_owner_username = <? value('owner') ?> "
                "<? endif ?>"
                "<? if exists('opportunity') ?>"
                "  <? if reExists('(task|project|contact|incident|account)') ?>"
                "  UNION ALL"
                "  <? endif ?>"  
                "SELECT ophead_id AS id, "
                "       ophead_owner_username AS owner_username, "
                "       ophead_name AS name, "
                "       ophead_notes AS description, "
                "       <? value('opportunity') ?> AS type_name, "
                "       'ophead' AS table "
                "FROM ophead "
                "WHERE ophead_owner_username = <? value('owner') ?>"
                "<? endif ?>;" ;
  if(_task->isChecked())
    params.append("task", tr("Task"));
  if(_project->isChecked())
    params.append("project", tr("Project"));
  if(_contact->isChecked())
    params.append("contact", tr("Contact"));
  if(_incident->isChecked())
    params.append("incident", tr("Incident"));
  if(_account->isChecked())
    params.append("account", tr("Account"));
  if(_opportunity->isChecked())
    params.append("opportunity", tr("Opportunity"));

  if(!_task->isChecked() && !_project->isChecked() && !_contact->isChecked() 
     && !_incident->isChecked() && !_account->isChecked() && !_opportunity->isChecked())
    _list->clear();
  else
  {
    MetaSQLQuery mql(qry);
    params.append("owner", _owner->username());
    editFillList = mql.toQuery(params);
    _list->populate(editFillList);
  }

  _modifyAll->setEnabled(_list->topLevelItemCount() > 0);
  _modify->setEnabled(false);
}

void editOwners::sClose()
{
  close();
}

void editOwners::sModify()
{
  QString table;

  if(!_newOwner->isValid())
  {
    QMessageBox::critical( this, tr("No New Owner"),
      tr("A new owner must be selected before you can continue."));
    _newOwner->setFocus();
    return;
  }
  int ret = QMessageBox::warning(this, tr("Confirm Ownership Modification"),
                   tr("<p>Are you sure that you want to change the new owner "
                      "to '%1' for the selected records?")
                      .arg(_newOwner->username()),
                   QMessageBox::Yes | QMessageBox::No,
                   QMessageBox::Yes);

  if(ret == QMessageBox::Yes)
  {
  if (modifyOne(_list->currentItem()))
    sFillList();
  }
}

void editOwners::sModifyAll()
{
  if(!_newOwner->isValid())
  {
    QMessageBox::critical( this, tr("No New Owner"),
      tr("A new owner must be selected before you can continue."));
    _newOwner->setFocus();
    return;
  }

  int ret = QMessageBox::warning(this, tr("Confirm Ownership Modification"),
                   tr("<p>Are you sure that you want to change the new owner "
                      "to '%1' for the selected records?")
                      .arg(_newOwner->username()),
                   QMessageBox::Yes | QMessageBox::No,
                   QMessageBox::Yes);
  
  if(ret == QMessageBox::Yes)
  {

    QList<XTreeWidgetItem*> all = _list->findItems("", Qt::MatchContains);

    for (int i = 0; i < all.size(); i++)
    {
      XTreeWidgetItem *currentItem = static_cast<XTreeWidgetItem*>(all[i]);
      if (currentItem->rawValue("type_name").toString() != "")
        modifyOne(currentItem);
    }
    sFillList();
  }
}

bool editOwners::modifyOne(XTreeWidgetItem * currentItem)
{
  XSqlQuery editmodifyOne;
  QString table;

  if(currentItem->rawValue("type_name").toString() == "Task") table = "task";
  if(currentItem->rawValue("type_name").toString() == "Project") table = "prj";
  if(currentItem->rawValue("type_name").toString() == "Contact") table = "cntct";
  if(currentItem->rawValue("type_name").toString() == "Incident") table = "incdt";
  if(currentItem->rawValue("type_name").toString() == "Account") table = "crmacct";
  if(currentItem->rawValue("type_name").toString() == "Opportunity") table = "ophead";

  editmodifyOne.prepare("UPDATE "+table+" "
            "SET "+table+"_owner_username = :new_owner_username "
            "WHERE "+table+"_id = :id ");
  editmodifyOne.bindValue(":new_owner_username", _newOwner->username());
  editmodifyOne.bindValue(":id", currentItem->id());
  editmodifyOne.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Changing Owner Information"),
                                editmodifyOne, __FILE__, __LINE__))
  {
    return false;
  }
  return true;
}

void editOwners::sItemClicked()
{
  _modify->setEnabled(true);
}

