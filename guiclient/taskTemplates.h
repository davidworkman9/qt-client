/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef TASKTEMPLATES_H
#define TASKTEMPLATES_H

#include "xwidget.h"
#include "ui_taskTemplates.h"

class taskTemplates : public XWidget, public Ui::taskTemplates
{
    Q_OBJECT

public:
    taskTemplates(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::Window);
    ~taskTemplates();

public slots:
    virtual void sDelete();
    virtual void sNew();
    virtual void sEdit();
    virtual void sUpdate(QString, int);
    virtual void sFillList();

protected slots:
    virtual void languageChange();

};

#endif // TASKTEMPLATE_H
