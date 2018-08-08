/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to y6ou under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "taskTemplates.h"

#include <QMessageBox>

#include <parameter.h>

#include "taskTemplate.h"
#include "errorReporter.h"
#include "guiclient.h"

taskTemplates::taskTemplates(QWidget* parent, const char* name, Qt::WindowFlags fl)
  : XWidget(parent, name, fl)
{
  setupUi(this);

  // signals and slots connections
  connect(_new, SIGNAL(clicked()), this, SLOT(sNew()));
  connect(_edit, SIGNAL(clicked()), this, SLOT(sEdit()));
  connect(_delete, SIGNAL(clicked()), this, SLOT(sDelete()));
  connect(_close, SIGNAL(clicked()), this, SLOT(close()));
  connect(_template, SIGNAL(valid(bool)), _edit, SLOT(setEnabled(bool)));

  _template->addColumn(tr("Name"),        -1, Qt::AlignLeft, true, "tasktmpl_name" );
  _template->addColumn(tr("Description"), -1, Qt::AlignLeft, true, "tasktmpl_descrip" );
  _template->addColumn(tr("Tasks"),      -1,  Qt::AlignLeft, true, "tasks" );
  
  if (_privileges->check("MaintainTaskTemplates"))
  {
    connect(_template, SIGNAL(valid(bool)), _edit, SLOT(setEnabled(bool)));
    connect(_template, SIGNAL(valid(bool)), _delete, SLOT(setEnabled(bool)));
    connect(_template, SIGNAL(itemSelected(int)), _edit, SLOT(animateClick()));
  }
  else
  {
    _new->setEnabled(false);
  }

  connect(omfgThis, SIGNAL(emitSignal(QString, int)), this, SLOT(sUpdate(QString, int)));

  sFillList();
}

taskTemplates::~taskTemplates()
{
  // no need to delete child widgets, Qt does it all for us
}

void taskTemplates::languageChange()
{
  retranslateUi(this);
}

void taskTemplates::sDelete()
{
  XSqlQuery tmplDelete;

/* TODO Check for existing assignments to objects

  tmplDelete.prepare( "SELECT 1 FROM prj WHERE prj_tasktmpl_id=:tasktmpl_id;" );
  tmplDelete.bindValue(":tasktmpl_id", _template->id());
  tmplDelete.exec();
  ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Project Type"),
                            tmplDelete, __FILE__, __LINE__);
  if (tmplDelete.size() > 0)
  {
    QMessageBox::warning( this, tr("Error Deleting Template"),
                          tr( "You cannot delete this Template as it is used." ) );
    return;
  }
*/
  tmplDelete.prepare( "DELETE FROM tasktmpl "
             "WHERE (tasktmpl_id=:tasktmpl_id);" );
  tmplDelete.bindValue(":tasktmpl_id", _template->id());
  tmplDelete.exec();

  ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Template"),
                            tmplDelete, __FILE__, __LINE__);

  sFillList();
}


void taskTemplates::sNew()
{
  ParameterList params;
  params.append("mode", "new");

  taskTemplate newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void taskTemplates::sEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("tasktmpl_id", _template->id());

  taskTemplate newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void taskTemplates::sUpdate(QString source, int id)
{
  if (source == "taskTemplate")
    sFillList();
}

void taskTemplates::sFillList()
{
  _template->populate( "SELECT tasktmpl_id, tasktmpl_name, tasktmpl_descrip, "
                       "       COUNT(tasktmplitem_id) AS tasks "
                       "FROM tasktmpl "
                       "LEFT OUTER JOIN tasktmplitem ON tasktmplitem_tasktmpl_id=tasktmpl_id "
                       "GROUP BY tasktmpl_id, tasktmpl_name, tasktmpl_descrip "
                       "ORDER BY tasktmpl_name;" );
}

