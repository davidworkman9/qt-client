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
#include "qfontproto.h"

#include <QString>

#define DEBUG false

static QScriptValue CapitalizationToScriptValue(QScriptEngine *engine, const enum QFont::Capitalization &p)
{
  return QScriptValue(engine, (int)p);
}
static void CapitalizationFromScriptValue(const QScriptValue &obj, enum QFont::Capitalization &p)
{
  p = (enum QFont::Capitalization)obj.toInt32();
}

static QScriptValue HintingPreferenceToScriptValue(QScriptEngine *engine, const enum QFont::HintingPreference &p)
{
  return QScriptValue(engine, (int)p);
}
static void HintingPreferenceFromScriptValue(const QScriptValue &obj, enum QFont::HintingPreference &p)
{
  p = (enum QFont::HintingPreference)obj.toInt32();
}

static QScriptValue SpacingTypeToScriptValue(QScriptEngine *engine, const enum QFont::SpacingType &p)
{
  return QScriptValue(engine, (int)p);
}
static void SpacingTypeFromScriptValue(const QScriptValue &obj, enum QFont::SpacingType &p)
{
  p = (enum QFont::SpacingType)obj.toInt32();
}

static QScriptValue StretchToScriptValue(QScriptEngine *engine, const enum QFont::Stretch &p)
{
  return QScriptValue(engine, (int)p);
}
static void StretchFromScriptValue(const QScriptValue &obj, enum QFont::Stretch &p)
{
  p = (enum QFont::Stretch)obj.toInt32();
}

static QScriptValue StyleToScriptValue(QScriptEngine *engine, const enum QFont::Style &p)
{
  return QScriptValue(engine, (int)p);
}
static void StyleFromScriptValue(const QScriptValue &obj, enum QFont::Style &p)
{
  p = (enum QFont::Style)obj.toInt32();
}

static QScriptValue StyleHintToScriptValue(QScriptEngine *engine, const enum QFont::StyleHint &p)
{
  return QScriptValue(engine, (int)p);
}
static void StyleHintFromScriptValue(const QScriptValue &obj, enum QFont::StyleHint &p)
{
  p = (enum QFont::StyleHint)obj.toInt32();
}

static QScriptValue StyleStrategyToScriptValue(QScriptEngine *engine, const enum QFont::StyleStrategy &p)
{
  return QScriptValue(engine, (int)p);
}
static void StyleStrategyFromScriptValue(const QScriptValue &obj, enum QFont::StyleStrategy &p)
{
  p = (enum QFont::StyleStrategy)obj.toInt32();
}

static QScriptValue WeightToScriptValue(QScriptEngine *engine, const enum QFont::Weight &p)
{
  return QScriptValue(engine, (int)p);
}
static void WeightFromScriptValue(const QScriptValue &obj, enum QFont::Weight &p)
{
  p = (enum QFont::Weight)obj.toInt32();
}

void setupQFontProto(QScriptEngine *engine)
{
  QScriptValue proto = engine->newQObject(new QFontProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QFont*>(), proto);
  engine->setDefaultPrototype(qMetaTypeId<QFont>(),  proto);

  QScriptValue constructor = engine->newFunction(constructQFont, proto);
  engine->globalObject().setProperty("QFont", constructor);

  qScriptRegisterMetaType(engine, CapitalizationToScriptValue, CapitalizationFromScriptValue);
  constructor.setProperty("MixedCase",    QScriptValue(engine, QFont::MixedCase),    ENUMPROPFLAGS);
  constructor.setProperty("AllUppercase", QScriptValue(engine, QFont::AllUppercase), ENUMPROPFLAGS);
  constructor.setProperty("AllLowercase", QScriptValue(engine, QFont::AllLowercase), ENUMPROPFLAGS);
  constructor.setProperty("SmallCaps",    QScriptValue(engine, QFont::SmallCaps),    ENUMPROPFLAGS);
  constructor.setProperty("Capitalize",   QScriptValue(engine, QFont::Capitalize),   ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, HintingPreferenceToScriptValue, HintingPreferenceFromScriptValue);
  constructor.setProperty("PreferDefaultHinting",  QScriptValue(engine, QFont::PreferDefaultHinting),  ENUMPROPFLAGS);
  constructor.setProperty("PreferNoHinting",       QScriptValue(engine, QFont::PreferNoHinting),       ENUMPROPFLAGS);
  constructor.setProperty("PreferVerticalHinting", QScriptValue(engine, QFont::PreferVerticalHinting), ENUMPROPFLAGS);
  constructor.setProperty("PreferFullHinting",     QScriptValue(engine, QFont::PreferFullHinting),     ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, SpacingTypeToScriptValue, SpacingTypeFromScriptValue);
  constructor.setProperty("PercentageSpacing", QScriptValue(engine, QFont::PercentageSpacing), ENUMPROPFLAGS);
  constructor.setProperty("AbsoluteSpacing",   QScriptValue(engine, QFont::AbsoluteSpacing),   ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, StretchToScriptValue, StretchFromScriptValue);
#if QT_VERSION >= 0x050800
  constructor.setProperty("AnyStretch",     QScriptValue(engine, QFont::AnyStretch),     ENUMPROPFLAGS);
#endif
  constructor.setProperty("UltraCondensed", QScriptValue(engine, QFont::UltraCondensed), ENUMPROPFLAGS);
  constructor.setProperty("ExtraCondensed", QScriptValue(engine, QFont::ExtraCondensed), ENUMPROPFLAGS);
  constructor.setProperty("Condensed",      QScriptValue(engine, QFont::Condensed),      ENUMPROPFLAGS);
  constructor.setProperty("SemiCondensed",  QScriptValue(engine, QFont::SemiCondensed),  ENUMPROPFLAGS);
  constructor.setProperty("Unstretched",    QScriptValue(engine, QFont::Unstretched),    ENUMPROPFLAGS);
  constructor.setProperty("SemiExpanded",   QScriptValue(engine, QFont::SemiExpanded),   ENUMPROPFLAGS);
  constructor.setProperty("Expanded",       QScriptValue(engine, QFont::Expanded),       ENUMPROPFLAGS);
  constructor.setProperty("ExtraExpanded",  QScriptValue(engine, QFont::ExtraExpanded),  ENUMPROPFLAGS);
  constructor.setProperty("UltraExpanded",  QScriptValue(engine, QFont::UltraExpanded),  ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine,         StyleToScriptValue, StyleFromScriptValue);
  constructor.setProperty("StyleNormal",  QScriptValue(engine, QFont::StyleNormal),  ENUMPROPFLAGS);
  constructor.setProperty("StyleItalic",  QScriptValue(engine, QFont::StyleItalic),  ENUMPROPFLAGS);
  constructor.setProperty("StyleOblique", QScriptValue(engine, QFont::StyleOblique), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, StyleHintToScriptValue, StyleHintFromScriptValue);
  constructor.setProperty("AnyStyle",   QScriptValue(engine, QFont::AnyStyle),   ENUMPROPFLAGS);
  constructor.setProperty("SansSerif",  QScriptValue(engine, QFont::SansSerif),  ENUMPROPFLAGS);
  constructor.setProperty("Helvetica",  QScriptValue(engine, QFont::Helvetica),  ENUMPROPFLAGS);
  constructor.setProperty("Serif",      QScriptValue(engine, QFont::Serif),      ENUMPROPFLAGS);
  constructor.setProperty("Times",      QScriptValue(engine, QFont::Times),      ENUMPROPFLAGS);
  constructor.setProperty("TypeWriter", QScriptValue(engine, QFont::TypeWriter), ENUMPROPFLAGS);
  constructor.setProperty("Courier",    QScriptValue(engine, QFont::Courier),    ENUMPROPFLAGS);
  constructor.setProperty("OldEnglish", QScriptValue(engine, QFont::OldEnglish), ENUMPROPFLAGS);
  constructor.setProperty("Decorative", QScriptValue(engine, QFont::Decorative), ENUMPROPFLAGS);
  constructor.setProperty("Monospace",  QScriptValue(engine, QFont::Monospace),  ENUMPROPFLAGS);
  constructor.setProperty("Fantasy",    QScriptValue(engine, QFont::Fantasy),    ENUMPROPFLAGS);
  constructor.setProperty("Cursive",    QScriptValue(engine, QFont::Cursive),    ENUMPROPFLAGS);
  constructor.setProperty("System",     QScriptValue(engine, QFont::System),     ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, StyleStrategyToScriptValue, StyleStrategyFromScriptValue);
  constructor.setProperty("PreferDefault",       QScriptValue(engine, QFont::PreferDefault),       ENUMPROPFLAGS);
  constructor.setProperty("PreferBitmap",        QScriptValue(engine, QFont::PreferBitmap),        ENUMPROPFLAGS);
  constructor.setProperty("PreferDevice",        QScriptValue(engine, QFont::PreferDevice),        ENUMPROPFLAGS);
  constructor.setProperty("PreferOutline",       QScriptValue(engine, QFont::PreferOutline),       ENUMPROPFLAGS);
  constructor.setProperty("ForceOutline",        QScriptValue(engine, QFont::ForceOutline),        ENUMPROPFLAGS);
  constructor.setProperty("NoAntialias",         QScriptValue(engine, QFont::NoAntialias),         ENUMPROPFLAGS);
  constructor.setProperty("NoSubpixelAntialias", QScriptValue(engine, QFont::NoSubpixelAntialias), ENUMPROPFLAGS);
  constructor.setProperty("PreferAntialias",     QScriptValue(engine, QFont::PreferAntialias),     ENUMPROPFLAGS);
  constructor.setProperty("OpenGLCompatible",    QScriptValue(engine, QFont::OpenGLCompatible),    ENUMPROPFLAGS);
  constructor.setProperty("NoFontMerging",       QScriptValue(engine, QFont::NoFontMerging),       ENUMPROPFLAGS);

  constructor.setProperty("PreferMatch",         QScriptValue(engine, QFont::PreferMatch),         ENUMPROPFLAGS);
  constructor.setProperty("PreferQuality",       QScriptValue(engine, QFont::PreferQuality),       ENUMPROPFLAGS);
  constructor.setProperty("ForceIntegerMetrics", QScriptValue(engine, QFont::ForceIntegerMetrics), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, WeightToScriptValue, WeightFromScriptValue);
  constructor.setProperty("Thin",       QScriptValue(engine, QFont::Thin),       ENUMPROPFLAGS);
  constructor.setProperty("ExtraLight", QScriptValue(engine, QFont::ExtraLight), ENUMPROPFLAGS);
  constructor.setProperty("Light",      QScriptValue(engine, QFont::Light),      ENUMPROPFLAGS);
  constructor.setProperty("Normal",     QScriptValue(engine, QFont::Normal),     ENUMPROPFLAGS);
  constructor.setProperty("Medium",     QScriptValue(engine, QFont::Medium),     ENUMPROPFLAGS);
  constructor.setProperty("DemiBold",   QScriptValue(engine, QFont::DemiBold),   ENUMPROPFLAGS);
  constructor.setProperty("Bold",       QScriptValue(engine, QFont::Bold),       ENUMPROPFLAGS);
  constructor.setProperty("ExtraBold",  QScriptValue(engine, QFont::ExtraBold),  ENUMPROPFLAGS);
  constructor.setProperty("Black",      QScriptValue(engine, QFont::Black),      ENUMPROPFLAGS);

}

QScriptValue constructQFont(QScriptContext *context,
                            QScriptEngine  *engine)
{
  QFont *obj = 0;

  if (DEBUG)
  {
    qDebug("constructQFont() entered");
    for (int i = 0; i < context->argumentCount(); i++)
      qDebug("context->argument(%d) = %s", i,
             qPrintable(context->argument(i).toString()));
  }

  if (context->argumentCount() >= 4)
    obj = new QFont(context->argument(0).toString(),
                    context->argument(1).toInt32(),
                    context->argument(2).toInt32(),
                    context->argument(3).toBool());
  else if (context->argumentCount() == 3)
    obj = new QFont(context->argument(0).toString(),
                    context->argument(1).toInt32(),
                    context->argument(2).toInt32());
  else if (context->argumentCount() == 2 &&
           qscriptvalue_cast<QPaintDevice*>(context->argument(1)))
    obj = new QFont(qscriptvalue_cast<QFont>(context->argument(0)),
                    qscriptvalue_cast<QPaintDevice*>(context->argument(1)));
  else if (context->argumentCount() == 2)
    obj = new QFont(context->argument(0).toString(),
                    context->argument(1).toInt32());
  else if (context->argumentCount() == 1 &&
          context->argument(0).isString())
  {
    if (DEBUG) qDebug("calling new QFont(QString)");
    obj = new QFont(context->argument(0).toString());
  }
  else if (context->argumentCount() == 1)
    obj = new QFont(qscriptvalue_cast<QFont>(context->argument(0)));
  else
    obj = new QFont();

  return engine->toScriptValue(obj);
}

QFontProto::QFontProto(QObject *parent)
    : QObject(parent)
{
}

bool QFontProto::bold() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->bold();
  return false;
}

int QFontProto::capitalization() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->capitalization();
  return 0;
}

QString QFontProto::defaultFamily() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->defaultFamily();
  return QString();
}

bool QFontProto::exactMatch() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->exactMatch();
  return false;
}

QString QFontProto::family() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->family();
  return QString();
}

bool QFontProto::fixedPitch() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->fixedPitch();
  return false;
}

/*
FT_Face QFontProto::freetypeFace() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->freetypeFace();
  return FT_Face();
}
*/

bool QFontProto::fromString(const QString &descrip)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->fromString(descrip);
  return false;
}

/*
HFONT QFontProto::handle() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->handle();
  return HFONT();
}
*/

bool QFontProto::isCopyOf(const QFont &f) const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->isCopyOf(f);
  return false;
}

bool QFontProto::italic() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->italic();
  return false;
}

bool QFontProto::kerning() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->kerning();
  return false;
}

QString QFontProto::key() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->key();
  return QString();
}

QString QFontProto::lastResortFamily() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->lastResortFamily();
  return QString();
}

QString QFontProto::lastResortFont() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->lastResortFont();
  return QString();
}

qreal QFontProto::letterSpacing() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->letterSpacing();
  return qreal();
}

int QFontProto::letterSpacingType() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->letterSpacingType();
  return 0;
}

bool QFontProto::overline() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->overline();
  return false;
}

int QFontProto::pixelSize() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->pixelSize();
  return 0;
}

int QFontProto::pointSize() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->pointSize();
  return 0;
}

qreal QFontProto::pointSizeF() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->pointSizeF();
  return 0;
}

bool QFontProto::rawMode() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->rawMode();
  return false;
}

QString QFontProto::rawName() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->rawName();
  return QString();
}

QFont QFontProto::resolve(const QFont &other) const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->resolve(other);
  return QFont();
}

void QFontProto::setBold(bool enable)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setBold(enable);
}

void QFontProto::setCapitalization(int caps)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setCapitalization((QFont::Capitalization)caps);
}

void QFontProto::setFamily(const QString &family)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setFamily(family);
}

void QFontProto::setFixedPitch(bool enable)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setFixedPitch(enable);
}

void QFontProto::setItalic(bool enable)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setItalic(enable);
}

void QFontProto::setKerning(bool enable)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setKerning(enable);
}

void QFontProto::setLetterSpacing(int type, qreal spacing)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setLetterSpacing((QFont::SpacingType)type, spacing);
}

void QFontProto::setOverline(bool enable)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setOverline(enable);
}

void QFontProto::setPixelSize(int pixelSize)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setPixelSize(pixelSize);
}

void QFontProto::setPointSize(int pointSize)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setPointSize(pointSize);
}

void QFontProto::setPointSizeF(qreal pointSize)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setPointSizeF(pointSize);
}

void QFontProto::setRawMode(bool enable)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setRawMode(enable);
}

void QFontProto::setRawName(const QString &name)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setRawName(name);
}

void QFontProto::setStretch(int factor)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setStretch(factor);
}

void QFontProto::setStrikeOut(bool enable)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setStrikeOut(enable);
}

void QFontProto::setStyle(int style)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setStyle((QFont::Style)style);
}

void QFontProto::setStyleHint(int hint, int strategy)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setStyleHint((QFont::StyleHint)hint, (QFont::StyleStrategy)strategy);
}

void QFontProto::setStyleStrategy(int s)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setStyleStrategy((QFont::StyleStrategy)s);
}

void QFontProto::setUnderline(bool enable)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setUnderline(enable);
}

void QFontProto::setWeight(int weight)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setWeight(weight);
}

void QFontProto::setWordSpacing(qreal spacing)
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    item->setWordSpacing(spacing);
}

int QFontProto::stretch() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->stretch();
  return 0;
}

bool QFontProto::strikeOut() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->strikeOut();
  return false;
}

int QFontProto::style() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->style();
  return 0;
}

int QFontProto::styleHint() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->styleHint();
  return 0;
}

int QFontProto::styleStrategy() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->styleStrategy();
  return 0;
}

QString QFontProto::toString() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->toString();
  return QString();
}

bool QFontProto::underline() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->underline();
  return false;
}

int QFontProto::weight() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->weight();
  return 0;
}

qreal QFontProto::wordSpacing() const
{
  QFont *item = qscriptvalue_cast<QFont*>(thisObject());
  if (item)
    return item->wordSpacing();
  return qreal();
}

