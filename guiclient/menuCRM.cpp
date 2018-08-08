/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "menuCRM.h"

#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>

#include "guiclient.h"

#include "contact.h"
#include "contacts.h"
#include "contactMerge.h"
#include "crmaccountMerge.h"
#include "dspOrderActivityByProject.h"
#include "dspProjectSummary.h"
#include "addressMerge.h"
#include "buildCRMGroups.h"
#include "address.h"
#include "addresses.h"
#include "createRecurringItems.h"
#include "crmaccount.h"
#include "crmaccounts.h"
#include "project.h"
#include "projects.h"
#include "crmGroups.h"
#include "prospect.h"
#include "prospects.h"
#include "incidentWorkbench.h"
#include "incident.h"
#include "opportunity.h"
#include "opportunityList.h"
#include "task.h"
#include "taskList.h"
#include "taskListCalendar.h"

#include "editOwners.h"
#include "setup.h"

menuCRM::menuCRM(GUIClient *Pparent) :
  QObject(Pparent)
{
  setObjectName("crmModule");
  parent = Pparent;
  
  toolBar = new QToolBar(tr("CRM Tools"));
  toolBar->setObjectName("CRM Tools");
  toolBar->setIconSize(QSize(32, 32));
  parent->addToolBar(toolBar);

  // Menus
  crmMenu           = new QMenu(parent);
  projectsMenu      = new QMenu(parent);
  incidentMenu      = new QMenu(parent);
  taskMenu          = new QMenu(parent);
  reportsMenu       = new QMenu(parent);
  accountsMenu      = new QMenu(parent);
  contactsMenu      = new QMenu(parent);
  addressMenu       = new QMenu(parent);
  prospectCRMMenu   = new QMenu(parent);
  utilitiesMenu     = new QMenu(parent);
  opportunityMenu   = new QMenu(parent);

  crmMenu->setObjectName("menu.crm");
  projectsMenu->setObjectName("menu.crm.projects");
  incidentMenu->setObjectName("menu.crm.incident");
  taskMenu->setObjectName("menu.crm.tasks");
  reportsMenu->setObjectName("menu.crm.reports");
  accountsMenu->setObjectName("menu.crm.accounts");
  contactsMenu->setObjectName("menu.crm.contacts");
  addressMenu->setObjectName("menu.crm.address");
  prospectCRMMenu->setObjectName("menu.crm.prospect");
  utilitiesMenu->setObjectName("menu.crm.utilities");
  opportunityMenu->setObjectName("menu.crm.opportunity");

  actionProperties acts[] = {
    // CRM | Incident
    { "menu",			tr("&Incident"),	(char*)incidentMenu,		crmMenu,	"true", NULL, NULL, true	, NULL },
    { "crm.incident",		tr("&New..."),		SLOT(sIncident()),		incidentMenu,	"MaintainPersonalIncidents MaintainAllIncidents", NULL, NULL, true , NULL },
    { "crm.incidentList",	tr("&List..."),	SLOT(sIncidentWorkbench()),	incidentMenu,	"ViewPersonalIncidents MaintainPersonalIncidents ViewAllIncidents MaintainAllIncidents", new QPixmap(":/images/incidents.png"), toolBar, true , tr("Incident List") },

    // CRM / Tasks
    { "menu",			tr("&Tasks"),            (char*)taskMenu,        crmMenu,	"true", NULL, NULL, true	, NULL },
    { "crm.task",		tr("&New..."),	         SLOT(sTask()),	        taskMenu,	"MaintainPersonalTaskItems MaintainAllTaskItems", NULL, NULL, true	, NULL },
    { "crm.taskList",		tr("&List..."),		 SLOT(sTaskList()),	taskMenu,	"MaintainPersonalTaskItems ViewPersonalTaskItems MaintainAllTaskItems ViewAllTaskItems", new QPixmap(":/images/toDoList.png"), toolBar, true	, tr("To-Do List") },
    { "crm.taskListCalendar",	tr("&Calendar List..."), SLOT(sTaskListCalendar()),	taskMenu,	"MaintainPersonalTaskItems ViewPersonalTaskItems MaintainAllTaskItems ViewAllTaskItems", NULL, NULL, true, NULL},

    //  Project
    { "menu", tr("Pro&ject"), (char*)projectsMenu, crmMenu, "true", NULL, NULL, true	, NULL },
    { "pm.newProject", tr("&New..."), SLOT(sNewProject()), projectsMenu, "MaintainPersonalProjects MaintainAllProjects", NULL, NULL, true , NULL },
    { "pm.projects", tr("&List..."), SLOT(sProjects()), projectsMenu, "ViewPersonalProjects MaintainPersonalProjects ViewAllProjects MaintainAllProjects", new QPixmap(":/images/projects.png"), toolBar, true , tr("List Projects") },
    
    // Opportunity
    { "menu",		tr("&Opportunity"),	(char*)opportunityMenu,	crmMenu,		"true", NULL, NULL, true	, NULL },
    { "crm.newOpportunity", tr("&New..."), SLOT(sNewOpportunity()), opportunityMenu, "MaintainPersonalOpportunities MaintainAllOpportunities", NULL, NULL, true , NULL },
    { "crm.listOpportunity", tr("&List..."), SLOT(sOpportunities()), opportunityMenu, "MaintainPersonalOpportunities ViewPersonalOpportunities MaintainAllOpportunities ViewAllOpportunities", NULL, NULL, true , NULL },

    { "separator",		NULL,				NULL,			crmMenu,	"true", NULL, NULL, true	, NULL },

    // Reports
    { "menu",				tr("&Reports"),		(char*)reportsMenu,			crmMenu,	"true", NULL, NULL, true	, NULL },
    { "pm.dspOrderActivityByProject", tr("Order &Activity by Project..."), SLOT(sDspOrderActivityByProject()), reportsMenu, "ViewAllProjects ViewPersonalProjects", NULL, NULL, true , NULL },
    { "pm.dspProjectSummary", tr("Project Summary..."), SLOT(sDspProjectSummary()), reportsMenu, "ViewAllProjects ViewPersonalProjects", NULL, NULL, true , NULL },
    { "separator",		NULL,				NULL,			crmMenu,	"true", NULL, NULL, true	, NULL },
    
    // CRM | Account
    { "menu",		tr("&Account"),		(char*)accountsMenu,	crmMenu,		"true", NULL, NULL, true	, NULL },
    { "crm.crmaccount",		tr("&New..."),	SLOT(sCRMAccount()),	accountsMenu,	"MaintainPersonalCRMAccounts MaintainAllCRMAccounts", NULL, NULL, true , NULL },
    { "crm.crmaccounts",	tr("&List..."),	SLOT(sCRMAccounts()),	accountsMenu,	"MaintainPersonalCRMAccounts ViewPersonalCRMAccounts MaintainAllCRMAccounts ViewAllCRMAccounts", new QPixmap(":/images/accounts.png"), toolBar, true , tr("List Accounts") },
    { "separator",		NULL,				NULL,	accountsMenu,	"true", NULL, NULL, true	, NULL },
    { "crm.crmaccountGroups",   tr("&Groups..."),  SLOT(sCRMAccountGroups()),   accountsMenu,   "MaintainAccountGroups ViewAccountGroups",	NULL, NULL, true, NULL },
      
    // CRM | Contact
    { "menu",		tr("&Contact"),		(char*)contactsMenu,	crmMenu,		"true", NULL, NULL, true	, NULL },
    { "crm.contact",	tr("&New..."),		SLOT(sContact()),	contactsMenu,	"MaintainPersonalContacts MaintainAllContacts", NULL, NULL, true	, NULL },
    { "crm.contacts",	tr("&List..."),		SLOT(sContacts()),	contactsMenu,	"MaintainPersonalContacts ViewPersonalContacts MaintainAllContacts ViewAllContacts", new QPixmap(":/images/contacts.png"), toolBar, true , tr("List Contacts") },
    { "separator",		NULL,				NULL,	contactsMenu,	"true", NULL, NULL, true	, NULL },
    { "crm.contactGroups", tr("&Groups..."),  SLOT(sContactGroups()),   contactsMenu,   "MaintainContactGroups ViewContactGroups",	NULL, NULL, true, NULL },
    
    // CRM | Address
    { "menu",		tr("A&ddress"),		(char*)addressMenu,	crmMenu,		"true", NULL, NULL, true	, NULL },
    { "crm.address",	tr("&New..."),		SLOT(sAddress()),	addressMenu,	"MaintainAddresses", NULL, NULL, true	, NULL },
    { "crm.addresses",	tr("&List..."),	        SLOT(sAddresses()),	addressMenu,	"MaintainAddresses ViewAddresses", NULL, NULL, true , NULL },
    { "separator",		NULL,				NULL,	addressMenu,	"true", NULL, NULL, true	, NULL },
    { "crm.addressGroups", tr("&Groups..."),  SLOT(sAddressGroups()),   addressMenu,   "MaintainAddressGroups ViewAddressGroups",	NULL, NULL, true, NULL },

    // CRM | Prospect
    { "menu",	tr("&Prospect"),              (char*)prospectCRMMenu,	crmMenu,	"true",	NULL, NULL, true, NULL },
    { "so.enterNewProspect", tr("&New..."),   SLOT(sNewProspect()), prospectCRMMenu, "MaintainProspectMasters",	NULL, NULL, true, NULL },
    { "so.prospects", tr("&List..."),	      SLOT(sProspects()), prospectCRMMenu, "MaintainProspectMasters ViewProspectMasters",	NULL, NULL, true, NULL },
    { "separator",		NULL,				NULL,  prospectCRMMenu,	"true", NULL, NULL, true	, NULL },
    { "so.prospectGroups", tr("&Groups..."),  SLOT(sProspectGroups()), prospectCRMMenu, "MaintainProspectGroups ViewProspectGroups",	NULL, NULL, true, NULL },

    { "separator",		NULL,				NULL,			crmMenu,	"true", NULL, NULL, true	, NULL },

    //Utilities
    { "menu",			tr("&Utilities"),		(char*)utilitiesMenu,		crmMenu,	"true", NULL, NULL, true	, NULL },
    { "crm.replaceOwner",	tr("Edit O&wners"),		SLOT(sEditOwners()),	utilitiesMenu,	"EditOwner", NULL, NULL, true, NULL },
    { "crm.createRecurringItems",tr("Create &Recurring Items..."), SLOT(sCreateRecurringItems()),utilitiesMenu, "MaintainPersonalIncidents MaintainPersonalIncidents MaintainAllIncidents MaintainAllProjects MaintainPersonalTaskItems MaintainAllTaskItems", NULL, NULL, true, NULL },
    { "separator",		NULL,				NULL,			utilitiesMenu,	"true", NULL, NULL, true	, NULL },
    { "crm.crmaccountMerge",     tr("Merge &Accounts"), SLOT(sCrmaccountMerge()),    utilitiesMenu, "MaintainAllCRMAccounts", NULL, NULL, true, NULL },
    { "crm.addressMerge",        tr("Merge Addresses"), SLOT(sAddressMerge()),       utilitiesMenu, "MaintainAddresses",      NULL, NULL, true, NULL },
    { "crm.contactMerge",        tr("&Merge Contacts"), SLOT(sContactMerge()),       utilitiesMenu, "MergeContacts",          NULL, NULL, true, NULL },
    { "separator",		NULL,				NULL,		     utilitiesMenu,	"true", NULL, NULL, true	, NULL },
    { "crm.buildCRMGroups",      tr("&Build CRM Groups"), SLOT(sBuildCRMGroups()),   utilitiesMenu, "MaintainCustomerGroups MaintainEmployeeGroups MaintainProspectGroups MaintainContactGroups", NULL, NULL, true, NULL },

    { "crm.setup",	tr("&Setup..."),	SLOT(sSetup()),	crmMenu,	NULL,	NULL,	NULL,	true, NULL}

  };

  addActionsToMenu(acts, sizeof(acts) / sizeof(acts[0]));

  parent->populateCustomMenu(crmMenu, "CRM");
  QAction * m = parent->menuBar()->addMenu(crmMenu);
  if(m)
    m->setText(tr("C&RM"));
}
  
void menuCRM::addActionsToMenu(actionProperties acts[], unsigned int numElems)
{
  QAction * m = 0;
  for (unsigned int i = 0; i < numElems; i++)
  {
    if (acts[i].actionName == QString("menu"))
    {
      m = acts[i].menu->addMenu((QMenu*)(acts[i].slot));
      if(m)
        m->setText(acts[i].actionTitle);
    }
    else if (acts[i].actionName == QString("separator"))
    {
      m = acts[i].menu->addSeparator();
    }
    else if ((acts[i].toolBar != NULL) && (!acts[i].toolTip.isEmpty()))
    {
      m = new Action( parent,
                  acts[i].actionName,
                  acts[i].actionTitle,
                  this,
                  acts[i].slot,
                  acts[i].menu,
                  acts[i].priv,
                  (acts[i].pixmap),
                  acts[i].toolBar,
                  acts[i].toolTip) ;
    }
    else if (acts[i].toolBar != NULL)
    {
      m = new Action( parent,
                  acts[i].actionName,
                  acts[i].actionTitle,
                  this,
                  acts[i].slot,
                  acts[i].menu,
                  acts[i].priv,
                  (acts[i].pixmap),
                  acts[i].toolBar,
                  acts[i].actionTitle) ;
    }
    else
    {
      m = new Action( parent,
                  acts[i].actionName,
                  acts[i].actionTitle,
                  this,
                  acts[i].slot,
                  acts[i].menu,
                  acts[i].priv ) ;
    }
    if (m) m->setVisible(acts[i].visible);
  }
}

void menuCRM::sNewProject()
{
  ParameterList params;
  params.append("mode", "new");

  project newdlg(omfgThis, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void menuCRM::sProjects()
{
  omfgThis->handleNewWindow(new projects());
}

void menuCRM::sDspOrderActivityByProject()
{
  omfgThis->handleNewWindow(new dspOrderActivityByProject());
}

void menuCRM::sDspProjectSummary()
{
  omfgThis->handleNewWindow(new dspProjectSummary());
}

void menuCRM::sCRMAccount()
{
  ParameterList params;
  params.append("mode", "new");
  crmaccount* newdlg = new crmaccount();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void menuCRM::sCRMAccounts()
{
  omfgThis->handleNewWindow(new crmaccounts());
}

void menuCRM::sCRMAccountGroups()
{
  ParameterList params;
  params.append("groupType", crmGroups::Account);

  crmGroups *newdlg = new crmGroups();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void menuCRM::sEditOwners()
{
  omfgThis->handleNewWindow(new editOwners());
}

void menuCRM::sCreateRecurringItems()
{
  omfgThis->handleNewWindow(new createRecurringItems(), Qt::ApplicationModal);
}

void menuCRM::sContact()
{
  ParameterList params;
  params.append("mode", "new");
  contact* newdlg = new contact();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void menuCRM::sContacts()
{
  omfgThis->handleNewWindow(new contacts());
}

void menuCRM::sContactGroups()
{
  ParameterList params;
  params.append("groupType", crmGroups::Contact);

  crmGroups *newdlg = new crmGroups();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void menuCRM::sContactMerge()
{
  omfgThis->handleNewWindow(new contactMerge());
}

void menuCRM::sCrmaccountMerge()
{
  omfgThis->handleNewWindow(new crmaccountMerge());
}

void menuCRM::sAddressMerge()
{
  omfgThis->handleNewWindow(new addressMerge());
}

void menuCRM::sBuildCRMGroups()
{
  omfgThis->handleNewWindow(new buildCRMGroups());
}

void menuCRM::sAddress()
{
  ParameterList params;
  params.append("mode", "new");
  address* newdlg = new address();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void menuCRM::sAddresses()
{
  omfgThis->handleNewWindow(new addresses());
}

void menuCRM::sAddressGroups()
{
  ParameterList params;
  params.append("groupType", crmGroups::Address);

  crmGroups *newdlg = new crmGroups();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void menuCRM::sIncidentWorkbench()
{
  omfgThis->handleNewWindow(new incidentWorkbench());
}

void menuCRM::sIncident()
{
  ParameterList params;
  params.append("mode", "new");
  incident* newdlg = new incident();
  newdlg->set(params);
  newdlg->exec();
}

void menuCRM::sTaskList()
{
  ParameterList params;
  params.append("run");
  taskList* win = new taskList();
  win->set(params);
  omfgThis->handleNewWindow(win);
}

void menuCRM::sTaskListCalendar()
{
  omfgThis->handleNewWindow(new taskListCalendar());
}

void menuCRM::sTask()
{
  ParameterList params;
  params.append("mode", "new");
  params.append("parent", "TASK");
  task* newdlg = new task();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void menuCRM::sNewOpportunity()
{
  ParameterList params;
  params.append("mode", "new");

  opportunity newdlg(omfgThis, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void menuCRM::sOpportunities()
{
  ParameterList params;
  params.append("run");
  opportunityList* win = new opportunityList();
  win->set(params);
  omfgThis->handleNewWindow(win);
}

void menuCRM::sNewProspect()
{
  ParameterList params;
  params.append("mode", "new");

  prospect *newdlg = new prospect();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void menuCRM::sProspects()
{
  omfgThis->handleNewWindow(new prospects());
}

void menuCRM::sProspectGroups()
{
  ParameterList params;
  params.append("groupType", crmGroups::Prospect);

  crmGroups *newdlg = new crmGroups();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void menuCRM::sSetup()
{
  ParameterList params;
  params.append("module", Xt::CRMModule);

  setup newdlg(parent);
  newdlg.set(params);
  newdlg.exec();
}
