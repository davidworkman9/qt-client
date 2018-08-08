/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef CRMGROUP_H
#define CRMGROUP_H

#include "guiclient.h"
#include "xdialog.h"
#include <parameter.h>

#include "crmGroups.h"

#include "ui_crmGroup.h"

class crmGroup : public XDialog, public Ui::crmGroup
{
    Q_OBJECT

public:
    crmGroup(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~crmGroup();

public slots:
    virtual enum SetResponse set(const ParameterList & pParams );
    virtual void sCheck();
    virtual void sClose();
    virtual void sSaveClicked();
    virtual void sSave(bool pPartial);
    virtual void sDelete();
    virtual void sNew();
    virtual void sFillList();
    virtual void populate();

protected slots:
    virtual void languageChange();

private:
    int _mode;
    int _grpid;
    bool _saved;

    static QMap<int, struct GroupMap*> _grpMap;
    GroupMap *_elem;
};

#endif // CRMGROUP_H
