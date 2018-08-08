/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef CRMROLES_H
#define CRMROLES_H

#include "xwidget.h"
#include "ui_crmRoles.h"

class crmRoles : public XWidget, public Ui::crmRoles
{
    Q_OBJECT

public:
    crmRoles(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::Window);
    ~crmRoles();

public slots:
    virtual void sDelete();
    virtual void sNew();
    virtual void sEdit();
    virtual void sView();
    virtual void sFillList();

protected slots:
    virtual void languageChange();

};

#endif // CRMROLES_H
