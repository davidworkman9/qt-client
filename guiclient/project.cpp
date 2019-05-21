  /*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "project.h"

#include <QMenu>
#include <QAction>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>
#include <openreports.h>
#include <comment.h>
#include <metasql.h>

#include "mqlutil.h"
#include "errorReporter.h"
#include "guiErrorCheck.h"
#include "incident.h"
#include "invoice.h"
#include "invoiceItem.h"
#include "opportunity.h"
#include "salesOrder.h"
#include "salesOrderItem.h"
#include "task.h"
#include "workOrder.h"
#include "purchaseRequest.h"
#include "purchaseOrder.h"
#include "purchaseOrderItem.h"

const char *_projectStatuses[] = { "N", "P", "O", "C" };

bool project::userHasPriv(const int pMode, const int pId)
{
  if (_privileges->check("MaintainAllProjects"))
    return true;
  bool personalPriv = _privileges->check("MaintainPersonalProjects");
  if(pMode==cView)
  {
    if(_privileges->check("ViewAllProjects"))
      return true;
    personalPriv = personalPriv || _privileges->check("ViewPersonalProjects");
  }

  if(pMode==cNew)
    return personalPriv;
  else
  {
    XSqlQuery usernameCheck;
    usernameCheck.prepare( "SELECT getEffectiveXtUser() IN (prj_owner_username, prj_username) AS canModify "
                           "FROM prj "
                            "WHERE (prj_id=:prj_id);" );
    usernameCheck.bindValue(":prj_id", pId);
    usernameCheck.exec();

    if (usernameCheck.first())
      return usernameCheck.value("canModify").toBool()&&personalPriv;
    return false;
  }
}

project::project(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : XDialog(parent, name, modal, fl),
      _prjid(-1)
{
  setupUi(this);

  _projectType->populate("SELECT prjtype_id, prjtype_descr FROM prjtype WHERE prjtype_active;");

  if(!_privileges->check("EditOwner")) _owner->setEnabled(false);

  connect(_buttonBox,     SIGNAL(rejected()),        this, SLOT(sClose()));
  connect(_buttonBox,     SIGNAL(accepted()),        this, SLOT(sSave()));
  connect(_newTask,       SIGNAL(clicked()),         this, SLOT(sNewTask()));
  connect(_printTasks,    SIGNAL(clicked()),         this, SLOT(sPrintTasks()));
  connect(_printOrders,   SIGNAL(clicked()),         this, SLOT(sPrintOrders()));
  connect(_editTask,      SIGNAL(clicked()),         this, SLOT(sEditTask()));
  connect(_editOrder,     SIGNAL(clicked()),         this, SLOT(sEditOrder()));
  connect(_viewTask,      SIGNAL(clicked()),         this, SLOT(sViewTask()));
  connect(_viewOrder,     SIGNAL(clicked()),         this, SLOT(sViewOrder()));
  connect(_deleteTask,    SIGNAL(clicked()),         this, SLOT(sDeleteTask()));
  connect(_number,        SIGNAL(editingFinished()), this, SLOT(sNumberChanged()));
  connect(_crmacct,       SIGNAL(newId(int)),        this, SLOT(sCRMAcctChanged(int)));
  connect(_prjtask,   SIGNAL(populateMenu(QMenu*,QTreeWidgetItem*,int)), this, SLOT(sPopulateTaskMenu(QMenu*, QTreeWidgetItem*)));
  connect(_prjorders, SIGNAL(populateMenu(QMenu*,QTreeWidgetItem*,int)), this, SLOT(sPopulateOrdersMenu(QMenu*, QTreeWidgetItem*)));
  connect(_showSo, SIGNAL(toggled(bool)), this, SLOT(sFillOrdersList()));
  connect(_showPo, SIGNAL(toggled(bool)), this, SLOT(sFillOrdersList()));
  connect(_showWo, SIGNAL(toggled(bool)), this, SLOT(sFillOrdersList()));
  connect(_showIn, SIGNAL(toggled(bool)), this, SLOT(sFillTaskList()));
  connect(_showOpp, SIGNAL(toggled(bool)), this, SLOT(sFillTaskList()));
  connect(_showCompleted, SIGNAL(toggled(bool)), this, SLOT(sFillTaskList()));

  connect(omfgThis, SIGNAL(salesOrdersUpdated(int, bool)), this, SLOT(sFillTaskList()));
  connect(omfgThis, SIGNAL(quotesUpdated(int, bool)), this, SLOT(sFillTaskList()));
  connect(omfgThis, SIGNAL(workOrdersUpdated(int, bool)), this, SLOT(sFillTaskList()));
  connect(omfgThis, SIGNAL(purchaseOrdersUpdated(int, bool)), this, SLOT(sFillTaskList()));

  _documents->setType("J");
  _charass->setType("PROJ");

  _prjtask->addColumn(tr("Number"),       _itemColumn, Qt::AlignLeft,   true,  "number" );
  _prjtask->addColumn(tr("Status"),      _orderColumn, Qt::AlignLeft,   true,  "status" );
  _prjtask->addColumn(tr("Name"),         _itemColumn, Qt::AlignLeft,   true,  "name"   );
  _prjtask->addColumn(tr("Description"),           -1, Qt::AlignLeft,   true,  "descrip");
  _prjtask->addColumn(tr("Assigned To"),           -1, Qt::AlignLeft,   true,  "assignedto");
  _prjtask->addColumn(tr("Due Date"),     _dateColumn, Qt::AlignRight,  true,  "due"  );
  _prjtask->addColumn(tr("Assigned"),     _dateColumn, Qt::AlignRight,  true,  "assigned" );
  _prjtask->addColumn(tr("Started"),      _dateColumn, Qt::AlignRight,  true,  "started"  );
  _prjtask->addColumn(tr("Completed"),    _dateColumn, Qt::AlignRight,  true,  "completed"  );
  _prjtask->addColumn(tr("Hrs. Budget"),   _qtyColumn, Qt::AlignRight,  true,  "hrs_budget" );
  _prjtask->addColumn(tr("Hrs. Actual"),   _qtyColumn, Qt::AlignRight,  true,  "hrs_actual" );
  _prjtask->addColumn(tr("Hrs. Balance"),  _qtyColumn, Qt::AlignRight,  true,  "hrs_balance" );
  _prjtask->addColumn(tr("Exp. Budget"),  _priceColumn, Qt::AlignRight,  true,  "exp_budget" );
  _prjtask->addColumn(tr("Exp. Actual"),  _priceColumn, Qt::AlignRight,  true,  "exp_actual" );
  _prjtask->addColumn(tr("Exp. Balance"), _priceColumn, Qt::AlignRight,  true,  "exp_balance" );
  _prjtask->setSortingEnabled(false);

  _prjorders->addColumn(tr("Number"),       _itemColumn, Qt::AlignLeft,   true,  "name"   );
  _prjorders->addColumn(tr("Status"),      _orderColumn, Qt::AlignLeft,   true,  "status"   );
  _prjorders->addColumn(tr("Item #"),       _itemColumn, Qt::AlignLeft,   true,  "item"   );
  _prjorders->addColumn(tr("Description"),           -1, Qt::AlignLeft,   true,  "descrip" );
  _prjorders->addColumn(tr("Account/Customer"),      -1, Qt::AlignLeft,   false,  "customer" );
  _prjorders->addColumn(tr("Contact"),               -1, Qt::AlignLeft,   false,  "contact" );
  _prjorders->addColumn(tr("City"),                  -1, Qt::AlignLeft,   false,  "city" );
  _prjorders->addColumn(tr("State"),                 -1, Qt::AlignLeft,   false,  "state" );
  _prjorders->addColumn(tr("Qty"),           _qtyColumn, Qt::AlignRight,  true,  "qty"  );
  _prjorders->addColumn(tr("UOM"),           _uomColumn, Qt::AlignLeft,   true,  "uom"  );
  _prjorders->addColumn(tr("Value"),         _qtyColumn, Qt::AlignRight,  true,  "value"  );

  _owner->setUsername(omfgThis->username());
  _assignedTo->setUsername(omfgThis->username());
  _owner->setType(UsernameLineEdit::UsersActive);
  _assignedTo->setType(UsernameLineEdit::UsersActive);

  _totalHrBud->setPrecision(omfgThis->qtyVal());
  _totalHrAct->setPrecision(omfgThis->qtyVal());
  _totalHrBal->setPrecision(omfgThis->qtyVal());
  _totalExpBud->setPrecision(omfgThis->moneyVal());
  _totalExpAct->setPrecision(omfgThis->moneyVal());
  _totalExpBal->setPrecision(omfgThis->moneyVal());
  
  _saved=false;
  _close = false;

  QMenu * newMenu = new QMenu;
  QAction *menuItem;
  newMenu->addAction(tr("Task..."), this, SLOT(sNewTask()));
  newMenu->addSeparator();
  menuItem = newMenu->addAction(tr("Incident"), this, SLOT(sNewIncident()));
  menuItem->setEnabled(_privileges->check("MaintainPersonalIncidents MaintainAllIncidents"));
  menuItem = newMenu->addAction(tr("Opportunity"), this, SLOT(sNewOpportunity()));
  menuItem->setEnabled(_privileges->check("MaintainPersonalOpportunities MaintainAllOpportunities"));
  _newTask->setMenu(newMenu);

  QMenu * newOrdMenu = new QMenu;
  QAction *menuOrdItem;
  menuOrdItem = newOrdMenu->addAction(tr("Quote"), this, SLOT(sNewQuotation()));
  menuOrdItem->setEnabled(_privileges->check("MaintainQuotes"));
  menuOrdItem = newOrdMenu->addAction(tr("Sales Order"), this, SLOT(sNewSalesOrder()));
  menuOrdItem->setEnabled(_privileges->check("MaintainSalesOrders"));
  menuOrdItem = newOrdMenu->addAction(tr("Purchase Order"),   this, SLOT(sNewPurchaseOrder()));
  menuOrdItem->setEnabled(_privileges->check("MaintainPurchaseOrders"));
  menuOrdItem = newOrdMenu->addAction(tr("Work Order"),   this, SLOT(sNewWorkOrder()));
  menuOrdItem->setEnabled(_privileges->check("MaintainWorkOrders"));
  _newOrder->setMenu(newOrdMenu);

}

project::~project()
{
  // no need to delete child widgets, Qt does it all for us
}

void project::languageChange()
{
  retranslateUi(this);
}

enum SetResponse project::set(const ParameterList &pParams)
{
  XSqlQuery projectet;
  XDialog::set(pParams);
  QVariant param;
  bool     valid;

  param = pParams.value("username", &valid);
  if (valid)
    _assignedTo->setUsername(param.toString());

  param = pParams.value("prj_id", &valid);
  if (valid)
  {
    _prjid = param.toInt();
    populate();
  }

  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "new")
    {
      _mode = cNew;

      projectet.exec("SELECT NEXTVAL('prj_prj_id_seq') AS prj_id, "
                     "COALESCE((SELECT incdtpriority_id FROM incdtpriority "
                     " WHERE incdtpriority_default), -1) AS prioritydefault, "
                     "COALESCE((SELECT prjtype_id FROM prjtype "
                     " WHERE prjtype_default), -1) AS typedefault;");
      if (projectet.first())
      {
        _prjid = projectet.value("prj_id").toInt();
        _priority->setId(projectet.value("prioritydefault").toInt());
        _projectType->setId(projectet.value("typedefault").toInt());
      }
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Project Information"),
                                    projectet, __FILE__, __LINE__))
          return UndefinedError;

      if(_metrics->value("ProjectNumberGeneration") != "M"
          && _number->text().isEmpty())
      {
        XSqlQuery numq;
        numq.exec("SELECT fetchProjectNumber() AS number;");
        if (numq.first())
        {
          _number->setText(numq.value("number"));
          _number->setEnabled(_metrics->value("ProjectNumberGeneration") == "O");
          _name->setFocus();
        }
      }
    }
    else if (param.toString() == "edit")
    {
      _mode = cEdit;
      _number->setEnabled(false);
    }
    else if (param.toString() == "view")
      setViewMode();

    if (_mode == cNew || _mode == cEdit)
    {
      connect(_assignedTo, SIGNAL(newId(int)), this, SLOT(sAssignedToChanged(int)));
      connect(_status,  SIGNAL(currentIndexChanged(int)), this, SLOT(sStatusChanged(int)));
      connect(_completed,  SIGNAL(newDate(QDate)), this, SLOT(sCompletedChanged()));
      connect(_pctCompl,  SIGNAL(valueChanged(int)), this, SLOT(sCompletedChanged()));
      connect(_prjtask, SIGNAL(valid(bool)), this, SLOT(sHandleButtons(bool)));
      connect(_prjtask, SIGNAL(itemSelected(int)), _editTask, SLOT(animateClick()));
      connect(_projectType, SIGNAL(newID(int)), this, SLOT(sProjectTypeChanged(int)));
    }
  }

  _comments->setId(_prjid);
  _documents->setId(_prjid);
  _charass->setId(_prjid);
  _recurring->setParent(_prjid, "J");

  param = pParams.value("crmacct_id", &valid);
  if (valid)
  {
    _crmacct->setId(param.toInt());
    _crmacct->setEnabled(false);
  }

  param = pParams.value("cntct_id", &valid);
  if (valid)
  {
    _cntct->setId(param.toInt());
  }
    
  return NoError;
}

void project::setViewMode()
{
  _mode = cView;

  _infoGroup->setEnabled(false);
  _number->setEnabled(false);
  _status->setEnabled(false);
  _name->setEnabled(false);
  _descrip->setEnabled(false);
  _so->setEnabled(false);
  _wo->setEnabled(false);
  _po->setEnabled(false);
  _cntct->setEnabled(false);
  _newTask->setEnabled(false);
  _comments->setReadOnly(true);
  _charass->setReadOnly(true);
  _documents->setReadOnly(true);
  _scheduleGroup->setEnabled(false);
  _recurring->setEnabled(false);
  _projectType->setEnabled(false);
  _buttonBox->removeButton(_buttonBox->button(QDialogButtonBox::Save));
  _buttonBox->removeButton(_buttonBox->button(QDialogButtonBox::Cancel));
  _buttonBox->addButton(QDialogButtonBox::Close);

  connect(_prjtask, SIGNAL(itemSelected(int)), _viewTask, SLOT(animateClick()));
}

void project::sHandleButtons(bool valid)
{
  _editTask->setEnabled(valid);
  _viewTask->setEnabled(valid);

  if(_prjtask->altId() == 5)
    _deleteTask->setEnabled(valid);
  else
    _deleteTask->setEnabled(false);

}

void project::sPopulateTaskMenu(QMenu *pMenu,  QTreeWidgetItem *selected)
{
  Q_UNUSED(selected);
  QAction *menuItem;

  if(_prjtask->altId() == 5)
  {
    menuItem = pMenu->addAction(tr("Edit Task"), this, SLOT(sEditTask()));
    menuItem->setEnabled(_privileges->check("MaintainAllProjects MaintainPersonalProjects"));

    menuItem = pMenu->addAction(tr("View Task"), this, SLOT(sViewTask()));
    menuItem->setEnabled(_privileges->check("MaintainAllProjects MaintainPersonalProjects ViewAllProjects ViewPersonalProjects"));
  }
  if(_prjtask->altId() == 105)
  {
    menuItem = pMenu->addAction(tr("Edit Incident"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainPersonalIncidents MaintainAllIncidents"));

    menuItem = pMenu->addAction(tr("View Incident"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("ViewPersonalIncidents ViewAllIncidents MaintainPersonalIncidents MaintainAllIncidents"));
  }
  if(_prjtask->altId() == 110)
  {
    menuItem = pMenu->addAction(tr("Edit Opportunity"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainPersonalOpportunities MaintainAllOpportunities"));

    menuItem = pMenu->addAction(tr("View Opportunity"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("ViewPersonalOpportunities ViewAllOpportunities MaintainPersonalOpportunities MaintainAllOpportunities"));
  }
}

void project::sPopulateOrdersMenu(QMenu *pMenu,  QTreeWidgetItem *selected)
{
  Q_UNUSED(selected);
  QAction *menuItem;

  if(_prjorders->altId() == 15)
  {
    menuItem = pMenu->addAction(tr("Edit Quote"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainQuotes"));

    menuItem = pMenu->addAction(tr("View Quote"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainQuotes") ||
                         _privileges->check("ViewQuotes") );
  }

  if(_prjorders->altId() == 17)
  {
    menuItem = pMenu->addAction(tr("Edit Quote Item"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainQuotes"));

    menuItem = pMenu->addAction(tr("View Quote Item"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainQuotes") ||
                         _privileges->check("ViewQuotes"));
  }

  if(_prjorders->altId() == 25)
  {
    menuItem = pMenu->addAction(tr("Edit Sales Order"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainSalesOrders"));

    menuItem = pMenu->addAction(tr("View Sales Order"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainSalesOrders") ||
                         _privileges->check("ViewSalesOrders"));
  }

  if(_prjorders->altId() == 27)
  {
    menuItem = pMenu->addAction(tr("Edit Sales Order Item"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainSalesOrders"));

    menuItem = pMenu->addAction(tr("View Sales Order Item"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainSalesOrders") ||
                         _privileges->check("ViewSalesOrders"));
  }

  if(_prjorders->altId() == 35)
  {
    menuItem = pMenu->addAction(tr("Edit Invoice"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainMiscInvoices"));

    menuItem = pMenu->addAction(tr("View Invoice"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainMiscInvoices") ||
                         _privileges->check("ViewMiscInvoices"));
  }

  if(_prjorders->altId() == 37)
  {
    menuItem = pMenu->addAction(tr("Edit Invoice Item"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainMiscInvoices"));

    menuItem = pMenu->addAction(tr("View Invoice Item"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainMiscInvoices") ||
                         _privileges->check("ViewMiscInvoices"));
  }

  if(_prjorders->altId() == 45)
  {
    menuItem = pMenu->addAction(tr("Edit Work Order"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainWorkOrders"));

    menuItem = pMenu->addAction(tr("View Work Order"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainWorkOrders") ||
                         _privileges->check("ViewWorkOrders"));
  }

  if(_prjorders->altId() == 55)
  {
    menuItem = pMenu->addAction(tr("View Purchase Request"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainPurchaseRequests") ||
                         _privileges->check("ViewPurchaseRequests"));
  }

  if(_prjorders->altId() == 65)
  {
    menuItem = pMenu->addAction(tr("Edit Purchase Order"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainPurchaseOrders"));

    menuItem = pMenu->addAction(tr("View Purchase Order"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainPurchaseOrders") ||
                         _privileges->check("ViewPurchaseOrders"));
  }

  if(_prjorders->altId() == 67)
  {
    menuItem = pMenu->addAction(tr("Edit Purchase Order Item"), this, SLOT(sEditOrder()));
    menuItem->setEnabled(_privileges->check("MaintainPurchaseOrders"));

    menuItem = pMenu->addAction(tr("View Purchase Order Item"), this, SLOT(sViewOrder()));
    menuItem->setEnabled(_privileges->check("MaintainPurchaseOrders") ||
                         _privileges->check("ViewPurchaseOrders"));
  }
}

void project::populate()
{
  if (_mode == cEdit && !_lock.acquire("prj", _prjid, AppLock::Interactive))
    setViewMode();

  _close = false;

  foreach (QWidget* widget, QApplication::allWidgets())
  {
    if (!widget->isWindow() || !widget->isVisible())
      continue;

    project *w = qobject_cast<project*>(widget);

    if (w && w != this && w->id()==_prjid)
    {
      // detect "i'm my own grandpa"
      QObject *p;
      for (p = parent(); p && p != w ; p = p->parent())
        ; // do nothing
      if (p == w)
      {
        QMessageBox::warning(this, tr("Cannot Open Recursively"),
                             tr("This project is already open and cannot be "
                                "raised. Please close windows to get to it."));
        _close = true;
      } else if (p) {
        w->setFocus();
        w->raise();
        w->activateWindow();
        _close = true;
      }
      break;
    }
  }

  XSqlQuery projectpopulate;
  projectpopulate.prepare( "SELECT * "
             "FROM prj "
             "WHERE (prj_id=:prj_id);" );
  projectpopulate.bindValue(":prj_id", _prjid);
  projectpopulate.exec();
  if (projectpopulate.first())
  {
    _saved = true;
    _owner->setUsername(projectpopulate.value("prj_owner_username").toString());
    _number->setText(projectpopulate.value("prj_number").toString());
    _name->setText(projectpopulate.value("prj_name").toString());
    _descrip->setText(projectpopulate.value("prj_descrip").toString());
    _so->setChecked(projectpopulate.value("prj_so").toBool());
    _wo->setChecked(projectpopulate.value("prj_wo").toBool());
    _po->setChecked(projectpopulate.value("prj_po").toBool());
    _assignedTo->setUsername(projectpopulate.value("prj_username").toString());
    _dept->setId(projectpopulate.value("prj_dept_id").toInt());
    _cntct->setId(projectpopulate.value("prj_cntct_id").toInt());
    _crmacct->setId(projectpopulate.value("prj_crmacct_id").toInt());
    _started->setDate(projectpopulate.value("prj_start_date").toDate());
    _assigned->setDate(projectpopulate.value("prj_assigned_date").toDate());
    _due->setDate(projectpopulate.value("prj_due_date").toDate());
    _completed->setDate(projectpopulate.value("prj_completed_date").toDate());
    _projectType->setId(projectpopulate.value("prj_prjtype_id").toInt());
    _priority->setId(projectpopulate.value("prj_priority_id").toInt());
    _pctCompl->setValue(projectpopulate.value("prj_pct_complete").toInt());
    for (int counter = 0; counter < _status->count(); counter++)
    {
      if (QString(projectpopulate.value("prj_status").toString()[0]) == _projectStatuses[counter])
        _status->setCurrentIndex(counter);
    }

    _recurring->setParent(projectpopulate.value("prj_recurring_prj_id").isNull() ?
                            _prjid : projectpopulate.value("prj_recurring_prj_id").toInt(),
                          "J");

    if (_projectType->id() < 0)
    {
      XSqlQuery projectType;
      projectType.prepare( "SELECT prjtype_id, prjtype_descr FROM prjtype WHERE prjtype_active "
                           "UNION "
                           "SELECT prjtype_id, prjtype_descr FROM prjtype "
                           "JOIN prj ON (prj_prjtype_id=prjtype_id) "
                           "WHERE (prj_id=:prj_id);" );
      projectType.bindValue(":prj_id", _prjid);
      projectType.exec();
      _projectType->populate(projectType);
      if (projectType.lastError().type() != QSqlError::NoError)
      {
        systemError(this, projectType.lastError().databaseText(), __FILE__, __LINE__);
        return;
      }
      _projectType->setId(projectpopulate.value("prj_prjtype_id").toInt());
    }
  }

  sFillTaskList();
  sFillOrdersList();

  emit populated(_prjid);
}

void project::sAssignedToChanged(const int newid)
{
  if (newid == -1)
    _assigned->clear();
  else
    _assigned->setDate(omfgThis->dbDate());
}

void project::sProjectTypeChanged(const int newType)
{
  if (!_saved)
  {
    if (!sSave(true))
      return;
  }

  XSqlQuery taskq;
  taskq.prepare("SELECT applyDefaultTasks('J', :category, :project, :override) AS ret;" );
  taskq.bindValue(":category", newType);
  taskq.bindValue(":project", _prjid);
  taskq.bindValue(":override", false);
  taskq.exec();

  /*
   The following checks whether tasks already exist and should be overridden.
   return code 0 means no templates exist
   return code < 0 means tasks already exist and user is questions whether to override
   return code > 0 means no existing tasks so they have been copied automatically
  */
  if (taskq.first()) 
  {
    if (taskq.value("ret").toInt() < 0)
    {
      if (QMessageBox::question(this, tr("Existing Tasks"),
                         tr("<p>Tasks already exist for this Project.\n"
                            "Do you want to replace tasks with the new template?"),
                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
      {
          return;
      }

      taskq.bindValue(":override", true);
      taskq.exec();
      if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Applying Template Tasks"),
                               taskq, __FILE__, __LINE__))
           return;

    }
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Applying Template Tasks"),
                                taskq, __FILE__, __LINE__))
            return;

  sFillTaskList();
}

void project::sStatusChanged(const int pStatus)
{
  switch(pStatus)
  {
    case 0: // Concept
    default:
      _started->clear();
      _completed->clear();
      break;
    case 1: // In Process
      if(!_started->isValid())
        _started->setDate(omfgThis->dbDate());
      _completed->clear();
      break;
    case 2: // Completed
      if(!_completed->isValid())
        _completed->setDate(omfgThis->dbDate());
      break;
  }
}

void project::sCompletedChanged()
{
  if (_completed->isValid())
    _pctCompl->setValue(100);
  if (_pctCompl->value() == 100 && !_completed->isValid())
    _completed->setDate(omfgThis->dbDate());
}

void project::sCRMAcctChanged(const int newid)
{
  _cntct->setSearchAcct(newid);
}

void project::sClose()
{
  XSqlQuery projectClose;
  if (_mode == cNew)
  {
    projectClose.prepare( "SELECT deleteproject(:prj_id);" );
    projectClose.bindValue(":prj_id", _prjid);
    projectClose.exec();
  }

  reject();
}

bool project::sSave(bool partial)
{
  XSqlQuery projectSave;
  QList<GuiErrorCheck> errors;
  errors<< GuiErrorCheck(_number->text().isEmpty(), _number,
                         tr("No Project Number was specified. You must specify a project number "
                            "before saving it."))
        << GuiErrorCheck(!partial && !_due->isValid(), _due,
                         tr("You must specify a due date before "
                            "saving it."))
  ;
  if (GuiErrorCheck::reportErrors(this, tr("Cannot Save Project"), errors))
    return false;

  RecurrenceWidget::RecurrenceChangePolicy cp = _recurring->getChangePolicy();
  if (cp == RecurrenceWidget::NoPolicy)
    return false;

  XSqlQuery rollbackq;
  rollbackq.prepare("ROLLBACK;");
  XSqlQuery begin("BEGIN;");

  if (!_saved)
    projectSave.prepare( "INSERT INTO prj "
               "( prj_id, prj_number, prj_name, prj_descrip,"
               "  prj_so, prj_wo, prj_po, prj_status, prj_owner_username, "
               "  prj_start_date, prj_due_date, prj_assigned_date,"
               "  prj_completed_date, prj_username, prj_recurring_prj_id,"
               "  prj_crmacct_id, prj_cntct_id, prj_prjtype_id, "
               "  prj_dept_id, prj_priority_id, prj_pct_complete ) "
               "VALUES "
               "( :prj_id, :prj_number, :prj_name, :prj_descrip,"
               "  :prj_so, :prj_wo, :prj_po, :prj_status, :prj_owner_username,"
               "  :prj_start_date, :prj_due_date, :prj_assigned_date,"
               "  :prj_completed_date, :username, :prj_recurring_prj_id,"
               "  :prj_crmacct_id, :prj_cntct_id, :prj_prjtype_id, "
               "  :prj_dept_id, :prj_priority_id, :prj_pct_complete );" );
  else
    projectSave.prepare( "UPDATE prj "
               "SET prj_number=:prj_number, prj_name=:prj_name, prj_descrip=:prj_descrip,"
               "    prj_so=:prj_so, prj_wo=:prj_wo, prj_po=:prj_po, prj_status=:prj_status, "
               "    prj_owner_username=:prj_owner_username, prj_start_date=:prj_start_date, "
               "    prj_due_date=:prj_due_date, prj_assigned_date=:prj_assigned_date,"
               "    prj_completed_date=:prj_completed_date,"
               "    prj_username=:username,"
               "    prj_recurring_prj_id=:prj_recurring_prj_id,"
               "    prj_crmacct_id=:prj_crmacct_id,"
               "    prj_cntct_id=:prj_cntct_id, "
               "    prj_prjtype_id=:prj_prjtype_id, "
               "    prj_dept_id=:prj_dept_id, "
               "    prj_priority_id=:prj_priority_id, "
               "    prj_pct_complete=:prj_pct_complete "
               "WHERE (prj_id=:prj_id);" );

  projectSave.bindValue(":prj_id", _prjid);
  projectSave.bindValue(":prj_number", _number->text().trimmed().toUpper());
  projectSave.bindValue(":prj_name", _name->text());
  projectSave.bindValue(":prj_descrip", _descrip->toPlainText());
  projectSave.bindValue(":prj_status", _projectStatuses[_status->currentIndex()]);
  projectSave.bindValue(":prj_priority_id", _priority->id());
  if (_dept->id() > 0)
    projectSave.bindValue(":prj_dept_id", _dept->id());
  projectSave.bindValue(":prj_so", QVariant(_so->isChecked()));
  projectSave.bindValue(":prj_wo", QVariant(_wo->isChecked()));
  projectSave.bindValue(":prj_po", QVariant(_po->isChecked()));
  projectSave.bindValue(":prj_owner_username", _owner->username());
  projectSave.bindValue(":username", _assignedTo->username());
  if (_crmacct->id() > 0)
    projectSave.bindValue(":prj_crmacct_id", _crmacct->id());
  if (_cntct->id() > 0)
    projectSave.bindValue(":prj_cntct_id", _cntct->id());
  if (_projectType->id() > 0)
    projectSave.bindValue(":prj_prjtype_id",   _projectType->id());
  projectSave.bindValue(":prj_start_date",     _started->date());
  projectSave.bindValue(":prj_due_date",       _due->date());
  projectSave.bindValue(":prj_assigned_date",  _assigned->date());
  projectSave.bindValue(":prj_completed_date", _completed->date());
  projectSave.bindValue(":prj_pct_complete",   _pctCompl->value());
  if (_recurring->isRecurring())
    projectSave.bindValue(":prj_recurring_prj_id", _recurring->parentId());

  projectSave.exec();
  if (projectSave.lastError().type() != QSqlError::NoError)
  {
    rollbackq.exec();
    ErrorReporter::error(QtCriticalMsg, this, tr("Error Saving Project Information"),
                         projectSave, __FILE__, __LINE__);
    return false;
  }

  QString errmsg;
  if (! _recurring->save(true, cp, &errmsg))
  {
    qDebug("recurring->save failed");
    rollbackq.exec();
    ErrorReporter::error(QtCriticalMsg, this, tr("Error Occurred"),
                         tr("%1: Error Saving Project Information \n %2")
                         .arg(windowTitle())
                         .arg(errmsg),__FILE__,__LINE__);
    return false;
  }

  projectSave.exec("COMMIT;");
  emit saved(_prjid);

  if (!partial)
  {
    omfgThis->sProjectsUpdated(_prjid);
    done(_prjid);
  }
  else
    _saved=true;
  return true;
}

void project::sPrintTasks()
{
  ParameterList params;

  params.append("prj_id", _prjid);
  params.append("assigned",   tr("Assigned"));
  params.append("complete",   tr("Completed"));
  params.append("deferred",   tr("Deferred"));
  params.append("new",        tr("New"));
  params.append("inprocess",  tr("In Process"));

  orReport report("ProjectTaskList", params);
  if(report.isValid())
    report.print();
  else
    report.reportError(this);
}

void project::sPrintOrders()
{
  if (!_showSo->isChecked() && !_showWo->isChecked() && !_showPo->isChecked())
  {
    QMessageBox::critical( this, tr("Cannot Print Orders"),
        tr("Please first select an order type or types to print."));
    return;
  }

  ParameterList params;

  params.append("prj_id", _prjid);

  params.append("so", tr("Sales Order"));
  params.append("wo", tr("Work Order"));
  params.append("po", tr("Purchase Order"));
  params.append("pr", tr("Purchase Request"));
  params.append("sos", tr("Sales Orders"));
  params.append("wos", tr("Work Orders"));
  params.append("pos", tr("Purchase Orders"));
  params.append("prs", tr("Purchase Requests"));
  params.append("quote", tr("Quote"));
  params.append("quotes", tr("Quotes"));
  params.append("invoice", tr("Invoice"));
  params.append("invoices", tr("Invoices"));

  params.append("open", tr("Open"));
  params.append("closed", tr("Closed"));
  params.append("converted", tr("Converted"));
  params.append("canceled", tr("Canceled"));
  params.append("expired", tr("Expired"));
  params.append("unposted", tr("Unposted"));
  params.append("posted", tr("Posted"));
  params.append("exploded", tr("Exploded"));
  params.append("released", tr("Released"));
  params.append("planning", tr("Concept"));
  params.append("inprocess", tr("In Process"));
  params.append("complete", tr("Complete"));
  params.append("unreleased", tr("Unreleased"));
  params.append("total", tr("Total"));

  if(_showSo->isChecked())
    params.append("showSo");

  if(_showWo->isChecked())
    params.append("showWo");

  if(_showPo->isChecked())
    params.append("showPo");

  if (! _privileges->check("ViewAllProjects") && ! _privileges->check("MaintainAllProjects"))
    params.append("owner_username", omfgThis->username());

  orReport report("OrderActivityByProject", params);
  if(report.isValid())
    report.print();
  else
    report.reportError(this);
}

void project::sNewTask()
{
  if (!_saved)
  {
    if (!sSave(true))
      return;
  }
    
  ParameterList params;
  params.append("mode", "new");
  params.append("parent", "J");
  params.append("parent_id", _prjid);
  params.append("parent_owner_username", _owner->username());
  params.append("parent_assigned_username", _assignedTo->username());
  params.append("parent_start_date", _started->date());
  params.append("parent_due_date",	_due->date());
  params.append("parent_assigned_date", _assigned->date());
  params.append("parent_completed_date", _completed->date());

  task newdlg(this, "", true);
  newdlg.set(params);
  if (newdlg.exec() != XDialog::Rejected)
    sFillTaskList();
}

void project::sEditTask()
{
  if(_prjtask->altId() == 5)
  {
    ParameterList params;
    params.append("mode", "edit");
    params.append("parent", "J");
    params.append("parent_id", _prjid);
    params.append("task_id", _prjtask->id());

    task newdlg(this, "", true);
    newdlg.set(params);
    if (newdlg.exec() != XDialog::Rejected)
      sFillTaskList();
  } else {
    sEditOrder();
  }

}

void project::sViewTask()
{
  if(_prjtask->altId() == 5)
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("parent", "J");
    params.append("parent_id", _prjid);
    params.append("task_id", _prjtask->id());

    task newdlg(this, "", true);
    newdlg.set(params);
    newdlg.exec();
  } else {
    sViewOrder();
  }

}

void project::sDeleteTask()
{
  if(_prjtask->altId() != 5)
    return;

  XSqlQuery projectDeleteTask;
  projectDeleteTask.prepare("SELECT deleteTask(:prjtask_id) AS result;");
  projectDeleteTask.bindValue(":prjtask_id", _prjtask->id());
  projectDeleteTask.exec();
  if (projectDeleteTask.first() && projectDeleteTask.value("result").toInt() == -1)
  {
    if (QMessageBox::question(this, tr("Sub-Tasks"),
                       tr("<p>Sub-tasks exist for this Task.\n"
                          "Do you also want to delete sub-tasks?"),
                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
      {
          return;
      }
      else
      {
        projectDeleteTask.prepare("SELECT deleteTask(:prjtask_id, true) AS result;");
        projectDeleteTask.bindValue(":prjtask_id", _prjtask->id());
        projectDeleteTask.exec();
        if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Project Task"),
                                projectDeleteTask, __FILE__, __LINE__))
            return;
      }
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Project Task"),
                                projectDeleteTask, __FILE__, __LINE__))
    return;

  emit deletedTask();
  sFillTaskList();
}

void project::sFillTaskList()
{
// Populate Summary of Task Activity
  MetaSQLQuery mql = mqlLoad("projectTasks", "detail");

  ParameterList params;
  params.append("prj_id", _prjid);
  XSqlQuery qry = mql.toQuery(params);
  if (qry.first())
  {
    _totalHrBud->setDouble(qry.value("totalhrbud").toDouble());
    _totalHrAct->setDouble(qry.value("totalhract").toDouble());
    _totalHrBal->setDouble(qry.value("totalhrbal").toDouble());
    _totalExpBud->setDouble(qry.value("totalexpbud").toDouble());
    _totalExpAct->setDouble(qry.value("totalexpact").toDouble());
    _totalExpBal->setDouble(qry.value("totalexpbal").toDouble());
  }
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Project Information"),
                                qry, __FILE__, __LINE__))
  {
    return;
  }

// Populate Task List
  MetaSQLQuery mqltask = mqlLoad("orderActivityByProject", "tasks");

  params.append("tasks",         tr("Tasks"));
  params.append("incidents",     tr("Incidents"));
  params.append("opportunities", tr("Opportunities"));
  params.append("assigned",   tr("Assigned"));
  params.append("complete",   tr("Completed"));
  params.append("confirmed",  tr("Confirmed"));
  params.append("deferred",   tr("Deferred"));
  params.append("feedback",   tr("Feedback"));
  params.append("inprocess",  tr("In Process"));
  params.append("new",        tr("New"));
  params.append("open",       tr("Open"));
  params.append("planning",   tr("Pending"));
  params.append("posted",     tr("Posted"));
  params.append("resolved",   tr("Resolved"));
  params.append("total", tr("Total"));

  if(_showIn->isChecked())
    params.append("showIn");
  if(_showOpp->isChecked())
    params.append("showOpp");

  if (_showCompleted->isChecked())
    params.append("showCompleted");

  if (! _privileges->check("ViewAllProjects") && ! _privileges->check("MaintainAllProjects"))
    params.append("owner_username", omfgThis->username());

  XSqlQuery qrytask = mqltask.toQuery(params);
  _prjtask->populate(qrytask, true);
  (void)ErrorReporter::error(QtCriticalMsg, this, tr("Could not get Task Information"),
                           qrytask, __FILE__, __LINE__);
  _prjtask->expandAll();
}

void project::sFillOrdersList()
{
  ParameterList params;
  params.append("prj_id", _prjid);
// Populate Project Orders List
  MetaSQLQuery mqltask = mqlLoad("orderActivityByProject", "orders");

  params.append("canceled",   tr("Canceled"));
  params.append("closed",     tr("Closed"));
  params.append("complete",   tr("Complete"));
  params.append("converted",  tr("Converted"));
  params.append("expired",    tr("Expired"));
  params.append("exploded",   tr("Exploded"));
  params.append("inprocess",  tr("In Process"));
  params.append("invoice",    tr("Invoice"));
  params.append("invoices",   tr("Invoices"));
  params.append("new",        tr("New"));
  params.append("open",       tr("Open"));
  params.append("deferred",   tr("Deferred"));
  params.append("planning",   tr("Pending"));
  params.append("po",         tr("Purchase Order"));
  params.append("pos",        tr("Purchase Orders"));
  params.append("posted",     tr("Posted"));
  params.append("pr",         tr("Purchase Request"));
  params.append("prs",        tr("Purchase Requests"));
  params.append("quote",      tr("Quote"));
  params.append("quotes",     tr("Quotes"));
  params.append("released",   tr("Released"));
  params.append("so",         tr("Sales Order"));
  params.append("sos",        tr("Sales Orders"));
  params.append("total",      tr("Total"));
  params.append("unposted",   tr("Unposted"));
  params.append("unreleased", tr("Unreleased"));
  params.append("wo",         tr("Work Order"));
  params.append("wos",        tr("Work Orders"));

  params.append("total", tr("Total"));

  if(_showSo->isChecked())
    params.append("showSo");

  if(_showWo->isChecked())
    params.append("showWo");

  if(_showPo->isChecked())
    params.append("showPo");

  if (_showCompleted->isChecked())
    params.append("showCompleted");

  if (! _privileges->check("ViewAllProjects") && ! _privileges->check("MaintainAllProjects"))
    params.append("owner_username", omfgThis->username());

  XSqlQuery qrytask = mqltask.toQuery(params);

  _prjorders->populate(qrytask, true);
  (void)ErrorReporter::error(QtCriticalMsg, this, tr("Could not get Project Order Information"),
                           qrytask, __FILE__, __LINE__);
  _prjorders->expandAll();
}

void project::sNumberChanged()
{
  XSqlQuery projectNumberChanged;
  if((cNew == _mode) && (_number->text().length()))
  {
    _number->setText(_number->text().trimmed().toUpper());

    projectNumberChanged.prepare( "SELECT prj_id"
               "  FROM prj"
               " WHERE (prj_number=:prj_number);" );
    projectNumberChanged.bindValue(":prj_number", _number->text());
    projectNumberChanged.exec();
    if(projectNumberChanged.first())
    {
      _number->setEnabled(false);
      _prjid = projectNumberChanged.value("prj_id").toInt();
      _mode = cEdit;
      populate();
    }
    else
    {
      _number->setEnabled(false);
      _mode = cNew;
    }
  }
}

void project::sNewIncident()
{
  ParameterList params;
  params.append("mode", "new");
  params.append("prj_id",  _prjid);
  params.append("crmacct_id",  _crmacct->id());
  params.append("cntct_id",  _cntct->id());
  
  incident *newdlg = new incident(this);
  newdlg->set(params);
  if (newdlg->exec() != XDialog::Rejected)
    sFillTaskList();
}

void project::sNewOpportunity()
{
  ParameterList params;
  params.append("mode", "new");
  params.append("prj_id",  _prjid);
  params.append("crmacct_id",  _crmacct->id());
  params.append("cntct_id",  _cntct->id());
  
  opportunity *newdlg = new opportunity(this);
  newdlg->set(params);
  if (newdlg->exec() != XDialog::Rejected)
    sFillTaskList();
}

void project::sNewQuotation()
{
  ParameterList params;
  params.append("mode", "newQuote");
  params.append("prj_id",  _prjid);

  salesOrder *newdlg = new salesOrder(this);
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void project::sNewSalesOrder()
{
  ParameterList params;
  params.append("mode", "new");
  params.append("prj_id",  _prjid);

  salesOrder *newdlg = new salesOrder(this);
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}


void project::sNewPurchaseOrder()
{
  ParameterList params;
  params.append("mode", "new");
  params.append("prj_id",  _prjid);

  purchaseOrder *newdlg = new purchaseOrder(this);
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void project::sNewWorkOrder()
{
  ParameterList params;
  params.append("mode", "new");
  params.append("prj_id",  _prjid);

  workOrder *newdlg = new workOrder(this);
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void project::sEditOrder()
{
  ParameterList params;

  if(_prjorders->altId() == 15)
  {
    params.append("mode", "editQuote");
    params.append("quhead_id", _prjorders->id());

    salesOrder *newdlg = new salesOrder(this);
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
  else if(_prjorders->altId() == 17)
  {
    params.append("mode", "editQuote");
    params.append("soitem_id", _prjorders->id());

    salesOrderItem newdlg(this);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjorders->altId() == 25)
  {
    params.append("mode",      "edit");
    params.append("sohead_id", _prjorders->id());
    salesOrder *newdlg = new salesOrder(this);
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg, Qt::WindowModal);
  }
  else if(_prjorders->altId() == 27)
  {
    params.append("mode", "edit");
    params.append("soitem_id", _prjorders->id());

    salesOrderItem newdlg(this);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjorders->altId() == 35)
  {
    invoice::editInvoice(_prjorders->id(), this);
  }
  else if(_prjorders->altId() == 37)
  {
    params.append("mode", "edit");
    params.append("invcitem_id", _prjorders->id());

    invoiceItem newdlg(this);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjorders->altId() == 45)
  {
    params.append("mode", "edit");
    params.append("wo_id", _prjorders->id());

    workOrder *newdlg = new workOrder(this);
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
  else if(_prjorders->altId() == 65)
  {
    params.append("mode", "edit");
    params.append("pohead_id", _prjorders->id());

    purchaseOrder *newdlg = new purchaseOrder(this);
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
  else if(_prjorders->altId() == 67)
  {
    ParameterList params;
    params.append("mode", "edit");
    params.append("poitem_id", _prjorders->id());

    purchaseOrderItem newdlg(this, "", true);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjtask->altId() == 105)
  {
    ParameterList params;
    params.append("mode", "edit");
    params.append("incdt_id", _prjtask->id());

    incident newdlg(this, "", true);
    newdlg.set(params);
    if (newdlg.exec() != XDialog::Rejected)
      sFillTaskList();
  }
  else if(_prjtask->altId() == 110)
  {
    ParameterList params;
    params.append("mode", "edit");
    params.append("ophead_id", _prjtask->id());

    opportunity newdlg(this, "", true);
    newdlg.set(params);
    if (newdlg.exec() != XDialog::Rejected)
      sFillTaskList();
  }
}

void project::sViewOrder()
{
  ParameterList params;

  if(_prjorders->altId() == 15)
  {
    params.append("mode", "viewQuote");
    params.append("quhead_id", _prjorders->id());

    salesOrder *newdlg = new salesOrder(this);
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
  else if(_prjorders->altId() == 17)
  {
    params.append("mode", "viewQuote");
    params.append("soitem_id", _prjorders->id());

    salesOrderItem newdlg(this);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjorders->altId() == 25)
  {
    params.append("mode",      "view");
    params.append("sohead_id", _prjorders->id());
    salesOrder *newdlg = new salesOrder(this);
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg, Qt::WindowModal);
  }
  else if(_prjorders->altId() == 27)
  {
    params.append("mode", "view");
    params.append("soitem_id", _prjorders->id());

    salesOrderItem newdlg(this);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjorders->altId() == 35)
  {
    invoice::viewInvoice(_prjorders->id(), this);
  }
  else if(_prjorders->altId() == 37)
  {
    params.append("mode", "view");
    params.append("invcitem_id", _prjorders->id());

    invoiceItem newdlg(this);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjorders->altId() == 45)
  {
    params.append("mode", "view");
    params.append("wo_id", _prjorders->id());

    workOrder *newdlg = new workOrder(this);
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
  else if(_prjorders->altId() == 55)
  {
    params.append("mode", "view");
    params.append("pr_id", _prjorders->id());

    purchaseRequest newdlg(this, "", true);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjorders->altId() == 65)
  {
    params.append("mode", "view");
    params.append("pohead_id", _prjorders->id());

    purchaseOrder *newdlg = new purchaseOrder(this);
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
  else if(_prjorders->altId() == 67)
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("poitem_id", _prjorders->id());

    purchaseOrderItem newdlg(this, "", true);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjtask->altId() == 105)
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("incdt_id", _prjtask->id());

    incident newdlg(this, "", true);
    newdlg.set(params);
    newdlg.exec();
  }
  else if(_prjtask->altId() == 110)
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("ophead_id", _prjtask->id());

    opportunity newdlg(this, "", true);
    newdlg.set(params);
    newdlg.exec();
  }
}

int project::id()
{
  return _prjid;
}

void project::setVisible(bool visible)
{
  if (_close)
    close();
  else
    XDialog::setVisible(visible);
}

void project::done(int result)
{
  if (!_lock.release())
    ErrorReporter::error(QtCriticalMsg, this, tr("Locking Error"),
                         _lock.lastError(), __FILE__, __LINE__);

  XDialog::done(result);
}
