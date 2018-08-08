/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "scriptapi_internal.h"
#include "qtoolbuttonproto.h"

#include <QToolButton>

QScriptValue QToolButtonToScriptValue(QScriptEngine *engine, QToolButton* const &item)
{
  return engine->newQObject(item);
}

void QToolButtonFromScriptValue(const QScriptValue &obj, QToolButton* &item)
{
  item = qobject_cast<QToolButton*>(obj.toQObject());
}

static QScriptValue ToolButtonPopupModeToScriptValue(QScriptEngine *engine, const enum QToolButton::ToolButtonPopupMode &p)
{
  return QScriptValue(engine, (int)p);
}
static void ToolButtonPopupModeFromScriptValue(const QScriptValue &obj, enum QToolButton::ToolButtonPopupMode &p)
{
  p = (enum QToolButton::ToolButtonPopupMode)obj.toInt32();
}

QScriptValue constructQToolButton(QScriptContext *context, QScriptEngine *engine)
{
  QToolButton *obj = 0;
  if(context->argumentCount() == 1 && context->argument(0).isQObject())
    obj = new QToolButton(qobject_cast<QWidget*>(context->argument(0).toQObject()));
  else
    obj = new QToolButton();
  return engine->toScriptValue(obj);
}

void setupQToolButtonProto(QScriptEngine *engine)
{
  qScriptRegisterMetaType(engine, QToolButtonToScriptValue, QToolButtonFromScriptValue);

  QScriptValue proto = engine->newQObject(new QToolButtonProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QToolButton*>(), proto);

  QScriptValue constructor = engine->newFunction(constructQToolButton,
                                                 proto);
  engine->globalObject().setProperty("QToolButton",  constructor);

  qScriptRegisterMetaType(engine, ToolButtonPopupModeToScriptValue, ToolButtonPopupModeFromScriptValue);
  constructor.setProperty("DelayedPopup",    QScriptValue(engine, QToolButton::DelayedPopup),    ENUMPROPFLAGS);
  constructor.setProperty("MenuButtonPopup", QScriptValue(engine, QToolButton::MenuButtonPopup), ENUMPROPFLAGS);
  constructor.setProperty("InstantPopup",    QScriptValue(engine, QToolButton::InstantPopup),    ENUMPROPFLAGS);
}

QToolButtonProto::QToolButtonProto(QObject *parent)
    : QObject(parent)
{
}

void QToolButtonProto::setMenu(QMenu *menu)
{
  QToolButton *item = qscriptvalue_cast<QToolButton*>(thisObject());
  if (item)
    item->setMenu(menu);
}

QMenu* QToolButtonProto::menu() const
{
  QToolButton *item = qscriptvalue_cast<QToolButton*>(thisObject());
  if (item)
    return item->menu();
  return 0;
}
