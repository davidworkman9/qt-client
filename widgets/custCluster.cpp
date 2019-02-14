/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQueryModel>
#include <QtScript>

#include <metasql.h>
#include <parameter.h>
#include <xsqlquery.h>

#include "custcluster.h"
#include "format.h"
#include "storedProcErrorLookup.h"

static const int CREDITSTATUS = 4;      // where do these come from?
static const int CRMACCT_ID   = 5;
static const int ISCUSTOMER   = 6;

#define DEBUG false

CLineEdit::CLineEdit(QWidget *pParent, const char *pName) :
  VirtualClusterLineEdit(pParent, "cust", "id", "number", "name", "description", 0, pName, "active")
{
  _crmacctId = -1;
  _type     = AllCustomers;
  _subtype  = CRMAcctLineEdit::Cust;
  _canEdit = false;
  _editMode = false;

  setTitles(tr("Customer"), tr("Customers"));

  _query = "SELECT cust.*,"
           "       addr_line1 AS description"
           "  FROM ("
           "    SELECT cust_id AS id,"
           "           cust_number AS number,"
           "           cust_name AS name,"
           "           cust_active AS active,"
           "           cust_creditstatus,"
           "           crmacct_id, true AS iscustomer,"
           "           cust_cntct_id"
           "      FROM custinfo"
           "      JOIN crmacct ON crmacct_cust_id = cust_id"
           "     WHERE :number IS NULL or cust_number ~ UPPER(:number)"
           "    UNION ALL"
           "    SELECT prospect_id AS id,"
           "           prospect_number AS number,"
           "           prospect_name AS name,"
           "           prospect_active AS active,"
           "           'G' AS cust_creditstatus,"
           "           crmacct_id, false AS iscustomer,"
           "           prospect_cntct_id AS cust_cntct_id"
           "      FROM prospect"
           "      LEFT OUTER JOIN crmacct ON crmacct_prospect_id = prospect_id"
           "     WHERE :number IS NULL or prospect_number ~ UPPER(:number)"
           "  ) cust"
           "  LEFT OUTER JOIN cntct ON cust_cntct_id = cntct_id"
           "  LEFT OUTER JOIN addr  ON cntct_addr_id = addr_id"
           " WHERE true ";

  _modeSep = 0;
  _modeAct = new QAction(tr("Edit Number"), this);
  _modeAct->setToolTip(tr("Sets number for editing"));
  _modeAct->setCheckable(true);

  setUiName("customer");
  setEditPriv("MaintainCustomerMasters");
  setViewPriv("ViewCustomerMasters");
  setNewPriv("MaintainCustomerMasters");

  connect(_modeAct, SIGNAL(triggered(bool)), this, SLOT(setEditMode(bool)));
}

void CLineEdit::sNew()
{
  QString uiName="customer";
  ParameterList params;
  QMessageBox ask(this);
  ask.setIcon(QMessageBox::Question);
  QPushButton *pbutton = ask.addButton(tr("Prospect"), QMessageBox::YesRole);
  QPushButton *cbutton = ask.addButton(tr("Customer"), QMessageBox::YesRole);
  ask.setDefaultButton(cbutton);
  ask.setWindowTitle(tr("Customer or Prospect?"));

  if (_subtype == CRMAcctLineEdit::Prospect ||
      (_subtype == CRMAcctLineEdit::CustAndProspect &&
       !_x_privileges->check("MaintainCustomerMasters")))
  {
    params.append("mode", "new");
    uiName="prospect";
  }
  if (_subtype == CRMAcctLineEdit::CustAndProspect &&
       !_x_privileges->check("MaintainProspectMasters"))
    params.append("mode", "new");
  else
  {
    if (_subtype == CRMAcctLineEdit::Cust)
      ask.setText(tr("<p>Would you like to create a new Customer or convert "
                     "an existing Prospect?"));
    else
      ask.setText(tr("<p>Would you like to create a new Customer or "
                     "a new Prospect?"));

    ask.exec();

    if (ask.clickedButton() == pbutton &&
        _subtype == CRMAcctLineEdit::Cust)  // converting prospect
    {
      int prospectid = -1;
      if (_x_preferences->value("DefaultEllipsesAction") == "search")
      {
        CRMAcctSearch* search = new CRMAcctSearch(this);
        search->setSubtype(CRMAcctLineEdit::Prospect);
        prospectid = search->exec();
      }
      else
      {
        CRMAcctList* list = new CRMAcctList(this);
        list->setSubtype(CRMAcctLineEdit::Prospect);
        prospectid = list->exec();
      }

      if (prospectid > 0)
      {
        XSqlQuery convertq;
        convertq.prepare("SELECT convertProspectToCustomer(:id) AS result;");
        convertq.bindValue(":id", prospectid);
        convertq.exec();
        if (convertq.first())
        {
          int result = convertq.value("result").toInt();
          if (result < 0)
          {
            QMessageBox::critical(this, tr("Processing Error"),
                                  storedProcErrorLookup("convertProspectToCustomer", result));
            return;
          }
          params.append("cust_id", prospectid);
          params.append("mode", "edit");
        }
      }
      else
        return;
    }
    else
    {
      params.append("mode", "new");
      if (ask.clickedButton() == pbutton)
        uiName = "prospect";
    }
  }

  sOpenWindow(uiName, params);
}

void CLineEdit::setId(int pId)
{
  VirtualClusterLineEdit::setId(pId);
  if (model() && _id != -1)
  {
    if (model()->data(model()->index(0,ISCUSTOMER)).toBool())
    {
      setUiName("customer");
      setEditPriv("MaintainCustomerMasters");
      setViewPriv("ViewCustomerMasters");
      setNewPriv("MaintainCustomerMasters");
      _idColName="cust_id";
    }
    else
    {
      setUiName("prospect");
      setEditPriv("MaintainProspectMasters");
      setViewPriv("ViewProspectMasters");
      setNewPriv("MaintainProspectMasters");
      _idColName="prospect_id";
    }
    sUpdateMenu();

    _crmacctId = model()->data(model()->index(0,CRMACCT_ID)).toInt();

    emit newCrmacctId(_crmacctId);

    // Handle Credit Status
    QString status = model()->data(model()->index(0,CREDITSTATUS)).toString();

    if (!editMode() && status != "G")
    {
      if (status == "W")
        _menuLabel->setPixmap(QPixmap(":/widgets/images/credit_warn.png"));
      else
        _menuLabel->setPixmap(QPixmap(":/widgets/images/credit_hold.png"));

      return;
    }
  }

  if (_editMode)
    _menuLabel->setPixmap(QPixmap(":/widgets/images/edit.png"));
  else
    _menuLabel->setPixmap(QPixmap(":/widgets/images/magnifier.png"));
}

void CLineEdit::setType(CLineEditTypes pType)
{
  _type = pType;
  QStringList list;
  switch (_type)
  {
  case ActiveCustomers:
    list.append("active");
    // fall-through
  case AllCustomers:
    list.append("iscustomer");
    _subtype = CRMAcctLineEdit::Cust;
    break;

  case ActiveProspects:
    list.append("active");
    // fall-through
  case AllProspects:
    list.append("NOT iscustomer");
    _subtype = CRMAcctLineEdit::Prospect;
    break;

  case ActiveCustomersAndProspects:
    list.append("active");
    // fall-through
  case AllCustomersAndProspects:
    _subtype = CRMAcctLineEdit::CustAndProspect;
    break;
  }
  list.removeDuplicates();
  setExtraClause(list.join(" AND "));
}

VirtualList* CLineEdit::listFactory()
{
  CRMAcctList* list = new CRMAcctList(this);
  list->setSubtype(_subtype);
  return list;
}

VirtualSearch* CLineEdit::searchFactory()
{
  CRMAcctSearch* search = new CRMAcctSearch(this);
  search->setSubtype(_subtype);
  return search;
}

bool CLineEdit::canEdit()
{
  return _canEdit;
}

void CLineEdit::setCanEdit(bool p)
{
  if (p == _canEdit || !_x_metrics)
    return;

  if (p)
  {
    if (_x_privileges && _subtype == CRMAcctLineEdit::Cust)
      _canEdit = _x_privileges->check("MaintainCustomerMasters") &&
                 (_newMode ? true : _x_privileges->check("MaintainCustomerNumbers"));
    else if (_x_privileges && _subtype == CRMAcctLineEdit::Prospect)
      _canEdit = _x_privileges->check("MaintainProspectMasters") &&
                 (_newMode ? true : _x_privileges->check("MaintainCustomerNumbers"));
    else if (_x_privileges)
      _canEdit = (_x_privileges->check("MaintainCustomerMasters") ||
                 _x_privileges->check("MaintainProspectMasters")) &&
                 (_newMode ? true : _x_privileges->check("MaintainCustomerNumbers"));
  }
  else
    _canEdit=p;

  if (!_canEdit)
    setEditMode(false);

  sUpdateMenu();
}

bool CLineEdit::setNewMode(bool p)
{
  return _newMode=p;
}

bool CLineEdit::editMode()
{
  return _editMode;
}

bool CLineEdit::setEditMode(bool p)
{
  if (p == _editMode)
    return p;

  if (!_canEdit)
    return false;

  _editMode=p;
  _modeAct->setChecked(p);

  if (_x_preferences)
  {
    if (!_x_preferences->boolean("ClusterButtons"))
    {
      if (_editMode)
        _menuLabel->setPixmap(QPixmap(":/widgets/images/edit.png"));
      else
        _menuLabel->setPixmap(QPixmap(":/widgets/images/magnifier.png"));
    }

    if (!_x_metrics->boolean("DisableAutoComplete") && _editMode)
      disconnect(this, SIGNAL(textEdited(QString)), this, SLOT(sHandleCompleter()));
    else if (!_x_metrics->boolean("DisableAutoComplete"))
      connect(this, SIGNAL(textEdited(QString)), this, SLOT(sHandleCompleter()));
  }
  sUpdateMenu();

  setDisabled(_editMode &&
              _x_metrics->value("CRMAccountNumberGeneration") == "A");

 if (!_editMode)
   selectAll();

  emit editable(p);
  return p;
}

void CLineEdit::sParse()
{
  if (_editMode)
    return;

  VirtualClusterLineEdit::sParse();
}

void CLineEdit::sUpdateMenu()
{
  VirtualClusterLineEdit::sUpdateMenu();
  if (_x_preferences)
  {
    if (_x_preferences->boolean("ClusterButtons"))
      return;
  }
  else
    return;

  if (_canEdit)
  {
    if (!menu()->actions().contains(_modeAct))
    {
      _infoAct->setVisible(false);
      menu()->addAction(_modeAct);
    }

    _listAct->setDisabled(_editMode);
    _searchAct->setDisabled(_editMode);
    _listAct->setVisible(!_editMode);
    _searchAct->setVisible(!_editMode);
  }
  else
  {
    if (menu()->actions().contains(_modeAct))
    {
      _infoAct->setVisible(true);
      menu()->removeAction(_modeAct);
    }
  }

  // Handle New
  bool canNew = false;

  if (_subtype == CRMAcctLineEdit::Cust)
    canNew = (_x_privileges->check("MaintainCustomerMasters"));
  else if (_subtype == CRMAcctLineEdit::Prospect)
    canNew = (_x_privileges->check("MaintainProspectMasters"));
  else
    canNew = (_x_privileges->check("MaintainCustomerMasters") ||
              _x_privileges->check("MaintainProspectMasters"));

  _newAct->setEnabled(canNew && isEnabled());
}

bool CLineEdit::canOpen()
{
  return VirtualClusterLineEdit::canOpen() && !canEdit();
}


//////////////////////////////////////////////////////////////

CustCluster::CustCluster(QWidget *pParent, const char *pName) :
    VirtualCluster(pParent, pName)
{
  addNumberWidget(new CLineEdit(this, pName));

  CLineEdit* number = static_cast<CLineEdit*>(_number);
  connect(number, SIGNAL(newCrmacctId(int)), this, SIGNAL(newCrmacctId(int)));
  connect(number, SIGNAL(editable(bool)), this, SIGNAL(editable(bool)));
  connect(number, SIGNAL(editable(bool)), this, SLOT(sHandleEditMode(bool)));

  setLabel(tr("Customer #:"));
  setNameVisible(true);
  setDescriptionVisible(true);
}

void CustCluster::setType(CLineEdit::CLineEditTypes pType)
{
  static_cast<CLineEdit*>(_number)->setType(pType);
}

bool CustCluster::setNewMode(bool p) const
{
  return static_cast<CLineEdit*>(_number)->setNewMode(p);
}

bool CustCluster::setEditMode(bool p) const
{
  return static_cast<CLineEdit*>(_number)->setEditMode(p);
}

void CustCluster::sHandleEditMode(bool p)
{
  CLineEdit* number = static_cast<CLineEdit*>(_number);

  if (p)
    connect(number, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
  else
    disconnect(number, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
}

// script api //////////////////////////////////////////////////////////////////

QScriptValue CLineEditTypesToScriptValue(QScriptEngine *engine,
                                         const CLineEdit::CLineEditTypes &type)
{
  return QScriptValue(engine, (int)type);
}

void CLineEditTypesFromScriptValue(const QScriptValue &obj, CLineEdit::CLineEditTypes &type)
{
  type = (CLineEdit::CLineEditTypes)obj.toInt32();
}

void setupCLineEdit(QScriptEngine *engine)
{
  if (! engine->globalObject().property("CLineEdit").isObject())
  {
#if QT_VERSION >= 0x050500
    qScriptRegisterMetaType(engine, CLineEditTypesToScriptValue, CLineEditTypesFromScriptValue);
#endif

    QScriptValue ctor = engine->newObject(); //engine->newFunction(scriptconstructor);
    QScriptValue meta = engine->newQMetaObject(&CLineEdit::staticMetaObject, ctor);

    engine->globalObject().setProperty("CLineEdit", meta,
                                       QScriptValue::ReadOnly | QScriptValue::Undeletable);
  }
}
