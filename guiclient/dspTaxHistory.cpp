/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "dspTaxHistory.h"

#include <QMessageBox>
#include <QSqlError>

#include <metasql.h>
#include <openreports.h>

#include "currdisplay.h"
#include "mqlutil.h"
#include "errorReporter.h"
#include "guiErrorCheck.h"

dspTaxHistory::dspTaxHistory(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : XWidget(parent, name, fl)
{
  setupUi(this);

//  (void)statusBar();

  connect(_query, SIGNAL(clicked()), this, SLOT(sFillList()));
  connect(_print, SIGNAL(clicked()), this, SLOT(sPrint()));
  connect(_filterOn, SIGNAL(currentIndexChanged(int)), this, SLOT(sHandleFilter()));
  connect(_showOnlyGroup, SIGNAL(toggled(bool)), this, SLOT(sHandleFilter()));
  connect(_summary, SIGNAL(toggled(bool)), this, SLOT(sHandleType()));
  connect(_sales, SIGNAL(toggled(bool)), this, SLOT(sHandleType()));
  connect(_purchases, SIGNAL(toggled(bool)), this, SLOT(sHandleType()));
  
  QString base = CurrDisplay::baseCurrAbbr();
  _taxdet->addColumn(tr("Source"),              _orderColumn,    Qt::AlignLeft,  true,  "source"   );
  _taxdet->addColumn(tr("Doc. Type"),           _orderColumn,    Qt::AlignLeft,  true,  "doctype"   );
  _taxdet->addColumn(tr("Doc#"),                _orderColumn,    Qt::AlignLeft,  true,  "docnumber"   );
  _taxdet->addColumn(tr("Order#"),              _orderColumn,    Qt::AlignLeft,  false, "ordernumber"   );
  _taxdet->addColumn(tr("Orig. Order#"),        _orderColumn,    Qt::AlignLeft,  false, "orignumber"   );
  _taxdet->addColumn(tr("Doc. Date"),           _dateColumn,     Qt::AlignCenter,true,  "docdate"  );
  _taxdet->addColumn(tr("Orig. Doc. Date"),     _dateColumn,     Qt::AlignCenter,false, "origdate"  );
  _taxdet->addColumn(tr("Number"),              _orderColumn,    Qt::AlignLeft,  false, "number"  );
  _taxdet->addColumn(tr("Name"),                _orderColumn,    Qt::AlignLeft,  false, "name"  );
  _taxdet->addColumn(tr("Currency"),            _currencyColumn, Qt::AlignRight, true,  "curr"  );
  _taxdet->addColumn(tr("Currency \nRate"),     _uomColumn,      Qt::AlignRight, false, "rate"  );
  _taxdet->addColumn(tr("Tax Service"),         _itemColumn,     Qt::AlignLeft,  false, "service"  );
  _taxdet->addColumn(tr("Tax Zone"),            _itemColumn,     Qt::AlignLeft,  false, "taxzone"  );
  _taxdet->addColumn(tr("Tax Zone Descrip."),   -1,              Qt::AlignLeft,  false, "taxzone_descrip"  );
  _taxdet->addColumn(tr("Exemption Code"),      _itemColumn,     Qt::AlignLeft,  false, "exemption"  );
  _taxdet->addColumn(tr("Order Discount"),      _moneyColumn,    Qt::AlignRight, false, "discountlocal"  );
  _taxdet->addColumn(tr("Order Discount %1").arg(base), _moneyColumn, Qt::AlignRight, false, "discountbase"  );
  _taxdet->addColumn(tr("Tax Paid \nto Vendor"), _moneyColumn,    Qt::AlignRight, false, "taxpaidlocal"  );
  _taxdet->addColumn(tr("Tax Paid \nto Vendor %1").arg(base), _moneyColumn, Qt::AlignRight, false, "taxpaidbase"  ); 
  _taxdet->addColumn(tr("Journal#"),            _orderColumn,    Qt::AlignRight, false, "journalnumber"  );
  _taxdet->addColumn(tr("Line"),                _itemColumn,     Qt::AlignLeft,  false, "line_number"  );
  _taxdet->addColumn(tr("Item#"),               _itemColumn,     Qt::AlignLeft,  true,  "item_number"  );
  _taxdet->addColumn(tr("Item Descrip."),       -1,              Qt::AlignLeft,  true,  "item_descrip"  );
  _taxdet->addColumn(tr("Tax Type"),            _itemColumn,     Qt::AlignLeft,  false, "taxtype"  );
  _taxdet->addColumn(tr("Tax Type Descrip."),   -1,              Qt::AlignLeft,  false, "taxtype_descrip"  );
  _taxdet->addColumn(tr("Qty"),                 _qtyColumn,      Qt::AlignRight, false, "qty"  );
  _taxdet->addColumn(tr("Unit Price"),          _moneyColumn,    Qt::AlignRight, false, "amountlocal"  );
  _taxdet->addColumn(tr("Unit Price %1").arg(base), _moneyColumn,Qt::AlignRight, false, "amountbase"  );
  _taxdet->addColumn(tr("Extension"),           _moneyColumn,    Qt::AlignRight, false, "extendedlocal"  );
  _taxdet->addColumn(tr("Extension %1").arg(base), _moneyColumn, Qt::AlignRight, false, "extendedbase"  );
  _taxdet->addColumn(tr("Dist. Date"),          _dateColumn,     Qt::AlignCenter,false, "distdate"  );   
  _taxdet->addColumn(tr("Taxable"),             _moneyColumn,    Qt::AlignRight, true,  "taxablelocal"  );
  _taxdet->addColumn(tr("Taxable %1").arg(base), _moneyColumn,   Qt::AlignRight, false,  "taxablebase"  );
  _taxdet->addColumn(tr("Tax Code"),            _itemColumn,     Qt::AlignLeft,  true,  "tax"  );
  _taxdet->addColumn(tr("Tax Code Descrip."),   -1,              Qt::AlignLeft,  false,  "tax_descrip"  );
  _taxdet->addColumn(tr("Tax Class"),           _itemColumn,     Qt::AlignLeft,  false, "taxclass"  );
  _taxdet->addColumn(tr("Tax Class Descrip."),  -1,              Qt::AlignLeft,  false, "taxclass_descrip"  );
  _taxdet->addColumn(tr("Tax Authority"),       _itemColumn,     Qt::AlignLeft,  false, "taxauth"   );
  _taxdet->addColumn(tr("Tax Authority Descrip."),-1,            Qt::AlignLeft,  false, "taxauth_descrip"   );
  _taxdet->addColumn(tr("Sequence"),            _orderColumn,    Qt::AlignLeft,  false, "taxdetail_sequence"   );
  _taxdet->addColumn(tr("Basis Tax Code"),      _itemColumn,     Qt::AlignLeft,  true,  "basistax"  );
  _taxdet->addColumn(tr("Amount"),              _moneyColumn,    Qt::AlignRight, false, "flatlocal"  );
  _taxdet->addColumn(tr("Amount %1").arg(base), _moneyColumn,    Qt::AlignRight, false, "flatbase"  );
  _taxdet->addColumn(tr("Percent"),             _moneyColumn,    Qt::AlignRight, false, "percent"  );
  _taxdet->addColumn(tr("Sales Tax"),           _moneyColumn,    Qt::AlignRight, true,  "salestaxlocal"  );
  _taxdet->addColumn(tr("Sales Tax %1").arg(base), _moneyColumn, Qt::AlignRight, true,  "salestaxbase"  );
  _taxdet->addColumn(tr("Use Tax"),             _moneyColumn,    Qt::AlignRight, false, "usetaxlocal"  );
  _taxdet->addColumn(tr("Use Tax %1").arg(base), _moneyColumn,   Qt::AlignRight, false, "usetaxbase"  );

  sHandleType();
}

dspTaxHistory::~dspTaxHistory()
{
  // no need to delete child widgets, Qt does it all for us
}

void dspTaxHistory::languageChange()
{
  retranslateUi(this);
}

enum SetResponse dspTaxHistory::set(const ParameterList &pParams)
{
  XWidget::set(pParams);
  QVariant param;
  bool     valid;

  param = pParams.value("type", &valid);
  if (valid)
  {
    if (param.toString() == "detail")
    _detail->setChecked(true);
  }
  
  param = pParams.value("showSales", &valid);
  if (valid)
    _sales->setChecked(param.toBool());
  
  param = pParams.value("showPurchases", &valid);
  if (valid)
    _purchases->setChecked(param.toBool());
  
  param = pParams.value("startDate", &valid);
  if (valid)
    _dates->setStartDate(param.toDate());
    
  param = pParams.value("endDate", &valid);
  if (valid)
    _dates->setEndDate(param.toDate());

  if (pParams.inList("run"))
  {
    sFillList();
    return NoError_Run;
  }

  return NoError;
}


bool dspTaxHistory::setParams(ParameterList &params)
{
  QList<GuiErrorCheck> errors;
    errors<< GuiErrorCheck(!(_sales->isChecked() || _purchases->isChecked()), _sales,
                           tr("You must select sales or purchase items to show.")),
    errors<< GuiErrorCheck(!_dates->startDate().isValid(), _dates,
                           tr("You must enter a valid Start Date to print this report.")),
    errors<< GuiErrorCheck(!_dates->endDate().isValid(), _dates,
                           tr("You must enter a valid End Date to print this report."))
    ;
    if (GuiErrorCheck::reportErrors(this, tr("Cannot Execute Tax History"), errors))
      return false;
  
  if (_metrics->boolean("CashBasedTax"))
    params.append("cashbasedtax", true);
  
  if (_summary->isChecked())
  {
    params.append("type", "summary");
    params.append("summary");
  }
  else
    params.append("type", "detail");
    
  if (_sales->isChecked())
    params.append("showSales");
  if (_purchases->isChecked())
    params.append("showPurchases");
    
  if (_distDate->isChecked())
    params.append("distDate");

  _dates->appendValue(params);
  
  params.append("selection", _selection->currentText());
  if (((_showOnlyGroup->isCheckable() && _showOnlyGroup->isChecked()) || 
       !_showOnlyGroup->isCheckable()) &&
        _selection->id() != -1)
  {
    switch (_filterOn->currentIndex())
    {
      case 0:
        params.append("tax_id",_selection->id());
        break;

      case 1:
        params.append("taxtype_id",_selection->id());
        break;

      case 2:
        params.append("taxclass_id",_selection->id());
        break;
 
      case 3:
        params.append("taxauth_id",_selection->id());
        break;
      
      case 4:
        params.append("taxzone_id",_selection->id());
        break;
    }
  }
    
  if ((_showOnlyGroup->isCheckable() && _showOnlyGroup->isChecked()) || 
       _summary->isChecked())
  {
    switch (_filterOn->currentIndex())
    {
      case 0:
        params.append("groupBy","tax");
        params.append("groupProper",tr("Tax Code"));
        break;

      case 1:
        params.append("groupBy","taxtype");
        params.append("groupProper",tr("Tax Type"));
        break;

      case 2:
        params.append("groupBy","taxclass");
        params.append("groupProper",tr("Tax Class"));
        break;
 
      case 3:
        params.append("groupBy","taxauth");
        params.append("groupProper",tr("Tax Authority"));
        break;
    
      case 4:
        params.append("groupBy","taxzone");
        params.append("groupProper",tr("Tax Zone"));
        break;
    }
  }
  
  params.append("invoice",tr("Invoice"));
  params.append("salescredit",tr("Sales Credit"));
  params.append("arcreditmemo",tr("A/R Credit Memo"));
  params.append("ardebitmemo",tr("A/R Debit Memo"));
  params.append("apcreditmemo",tr("A/P Credit Memo"));
  params.append("apdebitmemo",tr("A/P Debit Memo"));
  params.append("cashreceipt",tr("Cash Receipt"));
  params.append("other",tr("Other"));
  params.append("none",tr("None"));
  params.append("sales",tr("Sales"));
  params.append("purchase",tr("Purchase"));
  params.append("voucher",tr("Voucher"));
  params.append("check",tr("Misc. Check"));
  params.append("expense",tr("Expense"));
  params.append("reversecharge", tr("Reverse Charge"));
  params.append("avalara",tr("Avalara"));
  params.append("freight",tr("Freight"));
  params.append("misc",tr("Misc"));
  params.append("adjustment",tr("Adjustment"));

  return true;
}

void dspTaxHistory::sPrint()
{
  QString name;
  if (_summary->isChecked())
    name="TaxHistorySummary";
  else
    name="TaxHistoryDetail";

  ParameterList params;
  if (! setParams(params))
    return;
    
  orReport report(name, params);
  if (report.isValid())
    report.print();
  else
    report.reportError(this);
}

void dspTaxHistory::sFillList()
{
  XSqlQuery dspFillList;
  ParameterList params;
  if (! setParams(params))
    return;

  _taxsum->clear();
  _taxdet->clear();

  if (_summary->isChecked())
  {
    MetaSQLQuery mql = mqlLoad("taxHistory", "summary");
    dspFillList = mql.toQuery(params);
    dspFillList.exec();
    _taxsum->populate(dspFillList);
  }
  else
  {
    MetaSQLQuery mql = mqlLoad("taxHistory", "detail");
    dspFillList = mql.toQuery(params);
    dspFillList.exec();
    _taxdet->populate(dspFillList);
  }

  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Tax History Information"),
                                dspFillList, __FILE__, __LINE__))
  {
    return;
  }
}

void dspTaxHistory::sHandleFilter()
{ 
  _selection->setType(XComboBox::Adhoc); // Force to re-evaluate if null state changed
  switch (_filterOn->currentIndex())
  {
    case 0:
      _selection->setType(XComboBox::TaxCodes);
      break;

    case 1:
      _selection->setType(XComboBox::TaxTypes);
      break;

    case 2:
      _selection->setType(XComboBox::TaxClasses);
      break;
 
    case 3:
      _selection->setType(XComboBox::TaxAuths);
      break;
      
    case 4:
      _selection->setType(XComboBox::TaxZones);
      break;
  }
  if (_summary->isChecked())
  {
    QString base = CurrDisplay::baseCurrAbbr();
    _taxsum->clear();
    _taxsum->setColumnCount(0);
    
    switch (_filterOn->currentIndex())
    {
      case 0:
        _taxsum->addColumn(tr("Tax Code"), _itemColumn,  Qt::AlignLeft, true, "tax"   );
        break;

      case 1:
        _taxsum->addColumn(tr("Tax Type"), _itemColumn,  Qt::AlignLeft, true, "taxtype"   );
        break;

      case 2:
        _taxsum->addColumn(tr("Tax Class"), _itemColumn,  Qt::AlignLeft, true, "taxclass"   );
        break;
 
      case 3:
        _taxsum->addColumn(tr("Tax Authority"), _itemColumn,  Qt::AlignLeft, true, "taxauth"   );
        break;
      
      case 4:
        _taxsum->addColumn(tr("Tax Zone"), _itemColumn,  Qt::AlignLeft, true, "taxzone"   );
        break;
    }

    _taxsum->addColumn(tr("Description"),               -1,                 Qt::AlignLeft,   true,  "description"   );
    if (_sales->isChecked())
    {
      _taxsum->addColumn(tr("Sales %1").arg(base),        _bigMoneyColumn,    Qt::AlignRight,  true,  "salesbase"  );
      _taxsum->addColumn(tr("Sales Tax %1").arg(base),    _bigMoneyColumn,    Qt::AlignRight,  true,  "salestaxbase"  );
    }
    if (_purchases->isChecked())
    {
      _taxsum->addColumn(tr("Purchases %1").arg(base),    _bigMoneyColumn,    Qt::AlignRight,  true,  "purchasebase"  );
      _taxsum->addColumn(tr("Purchase Tax %1").arg(base), _bigMoneyColumn,    Qt::AlignRight,  true,  "purchasetaxbase"  );
    }
    if (_sales->isChecked() && _purchases->isChecked())
      _taxsum->addColumn(tr("Net Tax %1").arg(base),      _bigMoneyColumn,    Qt::AlignRight,  true,  "nettaxbase"  );
  }
}

void dspTaxHistory::sHandleType()
{ 
  if (_summary->isChecked())
  {
    _showOnlyGroup->setTitle(tr("Summarize by"));
    _showOnlyGroup->setCheckable(false);
    _showOnlyGroup->setEnabled(true);
    _taxdet->hide();
    _taxsum->show();
    _selection->setAllowNull(true);
  }
  else
  {
    _showOnlyGroup->setTitle(tr("Show only tax"));
    _showOnlyGroup->setCheckable(true);
    _showOnlyGroup->setChecked(false);
    _taxsum->hide();
    _taxdet->show();
    _selection->setAllowNull(false);
  }
  sHandleFilter();
}


