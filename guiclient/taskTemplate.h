/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef TASKTEMPLATE_H
#define TASKTEMPLATE_H

#include "guiclient.h"
#include "xdialog.h"
#include <parameter.h>
#include "ui_taskTemplate.h"

class taskTemplate : public XDialog, public Ui::taskTemplate
{
    Q_OBJECT

public:
    taskTemplate(QWidget* parent = 0, const char* name = 0, bool modal = true, Qt::WindowFlags fl = 0);
    ~taskTemplate();

public slots:
    virtual enum SetResponse set( const ParameterList & pParams );
    virtual void populate();
    virtual bool sSave( bool pPartial );

protected slots:
    virtual void languageChange();
    virtual void sSaveAndClose();
    virtual void sClose();
    virtual void sNewTask();
    virtual void sEditTask();
    virtual void sAssignTask();
    virtual void sRemoveTask();
    virtual void sFillTaskList();

private:
    int _mode;
    int _tmplid;

};

#endif // TASKTEMPLATE_H
