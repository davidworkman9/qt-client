/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QtScript>

#include <metasql.h>

#include "crmacctcluster.h"
#include "custcluster.h"
#include "errorReporter.h"
#include "xcombobox.h"
#include "xtreewidget.h"

/* _listAndSearchQueryString is, as you may have guessed, shared by the
   CRMAcctList and CRMAcctSearch classes. It's a big 'un and has a couple
   of tricky parts:
   - there's potential for a UNION of customers and prospects
   - the vendor case has an extra column
   - CRMAcctList can only control activeOnly in the WHERE clause but
     CRMAcctSearch can control lots of OR'ed search criteria + activeOnly
 */
static QString _listAndSearchQueryString(
      "SELECT *, formataddr(addr.addr_id) AS street"
      "  FROM ("
      "<? if exists('crmaccount') ?>"
      "    SELECT crmacct_id AS id,         crmacct_number AS number,"
      "           crmacct_name AS name,     crmacct_cntct_id_1 AS cntct_id,"
      "           crmacct_active AS active, cntct_addr_id AS addr_id"
      "      FROM crmacct()"
      "      LEFT OUTER JOIN cntct ON (crmacct_cntct_id_1=cntct_id)"
      "<? elseif exists('customer') ?>"
      "    SELECT cust_id AS id,         cust_number AS number,"
      "           cust_name AS name,     cust_cntct_id AS cntct_id,"
      "           cust_active AS active, cntct_addr_id AS addr_id"
      "      FROM custinfo"
      "      LEFT OUTER JOIN cntct ON (cust_cntct_id=cntct_id)"
      "<? elseif exists('employee') ?>"
      "    SELECT emp_id AS id,         emp_code AS number,"
      "           emp_number AS name,   emp_cntct_id AS cntct_id,"
      "           emp_active AS active, cntct_addr_id AS addr_id"
      "      FROM emp"
      "      LEFT OUTER JOIN cntct ON (emp_cntct_id=cntct_id)"
      "<? elseif exists('salesrep') ?>"
      "    SELECT salesrep_id AS id,         salesrep_number AS number,"
      "           salesrep_name AS name,     NULL::INTEGER AS cntct_id,"
      "           salesrep_active AS active, NULL::INTEGER AS addr_id"
      "      FROM salesrep"
      "<? elseif exists('taxauth') ?>"
      "    SELECT taxauth_id AS id,     taxauth_code AS number,"
      "           taxauth_name AS name, NULL::INTEGER AS cntct_id,"
      "           true AS active,       taxauth_addr_id AS addr_id"
      "      FROM taxauth"
      "<? elseif exists('user') ?>"
      "    SELECT usr_id AS id,           usr_username AS number,"
      "           usr_propername AS name, NULL::INTEGER AS cntct_id,"
      "           usr_active AS active,   NULL::INTEGER AS addr_id"
      "      FROM usr"
      "<? elseif exists('vendor') ?>"
      "    SELECT vend_id AS id,         vend_number AS number,"
      "           vend_name AS name,     vend_cntct1_id AS cntct_id,"
      "           vend_active AS active, COALESCE(cntct_addr_id, vend_addr_id) AS addr_id,"
      "           vend_vendtype_id AS combo_id,"
      "           vendtype_code AS type"
      "      FROM vendinfo"
      "      JOIN vendtype ON (vend_vendtype_id=vendtype_id)"
      "      LEFT OUTER JOIN cntct ON (vend_cntct1_id=cntct_id)"
      "<? endif ?>"
      "<? if exists('prospect') ?>"
      "    <? if exists('customer') ?>UNION<? endif ?>"
      "    SELECT prospect_id AS id,         prospect_number AS number,"
      "           prospect_name AS name,     prospect_cntct_id AS cntct_id,"
      "           prospect_active AS active, cntct_addr_id AS addr_id"
      "      FROM prospect"
      "      LEFT OUTER JOIN cntct ON (prospect_cntct_id=cntct_id)"
      "<? endif ?>"
      "  ) AS crminfo"
      "  LEFT OUTER JOIN cntct ON (crminfo.cntct_id=cntct.cntct_id)"
      "  LEFT OUTER JOIN addr  ON (crminfo.addr_id=addr.addr_id)"
      "<? if exists('searchString') ?>"
      "   WHERE "
      "    <? if exists('activeOnly') ?> active AND <? endif ?>"
      "    <? if exists('combo_id') ?> combo_id = <? value('combo_id') ?> AND <? endif ?>"
      "      (false "
      "    <? if exists('searchNumber') ?>"
      "       OR (UPPER(number) ~ <? value('searchString') ?>)"
      "    <? endif ?>"
      "    <? if exists('searchName') ?>"
      "       OR (UPPER(name) ~ <? value('searchString') ?>)"
      "    <? endif ?>"
      "    <? if exists('searchContactName') ?>"
      "       OR (UPPER(cntct_first_name || ' ' || cntct_last_name) "
      "                 ~ <? value('searchString') ?>)"
      "    <? endif ?>"
      "    <? if exists('searchPhone') ?>"
      "       OR (UPPER(cntct_phone || ' ' || cntct_phone2 || ' ' || "
      "                 cntct_fax) ~ <? value('searchString') ?>)"
      "    <? endif ?>"
      "    <? if exists('searchEmail') ?>"
      "       OR (cntct_email ~* <? value('searchString') ?>)"
      "    <? endif ?>"
      "    <? if exists('searchStreetAddr') ?>"
      "       OR (UPPER(addr_line1 || ' ' || addr_line2 || ' ' || "
      "                 addr_line3) ~ <? value('searchString') ?>)"
      "    <? endif ?>"
      "    <? if exists('searchCity') ?>"
      "       OR (UPPER(addr_city) ~ <? value('searchString') ?>)"
      "    <? endif ?>"
      "    <? if exists('searchState') ?>"
      "       OR (UPPER(addr_state) ~ <? value('searchString') ?>)"
      "    <? endif ?>"
      "    <? if exists('searchPostalCode') ?>"
      "       OR (UPPER(addr_postalcode) ~ <? value('searchString') ?>)"
      "    <? endif ?>"
      "    <? if exists('searchCountry') ?>"
      "       OR (UPPER(addr_country) ~ <? value('searchString') ?>)"
      "    <? endif ?>"
      "    )"
      "<? else ?>"
      "  <? if exists('activeOnly') ?>"
      "   WHERE active"
      "  <? endif ?>"
      "<? endif ?>"
      " ORDER BY number;"
);

static void getCRMAcctSubtypeFromParent(QObject *pParent, CRMAcctLineEdit::CRMAcctSubtype &pType, bool &pInactive)
{
  pInactive = false;
  CRMAcctCluster  *crmcluster   = qobject_cast<CRMAcctCluster*>(pParent);
  CRMAcctLineEdit *crmedit      = qobject_cast<CRMAcctLineEdit*>(pParent);
  CustCluster     *custcluster  = qobject_cast<CustCluster*>(pParent);
  CLineEdit       *custedit     = qobject_cast<CLineEdit*>(pParent);

  if (crmcluster)
    pType = crmcluster->subtype();
  else if (crmedit)
    pType = crmedit->subtype();
  else if (custcluster || custedit || pParent->inherits("CustInfo"))
  {
    CLineEdit::CLineEditTypes type = CLineEdit::AllCustomersAndProspects;

    if (custedit)
      type = custedit->type();
    else if (custcluster)
      type = custcluster->type();

    switch (type)
    {
      case CLineEdit::AllCustomers:
	pInactive = true;
	pType = CRMAcctLineEdit::Cust;
	break;

      case CLineEdit::ActiveCustomers:
	pType = CRMAcctLineEdit::Cust;
	break;

      case CLineEdit::AllProspects:
	pInactive = true;
	pType = CRMAcctLineEdit::Prospect;
	break;

      case CLineEdit::ActiveProspects:
	pType = CRMAcctLineEdit::Prospect;
	break;

      case CLineEdit::AllCustomersAndProspects:
	pInactive = true;
	pType = CRMAcctLineEdit::CustAndProspect;
	break;

      case CLineEdit::ActiveCustomersAndProspects:
	pType = CRMAcctLineEdit::CustAndProspect;
	break;
    }
  }

  else if (pParent->inherits("VendorLineEdit") || pParent->inherits("VendorCluster"))
    pType = CRMAcctLineEdit::Vend;
  else if (pParent->inherits("UsernameCluster"))
    pType = CRMAcctLineEdit::User;
  else
    pType = CRMAcctLineEdit::Crmacct;
}

CRMAcctCluster::CRMAcctCluster(QWidget* pParent, const char* pName) :
    VirtualCluster(pParent, pName)
{
    addNumberWidget(new CRMAcctLineEdit(this, pName));
    setNameVisible(true);
    setSubtype(CRMAcctLineEdit::Crmacct);
}

void CRMAcctCluster::setSubtype(const CRMAcctLineEdit::CRMAcctSubtype subtype)
{
  // is this calling setSubtype on a class that sets its own type in its
  // constructor?

  CRMAcctLineEdit *w = qobject_cast<CRMAcctLineEdit*>(_number);
  if (w) w->setSubtype(subtype);
}

///////////////////////////////////////////////////////////////////////

CRMAcctLineEdit::CRMAcctSubtype CRMAcctCluster::subtype() const
{
  CRMAcctLineEdit *w = qobject_cast<CRMAcctLineEdit*>(_number);
  return w ? w->subtype() : CRMAcctLineEdit::Crmacct;
}

CRMAcctLineEdit::CRMAcctLineEdit(QWidget* pParent, const char* pName) :
    CrmClusterLineEdit(pParent, "crmacct", "crmacct_id", "crmacct_number", "crmacct_name", 0, "crmacct_owner_username", 0, 0, pName, "crmacct_active")
{
  _objtype = "CRMA";
  setTitles(tr("Account"), tr("Accounts"));
  setUiName("crmaccount");
  setEditPriv("MaintainAllCRMAccounts");
  setViewPriv("ViewAllCRMAccounts");
  setNewPriv("MaintainAllCRMAccounts");
  setEditOwnPriv("MaintainPersonalCRMAccounts");
  setViewOwnPriv("ViewPersonalCRMAccounts");

  setSubtype(Crmacct);
}

VirtualList* CRMAcctLineEdit::listFactory()
{
  return new CRMAcctList(this);
}

VirtualSearch* CRMAcctLineEdit::searchFactory()
{
  return new CRMAcctSearch(this);
}

///////////////////////////////////////////////////////////////////////

void CRMAcctLineEdit::setSubtype(const CRMAcctSubtype subtype)
{
  _subtype = subtype;
  //TODO: refigure everything about this line edit, including find the id for the current text
}

CRMAcctLineEdit::CRMAcctSubtype CRMAcctLineEdit::subtype() const
{
  return _subtype;
}

///////////////////////////////////////////////////////////////////////

CRMAcctList::CRMAcctList(QWidget* pParent, const char* pName, bool, Qt::WindowFlags pFlags) :
  VirtualList(pParent, pFlags)
{
  _parent = pParent;
  _queryParams = 0;

  if (!pName)
    setObjectName("CRMAcctList");

  _listTab->setColumnCount(0);

  _listTab->addColumn(tr("Number"),      80, Qt::AlignLeft,  true, "number");
  _listTab->addColumn(tr("Name"),        75, Qt::AlignLeft,  true, "name"  );
  _listTab->addColumn(tr("First"),      100, Qt::AlignLeft,  true, "cntct_first_name");
  _listTab->addColumn(tr("Last"),       100, Qt::AlignLeft,  true, "cntct_last_name");
  _listTab->addColumn(tr("Phone"),      100, Qt::AlignLeft,  true, "cntct_phone");
  _listTab->addColumn(tr("Email"),      100, Qt::AlignLeft,  true, "cntct_email");
  _listTab->addColumn(tr("Address"),    100, Qt::AlignLeft|Qt::AlignTop,true,"street");
  _listTab->addColumn(tr("City"),        75, Qt::AlignLeft,  true, "addr_city");
  _listTab->addColumn(tr("State"),       50, Qt::AlignLeft,  true, "addr_state");
  _listTab->addColumn(tr("Country"),    100, Qt::AlignLeft,  true, "addr_country");
  _listTab->addColumn(tr("Postal Code"), 75, Qt::AlignLeft,  true, "addr_postalcode");

  _showInactive = false;
  CRMAcctLineEdit::CRMAcctSubtype type = CRMAcctLineEdit::Crmacct;
  getCRMAcctSubtypeFromParent(_parent, type, _showInactive);
  setSubtype(type);

  resize(800, 600);
}

void CRMAcctList::setId(const int id)
{
  _id = id;
  _listTab->setId(id);
}

void CRMAcctList::setShowInactive(const bool show)
{

  if (_showInactive != show)
  {

    _showInactive = show;
    sFillList();
  }
}

void CRMAcctList::setSubtype(const CRMAcctLineEdit::CRMAcctSubtype subtype)
{
  _subtype = subtype;
  if (_queryParams)
    delete _queryParams;
  _queryParams = new ParameterList();

  bool hasContact = true;
  bool hasAddress = true;

  switch (subtype)
  {
  case CRMAcctLineEdit::Cust:

    setWindowTitle(tr("Search For Customer"));
    _queryParams->append("customer");
    break;

  case CRMAcctLineEdit::Employee:
    setWindowTitle(tr("Search For Employee"));
    _queryParams->append("employee");
    break;

  case CRMAcctLineEdit::Prospect:
    setWindowTitle(tr("Search For Prospect"));
    _queryParams->append("prospect");
    break;

  case CRMAcctLineEdit::SalesRep:
    setWindowTitle(tr("Search For Sales Rep"));
    _queryParams->append("salesrep");
    hasContact = false;
    hasAddress = false;
    break;

  case CRMAcctLineEdit::Taxauth:
    setWindowTitle(tr("Search For Tax Authority"));
    _queryParams->append("taxauth");
    hasContact = false;
    break;

  case CRMAcctLineEdit::User:
    setWindowTitle(tr("Search For User Account"));
    _queryParams->append("user");
    hasContact = false;
    hasAddress = false;
    break;

  case CRMAcctLineEdit::Vend:
    setWindowTitle(tr("Search For Vendor"));
    _queryParams->append("vendor");
    if (!(_listTab->column("type") > 0))
      _listTab->addColumn("Vend. Type", _itemColumn, Qt::AlignLeft, true, "type");
    break;

  case CRMAcctLineEdit::CustAndProspect:
    setWindowTitle(tr("Search For Customer or Prospect"));
    _queryParams->append("customer");
    _queryParams->append("prospect");
    break;

  case CRMAcctLineEdit::Crmacct:
  case CRMAcctLineEdit::Competitor:
  case CRMAcctLineEdit::Partner:
  default:
    setWindowTitle(tr("Search For Account"));
    _queryParams->append("crmaccount");
    break;
  }

  _listTab->setColumnHidden(_listTab->column("cntct_first_name"), ! hasContact);
  _listTab->setColumnHidden(_listTab->column("cntct_last_name"),  ! hasContact);
  _listTab->setColumnHidden(_listTab->column("cntct_phone"),      ! hasContact);
  _listTab->setColumnHidden(_listTab->column("street"),           ! hasAddress);
  _listTab->setColumnHidden(_listTab->column("addr_city"),        ! hasAddress);
  _listTab->setColumnHidden(_listTab->column("addr_state"),       ! hasAddress);
  _listTab->setColumnHidden(_listTab->column("addr_country"),     ! hasAddress);
  _listTab->setColumnHidden(_listTab->column("addr_postalcode"),  ! hasAddress);

  // don't (re)populate the list if the widget isn't visible
  // this also fixes an issue where the subType is being set
  // multiple times before it is visible and thus running
  // multiple queries and processing results for no reason
  if(isVisible())
    sFillList();
}

void CRMAcctList::sFillList()
{

  MetaSQLQuery mql(_listAndSearchQueryString);
  ParameterList params(*_queryParams);
  if (! _showInactive)
    params.append("activeOnly");

  XSqlQuery fillq = mql.toQuery(params);

  _listTab->populate(fillq);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Database Error"),
                           fillq, __FILE__, __LINE__))
    return;
}

///////////////////////////////////////////////////////////////////////

CRMAcctSearch::CRMAcctSearch(QWidget* pParent, Qt::WindowFlags pFlags) :
  VirtualSearch(pParent, pFlags)
{

  // remove the stuff we won't use
  disconnect(_searchDescrip,	SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  selectorsLyt->removeWidget(_searchDescrip);
  delete _searchDescrip;

  _queryParams = 0;

  _listTab->setColumnCount(0);

  _addressLit	    = new QLabel(tr("Primary Contact Address:"),this);
  _searchStreet	    = new XCheckBox(tr("Street Address"), this);
  _searchCity	    = new XCheckBox(tr("City"),this);
  _searchState	    = new XCheckBox(tr("State"),this);
  _searchPostalCode = new XCheckBox(tr("Postal Code"),this);
  _searchCountry    = new XCheckBox(tr("Country"),this);
  _searchContact    = new XCheckBox(tr("Contact Name"),this);
  _searchPhone	    = new XCheckBox(tr("Contact Phone #"),this);
  _searchEmail	    = new XCheckBox(tr("Contact Email "),this);
  _showInactive	    = new QCheckBox(tr("Show Inactive"),this);
  _searchCombo      = new XCheckBox(tr("Search Combo"),this);
  _comboCombo       = new XComboBox(this, "_comboCombo");
  
  _addressLit->setObjectName("_addressLit");
  _searchStreet->setObjectName("_searchStreet");
  _searchCity->setObjectName("_searchCity");
  _searchState->setObjectName("_searchState");
  _searchPostalCode->setObjectName("_searchPostalCode");
  _searchCountry->setObjectName("_searchCountry");
  _searchContact->setObjectName("_searchContact");
  _searchPhone->setObjectName("_searchPhone");
  _searchEmail->setObjectName("_searchEmail");
  _showInactive->setObjectName("_showInactive");
  _searchCombo->setObjectName("_searchCombo");

  selectorsLyt->removeWidget(_searchName);
  selectorsLyt->removeWidget(_searchNumber);
  QLabel * lbl = new QLabel(tr("Search through:"), this);
  lbl->setObjectName("_searchLit");
  selectorsLyt->addWidget(lbl,                  0, 0);
  selectorsLyt->addWidget(_searchNumber,	1, 0);
  selectorsLyt->addWidget(_searchName,		2, 0);
  selectorsLyt->addWidget(_searchContact,	1, 1);
  selectorsLyt->addWidget(_searchPhone,		2, 1);
  selectorsLyt->addWidget(_searchEmail,		3, 1);
  selectorsLyt->addWidget(_addressLit,		0, 2);
  selectorsLyt->addWidget(_searchStreet,	1, 2);
  selectorsLyt->addWidget(_searchCity,		2, 2);
  selectorsLyt->addWidget(_searchState,		3, 2);
  selectorsLyt->addWidget(_searchPostalCode,	4, 2);
  selectorsLyt->addWidget(_searchCombo,         5, 0);
  selectorsLyt->addWidget(_comboCombo,          5, 1);
  selectorsLyt->addWidget(_searchCountry,	5, 2);
  selectorsLyt->addWidget(_showInactive,	5, 3);

  _listTab->addColumn(tr("Number"),      80, Qt::AlignLeft,  true, "number");
  _listTab->addColumn(tr("Name"),        75, Qt::AlignLeft,  true, "name"  );
  _listTab->addColumn(tr("First"),      100, Qt::AlignLeft,  true, "cntct_first_name");
  _listTab->addColumn(tr("Last"),       100, Qt::AlignLeft,  true, "cntct_last_name");
  _listTab->addColumn(tr("Phone"),      100, Qt::AlignLeft,  true, "cntct_phone");
  _listTab->addColumn(tr("Email"),      100, Qt::AlignLeft,  true, "cntct_email");
  _listTab->addColumn(tr("Address"),    100, Qt::AlignLeft|Qt::AlignTop,true,"street");
  _listTab->addColumn(tr("City"),        75, Qt::AlignLeft,  true, "addr_city");
  _listTab->addColumn(tr("State"),       50, Qt::AlignLeft,  true, "addr_state");
  _listTab->addColumn(tr("Country"),    100, Qt::AlignLeft,  true, "addr_country");
  _listTab->addColumn(tr("Postal Code"), 75, Qt::AlignLeft,  true, "addr_postalcode");

  setTabOrder(_search,		_searchNumber);
  setTabOrder(_searchNumber,	_searchName);
  setTabOrder(_searchName,	_searchContact);
  setTabOrder(_searchContact,	_searchPhone);
  setTabOrder(_searchPhone,	_searchEmail);
  setTabOrder(_searchEmail,	_searchStreet);
  setTabOrder(_searchStreet,	_searchCity);
  setTabOrder(_searchCity,	_searchState);
  setTabOrder(_searchState,	_searchPostalCode);
  setTabOrder(_searchPostalCode,_searchCountry);
  setTabOrder(_searchCountry,	_searchCombo);
  setTabOrder(_searchCombo,     _comboCombo);
  setTabOrder(_comboCombo,      _showInactive);
  setTabOrder(_showInactive,	_listTab);
  setTabOrder(_listTab,		_buttonBox);
  setTabOrder(_buttonBox,	_search);

  resize(800, 600);
  
  _parent = pParent;
  setObjectName("crmacctSearch");

  bool shouldShowInactive = false;
  CRMAcctLineEdit::CRMAcctSubtype type = CRMAcctLineEdit::Crmacct;
  getCRMAcctSubtypeFromParent(_parent, type, shouldShowInactive);
  setSubtype(type);
  _showInactive->setChecked(shouldShowInactive);

  // do this late so the constructor can set defaults without triggering queries
  connect(_searchStreet, SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_searchCity,   SIGNAL(toggled(bool)),	this, SLOT(sFillList()));
  connect(_searchState,  SIGNAL(toggled(bool)),	this, SLOT(sFillList()));
  connect(_searchPostalCode,SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_searchCountry,SIGNAL(toggled(bool)),	this, SLOT(sFillList()));
  connect(_searchContact,SIGNAL(toggled(bool)),	this, SLOT(sFillList()));
  connect(_searchPhone,	 SIGNAL(toggled(bool)),	this, SLOT(sFillList()));
  connect(_searchEmail,	 SIGNAL(toggled(bool)),	this, SLOT(sFillList()));
  connect(_searchCombo,  SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_comboCombo,   SIGNAL(newID(int)),    this, SLOT(sFillList()));

  _search->setFocus();
}

void CRMAcctSearch::setId(const int id)
{
  _id = id;
  _listTab->setId(id);
}

void CRMAcctSearch::setShowInactive(const bool show)
{
  if (show != _showInactive->isChecked())
    _showInactive->setChecked(show);
}

void CRMAcctSearch::setSubtype(const CRMAcctLineEdit::CRMAcctSubtype subtype)
{
  _subtype = subtype;
  if (_queryParams)
    delete _queryParams;
  _queryParams = new ParameterList();

  if(subtype != CRMAcctLineEdit::Vend)
  {
    _searchCombo->hide();
    _comboCombo->hide();
  }
 
  bool hasContact = true;
  bool hasAddress = true;

  switch (subtype)
  {
  case CRMAcctLineEdit::Cust:
    setWindowTitle(tr("Search For Customer"));
    _queryParams->append("customer");

    _searchNumber->setText(tr("Customer Number"));
    _searchName->setText(tr("Customer Name"));
    _searchContact->setText(tr("Billing Contact Name"));
    _searchPhone->setText(tr("Billing Contact Phone #"));
    _searchEmail->setText(tr("Billing Contact Email"));
    _addressLit->setText(tr("Billing Contact Address:"));
    break;

  case CRMAcctLineEdit::Employee:
    setWindowTitle(tr("Search For Employee"));
    _queryParams->append("employee");

    _searchNumber->setText(tr("Employee Code"));
    _searchName->setText(tr("Employee Number"));
    _searchContact->setText(tr("Contact Name"));
    _searchPhone->setText(tr("Contact Phone #"));
    _searchEmail->setText(tr("Contact Email"));
    _addressLit->setText(tr("Contact Address:"));
    break;

  case CRMAcctLineEdit::Prospect:
    setWindowTitle(tr("Search For Prospect"));
    _queryParams->append("prospect");

    _searchNumber->setText(tr("Prospect Number"));
    _searchName->setText(tr("Prospect Name"));
    break;

  case CRMAcctLineEdit::SalesRep:
    setWindowTitle(tr("Search For Sales Rep"));
    _queryParams->append("salesrep");

    _searchNumber->setText(tr("Sales Rep Number"));
    _searchName->setText(tr("Sales Rep Name"));

    hasContact = false;
    hasAddress = false;
    break;

  case CRMAcctLineEdit::Taxauth:
    setWindowTitle(tr("Search For Tax Authority"));
    _queryParams->append("taxauth");

    _searchNumber->setText(tr("Tax Authority Code"));
    _searchName->setText(tr("Tax Authority Name"));

    hasContact = false;
    _addressLit->setText(tr("Tax Authority Address:"));
    _showInactive->setVisible(false);
    break;

  case CRMAcctLineEdit::User:
    setWindowTitle(tr("Search For User Account"));
    _queryParams->append("user");

    _searchNumber->setText(tr("Username"));
    _searchName->setText(tr("User Proper Name"));

    hasContact = false;
    hasAddress = false;
    break;

  case CRMAcctLineEdit::Vend:
    setWindowTitle(tr("Search For Vendor"));
    _queryParams->append("vendor");

    _searchCombo->setText(tr("Vendor Type:"));
    _comboCombo->setType(XComboBox::VendorTypes);
    _searchNumber->setText(tr("Vendor Number"));
    _searchName->setText(tr("Vendor Name"));
    _addressLit->setText(tr("Main Address:"));
    if (!(_listTab->column("type") > 0))
      _listTab->addColumn("Vend. Type", _itemColumn, Qt::AlignLeft, true, "type");
    break;

  case CRMAcctLineEdit::CustAndProspect:
    setWindowTitle(tr("Search For Customer or Prospect"));
    _queryParams->append("customer");
    _queryParams->append("prospect");

    _searchNumber->setText(tr("Number"));
    _searchName->setText(tr("Name"));
    _searchContact->setText(tr("Billing or Primary Contact Name"));
    _searchPhone->setText(tr("Billing or Primary Contact Phone #"));
    _searchEmail->setText(tr("Billing or Primary Contact Email"));
    _addressLit->setText(tr("Billing or Primary Contact Address:"));
    break;

  case CRMAcctLineEdit::Crmacct:
  case CRMAcctLineEdit::Competitor:
  case CRMAcctLineEdit::Partner:
  default:
    setWindowTitle(tr("Search For Account"));
    _queryParams->append("crmaccount");

    _searchNumber->setText(tr("Account Number"));
    _searchName->setText(tr("Account Name"));
    _searchContact->setText(tr("Primary Contact Name"));
    _searchPhone->setText(tr("Primary Contact Phone #"));
    _searchEmail->setText(tr("Primary Contact Email"));
    _addressLit->setText(tr("Primary Contact Address:"));
    break;
  }

  _searchContact->setVisible(hasContact);
  _searchPhone->setVisible(hasContact);
  _searchEmail->setVisible(hasContact);
  _addressLit->setVisible(hasAddress);

  _listTab->setColumnHidden(_listTab->column("cntct_first_name"), ! hasContact);
  _listTab->setColumnHidden(_listTab->column("cntct_last_name"),  ! hasContact);
  _listTab->setColumnHidden(_listTab->column("cntct_phone"),      ! hasContact);
  _listTab->setColumnHidden(_listTab->column("street"),           ! hasAddress);
  _listTab->setColumnHidden(_listTab->column("addr_city"),        ! hasAddress);
  _listTab->setColumnHidden(_listTab->column("addr_state"),       ! hasAddress);
  _listTab->setColumnHidden(_listTab->column("addr_country"),     ! hasAddress);
  _listTab->setColumnHidden(_listTab->column("addr_postalcode"),  ! hasAddress);

}

void CRMAcctSearch::sFillList()
{
  if (_search->text().trimmed().length() == 0)
    return;

  MetaSQLQuery mql(_listAndSearchQueryString);
  ParameterList params(*_queryParams);
  params.append("searchString", _search->text().trimmed().toUpper());

  if (! _showInactive->isChecked())
    params.append("activeOnly");

  if (_searchNumber->isChecked())
    params.append("searchNumber");

  if (_searchName->isChecked())
    params.append("searchName");

  // some crmacct types don't have contacts (yet)
  if (_subtype != CRMAcctLineEdit::SalesRep &&
      _subtype != CRMAcctLineEdit::Taxauth  &&
      _subtype != CRMAcctLineEdit::User)
  {
    if (_searchContact->isChecked())
      params.append("searchContactName");

    if (_searchPhone->isChecked())
      params.append("searchPhone");
      
    if (_searchEmail->isChecked())
      params.append("searchEmail");
  }

  // some crmacct types don't have addresses (yet)
  if (_subtype != CRMAcctLineEdit::SalesRep &&
      _subtype != CRMAcctLineEdit::User)
  {
    if (_searchStreet->isChecked())
      params.append("searchStreetAddr");

    if (_searchCity->isChecked())
      params.append("searchCity");

    if (_searchState->isChecked())
      params.append("searchState");

    if (_searchPostalCode->isChecked())
      params.append("searchPostalCode");

    if (_searchCountry->isChecked())
      params.append("searchCountry");
  }

  if (_searchCombo->isChecked())
    params.append("combo_id", _comboCombo->id());

  XSqlQuery fillq = mql.toQuery(params);

  _listTab->populate(fillq);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Database Error"),
                           fillq, __FILE__, __LINE__))
    return;
}

// script exposure /////////////////////////////////////////////////////////////

QScriptValue CRMAcctSubtypeToScriptValue(QScriptEngine *engine, const enum CRMAcctLineEdit::CRMAcctSubtype &val)
{
  return QScriptValue(engine, (int)val);
}

void CRMAcctSubtypeFromScriptValue(const QScriptValue &obj, enum CRMAcctLineEdit::CRMAcctSubtype &val)
{
  val = (enum CRMAcctLineEdit::CRMAcctSubtype)obj.toInt32();
}

void setupCRMAcctLineEdit(QScriptEngine *engine)
{
  if (! engine->globalObject().property("CRMAcctLineEdit").isObject())
  {
#if QT_VERSION >= 0x050500
    qScriptRegisterMetaType(engine, CRMAcctSubtypeToScriptValue, CRMAcctSubtypeFromScriptValue);
#endif

    QScriptValue ctor = engine->newObject(); //engine->newFunction(scriptconstructor);
    QScriptValue meta = engine->newQMetaObject(&CRMAcctLineEdit::staticMetaObject, ctor);

    engine->globalObject().setProperty("CRMAcctLineEdit", meta,
                                       QScriptValue::ReadOnly | QScriptValue::Undeletable);
  }
}
