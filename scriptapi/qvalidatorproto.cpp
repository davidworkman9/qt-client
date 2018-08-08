/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

/** QValidator cannot itself be exposed to scripting because it is an abstract class.
 */

#include "scriptapi_internal.h"
#include "qvalidatorproto.h"

QScriptValue QValidatorToScriptValue(QScriptEngine *engine, QValidator* const &in)
 { return engine->newQObject(in); }

void QValidatorFromScriptValue(const QScriptValue &object, QValidator* &out)
 { out = qobject_cast<QValidator*>(object.toQObject()); }


static QScriptValue StateToScriptValue(QScriptEngine *engine, const enum QValidator::State &p)
{
  return QScriptValue(engine, (int)p);
}
static void StateFromScriptValue(const QScriptValue &obj, enum QValidator::State &p)
{
  p = (enum QValidator::State)obj.toInt32();
}

void setupQValidatorProto(QScriptEngine *engine)
{
  QScriptValue proto;
  engine->setDefaultPrototype(qMetaTypeId<QValidator*>(), proto);

  qScriptRegisterMetaType(engine, QValidatorToScriptValue, QValidatorFromScriptValue);

  engine->globalObject().setProperty("QValidator", proto);

  qScriptRegisterMetaType(engine, StateToScriptValue, StateFromScriptValue);
  proto.setProperty("Invalid",      QScriptValue(engine, QValidator::Invalid),      ENUMPROPFLAGS);
  proto.setProperty("Intermediate", QScriptValue(engine, QValidator::Intermediate), ENUMPROPFLAGS);
  proto.setProperty("Acceptable",   QScriptValue(engine, QValidator::Acceptable),   ENUMPROPFLAGS);
}

QValidatorProto::QValidatorProto(QObject *parent)
    : QObject(parent)
{
}

