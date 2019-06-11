/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which(including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __QWEBENGINEVIEWPROTO_H__
#define __QWEBENGINEVIEWPROTO_H__

#include <QIcon>
#include <QObject>
#include <QString>
#include <QtScript>
#include <QUrl>
#include <QWebEngineView>
#include <QWebEnginePage>

Q_DECLARE_METATYPE(QWebEngineView*)

void setupQWebEngineViewProto(QScriptEngine *engine);
QScriptValue constructQWebEngineView(QScriptContext *context, QScriptEngine *engine);

class QWebEngineViewProto : public QObject, public QScriptable
{
  Q_OBJECT

  public:
    QWebEngineViewProto(QObject *parent);
    virtual ~QWebEngineViewProto();

    Q_INVOKABLE void                    findText(const QString &subString, QWebEnginePage::FindFlags options = nullptr);
    Q_INVOKABLE QWebEngineHistory      *history() const;
    Q_INVOKABLE void                    load(const QUrl &url);
    //Q_INVOKABLE void                  load(const QWebEngineHttpRequest &request); TODO: Expose QWebEngineHttpRequest
    Q_INVOKABLE QWebEnginePage         *page() const;
    Q_INVOKABLE QAction                *pageAction(QWebEnginePage::WebAction action) const;
    Q_INVOKABLE void                    setContent(const QByteArray &data, const QString &mimeType = QString(), const QUrl &baseUrl = QUrl());
    Q_INVOKABLE void                    setHtml(const QString &html, const QUrl &baseUrl = QUrl());
    Q_INVOKABLE void                    setPage(QWebEnginePage *page);
    Q_INVOKABLE QWebEngineSettings     *settings() const;
    Q_INVOKABLE void                    setZoomFactor(qreal factor);
    Q_INVOKABLE void                    triggerPageAction(QWebEnginePage::WebAction action, bool checked = false);
    Q_INVOKABLE qreal                   zoomFactor() const;

  // Reimplemented Public Functions.
    Q_INVOKABLE QVariant                inputMethodQuery(Qt::InputMethodQuery property) const;
    Q_INVOKABLE QSize                   sizeHint() const;

};

#endif
