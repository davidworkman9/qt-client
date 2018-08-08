/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef CRMROLE_H
#define CRMROLE_H

#include "guiclient.h"
#include "xdialog.h"
#include <parameter.h>
#include "ui_crmRole.h"

class crmRole : public XDialog, public Ui::crmRole
{
    Q_OBJECT

public:
    crmRole(QWidget* parent = 0, const char* name = 0, bool modal = true, Qt::WindowFlags fl = 0);
    ~crmRole();

public slots:
    virtual enum SetResponse set( const ParameterList & pParams );
    virtual void populate();

protected slots:
    virtual void languageChange();
    virtual void sSave();

signals:
    virtual void saveBeforeBegin();
    virtual void saveAfterCommit();

private:
    int _mode;
    int _crmroleid;

};

#endif // CRMROLE_H
