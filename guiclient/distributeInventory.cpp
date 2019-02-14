/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "distributeInventory.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include <metasql.h>
#include "mqlutil.h"

#include "assignLotSerial.h"
#include "distributeToLocation.h"
#include "inputManager.h"
#include "errorReporter.h"
#include "storedProcErrorLookup.h"

#define cIncludeLotSerial   0x01
#define cNoIncludeLotSerial 0x02
#define DEBUG false

distributeInventory::distributeInventory(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : XDialog(parent, name, modal, fl)
{
  setupUi(this);

  connect(_bcDistribute,    SIGNAL(clicked()), this, SLOT(sBcDistribute()));
  connect(_default,         SIGNAL(clicked()), this, SLOT(sDefault()));
  connect(_defaultAndPost,  SIGNAL(clicked()), this, SLOT(sDefaultAndPost()));
  connect(_distribute,      SIGNAL(clicked()), this, SLOT(sSelectLocation()));
  connect(_itemloc, SIGNAL(itemSelected(int)), this, SLOT(sSelectLocation()));
  connect(_post,            SIGNAL(clicked()), this, SLOT(sPost()));
  connect(_taggedOnly,  SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_bc,   SIGNAL(textChanged(QString)), this, SLOT(sBcChanged(QString)));
  connect(_qtyOnly,     SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_zone, SIGNAL(currentIndexChanged(int)), this, SLOT(sFillList()));

  omfgThis->inputManager()->notify(cBCLotSerialNumber, this, this, SLOT(sCatchLotSerialNumber(QString)));

  _item->setReadOnly(true);
  _qtyToDistribute->setPrecision(omfgThis->qtyVal());
  _qtyTagged->setPrecision(omfgThis->qtyVal());
  _qtyRemaining->setPrecision(omfgThis->qtyVal());
  
  _itemloc->addColumn(tr("Location"),     _itemColumn, Qt::AlignLeft,  true, "locationname");
  _itemloc->addColumn(tr("Default"),      _ynColumn,   Qt::AlignLeft,  true, "defaultlocation");
  _itemloc->addColumn(tr("Netable"),      _ynColumn,   Qt::AlignCenter,true, "location_netable");
  _itemloc->addColumn(tr("Usable"),       _ynColumn,   Qt::AlignCenter,true, "location_usable");
  _itemloc->addColumn(tr("Lot/Serial #"), -1,          Qt::AlignLeft,  true, "lotserial");
  _itemloc->addColumn(tr("Expiration"),   _dateColumn, Qt::AlignCenter,true, "f_expiration");
  _itemloc->addColumn(tr("Qty. Before"),  _qtyColumn,  Qt::AlignRight, true, "qty");
  _itemloc->addColumn(tr("Tagged Qty."),  _qtyColumn,  Qt::AlignRight, true, "qtytagged");
  _itemloc->addColumn(tr("Qty. After"),   _qtyColumn,  Qt::AlignRight, true, "balance");
  
  //If not multi-warehouse hide whs control
  if (!_metrics->boolean("MultiWhs"))
  {
    _warehouseLit->hide();
    _warehouse->hide();
  }

// Populate Zone filter dropdown
  updateZoneList();  

  //If not lot serial control, hide info
  if (!_metrics->boolean("LotSerialControl"))
  {
    _lotSerial->hide();
    _lotSerialLit->hide();
    _bcLit->hide();
    _bc->hide();
    _bcQtyLit->hide();
    _bcQty->hide();
    _bcDistribute->hide();
  }

  _itemlocdistid = -1;
  _transtype = "O";
  _locationDefaultLit->hide();
  _locations->hide();
}

distributeInventory::~distributeInventory()
{
  // no need to delete child widgets, Qt does it all for us
}

void distributeInventory::languageChange()
{
  retranslateUi(this);
}

int distributeInventory::SeriesCreate(int itemsiteId, double qty, const QString & orderType, 
  const QString & transType, int orderitemId, int itemlocSeries, int itemlocdistId, int invhistId )
{
  bool controlled = false;

  // Generate a series id to be used for itemlocdist if controlled, and for returnWoMaterial()
  XSqlQuery parentSeries;
  parentSeries.prepare("SELECT COALESCE(:itemlocSeries, NEXTVAL('itemloc_series_seq')) AS result, isControlledItemsite(:itemsiteId) AS controlled;");
  parentSeries.bindValue(":itemsiteId", itemsiteId);
  if (itemlocSeries > 0)
    parentSeries.bindValue(":itemlocSeries", itemlocSeries);
  parentSeries.exec();
  if (parentSeries.first() && parentSeries.value("result").toInt() > 0)
  {
    itemlocSeries = parentSeries.value("result").toInt();
    controlled = parentSeries.value("controlled").toBool();
  }
  else
  {
    ErrorReporter::error(QtCriticalMsg, 0, tr("Failed to Retrieve the Next itemloc_series_seq"),
      parentSeries.lastError().databaseText(), __FILE__, __LINE__);
    return -1;
  }

  if (controlled)
  {
    // Create the itemlocdist record if controlled
    XSqlQuery parentItemlocdist;
    parentItemlocdist.prepare("SELECT createItemlocdistParent(:itemsiteId, :qty, :orderType, :orderitemId, "
                              " :itemlocSeries, :invhistId, :itemlocdistId, :transType) AS result;");
    parentItemlocdist.bindValue(":itemsiteId", itemsiteId);
    parentItemlocdist.bindValue(":qty", qty);
    parentItemlocdist.bindValue(":orderType", orderType);
    parentItemlocdist.bindValue(":transType", transType);
    parentItemlocdist.bindValue(":itemlocSeries", itemlocSeries);
    if (orderitemId > 0)
      parentItemlocdist.bindValue(":orderitemId", orderitemId);
    if (itemlocdistId > 0)
      parentItemlocdist.bindValue(":itemlocdistId", itemlocdistId);
    if (invhistId > 0)
      parentItemlocdist.bindValue(":invhistId", invhistId);
    parentItemlocdist.exec();
    if (ErrorReporter::error(QtCriticalMsg, 0, tr("Error Creating itemlocdist record for post "
        "production controlled item"), parentItemlocdist, __FILE__, __LINE__))
      return -1;
  }

  return itemlocSeries;
}

int distributeInventory::SeriesAdjust(int pItemlocSeries, QWidget *pParent, 
  const QString & pPresetLotnum, const QDate & pPresetLotexp, const QDate & pPresetLotwarr,
  bool pPreDistributed)
{
  int result;
  
  if (DEBUG)
    qDebug() << tr("DistributeInventory::SeriesAdjust pItemlocSeries: %1, pPreDistributed: %2")
    .arg(pItemlocSeries).arg(pPreDistributed);
  
  if (pItemlocSeries != 0)
  {
    XSqlQuery itemloc;
    itemloc.prepare( "SELECT itemlocdist_id, itemlocdist_reqlotserial, itemlocdist_transtype, " 
                     "       itemlocdist_distlotserial, itemlocdist_qty,"
                     "       itemsite_id, itemsite_loccntrl, itemsite_controlmethod,"
                     "       itemsite_perishable, itemsite_warrpurc,"
                     "       COALESCE(itemsite_lsseq_id,-1) AS itemsite_lsseq_id,"
                     "       COALESCE(itemlocdist_source_id,-1) AS itemlocdist_source_id,"
                     // TODO - remove invhist altogether after #22868 is complete
                     "       CASE WHEN (COALESCE(invhist_transtype, itemlocdist_transtype) IN ('RM','RP','RR','RX')) THEN 'R'"
                     "            WHEN (COALESCE(invhist_transtype, itemlocdist_transtype) = 'IM') THEN 'I'"
                     "            WHEN (COALESCE(invhist_transtype, itemlocdist_transtype) = 'SH' "
                     "              AND COALESCE(invhist_ordtype, itemlocdist_order_type) ='SO') THEN 'I'"
                     "            ELSE 'O'"
                     "       END AS trans_type,"
                     "       CASE WHEN (COALESCE(invhist_transtype, itemlocdist_transtype) IN ('RM','RP','RR','RX')"
                     "                  AND itemsite_recvlocation_dist) THEN true"
                     "            WHEN (COALESCE(invhist_transtype, itemlocdist_transtype) = 'IM'"
                     "                  AND itemsite_issuelocation_dist) THEN true"
                     "            WHEN (COALESCE(invhist_transtype, itemlocdist_transtype) NOT IN ('RM','RP','RR','RX','IM')"
                     "                  AND itemsite_location_dist) THEN true"
                     "            ELSE false"
                     "       END AS auto_dist "
                     "FROM itemlocdist JOIN itemsite ON (itemlocdist_itemsite_id=itemsite_id) "
                     "                 LEFT OUTER JOIN invhist ON (itemlocdist_invhist_id=invhist_id) "
                     "WHERE (itemlocdist_series=:itemlocdist_series) "
                     "ORDER BY itemlocdist_id;" );
    itemloc.bindValue(":itemlocdist_series", pItemlocSeries);
    itemloc.exec();
    while (itemloc.next())
    {
      // Requires lot/serial
      if (itemloc.value("itemlocdist_reqlotserial").toBool())
      {
        int itemlocSeries = -1;
        XSqlQuery query;

        // Check to see if this is a lot controlled item and if we have
        // a predefined lot#/expdate to use. If so assign that information
        // with itemlocdist_qty and move on. otherwise do the normal dialog
        // to ask the user for that information.
        if(itemloc.value("itemsite_controlmethod").toString() == "L" && !pPresetLotnum.isEmpty())
        {
          query.exec("SELECT nextval('itemloc_series_seq') AS _itemloc_series;");
          if(query.first())
          {
            itemlocSeries = query.value("_itemloc_series").toInt();
            query.prepare( "SELECT createlotserial(itemlocdist_itemsite_id,:lotserial,:itemlocdist_series,'I',NULL,itemlocdist_id,:qty,:expiration,:warranty) "
                           "FROM itemlocdist "
                           "WHERE (itemlocdist_id=:itemlocdist_id);");
            query.bindValue(":lotserial", pPresetLotnum);
            query.bindValue(":itemlocdist_series", itemlocSeries);
            query.bindValue(":itemlocdist_id", itemloc.value("itemlocdist_id"));
            query.bindValue(":qty", itemloc.value("itemlocdist_qty"));
            if (itemloc.value("itemsite_perishable").toBool())
              query.bindValue(":expiration", pPresetLotexp);
            else
              query.bindValue(":expiration", omfgThis->endOfTime());
            if (itemloc.value("itemsite_warrpurc").toBool())
              query.bindValue(":warranty", pPresetLotwarr);
            
            query.exec();
            if (!query.first() || ErrorReporter::error(QtCriticalMsg, 0, tr("Error Retrieving Lot/Serial Information"),
                                          query, __FILE__, __LINE__))
            {
              return XDialog::Rejected;
            }

            query.prepare( "UPDATE itemlocdist "
                           "SET itemlocdist_source_type='O' "
                           "WHERE (itemlocdist_series=:itemlocdist_series);");
            query.bindValue(":itemlocdist_series", itemlocSeries);
            query.exec();
          }
        }

        // Item did not have a preset lot number.
        // Check to see if Lot/Serial distributions should be created using
        // "from" side of transaction.  Transactions are related by itemlocdist_source_id.
        // InterWarehouseTransfer uses this technique.
        if(itemlocSeries == -1 && itemloc.value("itemlocdist_source_id").toInt() > -1)
        {
          XSqlQuery fromlots;
          fromlots.exec("SELECT nextval('itemloc_series_seq') AS _itemloc_series;");
          if(fromlots.first())
          {
            itemlocSeries = fromlots.value("_itemloc_series").toInt();
            fromlots.prepare("SELECT  createlotserial(s.itemlocdist_itemsite_id, ls_number, "
                             "        :itemlocdist_series, 'I', NULL, :itemlocdist_id, (d.itemlocdist_qty * -1.0), "
                             "        itemloc_expiration, itemloc_warrpurc) "
                             "FROM itemlocdist s JOIN itemlocdist o ON (o.itemlocdist_id=s.itemlocdist_source_id) "
                             "                   JOIN itemlocdist d ON (d.itemlocdist_itemlocdist_id=o.itemlocdist_id) "
                             "                   JOIN itemloc ON (itemloc_id=d.itemlocdist_source_id) "
                             "                   JOIN ls ON (ls_id=itemloc_ls_id) "
                             "WHERE (s.itemlocdist_id=:itemlocdist_id);");
            fromlots.bindValue(":itemlocdist_id", itemloc.value("itemlocdist_id"));
            fromlots.bindValue(":itemlocdist_series", itemlocSeries);
            fromlots.exec();
            if (ErrorReporter::error(QtCriticalMsg, 0, tr("Error Retrieving Lot/Serial Information"),
                                          fromlots, __FILE__, __LINE__))
            {
              return XDialog::Rejected;
            }

            fromlots.prepare("UPDATE itemlocdist "
                             "SET itemlocdist_source_type='O' "
                             "WHERE (itemlocdist_series=:itemlocdist_series);");
            fromlots.bindValue(":itemlocdist_series", itemlocSeries);
            fromlots.exec();
          }
        }

        // Open Assign Lot Serial dialog, populating with auto ls info if required. 
        if (itemlocSeries == -1)
        {
          ParameterList params;
          params.append("itemlocdist_id", itemloc.value("itemlocdist_id").toInt());
          
          // Auto assign lot/serial if applicable
          QStringList exclTransact;
          exclTransact << "TR" << "RR";
          if (itemloc.value("itemsite_lsseq_id").toInt() != -1 &&
              !itemloc.value("itemsite_perishable").toBool() &&
              !itemloc.value("itemsite_warrpurc").toBool() &&
              !exclTransact.contains(itemloc.value("itemlocdist_transtype").toString())) {
            XSqlQuery autocreatels;
            autocreatels.prepare("SELECT autocreatels(:itemlocdist_id) AS itemlocseries;");
            autocreatels.bindValue(":itemlocdist_id", itemloc.value("itemlocdist_id").toInt());
            autocreatels.exec();
            if (autocreatels.first())
              params.append("itemlocseries", autocreatels.value("itemlocseries").toInt());
            else if (ErrorReporter::error(QtCriticalMsg, 0, tr("Error Retrieving Lot/Serial Information"),
                                          autocreatels, __FILE__, __LINE__))
            {
              return XDialog::Rejected;
            }
          }
          assignLotSerial newdlg(pParent, "", true);
          newdlg.set(params);
          itemlocSeries = newdlg.exec();
          if (itemlocSeries == XDialog::Rejected)
            return XDialog::Rejected;
        }
        
        // Distribute location with Distribute Inventory dialog
        if (itemloc.value("itemsite_loccntrl").toBool())
        {
          query.prepare( "SELECT itemlocdist_id " 
                         "FROM itemlocdist "
                         "WHERE (itemlocdist_series=:itemlocdist_series) "
                         "ORDER BY itemlocdist_id;" );
          query.bindValue(":itemlocdist_series", itemlocSeries);
          query.exec();
          while (query.next())
          {
            ParameterList params;
            params.append("itemlocdist_id", query.value("itemlocdist_id").toInt());
            params.append("trans_type", itemloc.value("trans_type").toString());
            distributeInventory newdlg(pParent, "", true);
            newdlg.set(params);
            if (itemloc.value("auto_dist").toBool())
            {
              if (newdlg.sDefaultAndPost())
              {
                if (DEBUG)
                  qDebug() << tr("Pre-22868 itemlocdist_id array for distributeToLocations(). "
                                 "Auto Dist + Default Post: ildList.append(%1) - (itemlocdist_id from "
                                 "itemloc query above)").arg(itemloc.value("itemlocdist_id").toInt());
              }
              else
              {
                result = newdlg.exec();
                if (result == XDialog::Rejected)
                  return XDialog::Rejected;
              }
            }
            else
            {
              result = newdlg.exec();
              if (result == XDialog::Rejected)
                return XDialog::Rejected;
            }

            if (DEBUG && result > 0)
              qDebug() << tr("Pre-22868 itemlocdist_id array for distributeToLocations(). "
                             "Auto Dist: ildList.append(%1) - (itemlocdist_id from "
                             "distributeInventory newdlg)").arg(result);
          }
        }
        else
        {
          query.prepare( "UPDATE itemlocdist "
                         "SET itemlocdist_source_type='L', itemlocdist_source_id = -1 "
                         "WHERE (itemlocdist_series=:itemlocdist_series); ");
          query.bindValue(":itemlocdist_series", itemlocSeries);
          query.exec();

          // Append id to list and process at the end
          if (DEBUG)
            qDebug() << tr("Pre-22868 itemloc_series array for distributeItemlocSeries(). "
                           "Not Location Controlled: ildsList.append(%1) - (itemlocSeries)").
                           arg(itemlocSeries);
        }

        // Set itemlocdist_child_series of parent itemlocdist record
        query.prepare("UPDATE itemlocdist SET itemlocdist_child_series = :itemlocSeries "
                      "WHERE itemlocdist_id = :itemlocdist_id;");
        query.bindValue(":itemlocSeries", itemlocSeries);
        query.bindValue(":itemlocdist_id", itemloc.value("itemlocdist_id").toInt());
        query.exec();
      }
      else // NOT a lot/serial controlled item (or a neg. qty, requiring "distribution from")
      {
        ParameterList params;
        params.append("itemlocdist_id", itemloc.value("itemlocdist_id").toInt());
        params.append("trans_type", itemloc.value("trans_type").toString());

        if (itemloc.value("itemlocdist_distlotserial").toBool())
          params.append("includeLotSerialDetail");

        distributeInventory newdlg(pParent, "", true);
        newdlg.set(params);
        if (itemloc.value("auto_dist").toBool())
        {
          if (newdlg.sDefaultAndPost())
          {
            if (DEBUG)
              qDebug() << tr("Pre-22868 itemlocdist_id array for distributeToLocations(). "
                             "Auto Dist + Default Post: ildList.append(%1) - (itemlocdist_id from "
                             "itemloc query above)").arg(itemloc.value("itemlocdist_id").toInt());
          }
          else
          {
            result = newdlg.exec();
            if (result == XDialog::Rejected)
              return XDialog::Rejected;
          }
        }
        else
        {
          result = newdlg.exec();
          if (result == XDialog::Rejected)
            return XDialog::Rejected;
        }

        if (DEBUG && result > 0)
          qDebug() << tr("Pre-22868 itemlocdist_id array for distributeToLocations(). "
                         "Auto Dist: ildList.append(%1) - (itemlocdist_id from "
                         "distributeInventory newdlg)").arg(result);

        // Set itemlocdist_child_series of parent itemlocdist record. In this case there is no child, set it to itself.
        XSqlQuery query;
        query.prepare("UPDATE itemlocdist SET itemlocdist_child_series = itemlocdist_series "
                      "WHERE itemlocdist_id = :itemlocdist_id AND itemlocdist_qty < 0;"); //itemlocdist_id = :itemlocdist_id
        query.bindValue(":itemlocdist_id", itemloc.value("itemlocdist_id").toInt());
        query.exec();
        if (query.first())
        {
          if (query.numRowsAffected() != 1)
          {
            ErrorReporter::error(QtCriticalMsg, 0, tr("Updating Itemlocdist Parent Record Should Return One Row"),
                           query, __FILE__, __LINE__);
            return XDialog::Rejected;
          }
        }  
        else if (ErrorReporter::error(QtCriticalMsg, 0, tr("Error Updating itemlocdist Lot/Serial Information"),
                                      query, __FILE__, __LINE__))
          return XDialog::Rejected;
      }
    }

    // Post distribution detail here (pre-incident #28868)  
    if (!pPreDistributed)
    {
      XSqlQuery postDistDetail;
      postDistDetail.prepare("SELECT postdistdetail(:itemlocSeries) AS result;");
      postDistDetail.bindValue(":itemlocSeries", pItemlocSeries);
      postDistDetail.exec();
      if (postDistDetail.first())
      {
        // If result = 0 (no dist. records were posted) and there are itemlocdist records throw error
        if (postDistDetail.value("result").toInt() <= 0 && itemloc.size() > 0)
        {
          ErrorReporter::error(QtCriticalMsg, 0, tr("Posting distribution detail returned 0 results"),
            postDistDetail, __FILE__, __LINE__);
          return XDialog::Rejected;
        }
      }
      else 
      {
        ErrorReporter::error(QtCriticalMsg, 0, tr("Distribution Detail Posting Failed"),
          postDistDetail, __FILE__, __LINE__);
        return XDialog::Rejected;
      }
    }
  }
  return XDialog::Accepted;
}

enum SetResponse distributeInventory::set(const ParameterList &pParams)
{
  XDialog::set(pParams);
  QVariant param;
  bool     valid;

  param = pParams.value("includeLotSerialDetail", &valid);
  if (valid && _metrics->boolean("LotSerialControl"))
  {
    _mode = cIncludeLotSerial;
  }
  else
    _mode = cNoIncludeLotSerial;

  param = pParams.value("trans_type", &valid);
  if (valid)
  {
    _transtype = param.toString();
    populate();
  }

  param = pParams.value("itemlocdist_id", &valid);
  if (valid)
  {
    _itemlocdistid = param.toInt();
    populate();
  }

  return NoError;
}

void distributeInventory::closeEvent(QCloseEvent *pEvent)
{
  pEvent->accept();
}

void distributeInventory::populate()
{
  XSqlQuery distributepopulate;
  distributepopulate.prepare("SELECT itemsite_controlmethod "
	    "FROM itemsite, itemlocdist "
	    "WHERE ((itemlocdist_itemsite_id=itemsite_id)"
	    "  AND  (itemlocdist_id=:itemlocdist_id));");

  distributepopulate.bindValue(":itemlocdist_id", _itemlocdistid);
  distributepopulate.exec();
  if (distributepopulate.first())
  {
    _controlMethod = distributepopulate.value("itemsite_controlmethod").toString();
    _bc->setEnabled(_controlMethod == "L" || _controlMethod == "S");
    _bcQty->setEnabled(_controlMethod == "L");
    _bcDistribute->setEnabled(_controlMethod == "L" || _controlMethod == "S");
    if (_controlMethod == "S")
      _bcQty->setText("1");
    else
      _bcQty->clear();
  }
  else if (ErrorReporter::error(QtCriticalMsg, 0, tr("Error Retrieving Lot/Serial Information"),
                                distributepopulate, __FILE__, __LINE__))
  {
    return;
  }

//  Auto distribute location reservations
  if ( (_metrics->boolean("EnableSOReservationsByLocation")) && (_mode == cIncludeLotSerial) )
  {
    distributepopulate.prepare("SELECT distributeToReservedItemLoc(:itemlocdist_id) AS result;");
    distributepopulate.bindValue(":itemlocdist_id", _itemlocdistid);
    distributepopulate.exec();
  }

  sFillList();
  if (_metrics->boolean("SetDefaultLocations"))
    sPopulateDefaultSelector();
  
}

void distributeInventory::sSelectLocation()
{
  ParameterList params;
  params.append("source_itemlocdist_id", _itemlocdistid);

  if (_itemloc->altId() == cLocation)
    params.append("location_id", _itemloc->id());
  else if (_itemloc->altId() == cItemloc)
    params.append("itemlocdist_id", _itemloc->id());

  params.append("itemsite_controlmethod", _controlMethod);
  distributeToLocation newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() == XDialog::Accepted)
    sFillList();
}

void distributeInventory::sPost()
{
  if (_qtyRemaining->toDouble() != 0.0)
  {
    QMessageBox::critical( this, tr("Cannot Perform Partial Distribution"),
                           tr( "<p>You must completely distribute the quantity "
			       "before selecting the 'Post' button." ) );
    return;
  }

  // Append id to the list and process at the end   
  done(_itemlocdistid);
}

bool distributeInventory::sDefault()
{
  XSqlQuery distributeDefault;
   bool distribOk = true;
   double qty = 0.0;
   double availToDistribute = 0.0;
   int defaultlocid = 0;

   distributeDefault.prepare("SELECT itemsite_location_id, itemsite_recvlocation_id, itemsite_issuelocation_id "
             "   FROM   itemlocdist, itemsite "
             "  WHERE ( (itemlocdist_itemsite_id=itemsite_id)"
             "    AND (itemlocdist_id=:itemlocdist_id) ) ");
   distributeDefault.bindValue(":itemlocdist_id", _itemlocdistid);
   distributeDefault.exec();
   if (distributeDefault.first())
   {
     if (_transtype == "R")
       defaultlocid = distributeDefault.value("itemsite_recvlocation_id").toInt();
     else if (_transtype == "I")
       defaultlocid = distributeDefault.value("itemsite_issuelocation_id").toInt();
     else
       defaultlocid = distributeDefault.value("itemsite_location_id").toInt();
   }

   distributeDefault.prepare("SELECT   itemlocdist_qty AS qty, "
             "         qtyLocation(location_id, NULL, NULL, NULL, itemsite_id, itemlocdist_order_type, itemlocdist_order_id, itemlocdist_id) AS availToDistribute "
             "   FROM   itemlocdist, location, itemsite "
             "  WHERE ( (itemlocdist_itemsite_id=itemsite_id)"
             "    AND (itemsite_loccntrl)"
             "    AND (itemsite_warehous_id=location_warehous_id)"
             "    AND (location_id=:defaultlocid) "
             "    AND (itemlocdist_id=:itemlocdist_id) ) ");
   distributeDefault.bindValue(":itemlocdist_id", _itemlocdistid);
   distributeDefault.bindValue(":defaultlocid", defaultlocid);
   distributeDefault.exec();
   if (distributeDefault.first())
   {
       qty = distributeDefault.value("qty").toDouble();
       availToDistribute = distributeDefault.value("availToDistribute").toDouble();
   }

   if(qty < 0 && availToDistribute < qAbs(qty))
   {
       if (QMessageBox::question(this,  tr("Distribute to Default"),
                                tr("It appears you are trying to distribute more "
                                   "than is available to be distributed. "
                                   "<p>Are you sure you want to distribute this quantity?"),
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            distribOk = true;
       else
            distribOk = false;
   }
   if(distribOk)
   {
      if(_mode == cIncludeLotSerial)
        distributeDefault.prepare("SELECT distributeToDefaultItemLoc(:itemlocdist_id, :transtype) AS result;");
      else
        distributeDefault.prepare("SELECT distributeToDefault(:itemlocdist_id, :transtype) AS result;");
      distributeDefault.bindValue(":itemlocdist_id", _itemlocdistid);
      distributeDefault.bindValue(":transtype", _transtype);
      distributeDefault.exec();
      if (distributeDefault.first())
      {
        int result = distributeDefault.value("result").toInt();
        if (result < 0)
        {
          ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Lot/Serial Information"),
                                 storedProcErrorLookup("distributetodefault", result),
                                 __FILE__, __LINE__);
          
          return false;
        }
      }
      else if (ErrorReporter::error(QtWarningMsg, this, tr("Error Retrieving Lot/Serial Information"),
                              distributeDefault, __FILE__, __LINE__))
      {
        return false;
      }
      sFillList();
      //prevent default from been changed after default distribute
      //stopping the operator from thinking the inventory has been posted
      //to the new default
      _locations->setEnabled(false);
      return true;
    }
    else
       return false;
}

bool distributeInventory::sDefaultAndPost()
{
  if(sDefault())
  {
    sPost();
    return true;
  }
  else
    return false;
}

void distributeInventory::sFillList()
{
  XSqlQuery distributeFillList;
  distributeFillList.prepare( "SELECT itemsite_id, "
             "       COALESCE(itemsite_location_id,-1) AS itemsite_location_id,"
             "       formatlotserialnumber(itemlocdist_ls_id) AS lotserial,"
             "       itemlocdist_order_type || ' ' || formatOrderLineItemNumber(itemlocdist_order_type, itemlocdist_order_id) AS order, "
             "       (itemsite_controlmethod IN ('L', 'S')) AS lscontrol,"
             "       parent.itemlocdist_qty AS qtytodistribute,"
             "       ( ( SELECT COALESCE(SUM(child.itemlocdist_qty), 0)"
             "             FROM itemlocdist AS child"
             "            WHERE (child.itemlocdist_itemlocdist_id=parent.itemlocdist_id) ) ) AS qtytagged,"
             "       (parent.itemlocdist_qty - ( SELECT COALESCE(SUM(child.itemlocdist_qty), 0)"
             "                                     FROM itemlocdist AS child"
             "                                    WHERE (child.itemlocdist_itemlocdist_id=parent.itemlocdist_id) ) ) AS qtybalance "
             "FROM itemsite, itemlocdist AS parent "
             "WHERE ( (itemlocdist_itemsite_id=itemsite_id)"
             " AND (itemlocdist_id=:itemlocdist_id) );" );
  distributeFillList.bindValue(":itemlocdist_id", _itemlocdistid);
  distributeFillList.exec();
  if (distributeFillList.first())
  {
    _item->setItemsiteid(distributeFillList.value("itemsite_id").toInt());
    _lotSerial->setText(distributeFillList.value("lotserial").toString());
    _order->setText(distributeFillList.value("order").toString());
    _qtyToDistribute->setDouble(distributeFillList.value("qtytodistribute").toDouble());
    _qtyTagged->setDouble(distributeFillList.value("qtytagged").toDouble());
    _qtyRemaining->setDouble(distributeFillList.value("qtybalance").toDouble());

    if ( (distributeFillList.value("itemsite_location_id").toInt() != -1) &&
         ( (_mode == cNoIncludeLotSerial) || ( (_mode == cIncludeLotSerial) && (!distributeFillList.value("lscontrol").toBool()) ) ) )
    {
      _default->setEnabled(true);
      _defaultAndPost->setEnabled(true);
    }
    else
    {
      _default->setEnabled(false);
      _defaultAndPost->setEnabled(false);
    }

    ParameterList params;

    if (_mode == cNoIncludeLotSerial)
      params.append("cNoIncludeLotSerial");
    else if (_mode == cIncludeLotSerial)
      params.append("cIncludeLotSerial");

    if (_taggedOnly->isChecked())
      params.append("showOnlyTagged");
      
    if (_qtyOnly->isChecked())
      params.append("showQtyOnly");

    params.append("locationType",   cLocation);
    params.append("itemlocType",    cItemloc);
    params.append("yes",            tr("Yes"));
    params.append("no",             tr("No"));
    params.append("na",             tr("N/A"));
    params.append("undefined",      tr("Undefined"));
    params.append("itemlocdist_id", _itemlocdistid);
    params.append("itemsite_id",    distributeFillList.value("itemsite_id").toInt());
    params.append("transtype",      _transtype);

    if (_zone->id() > 0)
      params.append("zone",         _zone->id());

    MetaSQLQuery mql = mqlLoad("distributeInventory", "locations");
    distributeFillList = mql.toQuery(params);

    _itemloc->populate(distributeFillList, true);
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Lot/Serial Information"),
                                  distributeFillList, __FILE__, __LINE__))
    {
      return;
    }
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Lot/Serial Information"),
                                distributeFillList, __FILE__, __LINE__))
  {
    return;
  }
}

void distributeInventory::sBcDistribute()
{
  XSqlQuery distributeBcDistribute;
  if (_bc->text().isEmpty())
  {
    QMessageBox::warning(this, tr("No Bar Code scanned"),
			 tr("<p>Cannot search for Items by Bar Code without a "
			    "Bar Code."));
    _bc->setFocus();
    return;
  }

  distributeBcDistribute.prepare( "SELECT itemloc_id "
	     "FROM  itemlocdist, itemloc, itemsite, ls "
	     "WHERE ((itemlocdist_itemsite_id=itemloc_itemsite_id)"
	     "  AND  (itemloc_itemsite_id=itemsite_id)"
             "  AND  (itemsite_controlmethod IN ('L', 'S'))"
             "  AND  (itemsite_item_id=ls_item_id)"
             "  AND  (itemloc_ls_id=ls_id)"
             "  AND  (itemsite_warehous_id=:warehous_id)"
	     "  AND  (ls_number=:lotserial)"
	     "  AND  (itemlocdist_id=:itemlocdist_id));");

  distributeBcDistribute.bindValue(":itemlocdist_id", _itemlocdistid);
  distributeBcDistribute.bindValue(":lotserial",      _bc->text());
  distributeBcDistribute.bindValue(":warehous_id",    _warehouse->id());
  distributeBcDistribute.exec();

  if(!distributeBcDistribute.first())
  {
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Lot/Serial Information"),
                                  distributeBcDistribute, __FILE__, __LINE__))
    {
      return;
    }
    QMessageBox::warning(this, tr("No Match Found"),
			 tr("<p>No available lines match the specified Barcode.")); 
    _bc->clear();
    return;
  }

  ParameterList params;
  params.append("itemlocdist_id",        distributeBcDistribute.value("itemloc_id"));
  params.append("source_itemlocdist_id", _itemlocdistid);
  params.append("qty",                   _bcQty->text());
  params.append("distribute");

  params.append("itemsite_controlmethod", _controlMethod);

  distributeToLocation newdlg(this, "", true);
  if (newdlg.set(params) != NoError)
    return;

  _bc->clear();
  if (_controlMethod == "S")
    _bcQty->setText("1");
  else
    _bcQty->clear();
  sFillList();
  _bc->setFocus();
}

void distributeInventory::sCatchLotSerialNumber(const QString plotserial)
{
  _bc->setText(plotserial);
  if (_controlMethod == "S")
    _bcDistribute->setFocus();
  else
    _bcQty->setFocus();
}

void distributeInventory::sBcChanged(const QString p)
{
  _post->setDefault(p.isEmpty());
  _bcDistribute->setDefault(! p.isEmpty());
}

void distributeInventory::sPopulateDefaultSelector()
{
   XSqlQuery query;
   query.prepare(" SELECT itemsite_id, itemsite_loccntrl, itemsite_location_id"
                 "  FROM itemsite, itemlocdist"
                 "  WHERE (itemsite_id=itemlocdist_itemsite_id)"
                 "    AND (itemlocdist_id=:itemlocdist_id)");
   query.bindValue(":itemlocdist_id", _itemlocdistid);
   query.exec();
   if(query.first())
   {
      _itemsite_id = query.value("itemsite_id").toInt();
      if(_itemsite_id > -1
         && query.value("itemsite_loccntrl").toBool()
         && query.value("itemsite_location_id").toInt() != -1)
        {
            _locationDefaultLit->show();
            _locations->show();
            XSqlQuery loclist;
            loclist.prepare( " SELECT location_id, formatLocationName(location_id) AS locationname "
                           " FROM location "
                           " WHERE ( (location_warehous_id=:warehous_id)"
                           "   AND (NOT location_restrict) "
                           "   AND (location_active) ) "
                           " UNION SELECT location_id, formatLocationName(location_id) AS locationname "
                           "  FROM location, locitem, itemsite"
                           "  WHERE ( (location_warehous_id=:warehous_id)"
                           "    AND (location_restrict)"
                           "    AND (location_active)"
                           "    AND (locitem_location_id=location_id)"
                           "    AND (locitem_item_id=itemsite_item_id)"
                           "    AND (itemsite_id=:itemsite_id))"
                           " ORDER BY locationname;" );
            loclist.bindValue(":warehous_id", _warehouse->id());
            loclist.bindValue(":itemsite_id", _itemsite_id);
            loclist.exec();
            _locations->populate(loclist);
            if (!loclist.first())
            {
                _locationDefaultLit->hide();
                _locations->hide();
            }
            else
            {
               //Allow default location update with correct privileges
               if (_privileges->check("MaintainItemSites"))
                   _locations->setEnabled(true);
               else
                   _locations->setEnabled(false);

               XSqlQuery dfltLocation;
               dfltLocation.prepare( " SELECT itemsite_location_id"
                                     " FROM itemsite"
                                     " WHERE (itemsite_id=:itemsite_id)");
              dfltLocation.bindValue(":itemsite_id", _itemsite_id);
              dfltLocation.exec();
              if (!dfltLocation.first())
                 _locations->setId(-1);
              else
                 _locations->setId(dfltLocation.value("itemsite_location_id").toInt());

              connect(_locations,   SIGNAL(newID(int)),    this, SLOT(sChangeDefaultLocation()));
            }
        }
        else
        {
          _locationDefaultLit->hide();
          _locations->hide();
        }
    }
    else
    {
       _locationDefaultLit->hide();
       _locations->hide();
    }
}

void distributeInventory::sChangeDefaultLocation()
{
   XSqlQuery query;
   query.prepare( " UPDATE itemsite"
                  " SET itemsite_location_id=:itemsite_location_id"
                  " WHERE (itemsite_id=:itemsite_id);" );
   query.bindValue(":itemsite_location_id", _locations->id());
   query.bindValue(":itemsite_id", _itemsite_id);
   query.exec();
   sFillList();
   if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Updating Item Site Information"),
                                 query, __FILE__, __LINE__))
   {
     return;
   }
}

void distributeInventory::updateZoneList()
{
  XSqlQuery zoneFillList;
  QString zoneSql( "SELECT whsezone_id, whsezone_name||'-'||whsezone_descrip "
             " FROM whsezone  "
             " WHERE (whsezone_warehous_id=:warehous_id)"
             " ORDER BY whsezone_name;");

  zoneFillList.prepare(zoneSql);
  zoneFillList.bindValue(":warehous_id", _warehouse->id());
  zoneFillList.exec();
  _zone->populate(zoneFillList);
  
}
