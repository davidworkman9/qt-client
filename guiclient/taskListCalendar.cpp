/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "taskListCalendar.h"

#include "xdialog.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include <calendargraphicsitem.h>
#include <metasql.h>
#include <openreports.h>

#include "taskCalendarControl.h"
#include "storedProcErrorLookup.h"
#include "task.h"
#include "customer.h"
#include "errorReporter.h"

taskListCalendar::taskListCalendar(QWidget* parent, const char * name, Qt::WindowFlags f)
  : XWidget(parent, name, f)
{
  XSqlQuery taskListq;
  setupUi(this);

  taskCalendarControl * cc = new taskCalendarControl(this);
  QGraphicsScene * scene = new QGraphicsScene(this);
  calendar = new CalendarGraphicsItem(cc);
  calendar->setSelectedDay(QDate::currentDate());
  scene->addItem(calendar);

  _gview->setScene(scene);
  _gview->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

  _usr->setEnabled(_privileges->check("MaintainAllTaskItems") || _privileges->check("MaintainPersonalTaskItems")
                   || _privileges->check("MaintainAllProjects") || _privileges->check("MaintainPersonalProjects"));
  _usr->setType(ParameterGroup::User);
  taskListq.prepare("SELECT getUsrId(NULL) AS usr_id;");
  taskListq.exec();
  if (taskListq.first())
  {
    _myUsrId = taskListq.value("usr_id").toInt();
    _usr->setId(_myUsrId);
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving User Information"),
                                taskListq, __FILE__, __LINE__))
  {
    close();
  }

  connect(_active, SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_completed, SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_list, SIGNAL(populateMenu(QMenu*, QTreeWidgetItem*, int)), this, SLOT(sPopulateMenu(QMenu*)));
  connect(_list, SIGNAL(itemSelectionChanged()), this, SLOT(handlePrivs()));
  connect(_usr, SIGNAL(updated()), this, SLOT(sFillList()));
  connect(_usr, SIGNAL(updated()), this, SLOT(handlePrivs()));


  _list->addColumn(tr("Type"),    _statusColumn,  Qt::AlignCenter, true, "type");
  _list->addColumn(tr("Seq"),        _seqColumn,  Qt::AlignRight,  true, "seq");
  _list->addColumn(tr("Priority"),  _userColumn,  Qt::AlignLeft,   true, "priority");
  _list->addColumn(tr("Owner"),     _userColumn,  Qt::AlignLeft,  false, "owner");
  _list->addColumn(tr("Assigned"),  _userColumn,  Qt::AlignLeft,   true, "assigned");
  _list->addColumn(tr("Name"),              100,  Qt::AlignLeft,   true, "name");
  _list->addColumn(tr("Description"),        -1,  Qt::AlignLeft,   true, "descrip");
  _list->addColumn(tr("Status"),  _statusColumn,  Qt::AlignLeft,   true, "status");
  _list->addColumn(tr("Due Date"),  _dateColumn,  Qt::AlignLeft,   true, "due");
  _list->addColumn(tr("Incident"), _orderColumn,  Qt::AlignLeft,   true, "incdt");
  _list->addColumn(tr("Customer"), _orderColumn,  Qt::AlignLeft,   true, "cust");
  _list->addColumn(tr("Owner"),     _userColumn,  Qt::AlignLeft,   false,"owner");

  if (_preferences->boolean("XCheckBox/forgetful"))
    _active->setChecked(true);

  sFillList(QDate::currentDate());

  connect(_list, SIGNAL(itemSelected(int)), this, SLOT(sOpen()));
  connect(cc, SIGNAL(selectedDayChanged(QDate)), this, SLOT(sFillList(QDate)));
}

void taskListCalendar::languageChange()
{
  retranslateUi(this);
}

enum SetResponse taskListCalendar::set(const ParameterList& pParams)
{
  XWidget::set(pParams);
  QVariant param;
  bool           valid;

  param = pParams.value("usr_id", &valid);
  if (valid)
  {
    _usr->setId(param.toInt());
    sFillList();
  }
  return NoError;
}

void taskListCalendar::sOpen()
{
  if (_list->rawValue("type") == "T")
  {
    bool editPriv =
        (omfgThis->username() == _list->currentItem()->rawValue("owner") && _privileges->check("MaintainPersonalTaskItems")) ||
        (_list->currentItem()->rawValue("assigned").toString().indexOf(omfgThis->username()) > 0 && _privileges->check("MaintainPersonalTaskItems")) ||
        (_privileges->check("MaintainAllTaskItems"));

    bool viewPriv =
        (omfgThis->username() == _list->currentItem()->rawValue("owner") && _privileges->check("ViewPersonalTaskItems")) ||
        (_list->currentItem()->rawValue("assigned").toString().indexOf(omfgThis->username()) > 0 && _privileges->check("ViewPersonalTaskItems")) ||
        (_privileges->check("ViewAllTaskItems"));

    if (editPriv)
      sEditTask();
    else if (viewPriv)
      sViewTask();
  }
   else if (_list->rawValue("type") == "PR")
  {
    bool editPriv =
        (omfgThis->username() == _list->currentItem()->rawValue("owner") && _privileges->check("MaintainPersonalProjects")) ||
        (_list->currentItem()->rawValue("assigned").toString().indexOf(omfgThis->username()) > 0 && _privileges->check("MaintainPersonalProjects")) ||
        (_privileges->check("MaintainAllProjects"));

    bool viewPriv =
        (omfgThis->username() == _list->currentItem()->rawValue("owner") && _privileges->check("ViewPersonalProjects")) ||
        (_list->currentItem()->rawValue("assigned").toString().indexOf(omfgThis->username()) > 0 && _privileges->check("ViewPersonalProjects")) ||
        (_privileges->check("ViewAllProjects"));

    if (editPriv)
      sEditTask();
    else if (viewPriv)
      sViewTask();
  }
}

void taskListCalendar::sPopulateMenu(QMenu *pMenu)
{
  QAction *menuItem;

  if (_list->rawValue("type") == "T")
  {
    bool editPriv =
        (omfgThis->username() == _list->currentItem()->rawValue("owner") && _privileges->check("MaintainPersonalTaskItems")) ||
        (_list->currentItem()->rawValue("assigned").toString().indexOf(omfgThis->username()) > 0 && _privileges->check("MaintainPersonalTaskItems")) ||
        (_privileges->check("MaintainAllTaskItems"));

    bool viewPriv =
        (omfgThis->username() == _list->currentItem()->rawValue("owner") && _privileges->check("ViewPersonalTaskItems")) ||
        (_list->currentItem()->rawValue("assigned").toString().indexOf(omfgThis->username()) > 0 && _privileges->check("ViewPersonalTaskItems")) ||
        (_privileges->check("ViewAllTaskItems"));

    menuItem = pMenu->addAction(tr("New..."), this, SLOT(sNewTask()));
    menuItem->setEnabled(editPriv);

    menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sEditTask()));
    menuItem->setEnabled(editPriv);

   menuItem = pMenu->addAction(tr("View..."), this, SLOT(sViewTask()));
    menuItem->setEnabled(viewPriv);

    menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sDelete()));
    menuItem->setEnabled(editPriv);
  }

  if (_list->rawValue("type") == "PR")
  {
    bool editPriv =
        (omfgThis->username() == _list->currentItem()->rawValue("owner") && _privileges->check("MaintainPersonalProjects")) ||
        (_list->currentItem()->rawValue("assigned").toString().indexOf(omfgThis->username()) > 0 && _privileges->check("MaintainPersonalProjects")) ||
        (_privileges->check("MaintainAllProjects"));

    bool viewPriv =
        (omfgThis->username() == _list->currentItem()->rawValue("owner") && _privileges->check("ViewPersonalProjects")) ||
        (_list->currentItem()->rawValue("assigned").toString().indexOf(omfgThis->username()) > 0 && _privileges->check("ViewPersonalProjects")) ||
        (_privileges->check("ViewAllProjects"));

    menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sEditTask()));
    menuItem->setEnabled(editPriv);

    menuItem = pMenu->addAction(tr("View..."), this, SLOT(sViewTask()));
    menuItem->setEnabled(viewPriv);

    menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sDelete()));
    menuItem->setEnabled(editPriv);
  }

  if (! (_list->rawValue("cust") == ""))
  {
    menuItem = pMenu->addAction(tr("Edit Customer"), this, SLOT(sEditCustomer()));
    menuItem->setEnabled(_privileges->check("MaintainCustomerMasters"));
    menuItem = pMenu->addAction(tr("View Customer"), this, SLOT(sViewCustomer()));
    menuItem->setEnabled(_privileges->check("MaintainCustomerMasters"));
  }
}

void taskListCalendar::sNewTask()
{
  ParameterList params;
  params.append("mode", "new");
  params.append("parent", "TASK");
  
  if (_usr->isSelected())
    _usr->appendValue(params);
  else
    params.append("parent_assigned_username", omfgThis->username());

  task newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void taskListCalendar::sEditTask()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("task_id", _list->id());

  task newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void taskListCalendar::sViewTask()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("task_id", _list->id());

  task newdlg(this, "", true);
  newdlg.set(params);

  newdlg.exec();
}

void taskListCalendar::sDelete()
{
  XSqlQuery taskDelete;
  taskDelete.prepare("SELECT deleteTask(:task_id) AS result;");
  taskDelete.bindValue(":task_id", _list->id());
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
      taskDelete.bindValue(":task_id", _list->id());
      taskDelete.exec();
      if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Task"),
                               taskDelete, __FILE__, __LINE__))
         return;
    }
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Task"),
                            taskDelete, __FILE__, __LINE__))
    return;
  
  sFillList();
}

void taskListCalendar::sEditCustomer()
{
  XSqlQuery cust;
  cust.prepare("SELECT cust_id FROM custinfo WHERE (cust_number=:number);");
  cust.bindValue(":number", _list->rawValue("cust"));
  if (cust.exec() && cust.first())
  {
    ParameterList params;
    params.append("cust_id", cust.value("cust_id").toInt());
    params.append("mode","edit");

    customer *newdlg = new customer();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
  else if (cust.lastError().type() != QSqlError::NoError)
    ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Customer Information"),
                       cust, __FILE__, __LINE__);

}

void taskListCalendar::sViewCustomer()
{
  XSqlQuery cust;
  cust.prepare("SELECT cust_id FROM custinfo WHERE (cust_number=:number);");
  cust.bindValue(":number", _list->rawValue("cust"));
  if (cust.exec() && cust.first())
  {
    ParameterList params;
    params.append("cust_id", cust.value("cust_id").toInt());
    params.append("mode","view");

    customer *newdlg = new customer();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
  else if (cust.lastError().type() != QSqlError::NoError)
    ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Customer Information"),
                       cust, __FILE__, __LINE__);

}

void taskListCalendar::setParams(ParameterList &params)
{
  if (_active->isChecked())
    params.append("active");
  if (_completed->isChecked())
    params.append("completed");
  _usr->appendValue(params);
}

void taskListCalendar::sFillList()
{
  sFillList(_lastDate);
}

void taskListCalendar::sFillList(const QDate & date)
{
  static bool dontBotherMe = false;
  if(dontBotherMe)
    return;
  dontBotherMe = true;
  _lastDate = date;
  calendar->setSelectedDay(_lastDate);
  QString sql = "SELECT task_id AS id, task_owner_username AS owner, "
                "       CASE task_parent_type WHEN 'J' THEN 'PR' ELSE 'T' END AS type, "
                "       incdtpriority_order AS seq, incdtpriority_name AS priority, "
                "       task_name AS name, "
                "       firstLine(task_descrip) AS descrip, "
                "       task_status AS status, task_due_date AS due, "
                "       (SELECT array_to_string(array_agg(taskass_username),',') FROM taskass "
                "                                 WHERE taskass_task_id=task_id) AS assigned, "
                "       task_owner_username AS owner, "
                "       incdt_number AS incdt, cust_number AS cust, "
                "       CASE WHEN (task_status != 'C'AND "
                "                  task_due_date < CURRENT_DATE) THEN 'expired'"
                "            WHEN (task_status != 'C'AND "
                "                  task_due_date > CURRENT_DATE) THEN 'future'"
                "       END AS due_qtforegroundrole "
                "  FROM task() LEFT OUTER JOIN incdt ON (incdt_id=task_parent_id AND task_parent_type='INCDT') "
                "              LEFT OUTER JOIN crmacct ON (crmacct_id=task_parent_id AND task_parent_type='CRMA') "
                "              LEFT OUTER JOIN custinfo ON (cust_crmacct_id=crmacct_id) "
                "              LEFT OUTER JOIN incdtpriority ON (incdtpriority_id=task_priority_id) "
                " WHERE task_due_date = <? value('date') ?> "
                "   AND NOT task_istemplate "
                "  <? if not exists('completed') ?>"
                "  AND   task_status != 'C'"
                "  <? endif ?>"
                "  <? if exists('username') ?> "
                "  AND task_id IN (SELECT taskass_task_id "
                "                  FROM taskass "
                "                  WHERE taskass_username = <? value('username') ?>) "
                "  <? elseif exists('usr_pattern') ?>"
                "  AND task_id IN (SELECT taskass_task_id "
                "                  FROM taskass "
                "                  WHERE taskass_username ~ <? value('username') ?>) "
                "  <? endif ?>"
                "  <? if exists('active') ?>AND task_status <> 'C' <? endif ?>"
                "ORDER BY due, seq, assigned;";

  ParameterList params;
  params.append("date", date);
  setParams(params);

  MetaSQLQuery mql(sql);
  XSqlQuery itemQ = mql.toQuery(params);

  _list->populate(itemQ);

  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving To Do Item Information"),
                                itemQ, __FILE__, __LINE__))
  {
    dontBotherMe = false;
    return;
  }
  dontBotherMe = false;
}

void taskListCalendar::resizeEvent(QResizeEvent* event)
{
  XWidget::resizeEvent(event);

  _gview->setMinimumWidth((int)(_gview->height() * (_gview->scene()->sceneRect().width() / _gview->scene()->sceneRect().height())));
  _gview->fitInView(_gview->scene()->sceneRect(), Qt::KeepAspectRatio);
}

void taskListCalendar::showEvent(QShowEvent * event)
{
  XWidget::showEvent(event);

  _gview->setMinimumWidth((int)(_gview->height() * (_gview->scene()->sceneRect().width() / _gview->scene()->sceneRect().height())));
  _gview->fitInView(_gview->scene()->sceneRect(), Qt::KeepAspectRatio);
}
