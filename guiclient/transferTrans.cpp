/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "transferTrans.h"

#include <QMessageBox>
#include <QSqlError>
#include <QValidator>
#include <QVariant>

#include "distributeInventory.h"
#include "inputManager.h"
#include "storedProcErrorLookup.h"
#include "warehouseCluster.h"
#include "errorReporter.h"

transferTrans::transferTrans(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : XWidget(parent, name, fl)
{
  setupUi(this);

//  (void)statusBar();

  connect(_item, SIGNAL(newId(int)), this, SLOT(sHandleItem()));
  connect(_fromWarehouse, SIGNAL(newID(int)), this, SLOT(sPopulateFromQty()));
  connect(_post, SIGNAL(clicked()),                   this, SLOT(sPost()));
  connect(_qty,  SIGNAL(textChanged(const QString&)), this, SLOT(sUpdateQty(const QString&)));
  connect(_toWarehouse, SIGNAL(newID(int)), this, SLOT(sPopulateToQty(int)));

  _captive = false;

  _item->setType(ItemLineEdit::cGeneralInventory | ItemLineEdit::cActive);
  _fromWarehouse->setType(WComboBox::AllActiveInventory);
  _toWarehouse->setType(WComboBox::AllActiveInventory);
  _qty->setValidator(omfgThis->qtyVal());
  _fromBeforeQty->setPrecision(omfgThis->qtyVal());
  _toBeforeQty->setPrecision(omfgThis->qtyVal());
  _fromAfterQty->setPrecision(omfgThis->qtyVal());
  _toAfterQty->setPrecision(omfgThis->qtyVal());

  omfgThis->inputManager()->notify(cBCItem, this, _item, SLOT(setItemid(int)));
  omfgThis->inputManager()->notify(cBCItemSite, this, _item, SLOT(setItemsiteid(int)));

  _item->setFocus();
}

transferTrans::~transferTrans()
{
  // no need to delete child widgets, Qt does it all for us
}

void transferTrans::languageChange()
{
  retranslateUi(this);
}

enum SetResponse transferTrans::set(const ParameterList &pParams)
{
  XSqlQuery transferet;
  XWidget::set(pParams);
  QVariant param;
  bool     valid;
  int      invhistid = -1;

  param = pParams.value("itemsite_id", &valid);
  if (valid)
  {
    _captive = true;

    _item->setItemsiteid(param.toInt());
    _item->setEnabled(false);
    _fromWarehouse->setEnabled(false);
  }

  param = pParams.value("qty", &valid);
  if (valid)
  {
    _captive = true;

    _qty->setText(formatQty(param.toDouble()));
    _qty->setEnabled(false);
  }

  param = pParams.value("invhist_id", &valid);
  if (valid)
    invhistid = param.toInt();

  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "new")
    {
      _mode = cNew;

      _usernameLit->clear();
      _transDate->setEnabled(_privileges->check("AlterTransactionDates"));
      _transDate->setDate(omfgThis->dbDate());
    }
    else if (param.toString() == "view")
    {
      _mode = cView;

      _transDate->setEnabled(false);
      _item->setEnabled(false);
      _toWarehouse->setEnabled(false);
      _fromWarehouse->setEnabled(false);
      _qty->setEnabled(false);
      _documentNum->setEnabled(false);
      _notes->setReadOnly(true);
      _close->setText(tr("&Close"));
      _post->hide();

      transferet.prepare( "SELECT invhist.*, "
                 "       ABS(invhist_invqty) AS transqty,"
                 "       CASE WHEN (invhist_invqty > 0) THEN invhist_qoh_before"
                 "            ELSE NULL"
                 "       END AS qohbefore,"
                 "       CASE WHEN (invhist_invqty > 0) THEN invhist_qoh_after"
                 "            ELSE NULL"
                 "       END AS qohafter,"
                 "       CASE WHEN (invhist_invqty > 0) THEN NULL"
                 "            ELSE invhist_qoh_before"
                 "       END AS fromqohbefore,"
                 "       CASE WHEN (invhist_invqty > 0) THEN NULL"
                 "            ELSE invhist_qoh_after"
                 "       END AS fromqohafter,"
                 "       CASE WHEN (invhist_invqty > 0) THEN itemsite_warehous_id"
                 "            ELSE invhist_xfer_warehous_id"
                 "       END AS toWarehouse,"
                 "       CASE WHEN (invhist_invqty > 0) THEN invhist_xfer_warehous_id"
                 "            ELSE itemsite_warehous_id"
                 "       END AS fromWarehouse "
                 "  FROM invhist, itemsite"
                 " WHERE ((invhist_itemsite_id=itemsite_id)"
                 "   AND  (invhist_id=:invhist_id)); " );
      transferet.bindValue(":invhist_id", invhistid);
      transferet.exec();
      if (transferet.first())
      {
        _transDate->setDate(transferet.value("invhist_transdate").toDate());
        _username->setText(transferet.value("invhist_user").toString());
        _qty->setText(formatQty(transferet.value("transqty").toDouble()));
        _fromBeforeQty->setText(formatQty(transferet.value("fromqohbefore").toDouble()));
        _fromAfterQty->setText(formatQty(transferet.value("fromqohafter").toDouble()));
        _toBeforeQty->setText(formatQty(transferet.value("qohbefore").toDouble()));
        _toAfterQty->setText(formatQty(transferet.value("qohafter").toDouble()));
        _documentNum->setText(transferet.value("invhist_docnumber"));
        _notes->setText(transferet.value("invhist_comments").toString());
        _item->setItemsiteid(transferet.value("invhist_itemsite_id").toInt());
        _toWarehouse->setId(transferet.value("toWarehouse").toInt());
        _fromWarehouse->setId(transferet.value("fromWarehouse").toInt());
      }

    }
  }

  return NoError;
}

void transferTrans::sHandleItem()
{
  if (_item->isFractional())
    _qty->setValidator(omfgThis->transQtyVal());
  else
    _qty->setValidator(new QIntValidator(this));

  sPopulateFromQty();
}

void transferTrans::sPost()
{
  struct {
    bool        condition;
    QString     msg;
    QWidget     *widget;
  } error[] = {
    { ! _item->isValid(),
      tr("You must select an Item before posting this transaction."), _item },
    { _qty->text().length() == 0 || _qty->toDouble() <= 0,
      tr("<p>You must enter a positive Quantity before posting this Transaction."),
      _qty },
    { _fromWarehouse->id() == _toWarehouse->id(),
      tr("<p>The Target Site is the same as the Source Site. "
         "You must select a different Site for each before "
         "posting this Transaction"), _fromWarehouse },
    { true, "", NULL }
  };

  int errIndex;
  for (errIndex = 0; ! error[errIndex].condition; errIndex++)
    ;
  if (! error[errIndex].msg.isEmpty())
  {
    QMessageBox::critical(this, tr("Cannot Post Transaction"),
                          error[errIndex].msg);
    error[errIndex].widget->setFocus();
    return;
  }

  bool fromItemsiteControlled = false;
  bool toItemsiteControlled = false;
  int fromItemsiteId;
  int toItemsiteId;
  int itemlocSeries;
  int itemlocdistId = 0;

  // Stage cleanup function to be called on error
  XSqlQuery cleanup;
  cleanup.prepare("SELECT deleteitemlocseries(:itemlocSeries, TRUE);");

  // Get FROM and TO warehouse itemsites and control values
  XSqlQuery itemsiteInfo;
  itemsiteInfo.prepare("SELECT f.itemsite_id AS from_itemsite_id, "
               "  isControlledItemsite(f.itemsite_id) AS from_controlled, "
               "  t.itemsite_id AS to_itemsite_id, "
               "  isControlledItemsite(t.itemsite_id) AS to_controlled "
               "FROM itemsite AS f, "
               "  itemsite AS t "
               "WHERE f.itemsite_warehous_id=:fromWhId "
               " AND f.itemsite_item_id=:itemId "
               " AND t.itemsite_warehous_id=:toWhId "
               " AND t.itemsite_item_id=:itemId; ");
  itemsiteInfo.bindValue(":itemId", _item->id());
  itemsiteInfo.bindValue(":fromWhId", _fromWarehouse->id());
  itemsiteInfo.bindValue(":toWhId", _toWarehouse->id());
  itemsiteInfo.exec();
  if (itemsiteInfo.first())
  {
    fromItemsiteControlled = itemsiteInfo.value("from_controlled").toBool();
    toItemsiteControlled = itemsiteInfo.value("to_controlled").toBool();
    fromItemsiteId = itemsiteInfo.value("from_itemsite_id").toInt();
    toItemsiteId = itemsiteInfo.value("to_itemsite_id").toInt();
  }
  else
  {
    ErrorReporter::error(QtCriticalMsg, this, tr("Error Finding TO Warehouse Itemsite"),
      itemsiteInfo, __FILE__, __LINE__);
    return;
  }

  // Generate the series and itemlocdist record (if controlled)
  itemlocSeries = distributeInventory::SeriesCreate(fromItemsiteId, _qty->toDouble() * -1, "TW", "TW");
  if (itemlocSeries <= 0)
    return;
  cleanup.bindValue(":itemlocSeries", itemlocSeries);

  // If TO wh is controlled, get the itemlocdist_id from the FROM itemsite SeriesCreate above
  if (fromItemsiteControlled && toItemsiteControlled)
  {
    XSqlQuery itemlocdist;
    itemlocdist.prepare("SELECT itemlocdist_id FROM itemlocdist WHERE itemlocdist_series=:itemlocSeries;");
    itemlocdist.bindValue(":itemlocSeries", itemlocSeries);
    itemlocdist.exec();
    if (itemlocdist.size() != 1)
    {
      cleanup.exec();
      QMessageBox::information(this, tr("Site Transfer"),
                               tr("Error looking up itemlocdist info. Expected 1 record for itemlocdist_series %1").arg(itemlocSeries) );
      return;
    }
    else if (itemlocdist.first())
    {
      itemlocdistId = itemlocdist.value("itemlocdist_id").toInt();
      if (!(itemlocdistId > 0))
      {
        cleanup.exec();
        QMessageBox::information(this, tr("Site Transfer"),
                               tr("Error looking up itemlocdist info. Expected itemlocdist_id to be > 0, not %1 for itemlocdist_series %2")
                               .arg(itemlocdistId).arg(itemlocSeries) );
        return;   
      }
    }
    else 
    {
      cleanup.exec();
      ErrorReporter::error(QtCriticalMsg, this, tr("Error Finding Itemlocdist Info"),
        itemlocdist, __FILE__, __LINE__);
      return;
    }
  }

  if (toItemsiteControlled)
  {
    // FROM itemsite itemlocdist_id is valid, proceed to create itemlocdist record for TO itemsite
    int result = distributeInventory::SeriesCreate(toItemsiteId, _qty->toDouble(), "TW", "TW", 0, itemlocSeries, itemlocdistId);
    if (result != itemlocSeries)
    {
      cleanup.exec();
      return;
    }
  }

  // Distribute detail
  if ((fromItemsiteControlled || toItemsiteControlled) && distributeInventory::SeriesAdjust(itemlocSeries, this, QString(), QDate(), QDate(), true) ==
      XDialog::Rejected)
  {
    cleanup.exec();
    QMessageBox::information( this, tr("Site Transfer"), tr("Detail distribution was cancelled.") );
    return;
  }

  XSqlQuery transferPost;
  transferPost.prepare( "SELECT interWarehouseTransfer(:item_id, :from_warehous_id,"
             "                              :to_warehous_id, :qty, 'Misc', "
             "                              :docNumber, :comments, :itemlocSeries, :date, TRUE, TRUE ) AS result;");
  transferPost.bindValue(":item_id", _item->id());
  transferPost.bindValue(":from_warehous_id", _fromWarehouse->id());
  transferPost.bindValue(":to_warehous_id",   _toWarehouse->id());
  transferPost.bindValue(":qty",              _qty->toDouble());
  transferPost.bindValue(":docNumber", _documentNum->text());
  transferPost.bindValue(":comments", _notes->toPlainText());
  transferPost.bindValue(":date",        _transDate->date());
  transferPost.bindValue(":itemlocSeries", itemlocSeries);
  transferPost.exec();
  if (transferPost.first())
  {
    int result = transferPost.value("result").toInt();
    if (result < 0 || result != itemlocSeries)
    {
      cleanup.exec();
      ErrorReporter::error(QtCriticalMsg, this, tr("Error Posting Inter Warehouse Transfer"),
                             storedProcErrorLookup("interWarehouseTransfer", result),
                             __FILE__, __LINE__);
      return;
    }
    else if (transferPost.lastError().type() != QSqlError::NoError)
    {
      cleanup.exec();
      ErrorReporter::error(QtCriticalMsg, this, tr("Error Posting Inter Warehouse Transfer"),
                           transferPost, __FILE__, __LINE__);
      return;
    }

    if (_captive)
      close();
    else
    {
      _close->setText(tr("&Close"));
      _item->setId(-1);
      _qty->clear();
      _documentNum->clear();
      _notes->clear();
      _fromBeforeQty->clear();
      _fromAfterQty->clear();
      _toBeforeQty->clear();
      _toAfterQty->clear();

      _item->setFocus();
    }
  }
  else
  {
    cleanup.exec();
    ErrorReporter::error(QtCriticalMsg, this, tr("Error Posting Inter Warehouse Transfer"),
                         transferPost, __FILE__, __LINE__);
    return;
  }
}

void transferTrans::sPopulateFromQty()
{
  XSqlQuery transferPopulateFromQty;
  if (_mode != cView && _item->isValid() && _fromWarehouse->isValid())
  {
    transferPopulateFromQty.prepare( "SELECT itemsite_qtyonhand "
               "FROM itemsite "
               "WHERE ( (itemsite_item_id=:item_id)"
               " AND (itemsite_warehous_id=:warehous_id) );" );
    transferPopulateFromQty.bindValue(":item_id", _item->id());
    transferPopulateFromQty.bindValue(":warehous_id", _fromWarehouse->id());
    transferPopulateFromQty.exec();
    if (transferPopulateFromQty.first())
    {
      _cachedFromBeforeQty = transferPopulateFromQty.value("itemsite_qtyonhand").toDouble();
      _fromBeforeQty->setText(formatQty(transferPopulateFromQty.value("itemsite_qtyonhand").toDouble()));

      if (_qty->text().length())
        _fromAfterQty->setText(formatQty(transferPopulateFromQty.value("itemsite_qtyonhand").toDouble() - _qty->toDouble()));
    }
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving ItemSite Information"),
                                  transferPopulateFromQty, __FILE__, __LINE__))
    {
      return;
    }
  }
}

void transferTrans::sPopulateToQty(int pWarehousid)
{
  XSqlQuery transferPopulateToQty;
  if (_mode != cView)
  {
    transferPopulateToQty.prepare( "SELECT itemsite_qtyonhand "
               "FROM itemsite "
               "WHERE ( (itemsite_item_id=:item_id)"
               " AND (itemsite_warehous_id=:warehous_id) );" );
    transferPopulateToQty.bindValue(":item_id", _item->id());
    transferPopulateToQty.bindValue(":warehous_id", pWarehousid);
    transferPopulateToQty.exec();
    if (transferPopulateToQty.first())
    {
      _cachedToBeforeQty = transferPopulateToQty.value("itemsite_qtyonhand").toDouble();
      _toBeforeQty->setText(formatQty(transferPopulateToQty.value("itemsite_qtyonhand").toDouble()));

      if (_qty->text().length())
        _toAfterQty->setText(formatQty(transferPopulateToQty.value("itemsite_qtyonhand").toDouble() + _qty->toDouble()));
    }
  }
}

void transferTrans::sUpdateQty(const QString &pQty)
{
  if (_mode != cView)
  {
    _fromAfterQty->setText(formatQty(_cachedFromBeforeQty - pQty.toDouble()));
    _toAfterQty->setText(formatQty(_cachedToBeforeQty + pQty.toDouble()));
  }
}
