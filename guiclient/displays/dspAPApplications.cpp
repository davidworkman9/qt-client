/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "dspAPApplications.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include "guiErrorCheck.h"
#include <QSqlError>

#include "apOpenItem.h"
#include "check.h"
#include "voucher.h"
#include "miscVoucher.h"
#include "errorReporter.h"

dspAPApplications::dspAPApplications(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "dspAPApplications", fl)
{
  setupUi(optionsWidget());
  setWindowTitle(tr("A/P Applications"));
  setListLabel(tr("Accounts Payable Applications"));
  setReportName("APApplications");
  setMetaSQLOptions("apApplications", "detail");

  _dates->setStartNull(tr("Earliest"), omfgThis->startOfTime(), true);
  _dates->setEndNull(tr("Latest"),     omfgThis->endOfTime(),   true);
    
  list()->addColumn(tr("Vend. #"),    _orderColumn, Qt::AlignLeft,  true, "vend_number");
  list()->addColumn(tr("Vendor"),               -1, Qt::AlignLeft,  true, "vend_name");
  list()->addColumn(tr("Post Date"),   _dateColumn, Qt::AlignCenter,true, "apapply_postdate");
  list()->addColumn(tr("Source"),      _itemColumn, Qt::AlignCenter,true, "apapply_source_doctype");
  list()->addColumn(tr("Doc #"),      _orderColumn, Qt::AlignRight, true, "apapply_source_docnumber");
  list()->addColumn(tr("Apply-To"),    _itemColumn, Qt::AlignCenter,true, "apapply_target_doctype");
  list()->addColumn(tr("Doc #"),      _orderColumn, Qt::AlignRight, true, "apapply_target_docnumber");
  list()->addColumn(tr("Amount"),     _moneyColumn, Qt::AlignRight, true, "apapply_amount");
  list()->addColumn(tr("Currency"),_currencyColumn, Qt::AlignLeft,  true, "currAbbr");
  list()->addColumn(tr("Amount (in %1)").arg(CurrDisplay::baseCurrAbbr()),_moneyColumn, Qt::AlignRight, true, "base_applied");
  list()->addColumn(tr("Reversed"),      _ynColumn, Qt::AlignRight, false, "apapply_reversed");

}

void dspAPApplications::languageChange()
{
  display::languageChange();
  retranslateUi(this);
}

void dspAPApplications::sViewCheck()
{
  int checkid = list()->currentItem()->id("apapply_source_docnumber");
  if ((checkid == -1) || (checkid == 0))
  {
    XSqlQuery countq;
    countq.prepare("SELECT COUNT(*) AS count "
                 "FROM checkhead "
                 "JOIN checkitem ON (checkhead_id=checkitem_checkhead_id) "
                 "WHERE ((checkhead_number=:number)"
                 "   AND (checkitem_amount=:amount));");
    countq.bindValue(":number", list()->currentItem()->text("apapply_source_docnumber"));
    countq.bindValue(":amount", list()->currentItem()->rawValue("apapply_amount"));
    countq.exec();
    if (countq.first())
    {
      if (countq.value("count").toInt() > 1)
      {
        QMessageBox::warning(this, tr("Check Look-Up Failed"),
                             tr("Found multiple checks with this check number."));
        return;
      }
      else if (countq.value("count").toInt() < 1)
      {
        QMessageBox::warning(this, tr("Check Look-Up Failed"),
                             tr("Could not find the record for this check."));
        return;
      }
    }
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving AP Check Information"),
                                  countq, __FILE__, __LINE__))
    {
      return;
    }

    XSqlQuery chkq;
    chkq.prepare("SELECT checkhead_id "
                 "FROM checkhead "
                 "JOIN checkitem ON (checkhead_id=checkitem_checkhead_id) "
                 "WHERE ((checkhead_number=:number)"
                 "   AND (checkitem_amount=:amount));");
    chkq.bindValue(":number", list()->currentItem()->text("apapply_source_docnumber"));
    chkq.bindValue(":amount", list()->currentItem()->rawValue("apapply_amount"));
    chkq.exec();
    if (chkq.first())
      checkid = chkq.value("checkhead_id").toInt();
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving AP Check Information"),
                                  chkq, __FILE__, __LINE__))
    {
      return;
    }
  }

  ParameterList params;
  params.append("checkhead_id", checkid);
  check *newdlg = new check(this, "check");
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void dspAPApplications::sViewCreditMemo()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("apopen_id", list()->id("apapply_source_docnumber"));
  params.append("docType",   "creditMemo");
  apOpenItem newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void dspAPApplications::sViewDebitMemo()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("apopen_id", list()->id("apapply_target_docnumber"));
  params.append("docType", "debitMemo");
  apOpenItem newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void dspAPApplications::sViewVoucher()
{
  XSqlQuery dspViewVoucher;
  dspViewVoucher.prepare("SELECT vohead_id, COALESCE(pohead_id, -1) AS pohead_id"
                         "  FROM vohead LEFT OUTER JOIN pohead ON (vohead_pohead_id=pohead_id)"
                         " WHERE (vohead_id=:vohead_id);");
  dspViewVoucher.bindValue(":vohead_id", list()->id("apapply_target_docnumber"));
  dspViewVoucher.exec();
  if(dspViewVoucher.first())
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("vohead_id", dspViewVoucher.value("vohead_id").toInt());
    
    if (dspViewVoucher.value("pohead_id").toInt() == -1)
    {
      miscVoucher *newdlg = new miscVoucher();
      newdlg->set(params);
      omfgThis->handleNewWindow(newdlg);
    }
    else
    {
      voucher *newdlg = new voucher();
      newdlg->set(params);
      omfgThis->handleNewWindow(newdlg);
    }
  }
  else
    ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving AP Voucher Information"),
                       dspViewVoucher, __FILE__, __LINE__);
}

void dspAPApplications::sReverseApplication()
{
  XSqlQuery reverseApp;

  reverseApp.prepare("SELECT reverseapapplication(:applyid)");
  reverseApp.bindValue(":applyid", list()->id());
  reverseApp.exec();
  if (reverseApp.first())
    QMessageBox::critical( this, tr("Reverse Application"),
                           tr("A/R Application has been reversed.") );
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Reversing A/P Application"),
                                reverseApp, __FILE__, __LINE__))
  {
    return;
  }

  sFillList();
}

void dspAPApplications::sPopulateMenu(QMenu* pMenu, QTreeWidgetItem*, int)
{
  QAction *menuItem;

  if (list()->currentItem()->rawValue("apapply_source_doctype") == "C")
  {
    menuItem = pMenu->addAction(tr("View Source Credit Memo"), this, SLOT(sViewCreditMemo()));
    menuItem->setEnabled(_privileges->check("MaintainAPMemos") ||
                         _privileges->check("ViewAPMemos"));
  }
  else if (list()->currentItem()->rawValue("apapply_source_doctype") == "K")
  {
    menuItem = pMenu->addAction(tr("View Source Check"), this, SLOT(sViewCheck()));
    menuItem->setEnabled(_privileges->check("MaintainPayments"));
  }

  if (list()->currentItem()->rawValue("apapply_target_doctype") == "D")
  {
    menuItem = pMenu->addAction(tr("View Apply-To Debit Memo"), this, SLOT(sViewDebitMemo()));
    menuItem->setEnabled(_privileges->check("MaintainAPMemos") ||
                         _privileges->check("ViewAPMemos"));
  }
  else if (list()->currentItem()->rawValue("apapply_target_doctype") == "V")
  {
    menuItem = pMenu->addAction(tr("View Apply-To Voucher"), this, SLOT(sViewVoucher()));
    menuItem->setEnabled(_privileges->check("MaintainVouchers") ||
                         _privileges->check("ViewVouchers"));
  }

  if (! list()->currentItem()->rawValue("apapply_reversed").toBool())
  {
    menuItem = pMenu->addAction(tr("Reverse Application"), this, SLOT(sReverseApplication()));
    if (! _privileges->check("ReverseAPApplication"))
      menuItem->setEnabled(false);
  }

}

bool dspAPApplications::setParams(ParameterList & params)
{
  QList<GuiErrorCheck> errors;
  errors<< GuiErrorCheck(! _vendorgroup->isValid(), _vendorgroup,
            tr("You must select the Vendor(s) whose A/R Applications you wish to view."))
        << GuiErrorCheck(!_dates->startDate().isValid(), _dates,
            tr("You must enter a valid Start Date."))
        << GuiErrorCheck(!_dates->endDate().isValid(), _dates,
            tr("You must enter a valid End Date."))
        << GuiErrorCheck(!_showChecks->isChecked() && !_showCreditMemos->isChecked(), _showChecks,
            tr("You must indicate which Document Type(s) you wish to view."))
  ;
  if (GuiErrorCheck::reportErrors(this, tr("Cannot set Parameters"), errors))
    return false;
  
  if (_showChecks->isChecked() && _showCreditMemos->isChecked())
    params.append("doctypeList", "'C', 'K'");
  else if (_showChecks->isChecked())
    params.append("doctypeList", "'K'");
  else if (_showCreditMemos->isChecked())
    params.append("doctypeList", "'C'");
  if (_showChecks->isChecked())
    params.append("showChecks");
  if (_showCreditMemos->isChecked())
    params.append("showCreditMemos");

  _dates->appendValue(params);
  params.append("creditMemo", tr("Credit"));
  params.append("debitMemo",  tr("Debit"));
  params.append("check",      tr("Check"));
  params.append("voucher",    tr("Voucher"));

  _vendorgroup->appendValue(params);

  return true;
}
