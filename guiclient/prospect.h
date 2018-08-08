/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef PROSPECT_H
#define PROSPECT_H

#include "applock.h"
#include "guiclient.h"
#include "taskList.h"
#include "xwidget.h"
#include <parameter.h>
#include "contacts.h"

#include "ui_prospect.h"

class prospect : public XWidget, public Ui::prospect
{
    Q_OBJECT

public:
    prospect(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::Window);
    ~prospect();

    Q_INVOKABLE virtual int id()   const;
    Q_INVOKABLE virtual int mode() const;

public slots:
    virtual SetResponse set(const ParameterList & pParams );

    virtual void sCheckNumber();
    virtual void sDeleteQuote();
    virtual void sEditQuote();
    virtual void sNewQuote();
    virtual void sPrintQuote();
    virtual void sSaveClicked();
    virtual bool sSave(bool pPartial);
    virtual void sViewQuote();
    virtual void sFillQuotesList();
    virtual void sPopulateQuotesMenu(QMenu * menuThis);
    virtual bool sPopulate();
    virtual void sSetCrmAccountId();
    virtual void sAddContact();
    virtual void setViewMode();
    virtual void setVisible(bool);

signals:
    virtual void saveBeforeBegin();
    virtual void saveAfterCommit();
            void newId(int);
            void populated();
            void saved(int);

protected slots:
    virtual void languageChange();
    virtual void sCrmAccount();

protected:
    virtual void closeEvent(QCloseEvent*);
    contacts *_contacts;
    taskList *_taskList;
    QPushButton *_add;

private:
    int  _crmacctid;
    int  _mode;
    int  _prospectid;
    int  _NumberGen;
    int  _rowId;
    bool _isSaved;
    bool _saved;
    bool _fromCRM;
    QMap<QString, int> cmap;
    QString _cachedNumber;
    QString _crmowner;
    bool _closed;
    AppLock _lock;
};

#endif // PROSPECT_H
