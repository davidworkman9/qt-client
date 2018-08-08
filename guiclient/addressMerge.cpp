/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "addressMerge.h"
#include "address.h"

#include <QMessageBox>
#include <QSqlError>
#include <QCloseEvent>

#include <metasql.h>
#include "mqlutil.h"
#include "parameterwidget.h"
#include "errorReporter.h"

addressMerge::addressMerge(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : XWidget(parent, name, fl)
{
  setupUi(this);

  connect(_close,     SIGNAL(clicked()), this, SLOT(close()));
  connect(_process,   SIGNAL(clicked()), this, SLOT(sProcess()));
  connect(_query,     SIGNAL(clicked()), this, SLOT(sFillList()));
  connect(_addr,      SIGNAL(populateMenu(QMenu *, QTreeWidgetItem *)), this, SLOT(sPopulateAddrMenu(QMenu *)));
  connect(_srcaddr,   SIGNAL(populateMenu(QMenu *, QTreeWidgetItem *, int)), this, SLOT(sPopulateSrcMenu(QMenu *,QTreeWidgetItem *, int)));
  connect(_addr,      SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(sAddrEdit()));
  connect(_srcaddr,   SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(sSrcAddrEdit()));
  connect(_target,    SIGNAL(valid(bool)), this, SLOT(sHandleProcess()));

  _filter->appendComboBox(tr("Country"), "countries", XComboBox::Countries);
  _filter->append(tr("State"), "states", ParameterWidget::Text);
  _filter->append(tr("City"), "cities", ParameterWidget::Text);
  _filter->append(tr("Postal Code"), "postalcodes", ParameterWidget::Text);

  _addr->addColumn(tr("#"),             50, Qt::AlignLeft, true, "addr_number");
  _addr->addColumn(tr("Street 1"),     150, Qt::AlignLeft, true, "addr_line1");
  _addr->addColumn(tr("Street 2"),     150, Qt::AlignLeft, true, "addr_line2");
  _addr->addColumn(tr("Street 3"),     150, Qt::AlignLeft, true, "addr_line3");
  _addr->addColumn(tr("City"),         100, Qt::AlignLeft, true, "addr_city");
  _addr->addColumn(tr("State"),         50, Qt::AlignLeft, true, "addr_state");
  _addr->addColumn(tr("Postal Code"),   80, Qt::AlignLeft, true, "addr_postalcode");
  _addr->addColumn(tr("Country"),      150, Qt::AlignLeft, true, "addr_country");
  _addr->addColumn(tr("Used Count"),    100, Qt::AlignLeft, true, "addrusecount");

  _srcaddr->addColumn(tr("Address #"),  100, Qt::AlignLeft, true, "addr_number");
  _srcaddr->addColumn(tr("Street 1"),   100, Qt::AlignLeft, true, "addr_line1");
  _srcaddr->addColumn(tr("Street 2"),   100, Qt::AlignLeft, true, "addr_line2");
  _srcaddr->addColumn(tr("Street 3"),   100, Qt::AlignLeft, true, "addr_line3");
  _srcaddr->addColumn(tr("City"),       100, Qt::AlignLeft, true, "addr_city");
  _srcaddr->addColumn(tr("State"),       50, Qt::AlignLeft, true, "addr_state");
  _srcaddr->addColumn(tr("Postal"),      80, Qt::AlignLeft, true, "addr_postalcode");
  _srcaddr->addColumn(tr("Country"),    100, Qt::AlignLeft, true, "addr_country");
  _srcaddr->addColumn(tr("Notes"),      100, Qt::AlignLeft, true, "addr_notes");

  // Column list must match the _srcaddr column numbers/positions
  _colMap << "NA" << "line1" << "line2" << "line3" << "city" << "state" 
          << "postalcode" << "country" << "notes";
  
  sPopulateSources();
  sPopulateTarget();
}

addressMerge::~addressMerge()
{
    // no need to delete child widgets, Qt does it all for us
}

void addressMerge::languageChange()
{
    retranslateUi(this);
}

void addressMerge::sAdd()
{
  sSelect(false);
}

void addressMerge::sAddrEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("addr_id", _addr->id());

  address newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
  sFillList();
}

void addressMerge::sAddrView()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("addr_id", _addr->id());

  address newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void addressMerge::sAddrDelete()
{
  XSqlQuery addressDelete;
  QString question = tr("The delete action cannot be undone. "
                        "Are you sure you want to proceed?");
  if (QMessageBox::question(this, tr("Delete Address?"), question,
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
    return;

  addressDelete.prepare("DELETE FROM addr WHERE addr_id=:addr_id");
  addressDelete.bindValue(":addr_id", _addr->id());
  addressDelete.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Address"),
                                addressDelete, __FILE__, __LINE__))
  {
    return;
  }
  sFillList();
}

void addressMerge::sDeselect(int id)
{
  XSqlQuery addrDeselect;
  addrDeselect.prepare("DELETE FROM addrsel WHERE addrsel_addr_id=:addr_id;");
  addrDeselect.bindValue(":addr_id", id);
  addrDeselect.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deselecting Address"),
                                addrDeselect, __FILE__, __LINE__))
  {
    return;
  }
  sFillList();
}

void addressMerge::sDeselectCntct()
{
  sDeselect(_addr->id());
}

void addressMerge::sDeselectSource()
{
  sDeselect(_srcaddr->id());
}

void addressMerge::sFillList()
{
  ParameterList params;
  params.append("searchText", _search->text());
  params.append("searchStreet", QVariant(_searchStreet->isChecked()));
  params.append("searchCity", QVariant(_searchCity->isChecked()));
  params.append("searchPostal", QVariant(_searchPostal->isChecked()));
  _filter->appendValue(params);

  MetaSQLQuery mql = mqlLoad("addressMerge", "search");
  XSqlQuery qry = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Address Information"),
                              qry, __FILE__, __LINE__))
  {
    return;
  }
  _addr->populate(qry, true);

  sPopulateSources();
  sPopulateTarget();
}

void addressMerge::sHandleProcess()
{
  _process->setEnabled(_target->isValid());
}

void addressMerge::sPopulateAddrMenu(QMenu *pMenu)
{
  XSqlQuery addrPopulateAddrMenu;
  if (_addr->id() == -1)
    return;

  QAction *menuItem;

  if (_addr->altId() == 0) // cNone
  {
    menuItem = pMenu->addAction(tr("Set as Source..."), this, SLOT(sAdd()));
  }

  if (_addr->altId() != 1) // cTarget
  {
    menuItem = pMenu->addAction(tr("Set as Target..."), this, SLOT(sSetTarget()));
  }

  if (_addr->altId() == 1 || // cTarget
      _addr->altId() == 2)  // cSource
  {
    menuItem = pMenu->addAction(tr("Deselect..."), this, SLOT(sDeselectCntct()));
  }

  pMenu->addSeparator();

  menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sAddrEdit()));
  menuItem->setEnabled(_privileges->check("MaintainAddresses"));

  menuItem = pMenu->addAction(tr("View..."), this, SLOT(sAddrView()));
  menuItem->setEnabled(_privileges->check("MaintainAddresses" "ViewAddresses"));

  // Check to see if this address is used, if not add delete action
  addrPopulateAddrMenu.prepare("SELECT addrusecount(:addr_id) > 0 AS used");
  addrPopulateAddrMenu.bindValue(":addr_id", _addr->id());
  addrPopulateAddrMenu.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Adding Delete Action To Menu"),
                                addrPopulateAddrMenu, __FILE__, __LINE__))
  {
    return;
  }
  if (addrPopulateAddrMenu.first() && !addrPopulateAddrMenu.value("used").toBool())
  {
      menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sAddrDelete()));
      menuItem->setEnabled(_privileges->check("MaintainAddresses"));
  }
}

void addressMerge::sPopulateSrcMenu(QMenu *pMenu, QTreeWidgetItem *pItem, int pCol)
{
  Q_UNUSED(pItem);
  QAction *menuItem;
  QString col = "";
  QString menuStr;

  QTreeWidgetItem* header = _srcaddr->headerItem();

  if (pCol > 1)
    col = header->text(pCol); 

  menuStr = tr("Merge %1 to target").arg(col);
  _selectCol = pCol;

  menuItem = pMenu->addAction(tr("Deselect"), this, SLOT(sDeselectSource()));

  if (col.length() > 0)
    menuItem = pMenu->addAction(menuStr, this, SLOT(sSelectCol()));

  pMenu->addSeparator();

  menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sAddrEdit()));
  menuItem->setEnabled(_privileges->check("MaintainAddresses"));

  menuItem = pMenu->addAction(tr("View..."), this, SLOT(sAddrView()));
  menuItem->setEnabled(_privileges->check("ViewAddresses"));
}

void addressMerge::sPopulateSources()
{
  XSqlQuery addrPopulateSources;
  ParameterList params;
  params.append("target", QVariant(false));

  MetaSQLQuery mql = mqlLoad("addressMerge", "populate");
  addrPopulateSources = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Address Information"),
                                addrPopulateSources, __FILE__, __LINE__))
  {
    return;
  }
  _srcaddr->populate(addrPopulateSources);
}

void addressMerge::sPopulateTarget()
{
  XSqlQuery addrPopulateTarget;
  ParameterList params;
  params.append("target", QVariant(true));

  MetaSQLQuery mql = mqlLoad("addressMerge", "populate");
  addrPopulateTarget = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Address Information"),
                                addrPopulateTarget, __FILE__, __LINE__))
  {
    return;
  }

  if (addrPopulateTarget.first())
  {
    _target->setId(addrPopulateTarget.value("addr_id").toInt());
    _targetGroup->setTitle(tr("Target Address (#%1)").arg(addrPopulateTarget.value("addr_number").toString()));
  }
  else
  {
    _target->setId(-1);
    _targetGroup->setTitle(tr("Target Address"));
  }

  sHandleProcess();
}

void addressMerge::sProcess()
{
  XSqlQuery addrProcess("SELECT addrmerge(src.addrsel_addr_id, trgt.addrsel_addr_id  ) AS result "
                      " FROM addrsel src, addrsel trgt "
                      " WHERE trgt.addrsel_target "
                      " AND NOT src.addrsel_target;");

  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Merging Addresses"),
                                addrProcess, __FILE__, __LINE__))
    return;

  sFillList();
}

void addressMerge::sSelect(bool target)
{
  XSqlQuery addrSelect;
  addrSelect.prepare("SELECT addrselect(:source, :target);");
  addrSelect.bindValue(":source", _addr->id());
  addrSelect.bindValue(":target", QVariant(target));
  addrSelect.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Processing Requested Action"),
                                addrSelect, __FILE__, __LINE__))
  {
    return;
  }
  sFillList();
}

void addressMerge::sSelectCol()
{
  XSqlQuery addrSelectCol;
  addrSelectCol.prepare("SELECT addrselectcol(:addr_id, :col_name);");
  addrSelectCol.bindValue(":addr_id", _srcaddr->id());
  addrSelectCol.bindValue(":col_name", _colMap[_selectCol]);
  addrSelectCol.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Processing Requested Action"),
                                addrSelectCol, __FILE__, __LINE__))
  {
    return;
  }

  sPopulateSources();
}

void addressMerge::sSetTarget()
{
  sSelect(true);
}

void addressMerge::sSrcAddrEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("addr_id", _srcaddr->id());

  address newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
  sFillList();
}

void addressMerge::sSrcAddrView()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("addr_id", _srcaddr->id());

  address newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void addressMerge::closeEvent(QCloseEvent *pEvent)
{
  XSqlQuery clear("DELETE FROM addrsel;");
  pEvent->accept();
}
