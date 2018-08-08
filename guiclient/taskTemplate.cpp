/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "taskTemplate.h"

#include <metasql.h>
#include "mqlutil.h"
#include "errorReporter.h"
#include "guiErrorCheck.h"
#include "task.h"

#include <QMessageBox>
#include <QVariant>
#include <QInputDialog>

taskTemplate::taskTemplate(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
  : XDialog(parent, name, modal, fl)
{
  setupUi(this);

  connect(_save, SIGNAL(clicked()), this, SLOT(sSaveAndClose()));
  connect(_close, SIGNAL(clicked()), this, SLOT(sClose()));
  connect(_new,   SIGNAL(clicked()), this, SLOT(sNewTask()));
  connect(_edit,  SIGNAL(clicked()), this, SLOT(sEditTask()));
  connect(_assign, SIGNAL(clicked()), this, SLOT(sAssignTask()));
  connect(_remove, SIGNAL(clicked()), this, SLOT(sRemoveTask()));
  connect(_tasks, SIGNAL(itemSelected(int)), _edit, SLOT(animateClick()));

  _tasks->addColumn(tr("Number"), -1,  Qt::AlignLeft, true, "task_number");
  _tasks->addColumn(tr("Name"),  -1,  Qt::AlignLeft,   true, "task_name");

}

taskTemplate::~taskTemplate()
{
  // no need to delete child widgets, Qt does it all for us
}

void taskTemplate::languageChange()
{
  retranslateUi(this);
}

enum SetResponse taskTemplate::set(const ParameterList &pParams)
{
  XSqlQuery templt;
  QVariant param;
  bool     valid;

  param = pParams.value("tasktmpl_id", &valid);
  if (valid)
  {
    _tmplid = param.toInt();
    populate();
  }

  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "new")
    {
      _mode = cNew;

      templt.exec("SELECT NEXTVAL('tasktmpl_tasktmpl_id_seq') AS tasktmpl_id;");
      if (templt.first())
        _tmplid = templt.value("tasktmpl_id").toInt();
    }
    else if (param.toString() == "edit")
      _mode = cEdit;
  }
  
  return NoError;
}

void taskTemplate::sClose()
{
  XSqlQuery typeClose;
  if (_mode == cNew)
  {
    typeClose.prepare( "DELETE FROM tasktmpl "
                "WHERE (tasktmpl_id=:tasktmpl_id);" );
    typeClose.bindValue(":tasktmpl_id", _tmplid);
    typeClose.exec();

    ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Template"),
                              typeClose, __FILE__, __LINE__);
  }

  close();
}

void taskTemplate::sSaveAndClose()
{
  sSave(false);
}

bool taskTemplate::sSave(bool pPartial)
{
  XSqlQuery tmplSave;
  QList<GuiErrorCheck> errors;

  errors << GuiErrorCheck(_name->text().trimmed().length() == 0, _name, 
                          tr("You must enter a Name for this Template before you may save it."))
  ;

  if (GuiErrorCheck::reportErrors(this, tr("Cannot Save Template"), errors))
      return false;

  tmplSave.prepare( "INSERT INTO tasktmpl "
             "(tasktmpl_id, tasktmpl_name, tasktmpl_descrip) "
             "VALUES "
             "(:tasktmpl_id, :tasktmpl_name, :tasktmpl_descrip) "
             "ON CONFLICT (tasktmpl_id) DO UPDATE "
             "  SET tasktmpl_name=:tasktmpl_name, tasktmpl_descrip=:tasktmpl_descrip; " );          

  tmplSave.bindValue(":tasktmpl_id", _tmplid);
  tmplSave.bindValue(":tasktmpl_name", _name->text());
  tmplSave.bindValue(":tasktmpl_descrip", _descr->text());
  tmplSave.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Saving Template"),
                                tmplSave, __FILE__, __LINE__))
  {
    return false;
  }

  omfgThis->sEmitSignal(QString("taskTemplate"), -1);

  if (!pPartial)
    close();

  return true;
}

void taskTemplate::populate()
{
  XSqlQuery typepopulate;
  typepopulate.prepare( "SELECT * FROM tasktmpl "
             "WHERE tasktmpl_id=:tasktmpl_id;" );
  typepopulate.bindValue(":tasktmpl_id", _tmplid);
  typepopulate.exec();
  if (typepopulate.first())
  {
    _name->setText(typepopulate.value("tasktmpl_name").toString());
    _descr->setText(typepopulate.value("tasktmpl_descrip").toString());
   }
  sFillTaskList();
}

void taskTemplate::sNewTask()
{
  XSqlQuery ntq;
  int _taskid;
  ParameterList params;

  if (!sSave(true))
    return;

  params.append("mode", "new");
  params.append("parent", "TASK");
  params.append("parent_id", _tmplid);
  params.append("isTemplate", true);

  task newdlg(0, "", true);
  newdlg.set(params);
  newdlg.setWindowModality(Qt::WindowModal);

  if ((_taskid = newdlg.exec()) != XDialog::Rejected)
  {
    ntq.prepare("INSERT INTO tasktmplitem (tasktmplitem_tasktmpl_id, tasktmplitem_task_id) "
                "VALUES (:tasktmpl, :taskid); ");
    ntq.bindValue(":tasktmpl", _tmplid);
    ntq.bindValue(":taskid", _taskid);
    ntq.exec();
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Creating Template Task"),
                             ntq, __FILE__, __LINE__))
       return;

    sFillTaskList();
  }
}

void taskTemplate::sEditTask()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("task_id", _tasks->altId());
  params.append("isTemplate", true);

  task newdlg(0, "", true);
  newdlg.set(params);
  if (newdlg.exec() != XDialog::Rejected)
    sFillTaskList();
}

void taskTemplate::sAssignTask()
{
  if (!sSave(true))
    return;

  QMap<QString, int> templateTasks;
  QStringList items;
  bool ok;
  XSqlQuery tlist;
  tlist.prepare("SELECT task_id, task_number ||'-'||task_name AS task_number "
                "FROM task "
                "WHERE task_istemplate " 
                "  AND task_id NOT IN (SELECT tasktmplitem_task_id FROM tasktmplitem  "
                "                      WHERE tasktmplitem_tasktmpl_id=:tasktmpl); ");
  tlist.bindValue(":tasktmpl", _tmplid);
  tlist.exec();
  while (tlist.next())
  {
    templateTasks[tlist.value("task_number").toString()] = tlist.value("task_id").toInt();
    items << tlist.value("task_number").toString();
  }
  QString item = QInputDialog::getItem(this, tr("Template Tasks"),
                                         tr("Task:"), items, 0, false, &ok);
  if (ok && !item.isEmpty())
  {
    XSqlQuery assignq;
    assignq.prepare("INSERT INTO tasktmplitem (tasktmplitem_tasktmpl_id, tasktmplitem_task_id) "
                    " VALUES(:tasktmpl, :task) "
                    "ON CONFLICT (tasktmplitem_tasktmpl_id, tasktmplitem_task_id) DO NOTHING ;" );
    assignq.bindValue(":tasktmpl", _tmplid);
    assignq.bindValue(":task", templateTasks[item]);
    assignq.exec();
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Assigning Template Task"),
                                assignq, __FILE__, __LINE__))
       return;
  }
  sFillTaskList();
}

void taskTemplate::sRemoveTask()
{
  XSqlQuery remq;
  remq.prepare("DELETE FROM tasktmplitem WHERE tasktmplitem_id=:tasktmplid;" );
  remq.bindValue(":tasktmplid", _tasks->id());
  remq.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Removing Template Task"),
                                remq, __FILE__, __LINE__))
    return;

  remq.prepare("SELECT count(*) AS taskcount FROM tasktmplitem "
               "WHERE tasktmplitem_task_id=:taskid; " );
  remq.bindValue(":taskid", _tasks->altId());
  remq.exec();
  if (remq.first())
  {
    if (remq.value("taskcount").toInt() == 0)
    {
       if (QMessageBox::question(this, tr("Delete Task"),
                             tr("<p>The removed task does not exist on any Templates.\n"
                                "Do you wish to also delete the task?"),
                                QMessageBox::Yes, QMessageBox::No | QMessageBox::Default) == QMessageBox::Yes)
       {
         remq.prepare("DELETE FROM task WHERE task_id=:taskid;");
         remq.bindValue(":taskid", _tasks->altId());
         remq.exec();
         if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Template Task"),
                                remq, __FILE__, __LINE__))
            return;
       }
     }
   }
   else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Removing Template Task"),
                                remq, __FILE__, __LINE__))
            return;

   sFillTaskList();
}

void taskTemplate::sFillTaskList()
{
  XSqlQuery taskq;
  taskq.prepare("SELECT tasktmplitem_id, tasktmplitem_task_id AS altid, task_number, task_name "
                " FROM tasktmplitem "
                " JOIN task ON tasktmplitem_task_id=task_id "
                " WHERE tasktmplitem_tasktmpl_id=:tasktmpl "
                " ORDER BY task_number, task_name; " );
  taskq.bindValue(":tasktmpl", _tmplid);
  taskq.exec();
  _tasks->populate(taskq, true);
}

