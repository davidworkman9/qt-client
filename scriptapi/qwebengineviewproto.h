/*
* This file is part of the xTuple ERP: PostBooks Edition, a free and
* open source Enterprise Resource Planning software suite,
* Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
* It is licensed to you under the Common Public Attribution License
* version 1.0, the full text of which (including xTuple-specific Exhibits)
* is available at www.xtuple.com/CPAL.  By using this software, you agree
* to be bound by its terms.
*/

#ifndef __QWEBENGINEVIEWPROTO_H__
#define __QWEBENGINEVIEWPROTO_H__

#include <QWebEngineView>
#include <QScriptable>
#include <QObject>
#include <QScriptEngine>
#include <QIcon>
#include <QUrl>
#include <QString>
#include <QWidget>
#include <QWebEnginePage>
#include <QWebEngineHistory>
#include <QByteArray>
#include <QWebEngineSettings>

Q_DECLARE_METATYPE(QWebEngineView*)

void setupQWebEngineViewProto(QScriptEngine *engine);
QScriptValue constructQWebEngineView(QScriptContext *context, QScriptEngine *engine);

class QWebEngineViewProto : public QObject, public QScriptable
{
  Q_OBJECT

  Q_PROPERTY (const bool hasSelection       READ hasSelection)
  Q_PROPERTY (const QIcon icon              READ icon)
  Q_PROPERTY (const QUrl iconUrl            READ iconUrl)
  Q_PROPERTY (const QString selectedText    READ selectedText)
  Q_PROPERTY (const QString title           READ title)
  Q_PROPERTY (QUrl url                      READ url            WRITE setUrl)
  Q_PROPERTY (double zoomFactor             READ zoomFactor     WRITE setZoomFactor)

  public:
    QWebEngineViewProto(QObject *parent = 0);
    ~QWebEngineViewProto();

    Q_INVOKABLE void findText(const QString & subString, QWebEnginePage::FindFlags options = QWebEnginePage::FindFlags());
    Q_INVOKABLE void findText(const QString & subString, QWebEnginePage::FindFlags options, std::function<void(bool)> resultCallback);
    Q_INVOKABLE bool hasSelection() const;
    Q_INVOKABLE QWebEngineHistory *history() const;
    Q_INVOKABLE QIcon icon() const;
    Q_INVOKABLE QUrl iconUrl() const;
    Q_INVOKABLE void load(const QUrl & url);
    Q_INVOKABLE void load(const QWebEngineHttpRequest & request);
    Q_INVOKABLE QWebEnginePage *page() const;
    Q_INVOKABLE QAction *pageAction(QWebEnginePage::WebAction action) const;
    Q_INVOKABLE QString selectedText() const;
    Q_INVOKABLE void setContent(const QByteArray & data, const QString & mimeType = QString(), const QUrl & baseUrl = QUrl());
    Q_INVOKABLE void setHtml(const QString & html, const QUrl & baseUrl = QUrl());
    Q_INVOKABLE void setPage(QWebEnginePage *page);
    Q_INVOKABLE void setUrl(const QUrl & url);
    Q_INVOKABLE void setZoomFactor(double factor);
    Q_INVOKABLE QWebEngineSettings *settings() const;
    Q_INVOKABLE QString title() const;
    Q_INVOKABLE void triggerPageAction(QWebEnginePage::WebAction action, bool checked = false);
    Q_INVOKABLE QUrl url() const;
    Q_INVOKABLE double zoomFactor() const;

    // Reimplemented Public Functions
    Q_INVOKABLE virtual QSize sizeHint() const;

  public slots:
    void back();
    void forward();
    void reload();
    void stop();

  signals:
    void iconChanged(const QIcon & icon);
    void iconUrlChanged(const QUrl & url);
    void loadFinished(bool ok);
    void loadProgress(int progress);
    void loadStarted();
    void renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationSatus, int exitCode);
    void selectionChagned();
    void titleChanged(const QString & title);
    void urlChanged(const QUrl & url);
};

#endif
