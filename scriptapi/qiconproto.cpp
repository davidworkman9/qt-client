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
#include "qiconproto.h"
#include "xsqlquery.h"

#include <quuencode.h>

#include <QIcon>
#include <QImage>

QScriptValue ModeToScriptValue(QScriptEngine *engine, const enum QIcon::Mode &p)
{
  return QScriptValue(engine, (int)p);
}
void ModeFromScriptValue(const QScriptValue &obj, enum QIcon::Mode &p)
{
  p = (enum QIcon::Mode)obj.toInt32();
}

QScriptValue StateToScriptValue(QScriptEngine *engine, const enum QIcon::State &p)
{
  return QScriptValue(engine, (int)p);
}
void StateFromScriptValue(const QScriptValue &obj, enum QIcon::State &p)
{
  p = (enum QIcon::State)obj.toInt32();
}

void setupQIconProto(QScriptEngine *engine)
{
  QScriptValue proto = engine->newQObject(new QIconProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QIcon*>(), proto);
  engine->setDefaultPrototype(qMetaTypeId<QIcon>(),  proto);

  QScriptValue constructor = engine->newFunction(constructQIcon,
                                                 proto);
  engine->globalObject().setProperty("QIcon",  constructor);

  qScriptRegisterMetaType(engine, ModeToScriptValue, ModeFromScriptValue);
  constructor.setProperty("Normal",   QScriptValue(engine, QIcon::Normal),   ENUMPROPFLAGS);
  constructor.setProperty("Disabled", QScriptValue(engine, QIcon::Disabled), ENUMPROPFLAGS);
  constructor.setProperty("Active",   QScriptValue(engine, QIcon::Active),   ENUMPROPFLAGS);
  constructor.setProperty("Selected", QScriptValue(engine, QIcon::Selected), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, StateToScriptValue, StateFromScriptValue);
  constructor.setProperty("Off", QScriptValue(engine, QIcon::Off), ENUMPROPFLAGS);
  constructor.setProperty("On",  QScriptValue(engine, QIcon::On),  ENUMPROPFLAGS);
}

QScriptValue constructQIcon(QScriptContext * context,
                                    QScriptEngine  *engine)
{
  QIcon *obj = 0;
  if(context->argumentCount() == 1)
    obj = new QIcon(context->argument(0).toString());
  else
    obj = new QIcon();
  return engine->toScriptValue(obj);
}

QIconProto::QIconProto(QObject *parent)
    : QObject(parent)
{
}

bool QIconProto::isNull() const
{
  QIcon *item = qscriptvalue_cast<QIcon*>(thisObject());
  if (item)
    return item->isNull();
  return true;
}

/*!
  Adds image \a name from the xTuple databes to the icon.
*/
void QIconProto::addDbImage(const QString& name)
{
  QIcon *item = qscriptvalue_cast<QIcon*>(thisObject());
  if (item)
  {
    QImage img;
    XSqlQuery image;
    image.prepare( "SELECT image_data "
                   "FROM image "
                   "WHERE (image_name=:name);" );
    image.bindValue(":name", name);
    image.exec();
    if (image.first())
    {
      img.loadFromData(QUUDecode(image.value("image_data").toString()));
      item->addPixmap(QPixmap::fromImage(img));
    }
  }
}

