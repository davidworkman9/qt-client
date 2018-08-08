/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which(including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "scriptapi_internal.h"
#include "qlayoutproto.h"

#include <QString>
#include <QWidget>
#include <QtScript>

QScriptValue SizeConstraintToScriptValue(QScriptEngine *engine, const enum QLayout::SizeConstraint &p)
{
  return QScriptValue(engine, (int)p);
}
void SizeConstraintFromScriptValue(const QScriptValue &obj, enum QLayout::SizeConstraint &p)
{
  p = (enum QLayout::SizeConstraint)obj.toInt32();
}

QScriptValue QLayouttoScriptValue(QScriptEngine *engine, QLayout* const &item)
{
  return engine->newQObject(item);
}

void QLayoutfromScriptValue(const QScriptValue &obj, QLayout* &item)
{
  item = qobject_cast<QLayout*>(obj.toQObject());
}

void setupQLayoutProto(QScriptEngine *engine)
{
  qScriptRegisterMetaType(engine, QLayouttoScriptValue, QLayoutfromScriptValue);

  QScriptValue layout = engine->newObject();
  engine->globalObject().setProperty("QLayout",  layout);

  qScriptRegisterMetaType(engine, SizeConstraintToScriptValue, SizeConstraintFromScriptValue);
  layout.setProperty("SetDefaultConstraint", QScriptValue(engine, QLayout::SetDefaultConstraint), ENUMPROPFLAGS);
  layout.setProperty("SetFixedSize",         QScriptValue(engine, QLayout::SetFixedSize),         ENUMPROPFLAGS);
  layout.setProperty("SetMinimumSize",       QScriptValue(engine, QLayout::SetMinimumSize),       ENUMPROPFLAGS);
  layout.setProperty("SetMaximumSize",       QScriptValue(engine, QLayout::SetMaximumSize),       ENUMPROPFLAGS);
  layout.setProperty("SetMinAndMaxSize",     QScriptValue(engine, QLayout::SetMinAndMaxSize),     ENUMPROPFLAGS);
  layout.setProperty("SetNoConstraint",      QScriptValue(engine, QLayout::SetNoConstraint),      ENUMPROPFLAGS);
}
