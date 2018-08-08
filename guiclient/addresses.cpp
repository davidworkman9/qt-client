/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "addresses.h"

#include <QAction>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>
#include <QToolBar>
#include <QToolButton>

#include <openreports.h>
#include <metasql.h>

#include "addresses.h"
#include "address.h"
#include "storedProcErrorLookup.h"
#include "parameterwidget.h"
#include "errorReporter.h"
#include "prospect.h"

addresses::addresses(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "addresses", fl)
{
  setReportName("AddressesMasterList");
  setWindowTitle(tr("Addresses"));
  setMetaSQLOptions("addresses", "detail");
  setNewVisible(true);
  setQueryOnStartEnabled(true);
  setParameterWidgetVisible(true);

  _crmacctid = -1;
  _detachAct = 0;

  parameterWidget()->append(tr("Account"), "crmacct_id", ParameterWidget::Crmacct);
  parameterWidget()->appendComboBox(tr("Address Group"), "addrgrp", XComboBox::AddressGroups);
  parameterWidget()->append(tr("Address"), "address", ParameterWidget::Text);
  parameterWidget()->append(tr("City"), "city", ParameterWidget::Text);
  parameterWidget()->append(tr("State"), "state", ParameterWidget::Text);
  parameterWidget()->appendComboBox(tr("Country"), "country", XComboBox::LocaleCountries);
  parameterWidget()->append(tr("Postal Code"), "postalcode", ParameterWidget::Text);
  parameterWidget()->append(tr("Show Inactive"), "showInactive", ParameterWidget::Exists);

  list()->addColumn(tr("Line 1"),	 -1, Qt::AlignLeft, true, "addr_line1");
  list()->addColumn(tr("Line 2"),	 150, Qt::AlignLeft, true, "addr_line2");
  list()->addColumn(tr("Line 3"),	 150, Qt::AlignLeft, true, "addr_line3");
  list()->addColumn(tr("City"),	         150, Qt::AlignLeft, true, "addr_city");
  list()->addColumn(tr("State"),	 75, Qt::AlignLeft, true, "addr_state");
  list()->addColumn(tr("Country"),	 150, Qt::AlignLeft, true, "addr_country");
  list()->addColumn(tr("Postal Code"),   75,Qt::AlignLeft, true, "addr_postalcode");
  list()->addColumn(tr("CRM Account(s)"), 150, Qt::AlignLeft, true, "crmacct");

  list()->setSelectionMode(QAbstractItemView::ExtendedSelection);

  setupCharacteristics("ADDR");

  QToolButton * detachBtn = new QToolButton(this);
  detachBtn->setText(tr("Detach"));
  _detachAct = toolBar()->insertWidget(filterSeparator(), detachBtn);
  _detachAct->setEnabled(false);
  _detachAct->setVisible(false);

  if (_privileges->check("MaintainAddresses"))
  {
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sEdit()));
    connect(list(), SIGNAL(valid(bool)), _detachAct, SLOT(setEnabled(bool)));
  }
  else
  {
    newAction()->setEnabled(false);
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sView()));
  }

}

void addresses::sPopulateMenu(QMenu *pMenu, QTreeWidgetItem*, int)
{
  QAction *menuItem;

  menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sEdit()));
  menuItem->setEnabled(_privileges->check("MaintainAddresses"));

  menuItem = pMenu->addAction(tr("View..."), this, SLOT(sView()));
  menuItem->setEnabled(_privileges->check("ViewAddresses"));

  menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sDelete()));
  menuItem->setEnabled(_privileges->check("MaintainAddresses"));

  // Create, Edit, View Prospect:
  XSqlQuery sql;
  sql.prepare("WITH crmaccts AS ( "
              "SELECT crmaccttypes(crmacctcntctass_crmacct_id)#>>'{prospect}' AS prospectid"
              "  FROM cntct"
              "  JOIN crmacctcntctass ON cntct_id=crmacctcntctass_cntct_id"
              " WHERE cntct_addr_id::TEXT IN (SELECT regexp_split_to_table(:addr_id, ','))"
              "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{customer}' IS NULL) "
              "SELECT EXISTS(SELECT 1"
              "                FROM crmaccts"
              "               WHERE prospectid IS NOT NULL) AS edit,"
              "       EXISTS(SELECT 1"
              "                FROM crmaccts"
              "               WHERE prospectid IS NULL) AS new;");
  QStringList ids;
  foreach (XTreeWidgetItem *item, list()->selectedItems())
    ids << QString::number(item->id());
  sql.bindValue(":addr_id", ids.join(","));
  sql.exec();

  if (sql.first())
  {
    bool editProspectPriv = _privileges->check("MaintainProspectMasters");
    bool viewProspectPriv = _privileges->check("MaintainProspectMasters ViewProspectMasters");
    
    if (sql.value("edit").toBool())
    {
      pMenu->addSeparator();
      menuItem = pMenu->addAction(tr("Edit Prospect"), this, SLOT(sEditProspect()));
      menuItem->setEnabled(editProspectPriv);
      menuItem = pMenu->addAction(tr("View Prospect"), this, SLOT(sViewProspect()));
      menuItem->setEnabled(viewProspectPriv);
    }
    if (sql.value("new").toBool())
    {
      pMenu->addSeparator();
      menuItem = pMenu->addAction(tr("Create Prospect..."), this, SLOT(sNewProspect()));
      menuItem->setEnabled(editProspectPriv);
    }
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Checking CRM Accounts"),
                                sql, __FILE__, __LINE__))
    return;
}

enum SetResponse addresses::set(const ParameterList& pParams)
{
  XWidget::set(pParams);
  QVariant param;
  bool	   valid;
  
  param = pParams.value("showRole", &valid);
  if (valid)
  {
    list()->addColumn(tr("Role"), 80, Qt::AlignLeft, true, "crmrole");
    list()->moveColumn(list()->column("crmrole"), 0);
  }

  sFillList();
  return NoError;
}

int addresses::crmacctId()
{
  return _crmacctid;
}

void addresses::sNew()
{
  ParameterList params;
  params.append("mode", "new");

  address newdlg(0, "", true);
  newdlg.set(params);
  newdlg.setWindowModality(Qt::WindowModal);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void addresses::sEdit()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("mode", "edit");
    params.append("addr_id", item->id());

    address* newdlg = new address(0, "", false);
    newdlg->set(params);
    newdlg->setAttribute(Qt::WA_DeleteOnClose);
    newdlg->show();
  }
}

void addresses::sView()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("addr_id", item->id());

    address* newdlg = new address(0, "", false);
    newdlg->set(params);
    newdlg->setAttribute(Qt::WA_DeleteOnClose);
    newdlg->show();
  }
}

void addresses::sDetach()
{
  int answer = QMessageBox::question(this, tr("Detach Address?"),
			tr("<p>Are you sure you want to detach this Address "
			   "from this Account?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
  if (answer == QMessageBox::Yes)
  {
    int cntctId = list()->id();
    XSqlQuery detq;
    detq.prepare("DELETE FROM  crmacctaddrass "
                 "WHERE crmacctaddrass_crmacct_id=:crmacct_id "
                 " AND crmacctaddrass_addr_id=:addr_id ");
    detq.bindValue(":addr_id", list()->id());
    detq.bindValue(":crmacct_id", _crmacctid);
    detq.exec();
    if (detq.first())
    {
      int returnVal = detq.value("returnVal").toInt();
      if (returnVal < 0)
      {
        ErrorReporter::error(QtCriticalMsg, this, tr("Error detaching Address from Account (%1)")
                        .arg(returnVal),detq, __FILE__, __LINE__);
        return;
      }
      emit cntctDetached(cntctId);
    }
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Detaching Address"),
                                  detq, __FILE__, __LINE__))
      return;

    sFillList();
  }
}

QAction* addresses::detachAction()
{
  return _detachAct;
}

void addresses::setCrmacctid(int crmacctId)
{
  _crmacctid = crmacctId;
  if (_crmacctid == -1)
  {
    parameterWidget()->setDefault(tr("Account"), QVariant(), true);
    _detachAct->setVisible(false);
  }
  else
  {
    parameterWidget()->setDefault(tr("Account"), _crmacctid, true);
    setNewVisible(false);
    _detachAct->setVisible(true);
    list()->hideColumn(list()->column("crmacct"));
  }
}

void addresses::sDelete()
{
  XSqlQuery deleteAddress;
  deleteAddress.prepare("SELECT deleteAddress(:addr_id) AS result;");

  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    deleteAddress.bindValue(":addr_id", item->id());
    deleteAddress.exec();
    if (deleteAddress.first())
    {
      int result = deleteAddress.value("result").toInt();
      if (result < 0)
      {
        QMessageBox::warning(this, tr("Cannot Delete Selected Address"),
                             storedProcErrorLookup("deleteAddress", result));
        return;
      }
    }
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Address"),
                                deleteAddress, __FILE__, __LINE__))
    {
      return;
    }
  }

  sFillList();
}

void addresses::sNewProspect()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("mode", "new");

    QStringList crmaccts;

    XSqlQuery sql;
    sql.prepare("SELECT DISTINCT crmacct_number"
                "  FROM cntct"
                "  JOIN crmacctcntctass ON cntct_id=crmacctcntctass_cntct_id"
                "  JOIN crmacct ON crmacct_id=crmacctcntctass_crmacct_id"
                " WHERE cntct_addr_id=:addr_id"
                "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{customer}' IS NULL"
                "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{prospect}' IS NULL"
                " ORDER BY crmacct_number;");
    sql.bindValue(":addr_id", item->id());
    sql.exec();
    while (sql.next())
      crmaccts << sql.value("crmacct_number").toString();

    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Fetching CRM Accounts"),
                             sql, __FILE__, __LINE__))
      return;

    if (crmaccts.size() == 0)
      continue;

    QString crmacctsel;
    if (crmaccts.size() == 1)
      crmacctsel = crmaccts[0];
    else
    {
      QString addr;
      sql.prepare("SELECT formatAddr(:addr_id) AS addr;");
      sql.bindValue(":addr_id", item->id());
      sql.exec();
      if (sql.first())
        addr = sql.value("addr").toString();
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Formatting Address"),
                                    sql, __FILE__, __LINE__))
        return;

      bool ok;
      crmacctsel = QInputDialog::getItem(this, tr("Multiple CRM Accounts"),
                                         tr("There are Multiple CRM Accounts with\n%1\nas "
                                            "an Address. Please select a CRM Account "
                                            "to use for the "
                                            "new Prospect:").arg(addr), crmaccts, 0, false, &ok);
      if (!ok)
        continue;
    }

    sql.prepare("SELECT crmacct_id"
                "  FROM crmacct"
                " WHERE crmacct_number=:crmacct_number;");
    sql.bindValue(":crmacct_number", crmacctsel);
    sql.exec();
    if (sql.first())
      params.append("crmacct_id", sql.value("crmacct_id").toInt());
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Fetching CRM Accounts"),
                                  sql, __FILE__, __LINE__))
      return;

    prospect *newdlg = new prospect();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}

void addresses::sEditProspect()
{
  sOpenProspect("edit");
}

void addresses::sViewProspect()
{
  sOpenProspect("view");
}

void addresses::sOpenProspect(QString mode)
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("mode", mode);

    QStringList prospects;

    XSqlQuery sql;
    sql.prepare("SELECT DISTINCT prospect_number"
                "  FROM cntct"
                "  JOIN crmacctcntctass ON cntct_id=crmacctcntctass_cntct_id"
                "  JOIN prospect ON crmacctcntctass_crmacct_id=prospect_crmacct_id"
                " WHERE cntct_addr_id=:addr_id"
                "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{customer}' IS NULL"
                "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{prospect}' IS NOT NULL"
                " ORDER BY prospect_number;");
    sql.bindValue(":addr_id", item->id());
    sql.exec();
    while (sql.next())
      prospects << sql.value("prospect_number").toString();

    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Fetching Prospects"),
                             sql, __FILE__, __LINE__))
      return;

    if (prospects.size() == 0)
      continue;

    QString prospectsel;
    if (prospects.size() == 1)
      prospectsel = prospects[0];
    else
    {
      QString addr;
      sql.prepare("SELECT formatAddr(:addr_id) AS addr;");
      sql.bindValue(":addr_id", item->id());
      sql.exec();
      if (sql.first())
        addr = sql.value("addr").toString();
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Formatting Address"),
                                    sql, __FILE__, __LINE__))
        return;

      bool ok;
      prospectsel = QInputDialog::getItem(this, tr("Multiple Prospects"),
                                      tr("There are multiple Prospects with\n%1\nas an Address. "
                                         "Please select a "
                                         "Prospect to edit:").arg(addr), prospects, 0, false, &ok);
      if (!ok)
        continue;
    }

    sql.prepare("SELECT prospect_id"
                "  FROM prospect"
                " WHERE prospect_number=:prospect_number;");
    sql.bindValue(":prospect_number", prospectsel);
    sql.exec();
    if (sql.first())
      params.append("prospect_id", sql.value("prospect_id").toInt());
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Fetching Prospects"),
                                  sql, __FILE__, __LINE__))
      return;

    prospect *newdlg = new prospect();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}
