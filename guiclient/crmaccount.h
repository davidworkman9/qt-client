/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef CRMACCOUNT_H
#define CRMACCOUNT_H

#include "applock.h"
#include "addresses.h"
#include "contacts.h"
#include "guiclient.h"
#include "taskList.h"
#include "xwidget.h"

#include <QSqlError>
#include <QMap>
#include "ui_crmaccount.h"

class crmaccount : public XWidget, public Ui::crmaccount
{
    Q_OBJECT

public:
    crmaccount(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::Window);
    ~crmaccount();
    static      void doDialog(QWidget *, const ParameterList &);
    static bool userHasPriv(const int = cView, const int = 0);
    Q_INVOKABLE int  id();

public slots:
    virtual enum SetResponse set(const ParameterList&);
    virtual void sPopulate();
    virtual void sPopulateRegistrations();
    virtual void setId(int id);

signals:
    int newId(int id);

protected slots:
    virtual void languageChange();

    virtual void sAddAddress();
    virtual void sAddContact();
    virtual void sClose();
    virtual void sCustomer();
    virtual void sDeleteReg();
    virtual void sEditReg();
    virtual void sEmployee();
    virtual void sNewReg();
    virtual void sProspect();
    virtual void sSave();
    virtual void sSalesRep();
    virtual void setViewMode();
    virtual void sTaxAuth();
    virtual void sUser();
    virtual void sUpdateRelationships();
    virtual void sEditVendor();
    virtual void sViewVendor();
    virtual void sCustomerToggled();
    virtual void sProspectToggled();
    virtual void sCheckNumber();
    virtual void sHandleButtons();
    virtual void sHandleChildButtons();
    virtual void setVisible(bool);
    virtual void setupAdditionalRoles();

protected:
    virtual void closeEvent(QCloseEvent*);
    
    taskList *_taskList;
    contacts *_contacts;
    addresses *_addresses;

signals:
    void saved(int);

private:
    bool        _modal;
    int         _mode;
    int         _crmacctId;
    int         _custId;
    int         _empId;
    int         _prospectId;
    int         _salesrepId;
    int         _taxauthId;
    AppLock     _lock;
    QString     _username;
    int         _vendId;
    int         _cntct1Id;
    int         _cntct2Id;
    int         _NumberGen;
    bool        _canCreateUsers;
    bool        _closed;

    QSqlError   saveNoErrorCheck(bool pInTxn = false);
    QMap<QString, int> _roles;

};

#endif // CRMACCOUNT_H
