/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "prospects.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include "errorReporter.h"
#include "parameterwidget.h"
#include "prospect.h"
#include "salesOrder.h"
#include "opportunity.h"
#include "task.h"
#include "project.h"
#include "storedProcErrorLookup.h"

prospects::prospects(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "prospects", fl)
{
  setWindowTitle(tr("Prospects"));
  setMetaSQLOptions("prospects", "detail");
  setParameterWidgetVisible(true);
  setNewVisible(true);
  setSearchVisible(true);
  setQueryOnStartEnabled(true);
  setUseAltId(true);

  parameterWidget()->append(tr("Show Inactive"), "showInactive", ParameterWidget::Exists);
  parameterWidget()->append(tr("Prospect Number Pattern"), "prospect_number_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Prospect Name Pattern"), "prospect_name_pattern", ParameterWidget::Text);
  parameterWidget()->appendComboBox(tr("Prospect Group"), "pspctgrp", XComboBox::ProspectGroups);
  parameterWidget()->append(tr("Owner"), "owner", ParameterWidget::User);
  parameterWidget()->append(tr("Assigned"), "assigned", ParameterWidget::User);
  parameterWidget()->appendComboBox(tr("Source"), "source", XComboBox::OpportunitySources);
  parameterWidget()->append(tr("Contact Name Pattern"), "cntct_name_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Phone Pattern"), "cntct_phone_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Email Pattern"), "cntct_email_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Street Pattern"), "addr_street_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("City Pattern"), "addr_city_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("State Pattern"), "addr_state_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Postal Code Pattern"), "addr_postalcode_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Country Pattern"), "addr_country_pattern", ParameterWidget::Text);

  if (_privileges->check("MaintainProspectMasters"))
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sEdit()));
  else
  {
    newAction()->setEnabled(false);
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sView()));
  }

  list()->addColumn(tr("Number"),  _orderColumn, Qt::AlignCenter, true, "prospect_number" );
  list()->addColumn(tr("Name"),    -1,           Qt::AlignLeft,   true, "prospect_name"   );
  list()->addColumn(tr("Created"), _dateColumn,  Qt::AlignLeft,   true, "created" );
  list()->addColumn(tr("Owner"),   _userColumn,  Qt::AlignLeft,   true, "prospect_owner_username" );
  list()->addColumn(tr("Assigned"),_userColumn,  Qt::AlignLeft,   true, "prospect_assigned_username" );
  list()->addColumn(tr("Source"),  100,          Qt::AlignLeft,   true, "prospect_source" );
  list()->addColumn(tr("First"),   50,           Qt::AlignLeft,   true, "cntct_first_name" );
  list()->addColumn(tr("Last"),    -1,           Qt::AlignLeft,   true, "cntct_last_name" );
  list()->addColumn(tr("Phone"),   100,          Qt::AlignLeft,   true, "contact_phone" );
  list()->addColumn(tr("Email"),   100,          Qt::AlignLeft,   true, "cntct_email" );
  list()->addColumn(tr("Address"), -1,           Qt::AlignLeft,   false, "addr_line1" );
  list()->addColumn(tr("City"),    75,           Qt::AlignLeft,   false, "addr_city" );
  list()->addColumn(tr("State"),   50,           Qt::AlignLeft,   false, "addr_state" );
  list()->addColumn(tr("Country"), 100,          Qt::AlignLeft,   false, "addr_country" );
  list()->addColumn(tr("Postal Code"), 75,       Qt::AlignLeft,   false, "addr_postalcode" );

  setupCharacteristics("PSPCT");

  list()->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(omfgThis, SIGNAL(prospectsUpdated()), SLOT(sFillList()));
}

void prospects::sNew()
{
  ParameterList params;
  params.append("mode", "new");

  prospect *newdlg = new prospect();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void prospects::sEdit()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("prospect_id", item->id());
    params.append("mode", "edit");

    prospect *newdlg = new prospect();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}

void prospects::sView()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("prospect_id", item->id());
    params.append("mode", "view");

    prospect *newdlg = new prospect();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}

void prospects::sDelete()
{
  if (QMessageBox::question(this, tr("Delete?"),
                            tr("<p>Are you sure you want to delete the "
                               "selected Prospect(s)?"),
                            QMessageBox::Yes,
                            QMessageBox::No | QMessageBox::Default) == QMessageBox::No)
    return;

  XSqlQuery delq;
  delq.prepare("DELETE FROM prospect WHERE (prospect_id=:prospect_id);");

  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    delq.bindValue(":prospect_id", item->id());
    delq.exec();
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error deleting"),
                             delq, __FILE__, __LINE__))
      return;
  }

  sFillList();
  omfgThis->sProspectsUpdated();
}

void prospects::sCreateQuote()
{
  ParameterList params;
  params.append("mode", "newQuote");
  params.append("cust_id", list()->id());

  salesOrder *newdlg = new salesOrder();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void prospects::sCreateProject()
{
  XSqlQuery cntctq;
  ParameterList params;

  params.append("mode", cNew);
  params.append("crmacct_id", list()->altId());
  params.append("username", list()->rawValue("prospect_assigned_username").toString());

  cntctq.prepare("SELECT getcrmaccountcontact(:crmacct) AS ret;");
  cntctq.bindValue(":crmacct", list()->altId());
  cntctq.exec();
  if (cntctq.first())
    params.append("cntct_id", cntctq.value("ret"));
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Determining Contact"),
                             cntctq, __FILE__, __LINE__))
      return;
   
  project* newdlg = new project(0, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  newdlg->show();
}

void prospects::sCreateTask()
{
  ParameterList params;

  params.append("mode", "new");
  params.append("parent", "CRMA");
  params.append("parent_id", list()->altId());
  params.append("parent_owner_username", list()->rawValue("prospect_owner_username").toString());
  params.append("parent_assigned_username", list()->rawValue("prospect_assigned_username").toString());

  task* newdlg = new task(0, "", false);
  newdlg->set(params);
  newdlg->setAttribute(Qt::WA_DeleteOnClose);
  newdlg->show();
}

void prospects::sCreateOpportunity()
{
  XSqlQuery cntctq;
  ParameterList params;
  params.append("mode", "new");
  params.append("crmacct_id",list()->altId());
  params.append("parent_owner_username", list()->rawValue("prospect_owner_username").toString());
  params.append("parent_assigned_username", list()->rawValue("prospect_assigned_username").toString());

  cntctq.prepare("SELECT getcrmaccountcontact(:crmacct) AS ret;");
  cntctq.bindValue(":crmacct", list()->altId());
  cntctq.exec();
  if (cntctq.first())
    params.append("cntct_id", cntctq.value("ret"));
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Determining Contact"),
                             cntctq, __FILE__, __LINE__))
      return;

  opportunity newdlg(0, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void prospects::sPopulateMenu(QMenu * pMenu, QTreeWidgetItem *, int)
{
  QAction *menuItem;

  menuItem = pMenu->addAction("View...", this, SLOT(sView()));

  menuItem = pMenu->addAction("Edit...", this, SLOT(sEdit()));
  menuItem->setEnabled(_privileges->check("MaintainProspectMasters"));

  menuItem = pMenu->addAction("Delete", this, SLOT(sDelete()));
  menuItem->setEnabled(_privileges->check("MaintainProspectMasters"));

  pMenu->addSeparator();

  if (_privileges->check("MaintainPersonalProjects"))
  {
    menuItem = pMenu->addAction(tr("Create Project"), this, SLOT(sCreateProject()));
    menuItem->setEnabled(true);
  }

  menuItem = pMenu->addAction(tr("Create Task"), this, SLOT(sCreateTask()));
  menuItem->setEnabled(true);

  if (_privileges->check("MaintainAllOpportunities MaintainPersonalOpportunities"))
  {
    menuItem = pMenu->addAction(tr("Create Opportunity"), this, SLOT(sCreateOpportunity()));
    menuItem->setEnabled(true);
  }

  if (_privileges->check("MaintainQuotes"))
  {
    menuItem = pMenu->addAction(tr("Create Quote"), this, SLOT(sCreateQuote()));
    menuItem->setEnabled(true);
  }
}
