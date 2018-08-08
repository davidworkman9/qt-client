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
#include "qhostaddressproto.h"

#if QT_VERSION >= 0x050800
QScriptValue ConversionModeToScriptValue(QScriptEngine *engine, const QHostAddress::ConversionMode &item)
{
  return engine->newVariant((int)item);
}
void ConversionModeFromScriptValue(const QScriptValue &obj, QHostAddress::ConversionMode &item)
{
  item = (QHostAddress::ConversionMode)obj.toInt32();
}

QScriptValue ConversionModeFlagToScriptValue(QScriptEngine *engine, const QHostAddress::ConversionModeFlag &item)
{
  return engine->newVariant((int)item);
}
void ConversionModeFlagFromScriptValue(const QScriptValue &obj, QHostAddress::ConversionModeFlag &item)
{
  item = (QHostAddress::ConversionModeFlag)obj.toInt32();
}
#endif

QScriptValue SpecialAddressToScriptValue(QScriptEngine *engine, const QHostAddress::SpecialAddress &item)
{
  return engine->newVariant(item);
}
void SpecialAddressFromScriptValue(const QScriptValue &obj, QHostAddress::SpecialAddress &item)
{
  item = (QHostAddress::SpecialAddress)obj.toInt32();
}

QScriptValue parseSubnetForJS(QScriptContext* context, QScriptEngine* engine)
{
  if (context->argumentCount() == 1) {
    return engine->toScriptValue(QHostAddress::parseSubnet(context->argument(0).toString()));
  } else {
    return engine->undefinedValue();
  }
}

QScriptValue QPairQHostAddressintToScriptValue(QScriptEngine *engine, const QPair<QHostAddress, int> &pair)
{
  return engine->toScriptValue(QString(QHostAddress(pair.first).toString() + "/"  + QString::number(pair.second)));
}
void QPairQHostAddressintFromScriptValue(const QScriptValue &obj, QPair<QHostAddress, int> &pair)
{
  QStringList parts = obj.toString().split("/");
  pair = QPair<QHostAddress, int>(QHostAddress(parts.at(0)), parts.at(1).toInt());
}

void setupQHostAddressProto(QScriptEngine *engine)
{
  QScriptValue proto = engine->newQObject(new QHostAddressProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QHostAddress*>(), proto);
  engine->setDefaultPrototype(qMetaTypeId<QHostAddress>(),  proto);

  QScriptValue constructor = engine->newFunction(constructQHostAddress, proto);
  engine->globalObject().setProperty("QHostAddress",  constructor);

#if QT_VERSION >= 0x050800
  qScriptRegisterMetaType(engine, ConversionModeFlagToScriptValue, ConversionModeFlagFromScriptValue);
  qScriptRegisterMetaType(engine, ConversionModeToScriptValue,     ConversionModeFromScriptValue);
  constructor.setProperty("StrictConversion",          QScriptValue(engine, QHostAddress::StrictConversion),          ENUMPROPFLAGS);
  constructor.setProperty("ConvertV4MappedToIPv4",     QScriptValue(engine, QHostAddress::ConvertV4MappedToIPv4),     ENUMPROPFLAGS);
  constructor.setProperty("ConvertV4CompatToIPv4",     QScriptValue(engine, QHostAddress::ConvertV4CompatToIPv4),     ENUMPROPFLAGS);
  constructor.setProperty("ConvertLocalHost",          QScriptValue(engine, QHostAddress::ConvertLocalHost),          ENUMPROPFLAGS);
  constructor.setProperty("ConvertUnspecifiedAddress", QScriptValue(engine, QHostAddress::ConvertUnspecifiedAddress), ENUMPROPFLAGS);
  constructor.setProperty("TolerantConversion",        QScriptValue(engine, QHostAddress::TolerantConversion),        ENUMPROPFLAGS);
#endif

  qScriptRegisterMetaType(engine, SpecialAddressToScriptValue, SpecialAddressFromScriptValue);
  constructor.setProperty("Null",          QScriptValue(engine, QHostAddress::Null),          ENUMPROPFLAGS);
  constructor.setProperty("LocalHost",     QScriptValue(engine, QHostAddress::LocalHost),     ENUMPROPFLAGS);
  constructor.setProperty("LocalHostIPv6", QScriptValue(engine, QHostAddress::LocalHostIPv6), ENUMPROPFLAGS);
  constructor.setProperty("Broadcast",     QScriptValue(engine, QHostAddress::Broadcast),     ENUMPROPFLAGS);
  constructor.setProperty("AnyIPv4",       QScriptValue(engine, QHostAddress::AnyIPv4),       ENUMPROPFLAGS);
  constructor.setProperty("AnyIPv6",       QScriptValue(engine, QHostAddress::AnyIPv6),       ENUMPROPFLAGS);
  constructor.setProperty("Any",           QScriptValue(engine, QHostAddress::Any),           ENUMPROPFLAGS);

  QScriptValue parseSubnet = engine->newFunction(parseSubnetForJS);
  constructor.setProperty("parseSubnet", parseSubnet);

  qScriptRegisterMetaType(engine, QPairQHostAddressintToScriptValue, QPairQHostAddressintFromScriptValue);
}

QScriptValue constructQHostAddress(QScriptContext *context, QScriptEngine  *engine)
{
  QHostAddress *obj = 0;
  if (context->argumentCount() == 1 && context->argument(0).isString()) {
    obj = new QHostAddress(context->argument(0).toString());
  } else if (context->argumentCount() == 1 && context->argument(0).isNumber()) {
    if (context->argument(0).toInteger() >= 0 && context->argument(0).toInteger() < 7) {
      QHostAddress::SpecialAddress addr = (QHostAddress::SpecialAddress)context->argument(0).toInt32();
      obj = new QHostAddress(addr);
    } else {
      obj = new QHostAddress(context->argument(0).toInt32());
    }
  } else if (context->argumentCount() == 1 && context->argument(0).isObject()) {
    QHostAddress *hostAddr = qscriptvalue_cast<QHostAddress*>(context->argument(0));
    obj = new QHostAddress(*(hostAddr));
  } else {
    obj = new QHostAddress();
  }
  return engine->toScriptValue(obj);
}

QHostAddressProto::QHostAddressProto(QObject *parent) : QObject(parent)
{
}
QHostAddressProto::~QHostAddressProto()
{
}

void QHostAddressProto::clear()
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    item->clear();
}

bool QHostAddressProto::isInSubnet(const QHostAddress & subnet, int netmask) const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->isInSubnet(subnet, netmask);
  return false;
}

bool QHostAddressProto::isInSubnet(const QPair<QHostAddress, int> & subnet) const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->isInSubnet(subnet);
  return false;
}

bool QHostAddressProto::isLoopback() const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->isLoopback();
  return false;
}

bool QHostAddressProto::isNull() const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->isNull();
  return false;
}

QAbstractSocket::NetworkLayerProtocol QHostAddressProto::protocol() const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->protocol();
  return QAbstractSocket::NetworkLayerProtocol();
}

QString QHostAddressProto::scopeId() const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->scopeId();
  return QString();
}

void QHostAddressProto::setAddress(quint32 ip4Addr)
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    item->setAddress(ip4Addr);
}

void QHostAddressProto::setAddress(quint8 * ip6Addr)
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    item->setAddress(ip6Addr);
}

void QHostAddressProto::setAddress(const quint8 * ip6Addr)
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    item->setAddress(ip6Addr);
}

void QHostAddressProto::setAddress(const Q_IPV6ADDR & ip6Addr)
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    item->setAddress(ip6Addr);
}

void QHostAddressProto::setAddress(const sockaddr * sockaddr)
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    item->setAddress(sockaddr);
}

bool QHostAddressProto::setAddress(const QString & address)
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->setAddress(address);
  return false;
}

void QHostAddressProto::setScopeId(const QString & id)
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    item->setScopeId(id);
}

quint32 QHostAddressProto::toIPv4Address() const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->toIPv4Address();
  return quint32();
}

quint32 QHostAddressProto::toIPv4Address(bool * ok) const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->toIPv4Address(ok);
  return quint32();
}

Q_IPV6ADDR QHostAddressProto::toIPv6Address() const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->toIPv6Address();
  return Q_IPV6ADDR();
}

QString QHostAddressProto::toString() const
{
  QHostAddress *item = qscriptvalue_cast<QHostAddress*>(thisObject());
  if (item)
    return item->toString();
  return QString("[QHostAddress(unknown)]");
}

