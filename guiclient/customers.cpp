/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "customers.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include "customer.h"
#include "customerTypeList.h"
#include "errorReporter.h"
#include "metasql.h"
#include "storedProcErrorLookup.h"
#include "parameterwidget.h"

customers::customers(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "customers", fl)
{
  setUseAltId(true);
  setWindowTitle(tr("Customers"));
  setMetaSQLOptions("customers", "detail");
  setReportName("Customers");
  setParameterWidgetVisible(true);
  setNewVisible(true);
  setSearchVisible(true);
  setQueryOnStartEnabled(true);

  QString holdSql = QString("SELECT 0 AS code, '%1' AS desc "
                            " UNION SELECT 1, '%2' "
                            " UNION SELECT 2, '%3' "
                            " ORDER BY code; ")
          .arg(tr("None"))
          .arg(tr("Credit Warn"))
          .arg(tr("Credit Hold"));

  parameterWidget()->append(tr("Show Inactive"), "showInactive", ParameterWidget::Exists);
  parameterWidget()->append(tr("Customer Number Pattern"), "cust_number_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Customer Name Pattern"), "cust_name_pattern", ParameterWidget::Text);
  parameterWidget()->appendComboBox(tr("Customer Type"), "cust_custtype_id", XComboBox::CustomerTypes);
  parameterWidget()->append(tr("Customer Type Pattern"), "custtype_code_pattern", ParameterWidget::Text);
  parameterWidget()->appendComboBox(tr("Customer Group"), "custgrp", XComboBox::CustomerGroups);
  parameterWidget()->append(tr("Contact Name Pattern"), "cntct_name_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Phone Pattern"), "cntct_phone_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Email Pattern"), "cntct_email_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Street Pattern"), "addr_street_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("City Pattern"), "addr_city_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("State Pattern"), "addr_state_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Postal Code Pattern"), "addr_postalcode_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Country Pattern"), "addr_country_pattern", ParameterWidget::Text);
  parameterWidget()->appendComboBox(tr("Sales Rep."), "salesrep_id", XComboBox::SalesReps);
  parameterWidget()->appendComboBox(tr("Hold Type"), "holdtype", holdSql);

  if (_privileges->check("MaintainCustomerMasters"))
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sEdit()));
  else
  {
    newAction()->setEnabled(false);
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sView()));
  }

  list()->addColumn(tr("Number"), _orderColumn,Qt::AlignLeft,true,  "cust_number");
  list()->addColumn(tr("Active"),_ynColumn, Qt::AlignCenter, false, "cust_active");
  list()->addColumn(tr("Name"),         -1, Qt::AlignLeft,   true,  "cust_name");
  list()->addColumn(tr("Type"),_itemColumn, Qt::AlignLeft,   true,  "custtype_code");
  list()->addColumn(tr("Hold Type"),    -1, Qt::AlignLeft  ,false,  "holdtype");
  list()->addColumn(tr("Bill First"),   50, Qt::AlignLeft  , true,  "bill_first_name" );
  list()->addColumn(tr("Bill Last"),    -1, Qt::AlignLeft  , true,  "bill_last_name" );
  list()->addColumn(tr("Bill Title"),  100, Qt::AlignLeft  , true,  "bill_title" );
  list()->addColumn(tr("Bill Phone"),  100, Qt::AlignLeft  , true,  "bill_phone" );
  list()->addColumn(tr("Bill Fax"),    100, Qt::AlignLeft  , false, "bill_fax" );
  list()->addColumn(tr("Bill Email"),  100, Qt::AlignLeft  , true,  "bill_email" );
  list()->addColumn(tr("Bill Addr. 1"), -1, Qt::AlignLeft  , true,  "bill_line1" );
  list()->addColumn(tr("Bill Addr. 2"), -1, Qt::AlignLeft  , false, "bill_line2" );
  list()->addColumn(tr("Bill Addr. 3"), -1, Qt::AlignLeft  , false, "bill_line3" );
  list()->addColumn(tr("Bill City"),    75, Qt::AlignLeft  , false, "bill_city" );
  list()->addColumn(tr("Bill State"),   50, Qt::AlignLeft  , false, "bill_state" );
  list()->addColumn(tr("Bill Country"),100, Qt::AlignLeft  , false, "bill_country" );
  list()->addColumn(tr("Bill Postal"),  75, Qt::AlignLeft  , false, "bill_postalcode" );
  list()->addColumn(tr("Corr. First"),  50, Qt::AlignLeft  , false, "corr_first_name" );
  list()->addColumn(tr("Corr. Last"),   -1, Qt::AlignLeft  , false, "corr_last_name" );
  list()->addColumn(tr("Corr. Title"), 100, Qt::AlignLeft  , false, "corr_title" );
  list()->addColumn(tr("Corr. Phone"), 100, Qt::AlignLeft  , false, "corr_phone" );
  list()->addColumn(tr("Corr. Fax"),   100, Qt::AlignLeft  , false, "corr_fax" );
  list()->addColumn(tr("Corr. Email"), 100, Qt::AlignLeft  , false, "corr_email" );
  list()->addColumn(tr("Corr. Addr. 1"),-1, Qt::AlignLeft  , false, "corr_line1" );
  list()->addColumn(tr("Corr. Addr. 2"),-1, Qt::AlignLeft  , false, "corr_line2" );
  list()->addColumn(tr("Corr. Addr. 3"),-1, Qt::AlignLeft  , false, "corr_line3" );
  list()->addColumn(tr("Corr. City"),   75, Qt::AlignLeft  , false, "corr_city" );
  list()->addColumn(tr("Corr. State"),  50, Qt::AlignLeft  , false, "corr_state" );
  list()->addColumn(tr("Corr. Country"),100, Qt::AlignLeft , false, "corr_country" );
  list()->addColumn(tr("Corr. Postal"), 75, Qt::AlignLeft  , false, "corr_postalcode" );

  list()->setSelectionMode(QAbstractItemView::ExtendedSelection);

  setupCharacteristics("C");

  connect(omfgThis, SIGNAL(customersUpdated(int, bool)), SLOT(sFillList()));
}

void customers::sNew()
{
  ParameterList params;
  params.append("mode", "new");

  customer *newdlg = new customer();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void customers::sEdit()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("cust_id", item->id());
    params.append("mode", "edit");

    customer *newdlg = new customer();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}

void customers::sView()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("cust_id", item->id());
    params.append("mode", "view");

    customer *newdlg = new customer();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}

void customers::sReassignCustomerType()
{
  ParameterList params;
  params.append("custtype_id", list()->altId());

  customerTypeList *newdlg = new customerTypeList(this, "", true);
  newdlg->set(params);
  int custtypeid = newdlg->exec();
  if ( (custtypeid != -1) && (custtypeid != XDialog::Rejected) )
  {
    XSqlQuery infoq;
    infoq.prepare( "UPDATE custinfo "
               "SET cust_custtype_id=:custtype_id "
               "WHERE (cust_id=:cust_id);" );
    infoq.bindValue(":cust_id", list()->id());
    infoq.bindValue(":custtype_id", custtypeid);
    infoq.exec();
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Setting Customer Type"),
                            infoq, __FILE__, __LINE__))
      return;

    omfgThis->sCustomersUpdated(list()->id(), true);
    sFillList();
  }
}

void customers::sDelete()
{
  if (QMessageBox::question(this, tr("Delete Customer(s)?"),
                           tr("<p>Are you sure that you want to completely "
			      "delete the selected Customer(s)?"),
			   QMessageBox::Yes,
			   QMessageBox::No | QMessageBox::Default) == QMessageBox::No)
    return;

  XSqlQuery delq;
  delq.prepare("DELETE FROM custinfo WHERE (cust_id=:cust_id);");

  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    delq.bindValue(":cust_id", item->id());
    delq.exec();
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting"),
                             delq, __FILE__, __LINE__))
      return;

    omfgThis->sCustomersUpdated(item->id(), true); // TODO: true or false????
  }
}

void customers::sSendEmail()
{
  QList<QVariant> customers;
  foreach (XTreeWidgetItem* item, list()->selectedItems())
    customers.append(item->id());

  ParameterList params;
  params.append("customers", customers);

  MetaSQLQuery optin("SELECT BOOL_AND(cntct_email_optin) AS alloptin "
                      "  FROM custinfo "
                      "  JOIN cntct ON cust_corrcntct_id = cntct_id "
                      " WHERE cust_id IN (-1 "
                      "       <? foreach('customers') ?> "
                      "       , <? value('customers') ?> "
                      "       <? endforeach ?> "
                      "       );");

  XSqlQuery qry = optin.toQuery(params);

  if (qry.first() && !qry.value("alloptin").toBool() &&
      QMessageBox::question(this, tr("Include opt-out?"),
                            tr("Include people who have opted out of email contact?"),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::No) == QMessageBox::No)
      params.append("optinonly");
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error fetching emails"),
                                qry, __FILE__, __LINE__))
    return;

  MetaSQLQuery emails("SELECT string_agg(distinct cntct_email, ',') AS emails "
                      "  FROM custinfo "
                      "  JOIN cntct ON cust_corrcntct_id = cntct_id "
                      " WHERE cust_id IN (-1 "
                      "       <? foreach('customers') ?> "
                      "       , <? value('customers') ?> "
                      "       <? endforeach ?> "
                      "       ) "
                      " <? if exists('optinonly') ?> "
                      "   AND cntct_email_optin "
                      " <? endif ?>;");

  qry = emails.toQuery(params);

  if (qry.first())
    QDesktopServices::openUrl("mailto:" + qry.value("emails").toString());
  else
    ErrorReporter::error(QtCriticalMsg, this, tr("Error fetching emails"),
                         qry, __FILE__, __LINE__);
}

void customers::sPopulateMenu(QMenu * pMenu, QTreeWidgetItem *, int)
{
  QAction *menuItem;

  menuItem = pMenu->addAction("View", this, SLOT(sView()));

  menuItem = pMenu->addAction("Edit", this, SLOT(sEdit()));
  menuItem->setEnabled(_privileges->check("MaintainCustomerMasters"));

  menuItem = pMenu->addAction("Delete", this, SLOT(sDelete()));
  menuItem->setEnabled(_privileges->check("MaintainCustomerMasters"));

  if (list()->selectedItems().count() == 1)
  {
    pMenu->addSeparator();
    menuItem = pMenu->addAction("Reassign Customer Type", this, SLOT(sReassignCustomerType()));
    menuItem->setEnabled(_privileges->check("MaintainCustomerMasters"));
  }

  pMenu->addSeparator();
  pMenu->addAction(tr("Send Email..."), this, SLOT(sSendEmail()));
}

bool customers::setParams(ParameterList &params)
{
  if (!display::setParams(params))
    return false;

  params.append("none",   tr("None"));
  params.append("creditwarn", tr("Credit Warn"));
  params.append("credithold",   tr("Credit Hold"));

  return true;
}
