/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef TASKCALENDARCONTROL_H
#define TASKCALENDARCONTROL_H

#include <calendarcontrol.h>

class taskListCalendar;

class taskCalendarControl : public CalendarControl
{
  Q_OBJECT

  public:
    taskCalendarControl(taskListCalendar * parent = 0);

    QString contents(const QDate &);

  protected:
    taskListCalendar *_list;
};

#endif // TASKCALENDARCONTROL_H
