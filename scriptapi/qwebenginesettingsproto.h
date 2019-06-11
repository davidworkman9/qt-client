/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __QWEBENGINESETTINGSPROTO_H__
#define __QWEBENGINESETTINGSPROTO_H__

#include <QScriptEngine>

void setupQWebEngineSettingsProto(QScriptEngine *engine);

#include <QScriptable>
#include <QWebEngineSettings>

Q_DECLARE_METATYPE(QWebEngineSettings*)
Q_DECLARE_METATYPE(enum QWebEngineSettings::FontFamily)
Q_DECLARE_METATYPE(enum QWebEngineSettings::FontSize)
#if QT_VERSION >= QT_VERSION_CHECK(5,11,0)
Q_DECLARE_METATYPE(enum QWebEngineSettings::UnknownUrlSchemePolicy)
#endif
Q_DECLARE_METATYPE(enum QWebEngineSettings::WebAttribute)

QScriptValue constructQWebEngineSettings(QScriptContext *context, QScriptEngine *engine);

class QWebEngineSettingsProto : public QObject, public QScriptable
{
  Q_OBJECT

  public:
    QWebEngineSettingsProto(QObject *parent);

    Q_INVOKABLE QString                                     defaultTextEncoding() const;
    Q_INVOKABLE QString                                     fontFamily(QWebEngineSettings::FontFamily which) const;
    Q_INVOKABLE int                                         fontSize(QWebEngineSettings::FontSize type) const;
    Q_INVOKABLE void                                        resetAttribute(QWebEngineSettings::WebAttribute attribute);
    Q_INVOKABLE void                                        resetFontFamily(QWebEngineSettings::FontFamily which);
    Q_INVOKABLE void                                        resetFontSize(QWebEngineSettings::FontSize type);
    Q_INVOKABLE void                                        setAttribute(QWebEngineSettings::WebAttribute attribute, bool on);
    Q_INVOKABLE void                                        setDefaultTextEncoding(const QString & encoding);
    Q_INVOKABLE void                                        setFontFamily(QWebEngineSettings::FontFamily which, const QString & family);
    Q_INVOKABLE void                                        setFontSize(QWebEngineSettings::FontSize type, int size);
    Q_INVOKABLE bool                                        testAttribute(QWebEngineSettings::WebAttribute attribute) const;
  #if QT_VERSION >= QT_VERSION_CHECK(5,11,0)
    Q_INVOKABLE QWebEngineSettings::UnknownUrlSchemePolicy 	unknownUrlSchemePolicy() const;
  #endif
};

#endif
