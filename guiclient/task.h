/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef TASK_H
#define TASK_H

#include "applock.h"
#include "guiclient.h"
#include "xdialog.h"
#include <parameter.h>
#include "ui_task.h"

class task : public XDialog, public Ui::task
{
    Q_OBJECT

public:
    task(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~task();

    static bool userHasPriv(const int = cView, const QString = "TD", const int = 0);

    virtual SetResponse set(const ParameterList & pParams );

    Q_INVOKABLE virtual int id();

public slots:
    virtual void populate();
    virtual void setViewMode();
    virtual void sSave();
    virtual void sClose();
    virtual void sNewUser();
    virtual void sDeleteUser();
    virtual void sFillUserList();
    virtual void sStatusChanged(const int);
    virtual void sCompletedChanged();
    virtual void sHoursAdjusted();
    virtual void sExpensesAdjusted();
    virtual void closeEvent(QCloseEvent *pEvent);

protected slots:
    virtual void languageChange();
    virtual void setVisible(bool);

private:
    int     _mode;
    int     _parentid;
    QString _parenttype;
    int     _taskid;
    bool    _saved;
    QString _assignedTo;
    QDate   _assigned;
    bool    _close;
    AppLock _lock;
    bool    _isTemplate;
};

#endif // TASK_H
