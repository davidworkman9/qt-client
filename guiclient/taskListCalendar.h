/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef TASKLISTCALENDAR_H
#define TASKLISTCALENDAR_H

#include "guiclient.h"
#include "xwidget.h"
#include <parameter.h>

#include "ui_taskListCalendar.h"

class CalendarGraphicsItem;

class taskListCalendar : public XWidget, public Ui::taskListCalendar
{
  Q_OBJECT

  public:
    taskListCalendar(QWidget* parent = 0, const char * = 0, Qt::WindowFlags f = 0);

    virtual SetResponse	set(const ParameterList&);
    void setParams(ParameterList &);

  public slots:
    void languageChange();
    void sFillList();
    void sFillList(const QDate&);
    void sNewTask();
    void sEditTask();
    void sViewTask();
    void sOpen();
    void sDelete();
    void sEditCustomer();
    void sViewCustomer();

  protected:
    void resizeEvent(QResizeEvent*);
    void showEvent(QShowEvent*);

    QDate _lastDate;
    int   _myUsrId;
    CalendarGraphicsItem * calendar;

  protected slots:
    void sPopulateMenu(QMenu*);
};

#endif // TASKLISTCALENDAR_H
