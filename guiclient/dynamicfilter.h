/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef DYNAMICFILTER_H
#define DYNAMICFILTER_H

#include "guiclient.h"
#include "xdialog.h"
#include <parameter.h>

#include "ui_dynamicfilter.h"

class dynamicfilter : public XDialog, public Ui::dynamicfilter
{
    Q_OBJECT

public:
    dynamicfilter(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~dynamicfilter();

public slots:
    virtual enum SetResponse set(const ParameterList & pParams );
    virtual void sSave();
    virtual void populate();
    virtual void sCheckFilterValidity();

protected slots:
    virtual void languageChange();

private:
    int _mode;
    int _dynamicfilterid;
    bool _isValid;
    QRegularExpression rx;
};

#endif // DYNAMICFILTER_H
