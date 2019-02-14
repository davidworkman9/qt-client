/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "dspARApplications.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include "guiErrorCheck.h"
#include <QSqlError>

#include <datecluster.h>

#include "arOpenItem.h"
#include "creditMemo.h"
#include "dspInvoiceInformation.h"
#include "errorReporter.h"

dspARApplications::dspARApplications(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "dspARApplications", fl)
{
  setupUi(optionsWidget());
  setWindowTitle(tr("A/R Applications"));
  setListLabel(tr("A/R Applications"));
  setReportName("ARApplications");
  setMetaSQLOptions("arApplications", "detail");

  _cust->setType(CLineEdit::ActiveCustomers);
  _dates->setStartNull(tr("Earliest"), omfgThis->startOfTime(), true);
  _dates->setEndNull(tr("Latest"), omfgThis->endOfTime(), true);
    
  list()->addColumn(tr("Cust. #"),        _orderColumn, Qt::AlignCenter, true,  "cust_number" );
  list()->addColumn(tr("Customer"),                 -1, Qt::AlignLeft,   true,  "cust_name"   );
  list()->addColumn(tr("Post Date"),       _dateColumn, Qt::AlignCenter, true,  "arapply_postdate" );
  list()->addColumn(tr("Dist. Date"),      _dateColumn, Qt::AlignCenter, true,  "arapply_distdate" );
  list()->addColumn(tr("Source Doc Type"),          10, Qt::AlignCenter, true,  "arapply_source_doctype" );
  list()->addColumn(tr("Source"),	         _itemColumn, Qt::AlignCenter, true,  "doctype" );
  list()->addColumn(tr("Doc #"),          _orderColumn, Qt::AlignCenter, true,  "source" );
  list()->addColumn(tr("Apply-To Doc Type"),        10, Qt::AlignCenter, true,  "arapply_target_doctype" );
  list()->addColumn(tr("Apply-To"),        _itemColumn, Qt::AlignCenter, true,  "targetdoctype" );
  list()->addColumn(tr("Doc #"),          _orderColumn, Qt::AlignCenter, true,  "target" );
  list()->addColumn(tr("Amount"),         _moneyColumn, Qt::AlignRight,  true,  "arapply_applied"  );
  list()->addColumn(tr("Currency"),    _currencyColumn, Qt::AlignLeft,   true,  "currAbbr"   );
  list()->addColumn(tr("Base Amount"), _bigMoneyColumn, Qt::AlignRight,  true,  "base_applied"  );
  list()->addColumn(tr("Reversed"),          _ynColumn, Qt::AlignRight, false,  "arapply_reversed");

  list()->hideColumn(4);
  list()->hideColumn(7);

}

void dspARApplications::languageChange()
{
  display::languageChange();
  retranslateUi(this);
}

void dspARApplications::sViewCreditMemo()
{
  XSqlQuery dspViewCreditMemo;
  ParameterList params;
  params.append("mode", "view");

  dspViewCreditMemo.prepare("SELECT 1 AS type, cmhead_id AS id "
	    "FROM cmhead "
	    "WHERE (cmhead_number=:docnum) "
	    "UNION "
	    "SELECT 2 AS type, aropen_id AS id "
	    "FROM aropen "
	    "WHERE ((aropen_docnumber=:docnum)"
	    "  AND (aropen_doctype IN ('C', 'R')) "
	    ") ORDER BY type LIMIT 1;");
  dspViewCreditMemo.bindValue(":docnum", list()->currentItem()->text(6));
  dspViewCreditMemo.exec();
  if (dspViewCreditMemo.first())
  {
    if (dspViewCreditMemo.value("type").toInt() == 1)
    {
      params.append("cmhead_id", dspViewCreditMemo.value("id"));
      creditMemo* newdlg = new creditMemo();
      newdlg->set(params);
      omfgThis->handleNewWindow(newdlg);
    }
    else if (dspViewCreditMemo.value("type").toInt() == 2)
    {
      params.append("aropen_id", dspViewCreditMemo.value("id"));
      params.append("docType", "creditMemo");
      arOpenItem newdlg(this, "", true);
      newdlg.set(params);
      newdlg.exec();
    }
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving AR Credit Memo Information"),
                                dspViewCreditMemo, __FILE__, __LINE__))
  {
    return;
  }
  else
  {
    QMessageBox::information(this, tr("Credit Memo Not Found"),
			     tr("<p>The Credit Memo #%1 could not be found.")
			     .arg(list()->currentItem()->text(6)));
    return;
  }
}

void dspARApplications::sViewDebitMemo()
{
  XSqlQuery dspViewDebitMemo;
  ParameterList params;

  params.append("mode", "view");
  dspViewDebitMemo.prepare("SELECT aropen_id "
	    "FROM aropen "
	    "WHERE ((aropen_docnumber=:docnum) AND (aropen_doctype='D'));");
  dspViewDebitMemo.bindValue(":docnum", list()->currentItem()->text(9));
  dspViewDebitMemo.exec();
  if (dspViewDebitMemo.first())
  {
    params.append("aropen_id", dspViewDebitMemo.value("aropen_id"));
    params.append("docType", "debitMemo");
    arOpenItem newdlg(this, "", true);
    newdlg.set(params);
    newdlg.exec();
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving AR Debit Memo Information"),
                                dspViewDebitMemo, __FILE__, __LINE__))
  {
    return;
  }
}

void dspARApplications::sViewInvoice()
{
  ParameterList params;

  params.append("mode", "view");
  params.append("invoiceNumber", list()->currentItem()->text(9));
  dspInvoiceInformation* newdlg = new dspInvoiceInformation();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void dspARApplications::sReverseApplication()
{
  XSqlQuery reverseApp;

  reverseApp.prepare("SELECT reversearapplication(:applyid)");
  reverseApp.bindValue(":applyid", list()->id());
  reverseApp.exec();
  if (reverseApp.first())
    QMessageBox::critical( this, tr("Reverse Application"),
                           tr("A/R Application has been reversed.") );
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Reversing A/R Application"),
                                reverseApp, __FILE__, __LINE__))
  {
    return;
  }

  sFillList();
}

void dspARApplications::sPopulateMenu(QMenu* pMenu, QTreeWidgetItem*, int)
{
  QAction *menuItem;

  if (list()->currentItem()->rawValue("arapply_source_doctype") == "C")
  {
    menuItem = pMenu->addAction(tr("View Source Credit Memo"), this, SLOT(sViewCreditMemo()));
    if (! _privileges->check("MaintainARMemos") &&
	! _privileges->check("ViewARMemos"))
      menuItem->setEnabled(false);
  }

  if (list()->currentItem()->rawValue("arapply_target_doctype") == "D")
  {
    menuItem = pMenu->addAction(tr("View Apply-To Debit Memo"), this, SLOT(sViewDebitMemo()));
    if (! _privileges->check("MaintainARMemos") &&
	! _privileges->check("ViewARMemos"))
      menuItem->setEnabled(false);
  }
  else if (list()->currentItem()->rawValue("arapply_target_doctype") == "I")
  {
    menuItem = pMenu->addAction(tr("View Apply-To Invoice"), this, SLOT(sViewInvoice()));
    if (! _privileges->check("MaintainMiscInvoices") &&
	! _privileges->check("ViewMiscInvoices"))
      menuItem->setEnabled(false);
  }

  if (! list()->currentItem()->rawValue("arapply_reversed").toBool())
  {
    menuItem = pMenu->addAction(tr("Reverse Application"), this, SLOT(sReverseApplication()));
    if (! _privileges->check("ReverseARApplication"))
      menuItem->setEnabled(false);
  }
}

bool dspARApplications::setParams(ParameterList & params)
{
  QList<GuiErrorCheck> errors;
  errors<< GuiErrorCheck((_selectedCustomer->isChecked()) && (!_cust->isValid()), _cust,
                         tr("You must select a Customer whose A/R Applications you wish to view."))
        << GuiErrorCheck(!_dates->startDate().isValid(), _dates,
                         tr("You must enter a valid Start Date."))
        << GuiErrorCheck(!_dates->endDate().isValid(), _dates,
                         tr("You must enter a valid End Date."))
        << GuiErrorCheck((!_cashReceipts->isChecked()) && (!_creditMemos->isChecked()), _cashReceipts,
                         tr("You must indicate which Document Type(s) you wish to view."))
  ;
  if (GuiErrorCheck::reportErrors(this, tr("Cannot Set Parameters"), errors))
    return false;
  
  if (_cashReceipts->isChecked())
    params.append("includeCashReceipts");

  if (_creditMemos->isChecked())
    params.append("includeCreditMemos");

  _dates->appendValue(params);
  params.append("creditMemo", tr("C/M"));
  params.append("debitMemo", tr("D/M"));
  params.append("cashdeposit", tr("Cash Deposit"));
  params.append("invoice", tr("Invoice"));
  params.append("cash", tr("C/R"));
  params.append("check", tr("Check"));
  params.append("certifiedCheck", tr("Cert. Check"));
  params.append("masterCard", tr("M/C"));
  params.append("visa", tr("Visa"));
  params.append("americanExpress", tr("AmEx"));
  params.append("discoverCard", tr("Discover"));
  params.append("otherCreditCard", tr("Other C/C"));
  params.append("cash", tr("Cash"));
  params.append("wireTransfer", tr("Wire Trans."));
  params.append("other", tr("Other"));
  params.append("apcheck", tr("A/P Check"));

  if (_selectedCustomer->isChecked())
    params.append("cust_id", _cust->id());
  else if (_selectedCustomerType->isChecked())
    params.append("custtype_id", _customerTypes->id());
  else if (_customerTypePattern->isChecked())
    params.append("custtype_pattern", _customerType->text());

  return true; 
}
