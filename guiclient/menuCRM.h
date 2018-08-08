/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef menuCRM_h
#define menuCRM_h

#include <QObject>
#include <QPixmap>

class QToolBar;
class QMenu;
class GUIClient;

class menuCRM : public QObject
{
  Q_OBJECT

  struct actionProperties {
    const char*		actionName;
    const QString	actionTitle;
    const char*		slot;
    QMenu*		menu;
    QString		priv;
    QPixmap*	pixmap;
    QToolBar*		toolBar;
    bool		visible;
    const QString   toolTip;
  };

  public:
    menuCRM(GUIClient *);

  public slots:

    void sNewProject();
    void sProjects();
    void sDspOrderActivityByProject();
    void sDspProjectSummary();
    void sAddress();
    void sAddressGroups();
    void sAddresses();
    void sCRMAccount();
    void sCRMAccounts();
    void sCRMAccountGroups();
    void sContact();
    void sContacts();
    void sContactGroups();
    void sContactMerge();
    void sCrmaccountMerge();
    void sAddressMerge();
    void sBuildCRMGroups();

    void sCreateRecurringItems();
    void sIncident();
    void sIncidentWorkbench();
    void sTask();
    void sTaskList();
    void sTaskListCalendar();
    void sNewOpportunity();
    void sOpportunities();
    void sNewProspect();
    void sProspects();
    void sProspectGroups();
    void sEditOwners();
    void sSetup();

  private:
    GUIClient *parent;

    QToolBar   *toolBar;
    QMenu *crmMenu;
    QMenu *projectsMenu;
    QMenu *incidentMenu;
    QMenu *taskMenu;
    QMenu *reportsMenu;
    QMenu *accountsMenu;
    QMenu *contactsMenu;
    QMenu *addressMenu;
    QMenu *prospectCRMMenu;
    QMenu *utilitiesMenu;
    QMenu *opportunityMenu;
    
    void	addActionsToMenu(actionProperties [], unsigned int);
};

#endif

