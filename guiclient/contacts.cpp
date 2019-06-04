/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "contacts.h"

#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QVariant>
#include <QMessageBox>
#include <QInputDialog>

#include "characteristic.h"
#include "contact.h"
#include "contactMerge.h"
#include "errorReporter.h"
#include "metasql.h"
#include "parameterwidget.h"
#include "prospect.h"
#include "storedProcErrorLookup.h"
#include "xclusterinputdialog.h"
#include "xdateinputdialog.h"

contacts::contacts(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "contacts", fl)
{
  setupUi(optionsWidget());
  setWindowTitle(tr("Contacts"));
  setReportName("ContactsMasterList");
  setMetaSQLOptions("contacts", "detail");
  setParameterWidgetVisible(true);
  setNewVisible(true);
  setSearchVisible(true);
  setQueryOnStartEnabled(true);

  _crmacctid = -1;
  _attachAct = 0;
  _detachAct = 0;

  if (_privileges->check("MaintainAllContacts") || _privileges->check("ViewAllContacts"))
  {
    parameterWidget()->append(tr("Owner"), "owner_username", ParameterWidget::User);
    parameterWidget()->append(tr("Owner Pattern"), "owner_usr_pattern", ParameterWidget::Text);
  }
  parameterWidget()->append(tr("Account"), "crmacct_id", ParameterWidget::Crmacct);
  parameterWidget()->append(tr("Name Pattern"), "cntct_name_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Phone Pattern"), "cntct_phone_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Email Pattern"), "cntct_email_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Street Pattern"), "addr_street_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("City Pattern"), "addr_city_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("State Pattern"), "addr_state_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Postal Code Pattern"), "addr_postalcode_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Country Pattern"), "addr_country_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Marked"), "marked", ParameterWidget::CheckBox);
  parameterWidget()->append(tr("Created Before"), "createEndDate", ParameterWidget::Date);
  parameterWidget()->append(tr("Created After"), "createStartDate", ParameterWidget::Date);
  parameterWidget()->append(tr("Updated Before"), "updateEndDate", ParameterWidget::Date);
  parameterWidget()->append(tr("Updated After"), "updateStartDate", ParameterWidget::Date);

  list()->addColumn(tr("First Name"),          80, Qt::AlignLeft,  true, "cntct_first_name");
  list()->addColumn(tr("Last Name"),          100, Qt::AlignLeft,  true, "cntct_last_name");
  list()->addColumn(tr("Owner"),      _userColumn, Qt::AlignLeft, false, "cntct_owner_username");
  list()->addColumn(tr("Account #"),           80, Qt::AlignLeft,  true, "crmacct_number");
  list()->addColumn(tr("Account Name"),        -1, Qt::AlignLeft,  true, "crmacct_name");
  list()->addColumn(tr("Title"),               -1, Qt::AlignLeft,  true, "cntct_title");
  list()->addColumn(tr("Phone"),	      100, Qt::AlignLeft,  true, "cntct_phone");
  list()->addColumn(tr("Alternate"),          100, Qt::AlignLeft,  true, "cntct_phone2");
  list()->addColumn(tr("Fax"),                100, Qt::AlignLeft, false, "cntct_fax");
  list()->addColumn(tr("E-Mail"),             100, Qt::AlignLeft,  true, "cntct_email");
  list()->addColumn(tr("Web Address"),        100, Qt::AlignLeft, false, "cntct_webaddr");
  list()->addColumn(tr("Address"),             -1, Qt::AlignLeft, false, "addr_line1");
  list()->addColumn(tr("City"),                75, Qt::AlignLeft, false, "addr_city");
  list()->addColumn(tr("State"),               50, Qt::AlignLeft, false, "addr_state");
  list()->addColumn(tr("Country"),            100, Qt::AlignLeft, false, "addr_country");
  list()->addColumn(tr("Postal Code"),         75, Qt::AlignLeft, false, "addr_postalcode");
  list()->addColumn(tr("Created"),             -1, Qt::AlignLeft, false, "cntct_created");
  list()->addColumn(tr("Updated"),             -1, Qt::AlignLeft, false, "cntct_lastupdated");
  list()->addColumn(tr("Customer"),     _ynColumn,  Qt::AlignLeft, false, "cust");
  list()->addColumn(tr("Prospect"),    _ynColumn,  Qt::AlignLeft, false, "prospect");
  list()->addColumn(tr("Marked"),      _ynColumn,  Qt::AlignLeft, false, "marked");

  list()->setSelectionMode(QAbstractItemView::ExtendedSelection);

  setupCharacteristics("CNTCT");

  QToolButton * attachBtn = new QToolButton(this);
  attachBtn->setText(tr("Attach"));
  _attachAct = toolBar()->insertWidget(filterSeparator(), attachBtn);
  _attachAct->setEnabled(false);
  _attachAct->setVisible(false);

  QToolButton * detachBtn = new QToolButton(this);
  detachBtn->setText(tr("Detach"));
  _detachAct = toolBar()->insertWidget(filterSeparator(), detachBtn);
  _detachAct->setEnabled(false);
  _detachAct->setVisible(false);

  connect(attachBtn, SIGNAL(clicked()),      this, SLOT(sAttach()));
  connect(detachBtn, SIGNAL(clicked()),      this, SLOT(sDetach()));

  connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sOpen()));

  if (_privileges->check("MaintainAllContacts") || _privileges->check("MaintainPersonalContacts"))
  {
    _attachAct->setEnabled(true);
    connect(list(), SIGNAL(valid(bool)), _detachAct, SLOT(setEnabled(bool)));
  }
  else
  {
    newAction()->setEnabled(false);
  }
}

enum SetResponse contacts::set(const ParameterList& pParams)
{
  XWidget::set(pParams);
  QVariant param;
  bool	   valid;
  
  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "view")
    {
      _attachAct->setEnabled(false);
      disconnect(list(), SIGNAL(valid(bool)), _detachAct, SLOT(setEnabled(bool)));
    }
  }

  param = pParams.value("run", &valid);
  if (valid)
    sFillList();

  return NoError;
}

void contacts::sPopulateMenu(QMenu *pMenu, QTreeWidgetItem *, int)
{
  QAction *menuItem;

  bool editPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("MaintainPersonalContacts")) ||
      (_privileges->check("MaintainAllContacts"));

  bool viewPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("ViewPersonalContacts")) ||
      (_privileges->check("ViewAllContacts"));

  menuItem = pMenu->addAction(tr("Edit"), this, SLOT(sEdit()));
  menuItem->setEnabled(editPriv);

  menuItem = pMenu->addAction(tr("View"), this, SLOT(sView()));
  menuItem->setEnabled(viewPriv);

  menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sDelete()));
  menuItem->setEnabled(editPriv);

  if (list()->selectedItems().count() > 1)
  {
    pMenu->addSeparator();
    menuItem = pMenu->addAction(tr("Merge Contacts..."), this, SLOT(sMerge()));
    menuItem->setEnabled(_privileges->check("MergeContacts"));
  }

  bool foundMarked = false;
  bool foundUnmarked = false;

  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    if (item->rawValue("marked").toBool())
      foundMarked = true;
    else
      foundUnmarked = true;
  }

  if (foundUnmarked)
  {
    menuItem = pMenu->addAction(tr("Mark"), this, SLOT(sMark()));
    menuItem->setEnabled(editPriv);
  }

  if (foundMarked)
  {
    menuItem = pMenu->addAction(tr("Unmark"), this, SLOT(sUnmark()));
    menuItem->setEnabled(editPriv);
  }

  // Create, Edit, View Prospect

  bool editProspectPriv = false;
  bool viewProspectPriv = false;
  bool foundNewable = false;
  bool foundEditable = false;

  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    if (item->rawValue("cust").isNull()) // Can't be a Customer and Prospect
    {
      if (item->rawValue("prospect").toInt() > 0)
        foundEditable = true;
      else
        foundNewable = true;
    }

    editProspectPriv = editProspectPriv ||
        (omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("MaintainPersonalCRMAccounts")) ||
        (_privileges->check("MaintainAllCRMAccounts")); // TODO - replace if a new "ViewProspect" priv created

    viewProspectPriv = viewProspectPriv ||
        (omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("ViewPersonalCRMAccounts")) ||
        (_privileges->check("ViewAllCRMAccounts")); // TODO - replace if a new "ViewProspect" priv created
  }

  if (foundEditable)
  {
    pMenu->addSeparator();
    menuItem = pMenu->addAction(tr("Edit Prospect"), this, SLOT(sEditProspect()));
    menuItem->setEnabled(editProspectPriv);
    menuItem = pMenu->addAction(tr("View Prospect"), this, SLOT(sViewProspect()));
    menuItem->setEnabled(viewProspectPriv);
  }
  if (foundNewable)
  {
    pMenu->addSeparator();
    menuItem = pMenu->addAction(tr("Create Prospect..."), this, SLOT(sNewProspect()));
    menuItem->setEnabled(editProspectPriv);
  }
}

void contacts::sPopulateHeaderMenu(QMenu *pMenu, QTreeWidgetItem *, int index)
{
  _replace = index;

  if (list()->selectedItems().size() > 0)
  {
    QAction *menuItem = pMenu->addAction(tr("Replace..."), this, SLOT(sReplace()));
    menuItem->setEnabled(_privileges->check("MaintainAllContacts"));
    pMenu->addSeparator();
  }
}

void contacts::sReplace()
{
  QString label = tr("New %1").arg(list()->headerItem()->text(_replace).mid(3));
  QString col = list()->column(_replace);
  characteristic::Type chartype = characteristic::Text;
  int charid = -1;
  QVariant replace;

  if (col.contains("char"))
  {
    charid = col.mid(4).toInt();
    if (charid > 0)
    {
      XSqlQuery qry;
      qry.prepare("SELECT char_type "
                  "  FROM char "
                  " WHERE char_id=:char_id;");
      qry.bindValue(":char_id", charid);
      qry.exec();
      if (qry.first())
      {
        chartype = (characteristic::Type)(qry.value("char_type").toInt());
      }
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Characteristic lookup failed"),
                                    qry, __FILE__, __LINE__))
        return;
    }
  }

  bool ok;

  if (col.contains("cntct"))
    replace = QInputDialog::getText(this, tr("Replace"), label, QLineEdit::Normal, "", &ok);
  else if (col.contains("crmacct") || col.contains("addr"))
  {
    ParameterList params;
    params.append("label", label);
    if (col.contains("crmacct"))
      params.append("type", "crmacct");
    else if (col.contains("addr"))
      params.append("type", "addr");

    XClusterInputDialog newdlg(this, "", true);
    newdlg.set(params);
    if (newdlg.exec() == XDialog::Accepted)
    {
      replace = newdlg.getId() > 0 ? newdlg.getId() : QVariant();
      ok = true;
    }
  }
  else if (col.contains("char"))
  {
    if (chartype == characteristic::Text)
      replace = QInputDialog::getText(this, tr("Replace"), label, QLineEdit::Normal, "", &ok);
    else if (chartype == characteristic::Number)
      replace = QInputDialog::getDouble(this, tr("Replace"), label, 0, 0.0, 9999999.0,
                                        decimalPlaces("purchprice"), &ok);
    else if (chartype == characteristic::List)
    {
      QStringList options;

      XSqlQuery qry;
      qry.prepare("SELECT charopt_value "
                  "   FROM charopt "
                  " WHERE charopt_char_id=:char_id "
                  " ORDER BY charopt_order;");
      qry.bindValue(":char_id", charid);
      qry.exec();
      while (qry.next())
        options.append(qry.value("charopt_value").toString());
      if (ErrorReporter::error(QtCriticalMsg, this, tr("Characteristic option lookup failed"),
                                    qry, __FILE__, __LINE__))
        return;

      replace = QInputDialog::getItem(this, tr("Replace"), label, options, 0, false, &ok);
    }
    else if (chartype == characteristic::Date)
    {
      ParameterList params;
      params.append("label", label);

      XDateInputDialog newdlg(this, "", true);
      newdlg.set(params);
      if (newdlg.exec() == XDialog::Accepted)
      {
        replace = newdlg.getDate();
        ok = true;
      }
    }
  }

  if (ok && QMessageBox::warning(this, tr("Bulk Replace?"),
                                 tr("<p>The selected value will be replaced for all %1 selected "
                                    "items. Are you sure you want to do this?")
                                 .arg(list()->selectedItems().size()),
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::No) == QMessageBox::Yes)
  {
    ParameterList params;
    MetaSQLQuery insert;
    MetaSQLQuery update;
    if (!col.contains("char"))
    {
      QString column;
      if (col.contains("cntct"))
        column = col;
      else if(col.contains("crmacct"))
        column = "cntct_crmacct_id";
      else if(col.contains("addr"))
        column = "cntct_addr_id";

      update.setQuery(QString("UPDATE cntct "
                              "   SET %1 = <? value('replace') ?> "
                              " WHERE cntct_id IN (-1 "
                              "       <? foreach('contacts') ?> "
                              "       , <? value('contacts') ?> "
                              "       <? endforeach ?> "
                              "       );").arg(column));
    }
    else
    {
      insert.setQuery("INSERT INTO charass "
                      "(charass_target_type, charass_target_id, charass_char_id, charass_value) "
                      "SELECT 'CNTCT', cntct_id, <? value('char_id') ?>, <? value('replace') ?> "
                      "  FROM cntct "
                      " WHERE NOT EXISTS(SELECT 1 "
                      "                    FROM charass "
                      "                   WHERE charass_target_type = 'CNTCT' "
                      "                     AND charass_target_id = cntct_id "
                      "                     AND charass_char_id = <? value('char_id') ?>) "
                      "   AND cntct_id IN (-1 "
                      "       <? foreach('contacts') ?> "
                      "       , <? value('contacts') ?> "
                      "       <? endforeach ?> "
                      "       );");
      update.setQuery("UPDATE charass "
                      "   SET charass_value=<? value('replace') ?> "
                      " WHERE charass_target_type='CNTCT' "
                      "   AND charass_char_id = <? value('char_id') ?> "
                      "   AND charass_target_id IN (-1 "
                      "       <? foreach('contacts') ?> "
                      "       , <? value('contacts') ?> "
                      "       <? endforeach ?> "
                      "       );");

      params.append("char_id", charid);
    }

    QList<QVariant> contacts;
    foreach (XTreeWidgetItem* item, list()->selectedItems())
      contacts.append(item->id());

    params.append("replace", replace);
    params.append("contacts", contacts);

    XSqlQuery begin("BEGIN;");
    XSqlQuery rollback;
    rollback.prepare("ROLLBACK;");

    if (col.contains("char"))
    {
      XSqlQuery qry = insert.toQuery(params);

      if (ErrorReporter::error(QtCriticalMsg, this, tr("Replace failed"),
                               qry, __FILE__, __LINE__))
      {
        rollback.exec();
        return;
      }
    }

    XSqlQuery qry = update.toQuery(params);

    if (ErrorReporter::error(QtCriticalMsg, this, tr("Replace failed"),
                             qry, __FILE__, __LINE__))
    {
      rollback.exec();
      return;
    }

    XSqlQuery commit("COMMIT;");

    sFillList();
  }
}

void contacts::sNew()
{
  ParameterList params;
  setParams(params);
  params.append("mode", "new");

  contact newdlg(0, "", true);
  newdlg.set(params);
  newdlg.setWindowModality(Qt::WindowModal);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void contacts::sEdit()
{
  QList<XTreeWidgetItem*> selected = list()->selectedItems();
  for (int i = 0; i < selected.size(); i++)
  {
    ParameterList params;
    params.append("mode", "edit");
    params.append("cntct_id", ((XTreeWidgetItem*)(selected[i]))->id());

    contact* newdlg = new contact(this, "", false);
    newdlg->set(params);
    newdlg->setAttribute(Qt::WA_DeleteOnClose);
    omfgThis->handleNewWindow(newdlg);
  }
}

void contacts::sView()
{
  QList<XTreeWidgetItem*> selected = list()->selectedItems();
  for (int i = 0; i < selected.size(); i++)
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("cntct_id", ((XTreeWidgetItem*)(selected[i]))->id());

    contact* newdlg = new contact(this, "", false);
    newdlg->set(params);
    newdlg->setAttribute(Qt::WA_DeleteOnClose);
    omfgThis->handleNewWindow(newdlg);
  }
}

void contacts::sDelete()
{

  if ( QMessageBox::warning(this, tr("Delete Contacts?"),
                            tr("<p>Are you sure that you want to completely "
                               "delete the selected contact(s)?"),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::No) == QMessageBox::Yes)
  {
    XSqlQuery delq;
    XSqlQuery chk;
    chk.prepare("SELECT cntctused(:cntct_id) AS inUse;");
    delq.prepare("SELECT deleteCntct(:cntct_id, :cascade);");

    foreach (XTreeWidgetItem *selected, list()->selectedItems())
    {
      bool cascade = false;
      chk.bindValue(":cntct_id", selected->id());
      chk.exec();
      if (chk.first() && chk.value("inUse").toBool())
      {
        if (QMessageBox::warning(this, tr("Delete Parent Objects?"),
                                 tr("There are parent objects that use this contact. Do you wish "
                                    "to delete these objects as well?"),
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::No) == QMessageBox::Yes)
          cascade = true;
        else
        {
          XSqlQuery incdt;
          incdt.prepare("SELECT EXISTS(SELECT 1 "
                        "                FROM incdt "
                        "               WHERE incdt_cntct_id=:cntct_id) AS used;");
          incdt.bindValue(":cntct_id", selected->id());
          incdt.exec();

          if (incdt.first() && incdt.value("used").toBool())
          {
            QMessageBox::critical(this, tr("Contact in Use"), tr("Contact is used on an incident "
                                                                 "and cannot be deleted."));
            return;
          }
          else if (ErrorReporter::error(QtCriticalMsg, this, tr("Checking Usage"),
                                        incdt, __FILE__, __LINE__))
            return;
        }
      }
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Checking Usage"),
                                    chk, __FILE__, __LINE__))
        return;

      delq.bindValue(":cntct_id", selected->id());
      delq.bindValue(":cascade", cascade);
      delq.exec();
      if (ErrorReporter::error(QtCriticalMsg, this, tr("Deleting Contact"),
                               delq, __FILE__, __LINE__))
        return;
    }

    sFillList();
  }
}

void contacts::sMerge()
{
  QList<QVariant> ids;
  foreach (XTreeWidgetItem* item, list()->selectedItems())
    ids.append(item->id());

  ParameterList params;
  params.append("contacts", ids);

  contactMerge *newdlg = new contactMerge();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void contacts::sMark()
{
  ParameterList params;
  MetaSQLQuery mql("INSERT INTO mrkd "
                   "(mrkd_target_type, mrkd_target_id) "
                   "SELECT 'T', cntct_id "
                   "  FROM cntct "
                   " WHERE NOT EXISTS(SELECT 1 "
                   "                    FROM mrkd "
                   "                   WHERE mrkd_target_type = 'T' "
                   "                     AND mrkd_target_id = cntct_id) "
                   "   AND cntct_id IN (-1 "
                   "       <? foreach('contacts') ?> "
                   "       , <? value('contacts') ?> "
                   "       <? endforeach ?> "
                   "       );");

  QList<QVariant> contacts;
  foreach (XTreeWidgetItem* item, list()->selectedItems())
    contacts.append(item->id());

  params.append("contacts", contacts);
  XSqlQuery qry = mql.toQuery(params);

  if (ErrorReporter::error(QtCriticalMsg, this, tr("Marking failed"),
                           qry, __FILE__, __LINE__))
    return;

  sFillList();
}

void contacts::sUnmark()
{
  ParameterList params;
  MetaSQLQuery mql("DELETE FROM mrkd "
                   " WHERE mrkd_target_type = 'T' "
                   "   AND mrkd_target_id IN (-1 "
                   "       <? foreach('contacts') ?> "
                   "       , <? value('contacts') ?> "
                   "       <? endforeach ?> "
                   "       );");

  QList<QVariant> contacts;
  foreach (XTreeWidgetItem* item, list()->selectedItems())
    contacts.append(item->id());

  params.append("contacts", contacts);
  XSqlQuery qry = mql.toQuery(params);

  if (ErrorReporter::error(QtCriticalMsg, this, tr("Unmarking failed"),
                           qry, __FILE__, __LINE__))
    return;

  sFillList();
}

void contacts::sAttach()
{
  ContactCluster attached(this, "attached");
  attached.sEllipses();
  if (attached.id() > 0)
  {
    int answer = QMessageBox::Yes;

    if (attached.crmAcctId() > 0 && attached.crmAcctId() != _crmacctid)
      answer = QMessageBox::question(this, tr("Detach Contact?"),
			    tr("<p>This Contact is currently attached to a "
			       "different Account. Are you sure you want "
			       "to change the Account for this person?"),
			    QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
    if (answer == QMessageBox::Yes)
    {
      XSqlQuery attq;
      attq.prepare("SELECT attachContact(:cntct_id, :crmacct_id) AS returnVal;");
      attq.bindValue(":cntct_id", attached.id());
      attq.bindValue(":crmacct_id", _crmacctid);
      attq.exec();
      if (attq.first())
      {
        int returnVal = attq.value("returnVal").toInt();
        if (returnVal < 0)
        {
            ErrorReporter::error(QtCriticalMsg, this, tr("Error Attaching Contact"),
                                 storedProcErrorLookup("attachContact", returnVal),
                                 __FILE__, __LINE__);
            return;
        }
      }
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Attaching Contact"),
                                    attq, __FILE__, __LINE__))
	return;
    }
    sFillList();
  }
}

void contacts::sDetach()
{
  int answer = QMessageBox::question(this, tr("Detach Contact?"),
			tr("<p>Are you sure you want to detach this Contact "
			   "from this Account?"),
			QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
  if (answer == QMessageBox::Yes)
  {
    int cntctId = list()->id();
    XSqlQuery detq;
    detq.prepare("SELECT detachContact(:cntct_id, :crmacct_id) AS returnVal;");
    detq.bindValue(":cntct_id", cntctId);
    detq.bindValue(":crmacct_id", _crmacctid);
    detq.exec();
    if (detq.first())
    {
      int returnVal = detq.value("returnVal").toInt();
      if (returnVal < 0)
      {
        ErrorReporter::error(QtCriticalMsg, this, tr("Error detaching Contact from Account (%1)")
                        .arg(returnVal),detq, __FILE__, __LINE__);
        return;
      }
      emit cntctDetached(cntctId);
    }
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Detaching Contact"),
                                  detq, __FILE__, __LINE__))
      return;

    sFillList();
  }
}

void contacts::setCrmacctid(int crmacctId)
{
  _crmacctid = crmacctId;
  if (_crmacctid == -1)
  {
    parameterWidget()->setDefault(tr("Account"), QVariant(), true);
    _attachAct->setVisible(false);
    _detachAct->setVisible(false);
  }
  else
  {
    parameterWidget()->setDefault(tr("Account"), _crmacctid, true);
    _attachAct->setVisible(true);
    _detachAct->setVisible(true);
  }
}

int contacts::crmacctId()
{
  return _crmacctid;
}

QAction* contacts::attachAction()
{
  return _attachAct;
}

QAction* contacts::detachAction()
{
  return _detachAct;
}

bool contacts::setParams(ParameterList &params)
{
  if (!display::setParams(params))
    return false;

  if (_activeOnly->isChecked())
    params.append("activeOnly",true);

  return true;
}

void contacts::sOpen()
{
  bool editPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("MaintainPersonalContacts")) ||
      (_privileges->check("MaintainAllContacts"));

  bool viewPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("ViewPersonalContacts")) ||
      (_privileges->check("ViewAllContacts"));

  if (editPriv)
    sEdit();
  else if (viewPriv)
    sView();
  else
    QMessageBox::information(this, tr("Restricted Access"), tr("You have not been granted privileges to open this Contact."));
}

void contacts::sNewProspect()
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    if (item->rawValue("cust").isNull() && item->rawValue("prospect").toInt() <= 0 &&
        ((omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("MaintainPersonalCRMAccounts")) ||
         (_privileges->check("MaintainAllCRMAccounts"))))
    {
      ParameterList params;
      params.append("mode", "new");

      XSqlQuery sql;
      sql.prepare("SELECT cntct_crmacct_id"
                  "  FROM cntct"
                  " WHERE cntct_id=:cntct_id");
      sql.bindValue(":cntct_id", item->id());
      sql.exec();
      if (sql.first())
        params.append("crmacct_id", sql.value("cntct_crmacct_id").toInt());
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Fetching CRM Account"),
                                    sql, __FILE__, __LINE__))
        return;

      prospect *newdlg = new prospect();
      newdlg->set(params);
      omfgThis->handleNewWindow(newdlg);
    }
  }
}

void contacts::sEditProspect()
{
  sOpenProspect("edit");
}

void contacts::sViewProspect()
{
  sOpenProspect("view");
}

void contacts::sOpenProspect(QString mode)
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    if (item->rawValue("cust").isNull() && item->rawValue("prospect").toInt() > 0 &&
        ((omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("ViewPersonalCRMAccounts")) ||
         (_privileges->check("ViewAllCRMAccounts"))))
    {
      ParameterList params;
      params.append("mode", mode);
      params.append("prospect_id", item->rawValue("prospect"));

      prospect *newdlg = new prospect();
      newdlg->set(params);
      omfgThis->handleNewWindow(newdlg);
    }
  }
}
