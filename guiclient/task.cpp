/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "task.h"

#include <QCloseEvent>
#include <QSqlError>
#include <QVariant>
#include <QMessageBox>

#include "errorReporter.h"
#include "guiErrorCheck.h"
#include "userList.h"
#include "taskAssignment.h"

bool task::userHasPriv(const int pMode, const QString pType, const int pId)
{
  if (pType == "J" && _privileges->check("MaintainAllProjects"))
    return true;
  else if (_privileges->check("MaintainAllTaskItems"))
    return true;
  bool personalPriv = (pType == "J") ? _privileges->check("MaintainPersonalProjects") : _privileges->check("MaintainPersonalTaskItems");
  if(pMode==cView)
  {
    if(pType == "J" && _privileges->check("ViewAllProjects"))
      return true;
    else if (_privileges->check("ViewAllTaskItems"))
      return true;
    bool viewPriv = (pType == "J") ? _privileges->check("ViewPersonalProjects") : _privileges->check("ViewPersonalTaskItems");
    personalPriv = personalPriv || viewPriv;
  }

  if(pMode==cNew)
    return personalPriv;
  else
  {
    XSqlQuery usernameCheck;
    usernameCheck.prepare( "SELECT getEffectiveXtUser() IN "
                           " (SELECT task_owner_username "
                           "  FROM task WHERE task_id=:task_id "
                           "  UNION  "
                           "  SELECT taskass_username "
                           "  FROM taskass  "
                           "  WHERE taskass_task_id=:task_id) "
                           " AS canModify; ");
    usernameCheck.bindValue(":task_id", pId);
    usernameCheck.exec();

    if (usernameCheck.first())
      return usernameCheck.value("canModify").toBool()&&personalPriv;
    return false;
  }
}

task::task(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : XDialog(parent, name, modal, fl)
{
  setupUi(this);

  connect(_buttonBox, SIGNAL(accepted()), this, SLOT(sSave()));
  connect(_buttonBox, SIGNAL(rejected()), this, SLOT(sClose()));
  connect(_actualExp, SIGNAL(editingFinished()), this, SLOT(sExpensesAdjusted()));
  connect(_budgetExp, SIGNAL(editingFinished()), this, SLOT(sExpensesAdjusted()));
  connect(_actualHours, SIGNAL(editingFinished()), this, SLOT(sHoursAdjusted()));
  connect(_budgetHours, SIGNAL(editingFinished()), this, SLOT(sHoursAdjusted()));
  connect(_newUser,     SIGNAL(clicked()), this, SLOT(sNewUser()));
  connect(_deleteUser,  SIGNAL(clicked()), this, SLOT(sDeleteUser()));
  connect(_budgetHours, SIGNAL(editingFinished()), this, SLOT(sHoursAdjusted()));
  
  _budgetHours->setValidator(omfgThis->qtyVal());
  _actualHours->setValidator(omfgThis->qtyVal());
  _budgetExp->setValidator(omfgThis->costVal());
  _actualExp->setValidator(omfgThis->costVal());
  _balanceHours->setPrecision(omfgThis->qtyVal());
  _balanceExp->setPrecision(omfgThis->costVal());

  _parentid = -1;
  _taskid = -1;
  
  _owner->setType(UsernameLineEdit::UsersActive);
  _comments->setType(Comments::Task);
  _charass->setType("TASK");
  _documents->setType(Documents::ProjectTask);

  _status->append(0, tr("New"),        "N");
  _status->append(1, tr("Pending"),    "P");
  _status->append(2, tr("In-Process"), "O");
  _status->append(3, tr("Deferred"),   "D");
  _status->append(4, tr("Completed"),  "C");

  _assignments->addColumn(tr("User"),  _userColumn, Qt::AlignLeft, true, "taskass_username");
  _assignments->addColumn(tr("Assigned"),  _dateColumn, Qt::AlignLeft, true, "taskass_assigned_date");
  _assignments->addColumn(tr("Role"),  -1, Qt::AlignLeft, true, "crmrole_name");

  _saved = false;
  _close = false;
  _isTemplate = false;
}

task::~task()
{
  // no need to delete child widgets, Qt does it all for us
}

void task::languageChange()
{
  retranslateUi(this);
}

enum SetResponse task::set(const ParameterList &pParams)
{
  XSqlQuery tasket;
  XSqlQuery assign;
  XDialog::set(pParams);
  QVariant param;
  bool     valid;
  bool     prioritySet = false;

  param = pParams.value("parent", &valid);
  if (valid)
    _parenttype = param.toString();

  param = pParams.value("parent_id", &valid);
  if (valid)
    _parentid = param.toInt();

  param = pParams.value("prj_id", &valid);
  if (valid)
    _project->setId(param.toInt());

  param = pParams.value("isTemplate", &valid);
  if (valid)
    _isTemplate = true;
    
  param = pParams.value("parent_owner_username", &valid);
  if (valid)
    _owner->setUsername(param.toString());

  param = pParams.value("parent_assigned_username", &valid);
  if (valid)
    _assignedTo = param.toString();

  param = pParams.value("username", &valid);
  if (valid)
    _assignedTo = param.toString();

  param = pParams.value("parent_start_date", &valid);
  if (valid)
    _started->setDate(param.toDate());

  param = pParams.value("parent_assigned_date", &valid);
  if (valid)
    _assigned = param.toDate();

  param = pParams.value("parent_due_date", &valid);
  if (valid)
    _due->setDate(param.toDate());

  param = pParams.value("parent_completed_date", &valid);
  if (valid)
    _completed->setDate(param.toDate());

  param = pParams.value("priority_id", &valid);
  if (valid)
  {
    _priority->setId(param.toInt());
    prioritySet = true;
  }

  param = pParams.value("task_id", &valid);
  if (valid)
    _taskid = param.toInt();

  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "new")
    {
      _mode = cNew;

      tasket.prepare("INSERT INTO task (task_parent_type, task_parent_id, task_number, "
                     "                  task_name, task_istemplate) "
                     " VALUES (:parent, :parent_id, 'TEMP' || trunc(random()*100000000), "
                     "         'TEMP', :template) "
                     " RETURNING task_id; ");
      tasket.bindValue(":parent", _parenttype);
      tasket.bindValue(":parent_id", _parentid);
      tasket.bindValue(":template", _isTemplate);
      tasket.exec();
      if (tasket.first())
        _taskid = tasket.value("task_id").toInt();
      else
      {
        ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Task Information"),
                             tasket, __FILE__, __LINE__);
      }
      if(((_metrics->value("TaskNumberGeneration") == "A") ||
          (_metrics->value("TaskNumberGeneration") == "O"))
       && _number->text().isEmpty()
       && _parenttype != "J"
       && !_isTemplate)
      {
        XSqlQuery numq;
        numq.exec("SELECT fetchTaskNumber() AS number;");
        if (numq.first())
        {
          _number->setText(numq.value("number"));
          _number->setEnabled(_metrics->value("TaskNumberGeneration") == "O");
          _name->setFocus();
        }
      }

      if(!prioritySet)
      {
        tasket.exec("SELECT COALESCE((SELECT incdtpriority_id FROM incdtpriority "
                    "       WHERE incdtpriority_default), -1) AS prioritydefault;");
        if (tasket.first())
          _priority->setId(tasket.value("prioritydefault").toInt());
        else
        {
          ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Task Information"),
                               tasket, __FILE__, __LINE__);
        }
      }

      connect(_status,  SIGNAL(currentIndexChanged(int)), this, SLOT(sStatusChanged(int)));
      connect(_completed,  SIGNAL(newDate(QDate)), this, SLOT(sCompletedChanged()));
      connect(_pctCompl,  SIGNAL(valueChanged(int)), this, SLOT(sCompletedChanged()));

      _recurring->setParent(_taskid, "TASK");

      if (_assignedTo.length() > 0)
      {
        assign.prepare("INSERT INTO taskass (taskass_task_id, taskass_username, taskass_crmrole_id, "
                       "                     taskass_assigned_date) "
                       "VALUES (:task, :assigned, getcrmroleid('Primary'), COALESCE(:date, CURRENT_DATE)); ");
        assign.bindValue(":task", _taskid);
        assign.bindValue(":assigned", _assignedTo);
        if (_assigned.isValid())
          assign.bindValue(":date", _assigned);
        assign.exec();
        ErrorReporter::error(QtCriticalMsg, this, tr("Error Assigning Default to Task"),
                                 assign, __FILE__, __LINE__);
      }  
    }
    if (param.toString() == "edit")
    {
      _mode = cEdit;
      _number->setEnabled((_metrics->value("TaskNumberGeneration") != "A"
                          && _parenttype != "J")  || _isTemplate);
      connect(_status,  SIGNAL(currentIndexChanged(int)), this, SLOT(sStatusChanged(int)));
      connect(_completed,  SIGNAL(newDate(QDate)), this, SLOT(sCompletedChanged()));
      connect(_pctCompl,  SIGNAL(valueChanged(int)), this, SLOT(sCompletedChanged()));
    }
    if (param.toString() == "view")
    {
      _number->setEnabled((_metrics->value("TaskNumberGeneration") != "A"
                          && _parenttype != "J")  || _isTemplate);
      _mode = cView;
      setViewMode();
    }
  }

  if (_parenttype == "J")
  {
    XSqlQuery parentq;
    parentq.prepare("SELECT task_id, task_number||' - '||task_name FROM task "
                    "WHERE task_parent_type = 'J' AND task_parent_id = :project "
                    "  AND task_id <> :task "
                    "ORDER BY task_number;" );
    parentq.bindValue(":project", _parentid);
    parentq.bindValue(":task", _taskid);
    parentq.exec();
    if (parentq.first())
      _parentTask->populate(parentq);
  }
  else
  {
    _parentTaskLit->setVisible(false);
    _parentTask->setVisible(false);
  }

  if (_mode != cNew)
    populate();

  _alarms->setId(_taskid);
  _comments->setId(_taskid);
  _documents->setId(_taskid);
  _charass->setId(_taskid);

  _due->setVisible(!_isTemplate);
  _project->setVisible(!_isTemplate);
  _started->setVisible(!_isTemplate);
  _startedLit->setVisible(!_isTemplate);
  _completed->setVisible(!_isTemplate);
  _completedLit->setVisible(!_isTemplate);
  _pctCompl->setVisible(!_isTemplate);
  _pctComplLit->setVisible(!_isTemplate);
  _dueDays->setVisible(_isTemplate);
  _dueDaysLit->setVisible(_isTemplate);
  if (_isTemplate)
  {
    setWindowTitle(tr("Template Task"));
    _tab->removeTab(_tab->indexOf(_teTab));
    _tab->removeTab(_tab->indexOf(_commentsTab));
  }

  sFillUserList();

  return NoError;
}

void task::setViewMode()
{
  _number->setEnabled(false);
  _name->setEnabled(false);
  _descrip->setEnabled(false);
  _parentTask->setEnabled(false);
  _priority->setEnabled(false);
  _status->setEnabled(false);
  _notes->setEnabled(false);
  _budgetHours->setEnabled(false);
  _actualHours->setEnabled(false);
  _budgetExp->setEnabled(false);
  _actualExp->setEnabled(false);
  _scheduleGroup->setEnabled(false);
  _assignmentGroup->setEnabled(false);
  _alarms->setEnabled(false);
  _comments->setReadOnly(true);
  _charass->setReadOnly(true);
  _buttonBox->clear();
  _buttonBox->addButton(QDialogButtonBox::Close);
  _documents->setReadOnly(true);
}

void task::populate()
{
  if (!_lock.acquire("task", _taskid, AppLock::Interactive))
    setViewMode();

  _close = false;

  foreach (QWidget* widget, QApplication::allWidgets())
  {
    if (!widget->isWindow() || !widget->isVisible())
      continue;

    task *w = qobject_cast<task*>(widget);

    if (w && w->id()==_taskid)
    {
      w->setFocus();

      if (omfgThis->showTopLevel())
      {
        w->raise();
        w->activateWindow();
      }

      _close = true;
      break;
    }
  }

  XSqlQuery taskpopulate;
  taskpopulate.prepare( "SELECT task.*, "
             "            (task_due_date::DATE-task_created::DATE) AS duedays "
             "           FROM task "
             "           WHERE (task_id=:task_id);" );
  taskpopulate.bindValue(":task_id", _taskid);
  taskpopulate.exec();
  if (taskpopulate.first())
  {
    _number->setText(taskpopulate.value("task_number"));
    _name->setText(taskpopulate.value("task_name"));
    _descrip->setText(taskpopulate.value("task_descrip").toString());
    _priority->setId(taskpopulate.value("task_priority_id").toInt());
    _parentTask->setId(taskpopulate.value("task_parent_task_id").toInt());
    _owner->setUsername(taskpopulate.value("task_owner_username").toString());
    _started->setDate(taskpopulate.value("task_start_date").toDate());
    _due->setDate(taskpopulate.value("task_due_date").toDate());
    _dueDays->setValue(taskpopulate.value("duedays").toInt());
    _completed->setDate(taskpopulate.value("task_completed_date").toDate());
    _pctCompl->setValue(taskpopulate.value("task_pct_complete").toInt());
    _notes->setText(taskpopulate.value("task_notes").toString());
    _parenttype=taskpopulate.value("task_parent_type").toString();
    _parentid=taskpopulate.value("task_parent_id").toInt();
    _project->setId(taskpopulate.value("task_prj_id").toInt());
    _status->setCode(taskpopulate.value("task_status").toString());
    _budgetHours->setText(formatQty(taskpopulate.value("task_hours_budget").toDouble()));
    _actualHours->setText(formatQty(taskpopulate.value("task_hours_actual").toDouble()));
    _budgetExp->setText(formatCost(taskpopulate.value("task_exp_budget").toDouble()));
    _actualExp->setText(formatCost(taskpopulate.value("task_exp_actual").toDouble()));

    _recurring->setParent(taskpopulate.value("task_recurring_task_id").isNull() ?
                          _taskid : taskpopulate.value("task_recurring_task_id").toInt(),
                          "TASK");
    sHoursAdjusted();
    sExpensesAdjusted();
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Task Information"),
                                taskpopulate, __FILE__, __LINE__))
  {
    return;
  }
}

void task::sSave()
{
  XSqlQuery taskSave;
  QList<GuiErrorCheck> errors;
  errors<< GuiErrorCheck(_number->text().trimmed().length() == 0, _number,
                         tr("You must enter a valid Number."))
        << GuiErrorCheck(_name->text().length() == 0, _name,
                         tr("You must enter a valid Name."))
        << GuiErrorCheck(!_isTemplate && !_due->isValid(), _due,
                         tr("You must enter a valid due date."))
        << GuiErrorCheck(_isTemplate && _dueDays->value() < 0, _due,
                         tr("You must enter valid due days."))
  ;
  if (GuiErrorCheck::reportErrors(this, tr("Cannot Save Project Task"), errors))
    return;

  RecurrenceWidget::RecurrenceChangePolicy cp = _recurring->getChangePolicy();
  if (cp == RecurrenceWidget::NoPolicy)
    return;

  QString storedProc;
  XSqlQuery beginq("BEGIN;");
  XSqlQuery rollbackq;
  rollbackq.prepare("ROLLBACK;");

  taskSave.prepare( "UPDATE task "
               "SET task_number=:task_number, task_name=:task_name,"
               "    task_descrip=:task_descrip, task_status=:task_status,"
               "    task_parent_task_id=:task_parent_task_id, task_prj_id=:task_prj_id,"
               "    task_hours_budget=:task_hours_budget,"
               "    task_hours_actual=:task_hours_actual,"
               "    task_exp_budget=:task_exp_budget,"
               "    task_exp_actual=:task_exp_actual,"
               "    task_owner_username=:task_owner_username,"
               "    task_start_date=:task_start_date,"
               "    task_due_date=COALESCE(:task_due_date, (task_created::DATE + :task_due_days::INT)),"
               "    task_completed_date=:task_completed_date, "
               "    task_priority_id=:task_priority_id, "
               "    task_pct_complete=:task_pct_complete, "
               "    task_notes=:task_notes, "
               "    task_recurring_task_id=:parent_id "
               "WHERE (task_id=:task_id);" );

  taskSave.bindValue(":task_id", _taskid);
  taskSave.bindValue(":task_number", _number->text().trimmed());
  taskSave.bindValue(":task_name", _name->text().trimmed());
  taskSave.bindValue(":task_descrip", _descrip->toPlainText());
  taskSave.bindValue(":task_status", _status->code());
  taskSave.bindValue(":task_priority_id", _priority->id());
  taskSave.bindValue(":task_pct_complete", _pctCompl->value());
  taskSave.bindValue(":task_owner_username", _owner->username());
  taskSave.bindValue(":task_start_date", _started->date());
  if (_isTemplate)
    taskSave.bindValue(":task_due_days", _dueDays->value());
  else
    taskSave.bindValue(":task_due_date", _due->date());
  taskSave.bindValue(":task_completed_date", _completed->date());
  taskSave.bindValue(":task_hours_budget", _budgetHours->text().toDouble());
  taskSave.bindValue(":task_hours_actual", _actualHours->text().toDouble());
  taskSave.bindValue(":task_exp_budget", _budgetExp->text().toDouble());
  taskSave.bindValue(":task_exp_actual", _actualExp->text().toDouble());
  taskSave.bindValue(":task_notes", _notes->toPlainText());
  if (_parentTask->id() > 0)
    taskSave.bindValue(":task_parent_task_id", _parentTask->id());
  if (_parenttype == "J" && _parentid > 0)
    taskSave.bindValue(":task_prj_id", _parentid);
  else if (_project->isValid())
    taskSave.bindValue(":task_prj_id", _project->id());
  if (_recurring->isRecurring())
    taskSave.bindValue(":parent_id", _recurring->parentId());

  taskSave.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Saving Task Information"),
                                taskSave, __FILE__, __LINE__))
  {
    rollbackq.exec();
    return;
  }

  QString errmsg;
  if (! _recurring->save(true, cp, &errmsg))
  {
    rollbackq.exec();
    ErrorReporter::error(QtCriticalMsg, this, tr("Error Occurred"),
                         tr("%1: %2")
                         .arg(windowTitle())
                         .arg(errmsg),__FILE__,__LINE__);
    return;
  }

  XSqlQuery commitq("COMMIT;");

  omfgThis->sEmitSignal("tasks", _taskid);
  _saved = true;
  done(_taskid);
}

void task::closeEvent(QCloseEvent *)
{
  sClose();
}

void task::sClose()
{
  if (cNew == _mode && !_saved)
  {
    XSqlQuery query;
    query.prepare( "DELETE FROM task WHERE task_id=:task_id;" );
    query.bindValue(":task_id", _taskid);
    query.exec();
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Task Information"),
                             query, __FILE__, __LINE__))
        return;
  }
}

void task::sStatusChanged(const int pStatus)
{
  switch(pStatus)
  {
    case 0: // New
      default:
      _started->clear();
      _completed->clear();
      break;
    case 1: // Pending
      _started->clear();
      _completed->clear();
      break;
    case 2: // In Process
      _started->setDate(omfgThis->dbDate());
      _completed->clear();
      break;
    case 3: // Deferred
      break;
    case 4: // Completed
      _completed->setDate(omfgThis->dbDate());
      break;
  }
}

void task::sCompletedChanged()
{
  if (_completed->isValid())
    _pctCompl->setValue(100);
  if (_pctCompl->value() == 100)
    _completed->setDate(omfgThis->dbDate());
}

void task::sHoursAdjusted()
{
  _balanceHours->setText(formatQty(_budgetHours->text().toDouble() - _actualHours->text().toDouble()));
}

void task::sExpensesAdjusted()
{
  _balanceExp->setText(formatCost(_budgetExp->text().toDouble() - _actualExp->text().toDouble()));
}

void task::sNewUser()
{
  ParameterList params;
  params.append("task_id", _taskid);

  taskAssignment newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillUserList();
}

void task::sDeleteUser()
{
  XSqlQuery taskDeleteUser;
  taskDeleteUser.prepare("DELETE FROM taskass WHERE taskass_id=:taskass_id");
  taskDeleteUser.bindValue(":taskass_id", _assignments->id());
  taskDeleteUser.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Task Assignments"),
                           taskDeleteUser, __FILE__, __LINE__))
     return;

  sFillUserList();
}

void task::sFillUserList()
{
  XSqlQuery taskFillUserList;
  taskFillUserList.prepare("SELECT taskass_id, taskass_username, taskass_assigned_date, crmrole_name "
                           " FROM taskass "
                           " LEFT OUTER JOIN crmrole ON crmrole_id=taskass_crmrole_id "
                           " WHERE taskass_task_id=:task_id" );
  taskFillUserList.bindValue(":task_id", _taskid);
  taskFillUserList.exec();
  _assignments->populate(taskFillUserList);
  ErrorReporter::error(QtCriticalMsg, this, tr("Task Assignments"),
                           taskFillUserList, __FILE__, __LINE__);
}


int task::id()
{
  return _taskid;
}

void task::setVisible(bool visible)
{
  if (_close)
    close();
  else
    XDialog::setVisible(visible);
}
