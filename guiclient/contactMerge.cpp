/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "contactMerge.h"
#include "contact.h"

#include <QMessageBox>
#include <QSqlError>
#include <QCloseEvent>

#include <metasql.h>
#include "mqlutil.h"
#include "errorReporter.h"

contactMerge::contactMerge(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : XWidget(parent, name, fl)
{
  setupUi(this);

  connect(_close,		  SIGNAL(clicked()), this, SLOT(close()));
  connect(_mode,                  SIGNAL(currentIndexChanged(int)), this, SLOT(sHandleMode()));
  connect(_process,	          SIGNAL(clicked()), this, SLOT(sProcess()));
  connect(_query,	          SIGNAL(clicked()), this, SLOT(sFillList()));
  connect(_cntct,    SIGNAL(populateMenu(QMenu *, QTreeWidgetItem *)), this, SLOT(sPopulateCntctMenu(QMenu *)));
  connect(_srccntct, SIGNAL(populateMenu(QMenu *, QTreeWidgetItem *, int)), this, SLOT(sPopulateSrcMenu(QMenu *,QTreeWidgetItem *, int)));
  connect(_cntct,    SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(sCntctEdit()));
  connect(_srccntct, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(sSrcCntctEdit()));
  connect(_target,                SIGNAL(valid(bool)), this, SLOT(sHandleProcess()));

  _cntct->addColumn(tr("Contact#"),       100, Qt::AlignLeft, true,  "cntct_number");
  _cntct->addColumn(tr("Active"),          50, Qt::AlignLeft, true,  "cntct_active");
  _cntct->addColumn(tr("Acct.#"),         100, Qt::AlignLeft, false, "crmacct_number");
  _cntct->addColumn(tr("Acct. Name"),     100, Qt::AlignLeft, false, "crmacct_name");
  _cntct->addColumn(tr("Hnrfc"),           50, Qt::AlignLeft, false, "cntct_honorific");
  _cntct->addColumn(tr("First"),           80, Qt::AlignLeft, true,  "cntct_first_name");
  _cntct->addColumn(tr("Middle"),          50, Qt::AlignLeft, false, "cntct_middle");
  _cntct->addColumn(tr("Last"),            -1, Qt::AlignLeft, true,  "cntct_last_name");
  _cntct->addColumn(tr("Suffix"),          80, Qt::AlignLeft, false, "cntct_suffix");
  _cntct->addColumn(tr("Initials"),        80, Qt::AlignLeft, false, "cntct_initials");
  _cntct->addColumn(tr("Phone #s"),        -1, Qt::AlignLeft, true,  "contact_phones");
  _cntct->addColumn(tr("Email"),          100, Qt::AlignLeft, true,  "cntct_email");
  _cntct->addColumn(tr("Web"),            100, Qt::AlignLeft, false, "cntct_webaddr");
  _cntct->addColumn(tr("Title"),          100, Qt::AlignLeft, true,  "cntct_title");
  _cntct->addColumn(tr("Owner"),           80, Qt::AlignLeft, true,  "cntct_owner_username");
  _cntct->addColumn(tr("Notes"),          100, Qt::AlignLeft, false, "cntct_notes");
  _cntct->addColumn(tr("Address1"),       100, Qt::AlignLeft, false, "addr_line1");
  _cntct->addColumn(tr("Address2"),       100, Qt::AlignLeft, false, "addr_line2");
  _cntct->addColumn(tr("Address3"),       100, Qt::AlignLeft, false, "addr_line3");
  _cntct->addColumn(tr("City"),           100, Qt::AlignLeft, false, "addr_city");
  _cntct->addColumn(tr("State"),           50, Qt::AlignLeft, false, "addr_state");
  _cntct->addColumn(tr("Postal"),          80, Qt::AlignLeft, false, "addr_postalcode");
  _cntct->addColumn(tr("Country"),        100, Qt::AlignLeft, false, "addr_country");

  _srccntct->addColumn(tr("Contact#"),       100, Qt::AlignLeft, true,  "cntct_number");
  _srccntct->addColumn(tr("Active"),          50, Qt::AlignLeft, true,  "cntct_active");
  _srccntct->addColumn(tr("Acct.#"),         100, Qt::AlignLeft, false, "crmacct_number");
  _srccntct->addColumn(tr("Acct. Name"),     100, Qt::AlignLeft, false, "crmacct_name");
  _srccntct->addColumn(tr("Hnrfc"),           50, Qt::AlignLeft, false, "cntct_honorific");
  _srccntct->addColumn(tr("First"),           80, Qt::AlignLeft, true,  "cntct_first_name");
  _srccntct->addColumn(tr("Middle"),          50, Qt::AlignLeft, false, "cntct_middle");
  _srccntct->addColumn(tr("Last"),            -1, Qt::AlignLeft, true,  "cntct_last_name");
  _srccntct->addColumn(tr("Suffix"),          80, Qt::AlignLeft, false, "cntct_suffix");
  _srccntct->addColumn(tr("Initials"),        80, Qt::AlignLeft, false, "cntct_initials");
  _srccntct->addColumn(tr("Phone #s"),        -1, Qt::AlignLeft, true,  "contact_phones");
  _srccntct->addColumn(tr("Email"),          100, Qt::AlignLeft, true,  "cntct_email");
  _srccntct->addColumn(tr("Web"),            100, Qt::AlignLeft, false, "cntct_webaddr");
  _srccntct->addColumn(tr("Title"),          100, Qt::AlignLeft, true,  "cntct_title");
  _srccntct->addColumn(tr("Owner"),           80, Qt::AlignLeft, true,  "cntct_owner_username");
  _srccntct->addColumn(tr("Notes"),          100, Qt::AlignLeft, false, "cntct_notes");
  _srccntct->addColumn(tr("Address1"),       100, Qt::AlignLeft, false, "addr_line1");
  _srccntct->addColumn(tr("Address2"),       100, Qt::AlignLeft, false, "addr_line2");
  _srccntct->addColumn(tr("Address3"),       100, Qt::AlignLeft, false, "addr_line3");
  _srccntct->addColumn(tr("City"),           100, Qt::AlignLeft, false, "addr_city");
  _srccntct->addColumn(tr("State"),           50, Qt::AlignLeft, false, "addr_state");
  _srccntct->addColumn(tr("Postal"),          80, Qt::AlignLeft, false, "addr_postalcode");
  _srccntct->addColumn(tr("Country"),        100, Qt::AlignLeft, false, "addr_country");

  _target->setOwnerVisible(true);
  _target->setActiveVisible(false);
  _target->setInitialsVisible(false);

  // Column list must match the _srccntct column numbers/positions
  _colMap << "NA" << "NA" << "crmacct_id" << "crmacct_id" <<  "honorific" << "first_name" << "middle"
          << "last_name" << "suffix" << "initials" << "phones" << "email"
          << "webaddr" << "title" << "owner_username" << "notes" << "addr_id";
  
  sPopulateSources();
  sPopulateTarget();
}

contactMerge::~contactMerge()
{
    // no need to delete child widgets, Qt does it all for us
}

void contactMerge::languageChange()
{
    retranslateUi(this);
}

enum SetResponse contactMerge::set(const ParameterList &pParams)
{
  XWidget::set(pParams);
  QVariant param;
  bool valid;

  param = pParams.value("contacts", &valid);
  if (valid)
  {
    _contacts = param.toList();

    _searchLit->setVisible(false);
    _search->setVisible(false);
    _showGroup->setVisible(false);
    _searchGroup->setVisible(false);

    sFillList();
  }

  return NoError;
}

void contactMerge::sAdd()
{
  sSelect(false);
}

void contactMerge::sCntctEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("cntct_id", _cntct->id());

  contact newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
  sFillList();
}

void contactMerge::sCntctView()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("cntct_id", _cntct->id());

  contact newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void contactMerge::sCntctDelete()
{
  XSqlQuery contactCntctDelete;
  QString question = tr("The delete action cannot be undone. "
                        "Are you sure you want to proceed?");
  if (QMessageBox::question(this, tr("Delete Contact Merg?"), question,
                QMessageBox::Yes,
                QMessageBox::No | QMessageBox::Default) == QMessageBox::No)
    return;

  MetaSQLQuery mql = mqlLoad("contactmerge", "delete");

  ParameterList params;
  params.append("cntct_id", _cntct->id());
  contactCntctDelete = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Contact"),
                                contactCntctDelete, __FILE__, __LINE__))
  {
    return;
  }
  sFillList();
}

void contactMerge::sCntctDoubleClicked()
{
  if ((_cntct->altId() == 0) || (_cntct->altId() == 4)) // cNone or cError
  {
    sSelect(false);
  }
  else if ((_cntct->altId() == 1) || (_cntct->altId() == 2)) // cTarget or cSource
  {
    sDeselect(_cntct->id());
  }
}

void contactMerge::sDeselect(int id)
{
  XSqlQuery contactDeselect;
  MetaSQLQuery mql = mqlLoad("contactmerge", "deselect");

  ParameterList params;
  params.append("cntct_id", id);
  contactDeselect = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deselecting Contact"),
                                contactDeselect, __FILE__, __LINE__))
  {
    return;
  }
  sFillList();
}

void contactMerge::sDeselectCntct()
{
  sDeselect(_cntct->id());
}

void contactMerge::sDeselectSource()
{
  sDeselect(_srccntct->id());
}

void contactMerge::sFillList()
{
  if (_contacts.isEmpty())
  {
    if ((_mode->currentIndex() == 0) || (_mode->currentIndex() == 2)) // cMerge or cMergePurge
    {
      ParameterList params;
      params.append("searchText", _search->text());
      params.append("searchContactName", QVariant(_searchContact->isChecked()));
      params.append("searchPhone", QVariant(_searchPhone->isChecked()));
      params.append("searchEmail", QVariant(_searchEmail->isChecked()));
      params.append("searchNumber", QVariant(_searchNumber->isChecked()));
      params.append("searchName", QVariant(_searchName->isChecked()));
      params.append("showInactive", QVariant(_showInactive->isChecked()));
      params.append("ignoreBlanks", QVariant(!_blanks->isChecked()));
      params.append("IndentedDups", QVariant(_showGroup->isChecked()));
      params.append("CheckHnfc", QVariant(_showGroup->isChecked() && _checkHonorific->isChecked()));
      params.append("CheckFirst", QVariant(_showGroup->isChecked() && _checkFirst->isChecked()));
      params.append("CheckMiddle", QVariant(_showGroup->isChecked() && _checkMiddle->isChecked()));
      params.append("CheckLast", QVariant(_showGroup->isChecked() && _checkLast->isChecked()));
      params.append("CheckSuffix", QVariant(_showGroup->isChecked() && _checkSuffix->isChecked()));
      params.append("CheckPhone", QVariant(_showGroup->isChecked() && _checkPhone->isChecked()));
      params.append("CheckEmail", QVariant(_showGroup->isChecked() && _checkEmail->isChecked()));
      MetaSQLQuery mql = mqlLoad("contactmerge", "search");
      XSqlQuery qry = mql.toQuery(params);
      if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Contact Information"),
                                  qry, __FILE__, __LINE__))
      {
        return;
      }
      _cntct->populate(qry, true);
    }
    else
    {
      ParameterList params;
      MetaSQLQuery mql = mqlLoad("contactmerge", "merged");
      XSqlQuery qry = mql.toQuery(params);
      if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Contact Information"),
                                    qry, __FILE__, __LINE__))
      {
        return;
      }
      _cntct->populate(qry, true);
    }
  }
  else
  {
    ParameterList params;
    MetaSQLQuery mql("SELECT cntct_id, "
                     "       CASE WHEN cntctsel_cntct_id IS NOT NULL "
                     "             AND cntctsel_target THEN 1 "
                     "            WHEN cntctsel_cntct_id IS NOT NULL "
                     "             AND NOT cntctsel_target THEN 2 "
                     "            WHEN cntctmrgd_cntct_id IS NOT NULL "
                     "             AND NOT cntctUsed(cntctmrgd_cntct_id) THEN 3 "
                     "            WHEN cntctmrgd_cntct_id IS NOT NULL THEN 4 "
                     "            ELSE 0 "
                     "        END AS status, "
                     "       cntct_crmacct_id, cntct_first_name, cntct_last_name, cntct_honorific, "
                     "       cntct_initials, cntct_active, cntct_phone, cntct_phone2, cntct_fax, "
                     "       cntct_email, cntct_webaddr, cntct_notes, cntct_title, cntct_number, "
                     "       cntct_middle, cntct_suffix, cntct_owner_username, "
                     "       crmacct_number,crmacct_name, "
                     "       addr_id, addr_line1, addr_line2, addr_line3, addr_city, addr_state, "
                     "       addr_postalcode, addr_country, addr_active, addr_notes, addr_number, "
                     "       CASE WHEN cntctsel_cntct_id IS NOT NULL "
                     "             AND cntctsel_target THEN 'altemphasis' "
                     "            WHEN cntctsel_cntct_id IS NOT NULL "
                     "             AND NOT cntctsel_target THEN 'emphasis' "
                     "            WHEN cntctmrgd_cntct_id IS NOT NULL "
                     "             AND NOT cntctUsed(cntctmrgd_cntct_id) THEN 'warning' "
                     "            WHEN cntctmrgd_cntct_id IS NOT NULL THEN 'error' "
                     "        END AS qtforegroundrole "
                     "  FROM cntct "
                     "  LEFT OUTER JOIN addr ON cntct_addr_id=addr_id "
                     "  LEFT OUTER JOIN crmacct ON cntct_crmacct_id=crmacct_id "
                     "  LEFT OUTER JOIN cntctsel ON cntct_id=cntctsel_cntct_id "
                     "  LEFT OUTER JOIN cntctmrgd ON cntct_id=cntctmrgd_cntct_id "
                     " WHERE cntct_id IN (-1 "
                     "       <? foreach('contacts') ?> "
                     "       , <? value('contacts') ?> "
                     "       <? endforeach ?> "
                     "       ) "
                     "<? if not exists('merge') ?> "
                     "   AND cntctmrgd_cntct_id IS NOT NULL "
                     "<? endif ?>;");

    params.append("contacts", _contacts);
    if ((_mode->currentIndex() == 0) || (_mode->currentIndex() == 2)) // cMerge or cMergePurge
      params.append("merge");

    XSqlQuery qry = mql.toQuery(params);
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Contact Information"),
                                  qry, __FILE__, __LINE__))
    {
      return;
    }
    _cntct->populate(qry, true);
  }

  _cntct->expandAll();
  sPopulateSources();
  sPopulateTarget();
}

void contactMerge::sHandleMode()
{
  bool canSearch = ((_mode->currentIndex() == 0) || (_mode->currentIndex() == 2)); // cMerge or cMergePurge
  _searchLit->setVisible(canSearch && _contacts.isEmpty());
  _search->setVisible(canSearch && _contacts.isEmpty());
  _showGroup->setVisible(canSearch && _contacts.isEmpty());
  _searchGroup->setVisible(canSearch && _contacts.isEmpty());
  sHandleProcess();
  _process->setText(_mode->currentText());
  _tab->setTabEnabled(_tab->indexOf(_selTab), canSearch);
  if (_contacts.isEmpty())
  {
    if (canSearch)
      _cntct->clear();
    else
      sFillList();
  }
}

void contactMerge::sHandleProcess()
{
  bool valid = (_target->isValid());
  _process->setEnabled((valid &&
                       (_mode->currentIndex() == 0 || //cMerge
                        _mode->currentIndex() == 2))  || //cMergePurge
                       (_mode->currentIndex() == 3 || //cMerged
                        _mode->currentIndex() == 1)); //cPurge
}

void contactMerge::sPopulateCntctMenu(QMenu *pMenu)
{
  XSqlQuery contactPopulateCntctMenu;
  if (_cntct->id() == -1)
    return;

  QAction *menuItem;

  if (_cntct->altId() == 0 || // cNone
      _cntct->altId() == 4)  // cError
  {
    menuItem = pMenu->addAction(tr("Set as Source..."), this, SLOT(sAdd()));
  }

  if (_cntct->altId() != 1) // cTarget
  {
    menuItem = pMenu->addAction(tr("Set as Target..."), this, SLOT(sSetTarget()));
  }

  if (_cntct->altId() == 1 || // cTarget
      _cntct->altId() == 2)  // cSource
  {
    menuItem = pMenu->addAction(tr("Deselect..."), this, SLOT(sDeselectCntct()));
  }

  if (_cntct->altId() == 3) // cMerged
  {
    menuItem = pMenu->addAction(tr("Restore..."), this, SLOT(sRestore()));
    menuItem = pMenu->addAction(tr("Purge..."), this, SLOT(sPurge()));
  }

  pMenu->addSeparator();

  menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sCntctEdit()));
  menuItem->setEnabled(_privileges->check("MaintainAllContacts") ||
                      (_cntct->currentItem()->rawValue("cntct_owner_username") == omfgThis->username() && _privileges->check("MaintainPersonalContacts")));

  menuItem = pMenu->addAction(tr("View..."), this, SLOT(sCntctView()));
  menuItem->setEnabled(_privileges->check("MaintainAllContacts") ||
                       (_cntct->currentItem()->rawValue("cntct_owner_username") == omfgThis->username() && _privileges->check("MaintainPersonalContacts")) ||
                       _privileges->check("ViewAllContacts") ||
                       (_cntct->currentItem()->rawValue("cntct_owner_username") == omfgThis->username() && _privileges->check("ViewPersonalContacts")));

  if (_cntct->altId() != 3) // cMerged
  {
    // Check to see if this contact is used, if not add delete action
    ParameterList params;
    params.append("cntct_id", _cntct->id());
    MetaSQLQuery mql = mqlLoad("contactmerge", "contactused");
    contactPopulateCntctMenu = mql.toQuery(params);
    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Adding Delete Action To Menu"),
                                  contactPopulateCntctMenu, __FILE__, __LINE__))
    {
      return;
    }
    if (contactPopulateCntctMenu.first() && !contactPopulateCntctMenu.value("used").toBool())
    {
        menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sCntctDelete()));
        menuItem->setEnabled(_privileges->check("MaintainAllContacts"));
    }
  }
}

void contactMerge::sPopulateSrcMenu(QMenu *pMenu, QTreeWidgetItem *pItem, int pCol)
{
  Q_UNUSED(pItem);
  QAction *menuItem;
  QString col = "";
  QString menuStr;

  QTreeWidgetItem* header = _srccntct->headerItem();

  if (pCol > 1)
    col = header->text(pCol); 

  menuStr = tr("Merge ") + col + tr(" to target");
  _selectCol = pCol > 18 ? 18 : pCol;

  menuItem = pMenu->addAction(tr("Deselect"), this, SLOT(sDeselectSource()));

  if (col.length() > 0)
    menuItem = pMenu->addAction(menuStr, this, SLOT(sSelectCol()));

  pMenu->addSeparator();

  menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sCntctEdit()));
  menuItem->setEnabled(_privileges->check("MaintainAllContacts"));

  menuItem = pMenu->addAction(tr("View..."), this, SLOT(sCntctView()));
  menuItem->setEnabled(_privileges->check("MaintainAllContacts") || _privileges->check("ViewAllContacts"));
}

void contactMerge::sPopulateSources()
{
  XSqlQuery contactPopulateSources;
  ParameterList params;
  params.append("target", QVariant(false));

  MetaSQLQuery mql = mqlLoad("contactmerge", "populate");
  contactPopulateSources = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Contact Information"),
                                contactPopulateSources, __FILE__, __LINE__))
  {
    return;
  }
  _srccntct->populate(contactPopulateSources);
}

void contactMerge::sPopulateTarget()
{
  XSqlQuery contactPopulateTarget;
  QString grpTitle = tr("Target Contact");
  ParameterList params;
  params.append("target", QVariant(true));

  MetaSQLQuery mql = mqlLoad("contactmerge", "populate");
  contactPopulateTarget = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Contact Information"),
                                contactPopulateTarget, __FILE__, __LINE__))
  {
    return;
  }

  if (contactPopulateTarget.first())
  {
    _target->setId(contactPopulateTarget.value("cntct_id").toInt());
    _target->sBuildPhones();
    _targetGroup->setTitle(grpTitle + " (#" + contactPopulateTarget.value("cntct_number").toString() + ")");
  }
  else
  {
    _target->setId(-1);
    _targetGroup->setTitle(grpTitle);
  }

  sHandleProcess();
}

void contactMerge::sProcess()
{
  XSqlQuery contactProcess;
  ParameterList params;
  QString qry;

  if ((_mode->currentIndex() == 0) || (_mode->currentIndex() == 2)) // cMerge or cMergePurge
  {
    if (_mode->currentIndex() == 2 && !purgeConfirm()) // cMergePurge
      return;
    qry = "merge";
    if  (_mode->currentIndex() == 2)
      params.append("purge", true);
    else
      params.append("purge", false);
  }
  else if (_mode->currentIndex() == 1) // cPurge
  {
    if (!purgeConfirm())
      return;
    qry = "purge";
  }
  else
  {
    qry = "restore";
  }

  MetaSQLQuery mql = mqlLoad("contactmerge", qry);
  contactProcess = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Processing Requested Action"),
                                contactProcess, __FILE__, __LINE__))
  {
    return;
  }
  sFillList();
}

void contactMerge::sPurge()
{
  XSqlQuery contactPurge;
  if (!purgeConfirm())
    return;

  MetaSQLQuery mql = mqlLoad("contactmerge", "purge");

  ParameterList params;
  params.append("cntct_id", _cntct->id());
  contactPurge = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Processing Requested Action"),
                                contactPurge, __FILE__, __LINE__))
  {
    return;
  }
  sFillList();
}

bool contactMerge::purgeConfirm()
{
  QString question = tr("The purge action cannot be undone. "
                        "Are you sure you want to proceed?");
  if (QMessageBox::question(this, tr("Purge Contact Merge?"), question,
                QMessageBox::Yes,
                QMessageBox::No | QMessageBox::Default) == QMessageBox::No)
    return false;

  return true;
}

void contactMerge::sRestore()
{
  XSqlQuery contactRestore;
  MetaSQLQuery mql = mqlLoad("contactmerge", "restore");

  ParameterList params;
  params.append("cntct_id", _cntct->id());
  contactRestore = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Processing Requested Action"),
                                contactRestore, __FILE__, __LINE__))
  {
    return;
  }
  sFillList();
}

void contactMerge::sSelect(bool target)
{
  XSqlQuery contactSelect;
  MetaSQLQuery mql = mqlLoad("contactmerge", "select");

  ParameterList params;
  params.append("cntct_id", _cntct->id());
  params.append("target", QVariant(target));
  contactSelect = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Processing Requested Action"),
                                contactSelect, __FILE__, __LINE__))
  {
    return;
  }
  sFillList();
}

void contactMerge::sSelectCol()
{
  XSqlQuery contactSelectCol;
  MetaSQLQuery mql = mqlLoad("contactmerge", "selectcol");

  ParameterList params;
  params.append("cntct_id", _srccntct->id());
  params.append("col_name", _colMap[_selectCol]);
  contactSelectCol = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Processing Requested Action"),
                                contactSelectCol, __FILE__, __LINE__))
  {
    return;
  }

  sPopulateSources();
}

void contactMerge::sSetTarget()
{
  sSelect(true);
}

void contactMerge::sSrcCntctEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("cntct_id", _srccntct->id());

  contact newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
  sFillList();
}

void contactMerge::sSrcCntctView()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("cntct_id", _srccntct->id());

  contact newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
  sFillList();
}

void contactMerge::closeEvent(QCloseEvent *pEvent)
{
  XSqlQuery clear("DELETE FROM cntctsel;");
  pEvent->accept();
}
