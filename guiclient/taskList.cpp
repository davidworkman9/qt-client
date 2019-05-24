/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "taskList.h"
#include "xdialog.h"
#include "incident.h"
#include "customer.h"
#include "crmaccount.h"
#include "opportunity.h"
#include "project.h"
#include "prospect.h"
#include "storedProcErrorLookup.h"
#include "task.h"
#include "parameterwidget.h"

#include <QMessageBox>
#include <QSqlError>
#include <QToolBar>
#include "errorReporter.h"

static const int TASK        = 1;
static const int INCIDENT    = 2;
static const int PTASK       = 3;
static const int PROJECT     = 4;
static const int OPPORTUNITY = 5;
static const int ACCOUNT     = 6;

taskList::taskList(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "taskList", fl)
{
  _shown = false;
  _run = false;

  setupUi(optionsWidget());
  setWindowTitle(tr("Task List"));
  setReportName("TaskList");
  setMetaSQLOptions("tasklist", "detail");
  setUseAltId(true);
  setParameterWidgetVisible(true);
  setNewVisible(true);
  setQueryOnStartEnabled(true);

  parameterWidget()->append(tr("User"), "username", ParameterWidget::User, omfgThis->username());
  parameterWidget()->append(tr("Owner"), "owner_username", ParameterWidget::User);
  parameterWidget()->append(tr("Assigned To"), "assigned_username", ParameterWidget::User);
  parameterWidget()->append(tr("Account"), "crmacct_id", ParameterWidget::Crmacct);
  parameterWidget()->appendComboBox(tr("Account Group"), "crmacctgrp", XComboBox::AccountGroups);
  parameterWidget()->append(tr("Incident"), "incdt_id", ParameterWidget::Incident);
  parameterWidget()->append(tr("Opportunity"), "ophead_id", ParameterWidget::Opportunity);
  parameterWidget()->append(tr("Prospect"), "prospect_id", ParameterWidget::Prospect);
  parameterWidget()->append(tr("Project"), "prj_id", ParameterWidget::Project);
  parameterWidget()->append(tr("Start Date on or Before"), "startStartDate", ParameterWidget::Date);
  parameterWidget()->append(tr("Start Date on or After"), "startEndDate", ParameterWidget::Date);
  parameterWidget()->append(tr("Due Date on or Before"), "dueStartDate", ParameterWidget::Date);
  parameterWidget()->append(tr("Due Date on or After"), "dueEndDate", ParameterWidget::Date);
  parameterWidget()->append(tr("Show Completed Only"), "completedonly", ParameterWidget::Exists);

  connect(_opportunities, SIGNAL(toggled(bool)), this,            SLOT(sFillList()));
  connect(_tasks,         SIGNAL(toggled(bool)), this,            SLOT(sFillList()));
  connect(_incidents,     SIGNAL(toggled(bool)), this,            SLOT(sFillList()));
  connect(_projects,      SIGNAL(toggled(bool)), this,            SLOT(sFillList()));
  connect(_showCompleted, SIGNAL(toggled(bool)), this,	          SLOT(sFillList()));
  connect(list(),         SIGNAL(itemSelected(int)), this,        SLOT(sOpen()));
  connect(omfgThis,       SIGNAL(emitSignal(QString, int)), this, SLOT(sUpdate(QString, int)));

  list()->addColumn(tr("Type"),               75,  Qt::AlignCenter, true, "type");
  list()->addColumn(tr("Project"),           100,  Qt::AlignLeft,   false,  "prj_number");
  list()->addColumn(tr("Priority"),    _userColumn,  Qt::AlignLeft,   true, "priority");
  list()->addColumn(tr("Owner"),       _userColumn,  Qt::AlignLeft,   false,"owner");
  list()->addColumn(tr("Assigned To"), _userColumn,  Qt::AlignLeft,   true, "assigned");
  list()->addColumn(tr("Number"),            100,  Qt::AlignLeft,   true, "number");
  list()->addColumn(tr("Name"),              100,  Qt::AlignLeft,   true, "name");
  list()->addColumn(tr("Notes"),              -1,  Qt::AlignLeft,   true, "notes");
  list()->addColumn(tr("Stage"),   _statusColumn,  Qt::AlignLeft,   true, "stage");
  list()->addColumn(tr("Start Date"),_dateColumn,  Qt::AlignLeft,   false, "start");
  list()->addColumn(tr("Due Date"),  _dateColumn,  Qt::AlignLeft,   true,  "due");
  list()->addColumn(tr("Account#"), _orderColumn,  Qt::AlignLeft,   false, "crmacct_number");
  list()->addColumn(tr("Account Name"),      100,  Qt::AlignLeft,   true,  "crmacct_name");
  list()->addColumn(tr("Parent"),            100,  Qt::AlignLeft,   false, "parent");
  list()->addColumn(tr("Customer"),    _ynColumn,  Qt::AlignLeft,   false, "cust");

  list()->setSelectionMode(QAbstractItemView::ExtendedSelection);

  QToolButton * newBtn = (QToolButton*)toolBar()->widgetForAction(newAction());
  newBtn->setPopupMode(QToolButton::MenuButtonPopup);
  QAction *menuItem;
  QMenu * taskMenu = new QMenu;
  menuItem = taskMenu->addAction(tr("Task"),   this, SLOT(sNew()));
  if(task::userHasPriv(cNew))
    menuItem->setShortcut(QKeySequence::New);
  menuItem->setEnabled(task::userHasPriv(cNew));
  menuItem = taskMenu->addAction(tr("Opportunity"), this, SLOT(sNewOpportunity()));
  menuItem->setEnabled(opportunity::userHasPriv(cNew));
  menuItem = taskMenu->addAction(tr("Incident"), this, SLOT(sNewIncdt()));
  menuItem->setEnabled(incident::userHasPriv(cNew));
  menuItem = taskMenu->addAction(tr("Project"), this, SLOT(sNewProject()));
  menuItem->setEnabled(project::userHasPriv(cNew));
  newBtn->setMenu(taskMenu);
}

void taskList::showEvent(QShowEvent * event)
{
  display::showEvent(event);

  if(!_shown)
  {
    _shown = true;
    if(_run)
      sFillList();
  }
}

void taskList::sPopulateMenu(QMenu *pMenu, QTreeWidgetItem *, int)
{
  bool edit = false;
  bool view = false;
  bool del = false;
  bool foundDeletable = false;
  bool foundParent = false;
  bool editParent = false;
  bool viewParent = false;
  bool foundCustomer = false;

  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    edit = edit || getPriv(cEdit, item->altId(), item);
    view = view || getPriv(cView, item->altId(), item);

    if (item->altId() == TASK || item->altId() == PTASK || item->altId() == PROJECT)
    {
      foundDeletable = true;
      del = del || getPriv(cEdit, item->altId(), item);
    }

    if (getParentType(item))
    {
      foundParent = (item->rawValue("parent") != _parent);

      editParent = editParent || getPriv(cEdit, getParentType(item), item);
      viewParent = viewParent || getPriv(cView, getParentType(item), item);
    }

    if (item->rawValue("cust").toInt() > 0)
      foundCustomer = true;
  }

  QAction *menuItem;

  if (list()->selectedItems().size() > 0)
  {
    menuItem = pMenu->addAction(tr("Edit"), this, SLOT(sEdit()));
    menuItem->setEnabled(edit);

    menuItem = pMenu->addAction(tr("View"), this, SLOT(sView()));
    menuItem->setEnabled(view);
  }

  if (foundDeletable)
  {
    menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sDelete()));
    menuItem->setEnabled(del);
  }

  if (foundParent)
  {
    pMenu->addSeparator();

    menuItem = pMenu->addAction(tr("Edit Parent"), this, SLOT(sEditParent()));
    menuItem->setEnabled(editParent);

    menuItem = pMenu->addAction(tr("View Parent"), this, SLOT(sViewParent()));
    menuItem->setEnabled(viewParent);
  }

  if (foundCustomer)
  {
    pMenu->addSeparator();

    menuItem = pMenu->addAction(tr("Edit Customer"), this, SLOT(sEditCustomer()));
    menuItem->setEnabled(getPriv(cEdit));

    menuItem = pMenu->addAction(tr("View Customer"), this, SLOT(sViewCustomer()));
    menuItem->setEnabled(getPriv(cView));
  }
}

enum SetResponse taskList::set(const ParameterList& pParams)
{
  XWidget::set(pParams);
  QVariant param;
  bool	   valid;

  param = pParams.value("run", &valid);
  if (valid)
    sFillList();

  return NoError;
}

void taskList::sNew()
{
  //Need an extra priv check because of display trigger
  if (!task::userHasPriv(cNew))
    return;

  ParameterList params;
  QVariant param;
  bool valid;
  bool linkedid = false;
  parameterWidget()->appendValue(params);
  params.append("mode", "new");

  param = params.value("crmacct_id", &valid);
  if (valid)
  {
    params.append("parent", "CRMA");
    params.append("parent_id", param);
    linkedid = true;
  }

  param = params.value("prospect_id", &valid);
  if (valid)
  {
    params.append("parent", "PSPCT");
    params.append("parent_id", param);
    linkedid = true;
  }

  param = params.value("incdt_id", &valid);  
  if (valid)
  {
    params.append("parent", "INCDT");
    params.append("parent_id", param);
    linkedid = true;
  }

  param = params.value("ophead_id", &valid);  
  if (valid)
  {
    params.append("parent", "OPP");
    params.append("parent_id", param);
    linkedid = true;
  }

  if (!linkedid)
    params.append("parent", "TASK");

  param = params.value("prj_id", &valid);  
  if (valid)
    params.append("prj_id", param);

  params.append("parent_assigned_username", omfgThis->username());

  task newdlg(0, "", true);
  newdlg.set(params);
  newdlg.setWindowModality(Qt::WindowModal);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void taskList::sNewIncdt()
{
  ParameterList params;
  parameterWidget()->appendValue(params);
  params.append("mode", "new");

  incident newdlg(0, "", true);
  newdlg.set(params);
  newdlg.setWindowModality(Qt::WindowModal);

  if (newdlg.exec() > 0)
    sFillList();
}

void taskList::sEdit()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    bool edit = getPriv(cEdit, item->altId(), item);
    bool view = getPriv(cView, item->altId(), item);

    if (!edit && !view)
      continue;

    if (item->altId() == TASK || item->altId() == PTASK)
    {
      if (edit)
        sEditTask(item->id());
      else
        sViewTask(item->id());
    }
    else if (item->altId() == INCIDENT)
    {
      if (edit)
        sEditIncident(item->id());
      else
        sViewIncident(item->id());
    }
    else if (item->altId() == PROJECT)
    {
      if (edit)
        sEditProject(item->id());
      else
        sViewProject(item->id());
    }
    else if (item->altId() == OPPORTUNITY)
    {
      if (edit)
        sEditOpportunity(item->id());
      else
        sViewOpportunity(item->id());
    }
  }
}

void taskList::sView()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    if(!getPriv(cView, item->altId(), item))
      continue;

    if (item->altId() == TASK || item->altId() == PTASK)
      sViewTask(item->id());
    else if (item->altId() == INCIDENT)
      sViewIncident(item->id());
    else if (item->altId() == PROJECT)
      sViewProject(item->id());
    else if (item->altId() == OPPORTUNITY)
      sViewOpportunity(item->id());
  }
}

void taskList::sEditParent()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    bool edit = getPriv(cEdit, getParentType(item), item);
    bool view = getPriv(cView, getParentType(item), item);

    if (!edit && !view)
      continue;

    if (getParentType(item) == INCIDENT)
    {
      if (edit)
        sEditIncident(item->id("parent"));
      else
        sViewIncident(item->id("parent"));
    }
    if (getParentType(item) == PROJECT)
    {
      if (edit)
        sEditProject(item->id("parent"));
      else
        sViewProject(item->id("parent"));
    }
    if (getParentType(item) == OPPORTUNITY)
    {
      if (edit)
        sEditOpportunity(item->id("parent"));
      else
        sViewOpportunity(item->id("parent"));
    }
    if (getParentType(item) == ACCOUNT)
    {
      if (edit)
        sEditAccount(item->id("parent"));
      else
        sViewAccount(item->id("parent"));
    }
  }
}

void taskList::sViewParent()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    if(!getPriv(cView, getParentType(item), item))
      continue;

    if (getParentType(item) == INCIDENT)
      sViewIncident(item->id("parent"));
    if (getParentType(item) == PROJECT)
      sViewProject(item->id("parent"));
    if (getParentType(item) == OPPORTUNITY)
      sViewOpportunity(item->id("parent"));
    if (getParentType(item) == ACCOUNT)
      sViewAccount(item->id("parent"));
  }
}

void taskList::sDelete()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    if(!getPriv(cEdit, item->altId(), item))
      continue;

    XSqlQuery taskDelete;
    QString recurstr;
    QString recurtype;
    if (item->altId() == TASK || item->altId() == PTASK)
    {
      recurstr = "SELECT MAX(task_due_date) AS max"
                 "  FROM task"
                 " WHERE task_recurring_task_id=:id"
                 "   AND task_id!=:id;" ;
      recurtype = "TASK";
    }

    bool deleteAll  = false;
    bool deleteOne  = false;
    if (! recurstr.isEmpty())
    {
      XSqlQuery recurq;
      recurq.prepare(recurstr);
      recurq.bindValue(":id", item->id());
      recurq.exec();
      if (recurq.first() && !recurq.value("max").isNull())
      {
        QMessageBox askdelete(QMessageBox::Question, tr("Delete Recurring Item?"),
                              tr("<p>This is a recurring item. Do you want to "
                                 "delete just this one item or delete all open "
                                 "items in this recurrence?"),
                              QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::Cancel,
                              this);
        askdelete.setDefaultButton(QMessageBox::Cancel);
        int ret = askdelete.exec();
        if (ret == QMessageBox::Cancel)
          return;
        else if (ret == QMessageBox::YesToAll)
          deleteAll = true;
        // user said delete one but the only one that exists is the parent ToDo
        else if (ret == QMessageBox::Yes)
          deleteOne = true;
      }
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Task Information"),
                                    recurq, __FILE__, __LINE__))
      {
        return;
      }
      else if (QMessageBox::warning(this, tr("Delete List Item?"),
                                    tr("<p>Are you sure that you want to "
                                       "completely delete the selected item?"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No) == QMessageBox::No)
        return;
    }
    else if (QMessageBox::warning(this, tr("Delete List Item?"),
                                  tr("<p>Are you sure that you want to "
                                     "completely delete the selected item?"),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No) == QMessageBox::No)
      return;

    int procresult = 0;
    if (deleteAll)  // Delete all tasks in the recurring series
    {
      taskDelete.prepare("SELECT deleteOpenRecurringItems(:id, :type, NULL, true)"
                "       AS result;");
      taskDelete.bindValue(":id",   item->id());
      taskDelete.bindValue(":type", recurtype);
      taskDelete.exec();
      if (taskDelete.first())
        procresult = taskDelete.value("result").toInt();

      if (procresult < 0)
      {
        ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Recurring Task Information"),
                             storedProcErrorLookup("deleteOpenRecurringItems", procresult),
                             __FILE__, __LINE__);
        return;
      }
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Recurring Task Information"),
                                    taskDelete, __FILE__, __LINE__))
      {
        return;
      }
    }

    if (deleteOne) // The base task in a recurring series has been seleted.  Have to move
                   // recurrence to the next item else we hit foreign key errors.
                   // Make the next item on the list the parent in the series
    {
      taskDelete.prepare("UPDATE task SET task_recurring_task_id =("
                          "               SELECT MIN(task_id) FROM task"
                          "                 WHERE task_recurring_task_id=:id"
                          "                   AND task_id!=:id)"
                          "  WHERE task_recurring_task_id=:id"
                          "  AND task_id!=:id;");
      taskDelete.bindValue(":id",   item->id());
      taskDelete.exec();
      if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Recurring Task Information"),
                               taskDelete, __FILE__, __LINE__))
      {
        return;
      }
    }

    if (item->altId() == TASK || item->altId() == PTASK)
    {
      taskDelete.prepare("SELECT deleteTask(:task_id) AS result;");
      taskDelete.bindValue(":task_id", item->id());
      taskDelete.exec();
      if (taskDelete.first() && taskDelete.value("result").toInt() == -1)
      {
        if (QMessageBox::question(this, tr("Sub-Tasks"),
                       tr("<p>Sub-tasks exist for this Task.\n"
                          "Do you also want to delete sub-tasks?"),
                 QMessageBox::Yes, QMessageBox::No | QMessageBox::Default) == QMessageBox::No)
        {
          return;
        }
        else
        {
          taskDelete.prepare("SELECT deleteTask(:task_id, true) AS result;");
          taskDelete.bindValue(":task_id", item->id());
          taskDelete.exec();
          if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Task"),
                                taskDelete, __FILE__, __LINE__))
            return;
        }
      }
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Task"),
                                taskDelete, __FILE__, __LINE__))
        return;
    }
    else if (item->altId() == PROJECT)
    {
      taskDelete.prepare("SELECT deleteProject(:project_id) AS result");
      taskDelete.bindValue(":project_id", item->id());
      taskDelete.exec();
      if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Project"),
                                taskDelete, __FILE__, __LINE__))
         return;
    }
  }
  sFillList();
}

bool taskList::setParams(ParameterList &params)
{
  if (!_tasks->isChecked() &&
      !_opportunities->isChecked() &&
      !_incidents->isChecked() &&
      !_projects->isChecked())
  {
    list()->clear();
    return false;
  }

  if (_tasks->isChecked())
    params.append("tasks");
  if (_opportunities->isChecked())
    params.append("opportunities");
  if (_incidents->isChecked())
    params.append("incidents");
  if (_projects->isChecked())
    params.append("projects");
  if (_showCompleted->isChecked())
    params.append("completed");
  if (_parent.length() > 0)
    params.append("parent", _parent);

  params.append("incident", tr("Incident"));
  params.append("account", tr("Account"));
  params.append("task", tr("Task"));
  params.append("prjtask", tr("Project Task"));
  params.append("project", tr("Project"));
  params.append("opportunity", tr("Opportunity"));
  params.append("complete", tr("Completed"));
  params.append("deferred", tr("Deferred"));
  params.append("pending", tr("Pending"));
  params.append("inprocess", tr("InProcess"));
  params.append("feedback", tr("Feedback"));
  params.append("confirmed", tr("Confirmed"));
  params.append("assigned", tr("Assigned"));
  params.append("resolved", tr("Resolved"));
  params.append("closed", tr("Closed"));
  params.append("concept", tr("Concept"));
  params.append("new", tr("New"));

  if (!display::setParams(params))
    return false;

  return true;
}

bool taskList::getPriv(const int pMode, const int pType, XTreeWidgetItem* item)
{
  if (!item)
    item = list()->currentItem();

  int id = (item->altId() == pType) ? item->id() : item->id("parent");

  if (pType == TASK)
    return task::userHasPriv(pMode, "TD", id);
  else if (pType == INCIDENT)
    return incident::userHasPriv(pMode, id);
  else if (pType == PTASK)
    return task::userHasPriv(pMode, "J", id);
  else if (pType == PROJECT)
    return project::userHasPriv(pMode, id);
  else if (pType == OPPORTUNITY)
    return opportunity::userHasPriv(pMode, id);
  else if (pType == ACCOUNT)
    return crmaccount::userHasPriv(pMode, id);
  else
    return customer::userHasPriv(pMode);
}

int taskList::getParentType(XTreeWidgetItem* item)
{
  if (item->altId() == TASK && item->rawValue("parent") == "INCDT")
    return INCIDENT;
  if (item->altId() == TASK && item->rawValue("parent") == "OPP")
    return OPPORTUNITY;
  if (item->altId() == TASK && item->rawValue("parent") == "CRMA")
    return ACCOUNT;
  if (item->altId() == PTASK)
    return PROJECT;

  return 0;
}

void taskList::sEditIncident(int id)
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("incdt_id", id);

  incident* newdlg = new incident(this, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sViewIncident(int id)
{
  ParameterList params;
  params.append("mode", "view");
  params.append("incdt_id", id);

  incident* newdlg = new incident(this, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sNewProject()
{
  ParameterList params;
  parameterWidget()->appendValue(params);
  params.append("mode", "new");

  project newdlg(0, "", true);
  newdlg.set(params);
  newdlg.setWindowModality(Qt::WindowModal);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void taskList::sEditProject(int id)
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("prj_id", id);

  project* newdlg = new project(this, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sViewProject(int id)
{
  ParameterList params;
  params.append("mode", "view");
  params.append("prj_id", id);

  project* newdlg = new project(this, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sEditTask(int id)
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("task_id", id);

  task* newdlg = new task(this, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sViewTask(int id)
{
  ParameterList params;
  params.append("mode", "view");
  params.append("task_id", id);

  task* newdlg = new task(this, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sEditCustomer()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    if (item->rawValue("cust").toInt() > 0)
    {
      ParameterList params;
      params.append("cust_id", item->rawValue("cust").toInt());
      params.append("mode","edit");

      customer *newdlg = new customer(this);
      newdlg->set(params);
      omfgThis->handleNewWindow(newdlg);
    }
  }
}

void taskList::sViewCustomer()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    if (item->rawValue("cust").toInt() > 0)
    {
      ParameterList params;
      params.append("cust_id", item->rawValue("cust").toInt());
      params.append("mode","view");

      customer *newdlg = new customer(this);
      newdlg->set(params);
      omfgThis->handleNewWindow(newdlg);
    }
  }
}

void taskList::sNewOpportunity()
{
  ParameterList params;
  parameterWidget()->appendValue(params);
  params.append("mode", "new");

  opportunity newdlg(0, "", true);
  newdlg.set(params);
  newdlg.setWindowModality(Qt::WindowModal);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void taskList::sEditOpportunity(int id)
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("ophead_id", id);

  opportunity* newdlg = new opportunity(this, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sViewOpportunity(int id)
{
  ParameterList params;
  params.append("mode", "view");
  params.append("ophead_id", id);

  opportunity* newdlg = new opportunity(this, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sEditAccount(int id)
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("crmacct_id", id);

  crmaccount* newdlg = new crmaccount();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sViewAccount(int id)
{
  ParameterList params;
  params.append("mode", "view");
  params.append("crmacct_id", id);

  crmaccount* newdlg = new crmaccount();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void taskList::sOpen()
{
  bool editPriv = false;
  bool viewPriv = false;

    switch (list()->altId())
    {
    case 1:
      editPriv = task::userHasPriv(cEdit, "TD", list()->currentItem()->id());
      viewPriv = task::userHasPriv(cView, "TD", list()->currentItem()->id());
      break;
    case 2:
      editPriv = incident::userHasPriv(cEdit, list()->currentItem()->id());
      viewPriv = incident::userHasPriv(cView, list()->currentItem()->id());
      break;
    case 3:
      editPriv = task::userHasPriv(cEdit, "J", list()->currentItem()->id());
      viewPriv = task::userHasPriv(cView, "J", list()->currentItem()->id());
      break;
    case 4:
      editPriv = project::userHasPriv(cEdit, list()->currentItem()->id());
      viewPriv = project::userHasPriv(cView, list()->currentItem()->id());
      break;
    case 5:
      editPriv = opportunity::userHasPriv(cEdit, list()->currentItem()->id());
      viewPriv = opportunity::userHasPriv(cView, list()->currentItem()->id());
      break;
    default:
      break;
    }

  if(editPriv)
    sEdit();
  else if(viewPriv)
    sView();
  else
    QMessageBox::information(this, tr("Restricted Access"), tr("You have not been granted privileges to open this item."));
}

void taskList::setParent(QString parent)
{
  /* Used to define parent widget type for embedded taskList screen
     to prevent recursive opening of screens */
  _parent = parent;
}

void taskList::sFillList()
{
  if(_shown)
    display::sFillList();
  else
    _run = true;
}

void taskList::sUpdate(QString source, int id)
{
  if (source == "tasks")
    sFillList();
}
