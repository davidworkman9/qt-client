/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "dspProjectSummary.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QSqlError>

#include "guiclient.h"
#include "project.h"
#include "parameterwidget.h"
#include "errorReporter.h"

dspProjectSummary::dspProjectSummary(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "dspProjectSummary", fl)
{
  setWindowTitle(tr("Project Summary"));
  setReportName("ProjectsSummary");
  setMetaSQLOptions("projects", "summary");
  setNewVisible(true);
  setParameterWidgetVisible(true);
  setQueryOnStartEnabled(true);

  QString qryType   = "SELECT prjtype_id, prjtype_code FROM prjtype ORDER by prjtype_code;";
  QString qryStatus = "SELECT 1 AS id, 'Pending' AS status "
                      " UNION SELECT 2 AS id, 'Active' "
                      " UNION SELECT 3 AS id, 'Closed' "
                      " ORDER BY id;";

  parameterWidget()->append(tr("Project"), "prj_id", ParameterWidget::Project);
  parameterWidget()->append(tr("Project Type"), "prjtype_id", ParameterWidget::Multiselect, QVariant(), false, qryType );
  parameterWidget()->append(tr("Status"), "prj_stat_id", ParameterWidget::Multiselect, QVariant(), false, qryStatus );
  parameterWidget()->append(tr("Assigned To"), "assignto_id", ParameterWidget::User );
  parameterWidget()->append(tr("Show Completed"), "showCompleted", ParameterWidget::Exists);

  list()->addColumn(tr("Customer"),       _orderColumn, Qt::AlignLeft,  true, "crmacct_name");
  list()->addColumn(tr("Number"),         _orderColumn, Qt::AlignLeft,  true, "prj_number");
  list()->addColumn(tr("Project"),        -1,           Qt::AlignLeft,  true, "prj_name");
  list()->addColumn(tr("Type"),           _orderColumn, Qt::AlignLeft,  true, "prjtype_code");
  list()->addColumn(tr("Status"),         _orderColumn, Qt::AlignLeft,  true, "prj_stat");
  list()->addColumn(tr("Budget Hours"),   -1,           Qt::AlignRight, true, "budhrs");
  list()->addColumn(tr("Pending Budget Hrs"),   -1,     Qt::AlignRight, true, "pendbudhrs");
  list()->addColumn(tr("Actual Hours"),   -1,           Qt::AlignRight, true, "acthrs");
  list()->addColumn(tr("Hours Balance"),  -1,           Qt::AlignRight, true, "balhrs");
  list()->addColumn(tr("Budget Exp."),    -1,           Qt::AlignRight, true, "budexp");
  list()->addColumn(tr("Pending Budget Exp"),   -1,     Qt::AlignRight, true, "pendbudexp");
  list()->addColumn(tr("Actual Exp."),    -1,           Qt::AlignRight, true, "actexp");
  list()->addColumn(tr("Exp. Balance"),   -1,           Qt::AlignRight, true, "balexp");
  list()->addColumn(tr("Count"),          -1,           Qt::AlignRight, true, "prjcnt");

  if (_privileges->check("MaintainAllProjects"))
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sEdit()));
  else
    newAction()->setEnabled(false);

}

void dspProjectSummary::sPopulateMenu(QMenu *pMenu, QTreeWidgetItem *pSelected, int)
{
  QAction *menuItem;

  menuItem = pMenu->addAction(tr("Edit Project"), this, SLOT(sEdit()));
  menuItem->setEnabled(_privileges->check("MaintainAllProjects"));

  menuItem = pMenu->addAction(tr("View Project"), this, SLOT(sView()));
  menuItem->setEnabled(_privileges->check("MaintainAllProjects") ||
                       _privileges->check("ViewAllProjects"));
}

bool dspProjectSummary::setParams(ParameterList &params)
{
  if (!display::setParams(params))
    return false;

  params.append("active", tr("Active"));
  params.append("pending", tr("Pending"));
  params.append("closed", tr("Closed"));
}

void dspProjectSummary::sNew()
{
  sOpenProject("new");
}

void dspProjectSummary::sEdit()
{
  sOpenProject("edit");
}

void dspProjectSummary::sView()
{
  sOpenProject("view");
}

void dspProjectSummary::sOpenProject(QString mode)
{
  ParameterList params;
  params.append("mode", mode);
  if (mode != "new")
    params.append("prj_id", list()->id());

  project newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
  sFillList();
}


