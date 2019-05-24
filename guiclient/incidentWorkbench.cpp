/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "incidentWorkbench.h"

#include <QInputDialog>
#include <QSqlError>
#include <QMessageBox>

#include "errorReporter.h"
#include "guiclient.h"
#include "incident.h"
#include "project.h"
#include "task.h"
#include "parameterwidget.h"

incidentWorkbench::incidentWorkbench(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "incidentWorkbench", fl)
{
  setWindowTitle(tr("Incidents"));
  setListLabel(tr("Incidents"));
  setReportName("IncidentWorkbenchList");
  setMetaSQLOptions("incidents", "detail");
  setParameterWidgetVisible(true);
  setNewVisible(_privileges->check("MaintainAllIncidents") || _privileges->check("MaintainPersonalIncidents"));
  setSearchVisible(true);
  setQueryOnStartEnabled(true);
  setAutoUpdateEnabled(true);

  QString qryStatus = QString("SELECT status_seq, "
                              " CASE WHEN status_code = 'N' THEN '%1' "
                              " WHEN status_code = 'F' THEN '%2' "
                              " WHEN status_code = 'C' THEN '%3' "
                              " WHEN status_code = 'A' THEN '%4' "
                              " WHEN status_code = 'R' THEN '%5' "
                              " WHEN status_code = 'L' THEN '%6' "
                              " END AS name, status_code AS code "
                              "FROM status; ")
      .arg(tr("New"))
      .arg(tr("Feedback"))
      .arg(tr("Confirmed"))
      .arg(tr("Assigned"))
      .arg(tr("Resolved"))
      .arg(tr("Closed"));

  QString qryPriority = "SELECT incdtpriority_id, incdtpriority_name "
                        "FROM incdtpriority "
                        "ORDER BY incdtpriority_order, incdtpriority_name ";
  QString qryResolution = "SELECT incdtresolution_id, incdtresolution_name "
                        "FROM incdtresolution "
                        "ORDER BY incdtresolution_order, incdtresolution_name ";

  parameterWidget()->append(tr("Account"), "crmAccountId", ParameterWidget::Crmacct);
  parameterWidget()->append(tr("Contact"),"cntct_id", ParameterWidget::Contact);
  parameterWidget()->append(tr("Category"), "categorylist",
                           ParameterWidget::Multiselect, QVariant(), false,
                           "SELECT incdtcat_id, incdtcat_name"
                           "  FROM incdtcat"
                           " ORDER BY incdtcat_name;");
  parameterWidget()->appendComboBox(tr("Status Above"), "status_above", qryStatus, 4);
  parameterWidget()->append(tr("Status"), "statuslist",
                           ParameterWidget::Multiselect, QVariant(), false,
                           qryStatus);
  parameterWidget()->appendComboBox(tr("Severity"), "severity_id", XComboBox::IncidentSeverity);
  parameterWidget()->append(tr("User"), "username", ParameterWidget::User, omfgThis->username());
  parameterWidget()->append(tr("Owner"), "owner_username", ParameterWidget::User);
  parameterWidget()->append(tr("Assigned To"), "assigned_username", ParameterWidget::User);
  parameterWidget()->append(tr("Start Date"), "startDate", ParameterWidget::Date);
  parameterWidget()->append(tr("End Date"), "endDate", ParameterWidget::Date);
  parameterWidget()->append(tr("Priority"), "incdtpriority_id_list", ParameterWidget::Multiselect, QVariant(), false, qryPriority);
  parameterWidget()->append(tr("Resolution"), "incdtresolution_id_list", ParameterWidget::Multiselect, QVariant(), false, qryResolution);
  parameterWidget()->append(tr("Project"), "prj_id", ParameterWidget::Project);
  if(_metrics->boolean("IncidentsPublicPrivate"))
    parameterWidget()->append(tr("Public"), "public", ParameterWidget::CheckBox);
  parameterWidget()->append(tr("Item"), "item_id", ParameterWidget::Item);
  if (_metrics->boolean("LotSerialControl"))
    parameterWidget()->append(tr("Lot/Serial Pattern"), "lspattern", ParameterWidget::Text);

  connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sOpen()));

  if (!_privileges->check("MaintainAllIncidents") && !_privileges->check("MaintainPersonalIncidents"))
    newAction()->setEnabled(false);

  list()->addColumn(tr("Number"),      _orderColumn,Qt::AlignLeft, true, "incdt_number" );
  list()->addColumn(tr("Created"),     _dateColumn, Qt::AlignLeft, true, "incdt_timestamp" );
  list()->addColumn(tr("Account"),     _itemColumn, Qt::AlignLeft, true, "crmacct_name" );
  list()->addColumn(tr("Status"),      _itemColumn, Qt::AlignLeft, true, "incdt_status" );
  list()->addColumn(tr("Updated"),     _dateColumn, Qt::AlignLeft, true, "incdt_updated" );
  list()->addColumn(tr("Assigned To"), _userColumn, Qt::AlignLeft, true, "incdt_assigned_username" );
  list()->addColumn(tr("Owner"),       _userColumn, Qt::AlignLeft, true, "incdt_owner_username" );
  list()->addColumn(tr("Summary"),     -1,          Qt::AlignLeft, true, "incdt_summary" );
  list()->addColumn(tr("Category"),    _userColumn, Qt::AlignLeft, false, "incdtcat_name");
  list()->addColumn(tr("Severity"),    _userColumn, Qt::AlignLeft, false, "incdtseverity_name");
  list()->addColumn(tr("Priority"),    _userColumn, Qt::AlignLeft, false, "incdtpriority_name");
  list()->addColumn(tr("Resolution"),    _userColumn, Qt::AlignLeft, false, "incdtresolution_name");
  list()->addColumn(tr("Contact"),     _userColumn, Qt::AlignLeft, false, "cntct_name");
  list()->addColumn(tr("Project"),     _userColumn, Qt::AlignLeft, false, "prj_number");
  if(_metrics->boolean("IncidentsPublicPrivate"))
    list()->addColumn(tr("Public"),     _userColumn, Qt::AlignLeft, false, "incdt_public");
  list()->addColumn(tr("Item Number"),  _itemColumn, Qt::AlignLeft, false, "item_number");
  list()->addColumn(tr("Lot/Serial"),   _itemColumn, Qt::AlignLeft, false, "ls_number");

  list()->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(omfgThis, SIGNAL(emitSignal(QString, int)), this, SLOT(sUpdate(QString, int)));

  setupCharacteristics("INCDT");
}

enum SetResponse incidentWorkbench::set(const ParameterList &pParams)
{
  XWidget::set(pParams);

  QVariant param;
  bool     valid;

  param = pParams.value("run", &valid);
  if (valid)
    sFillList();

  return NoError;
}

void incidentWorkbench::sNew()
{
  ParameterList params;
  params.append("mode", "new");

  incident newdlg(0, 0, true);
  newdlg.set(params);
  newdlg.setWindowModality(Qt::WindowModal);

  if(newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void incidentWorkbench::sEdit()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("mode", "edit");
    params.append("incdt_id", item->id());

    incident* newdlg = new incident(this, 0, false);
    newdlg->set(params);
    newdlg->setAttribute(Qt::WA_DeleteOnClose);
    omfgThis->handleNewWindow(newdlg);
  }
}

void incidentWorkbench::sView()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("incdt_id", item->id());

    incident* newdlg = new incident(this, 0, false);
    newdlg->set(params);
    newdlg->setAttribute(Qt::WA_DeleteOnClose);
    omfgThis->handleNewWindow(newdlg);
  }
}

bool incidentWorkbench::setParams(ParameterList & params)
{
  params.append("new",		tr("New"));
  params.append("feedback",	tr("Feedback"));
  params.append("confirmed",	tr("Confirmed"));
  params.append("assigned",	tr("Assigned"));
  params.append("resolved",	tr("Resolved"));
  params.append("closed",	tr("Closed"));

  params.append("startDate", omfgThis->startOfTime());
  params.append("endDate", omfgThis->endOfTime());
  if (_metrics->boolean("LotSerialControl"))
    params.append("LotSerialControl", true);

  if (!display::setParams(params))
    return false;

  return true;
}

void incidentWorkbench::sPopulateMenu(QMenu *pMenu, QTreeWidgetItem *, int)
{
  QAction *menuItem;
  XSqlQuery prjq;
  bool hasProj = false;

  bool editPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("incdt_owner_username") && _privileges->check("MaintainPersonalIncidents")) ||
      (omfgThis->username() == list()->currentItem()->rawValue("incdt_assigned_username") && _privileges->check("MaintainPersonalIncidents")) ||
      (_privileges->check("MaintainAllIncidents"));

  bool viewPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("incdt_owner_username") && _privileges->check("ViewPersonalIncidents")) ||
      (omfgThis->username() == list()->currentItem()->rawValue("incdt_username") && _privileges->check("ViewPersonalIncidents")) ||
      (_privileges->check("ViewAllIncidents"));

  menuItem = pMenu->addAction(tr("Edit"), this, SLOT(sEdit()));
  menuItem->setEnabled(editPriv);
  menuItem = pMenu->addAction(tr("View"), this, SLOT(sView()));
  menuItem->setEnabled(viewPriv);

  pMenu->addSeparator();

  prjq.prepare("SELECT 1 FROM incdt WHERE incdt_id=:incdt AND incdt_prj_id IS NOT NULL;");
  prjq.bindValue(":incdt", list()->id());
  prjq.exec();
  if (prjq.first())
    hasProj = true;
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Determining Incident Project"),
                              prjq, __FILE__, __LINE__))
    return;

  if (_privileges->check("MaintainAllProjects MaintainPersonalProjects") && !hasProj)
  {
    menuItem = pMenu->addAction(tr("Create Project"), this, SLOT(sCreateProject()));
    menuItem->setEnabled(true);
  }

  if (_privileges->check("MaintainAllTaskItems MaintainPersonalTaskItems"))
  {
    menuItem = pMenu->addAction(tr("Create Task"), this, SLOT(sCreateTask()));
    menuItem->setEnabled(true);
  }

}

void incidentWorkbench::sOpen()
{
  bool editPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("incdt_owner_username") && _privileges->check("MaintainPersonalIncidents")) ||
      (omfgThis->username() == list()->currentItem()->rawValue("incdt_assigned_username") && _privileges->check("MaintainPersonalIncidents")) ||
      (_privileges->check("MaintainAllIncidents"));

  bool viewPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("incdt_owner_username") && _privileges->check("ViewPersonalIncidents")) ||
      (omfgThis->username() == list()->currentItem()->rawValue("incdt_username") && _privileges->check("ViewPersonalIncidents")) ||
      (_privileges->check("ViewAllIncidents"));

  if (editPriv)
    sEdit();
  else if (viewPriv)
    sView();
}

void incidentWorkbench::sCreateProject()
{
  XSqlQuery prjq;
  ParameterList params;
  int prjid = -1;
  bool openw = false;
  
  int answer = QMessageBox::question(this,  tr("Open Projects"),
            tr("Do you want to open the Project(s) after creation?"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
  if (answer == QMessageBox::Yes) 
    openw = true;
  else if (answer == QMessageBox::Cancel)
    return;

  prjq.prepare("SELECT createProjectFromIncident(:incdt_id) AS projectid;" );

  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    prjq.bindValue(":incdt_id", item->id());
    prjq.exec();
    if (prjq.first())
      prjid = prjq.value("projectid").toInt();
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Creating Project"),
                           prjq, __FILE__, __LINE__))
      return;

    if (openw && prjid > 0)
    {
      params.append("prj_id", prjid);
      params.append("mode", cEdit);
   
      project* newdlg = new project(0, "", false);
      newdlg->set(params);
      newdlg->setAttribute(Qt::WA_DeleteOnClose);
      newdlg->show();
    }
  }
}

void incidentWorkbench::sCreateTask()
{
  XSqlQuery taskq;
  ParameterList params;
  int taskid = -1;
  bool openw = false;
  QString taskname;
  bool ok;
  
  int answer = QMessageBox::question(this,  tr("Open Tasks"),
            tr("Do you want to open the Task(s) after creation?"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
  if (answer == QMessageBox::Yes) 
    openw = true;
  else if (answer == QMessageBox::Cancel)
    return;

  taskq.prepare("SELECT createTaskFromIncident(:ophead_id, :task) AS taskid;" );

  taskname = QInputDialog::getText(this, tr("Task Name"),
        tr("Enter a Task Name:"), QLineEdit::Normal, taskname, &ok);
  if(!ok)
    return;

  taskq.bindValue(":task", taskname);

  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    taskq.bindValue(":ophead_id", item->id());
    taskq.exec();
    if (taskq.first())
      taskid = taskq.value("taskid").toInt();
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Creating Task"),
                           taskq, __FILE__, __LINE__))
      return;

    if (openw && taskid > 0)
    {
      params.append("mode", "edit");
      params.append("task_id", taskid);

      task* newdlg = new task(0, "", false);
      newdlg->set(params);
      newdlg->setAttribute(Qt::WA_DeleteOnClose);
      newdlg->show();
    }
  }
}

void incidentWorkbench::sUpdate(QString source, int id)
{
  Q_UNUSED(id);
  if (source == "Incident")
    sFillList();
}
