/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "taskCalendarControl.h"
#include "guiclient.h"
#include <parameter.h>
#include <QDebug>
#include <metasql.h>

#include "taskListCalendar.h"

taskCalendarControl::taskCalendarControl(taskListCalendar * parent)
  : CalendarControl(parent)
{
  _list = parent;
}

QString taskCalendarControl::contents(const QDate & date)
{
  QString sql = "SELECT sum(count) AS result "
                " FROM ( "
                "  SELECT count(*) "
                "  FROM task() "
                " WHERE task_due_date = <? value('date') ?> "
                "   AND NOT task_istemplate "
                "  <? if not exists('completed') ?> "
                "  AND   task_status != 'C' "
                "  <? endif ?>"
                "  <? if exists('username') ?> "
                "  AND task_id IN (SELECT taskass_task_id "
                "                  FROM taskass "
                "                  WHERE taskass_username = <? value('username') ?>) "
                "  <? elseif exists('usr_pattern') ?>"
                "  AND task_id IN (SELECT taskass_task_id "
                "                  FROM taskass "
                "                  WHERE taskass_username ~ <? value('username') ?>) "
                "  <? endif ?>"
                "  <? if exists('active') ?>AND task_status <> 'C' <? endif ?>"
                " ) data;";     

  ParameterList params;
  params.append("date", date);
  if(_list)
    _list->setParams(params);

  MetaSQLQuery mql(sql);
  XSqlQuery qry = mql.toQuery(params);
  if(qry.first())
  {
    if(qry.value(0).toInt() != 0)
      return qry.value(0).toString();
  }
  return QString::null;
}

