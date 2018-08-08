/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef TASKASSIGNMENT_H
#define TASKASSIGNMENT_H

#include "guiclient.h"
#include "xdialog.h"
#include <parameter.h>
#include "ui_taskAssignment.h"

class taskAssignment : public XDialog, public Ui::taskAssignment
{
    Q_OBJECT

public:
    taskAssignment(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~taskAssignment();

    virtual SetResponse set(const ParameterList & pParams );

public slots:
    virtual void sSave();

protected slots:
    virtual void languageChange();

private:
    int     _taskid;

};

#endif // TASKASSIGNMENT_H
