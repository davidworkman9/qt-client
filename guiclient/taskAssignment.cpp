/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "taskAssignment.h"

#include <QVariant>

#include "errorReporter.h"
#include "guiErrorCheck.h"

taskAssignment::taskAssignment(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : XDialog(parent, name, modal, fl)
{
  setupUi(this);

  connect(_save, SIGNAL(clicked()), this, SLOT(sSave()));

  _role->setType(XComboBox::XComboBox::CRMRoleContact);
}

taskAssignment::~taskAssignment()
{
  // no need to delete child widgets, Qt does it all for us
}

void taskAssignment::languageChange()
{
  retranslateUi(this);
}

enum SetResponse taskAssignment::set(const ParameterList &pParams)
{
  XDialog::set(pParams);
  QVariant param;
  bool     valid;

  param = pParams.value("task_id", &valid);
  if (valid)
    _taskid = param.toInt();

  return NoError;
}

void taskAssignment::sSave()
{
  XSqlQuery taskSave;
  QList<GuiErrorCheck> errors;

  errors<< GuiErrorCheck(_user->username().length() < 1, _user,
                         tr("You must select a User."))
        << GuiErrorCheck(!_role->isValid(), _role,
                         tr("You must enter a CRM Role."))
  ;
  if (GuiErrorCheck::reportErrors(this, tr("Cannot Save Task Assignment"), errors))
    return;

  taskSave.prepare("INSERT INTO taskass (taskass_task_id, taskass_username, taskass_crmrole_id, "
                   "                     taskass_assigned_date) "
                   "VALUES (:task, :user, :role, COALESCE(:date, CURRENT_TIMESTAMP) ) "
                   " ON CONFLICT (taskass_task_id, taskass_username) DO NOTHING; " );
  taskSave.bindValue(":task", _taskid);
  taskSave.bindValue(":user", _user->username());
  taskSave.bindValue(":role", _role->id());
  taskSave.bindValue(":date", _date->date());
  taskSave.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Saving Task Information"),
                                taskSave, __FILE__, __LINE__))
     return;

  done(_taskid); 
}
