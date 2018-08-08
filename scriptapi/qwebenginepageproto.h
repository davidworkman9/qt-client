/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __QWEBENGINEPAGEPROTO_H__
#define __QWEBENGINEPAGEPROTO_H__

#include <QWebEnginePage>
#include <QScriptable>
#include <QtScript>
#include <QColor>
#include <QSizeF>
#include <QIcon>
#include <QUrl>
#include <QPointF>
#include <QString>
#include <QObject>
#include <QWebEngineProfile>
#include <QAction>
#include <QSizeF>
#include <QWebEngineContextMenuData>
#include <QWebEngineHistory>
#include <QWebEngineHttpRequest>
#include <QPageLayout>
#include <QPageSize>
#include <QWebEngineDownloadItem>
#include <QWebEngineScriptCollection>
#include <QByteArray>
#include <QWidget>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QEvent>
#include <QAuthenticator>
#include <QWebEngineFullScreenRequest>
#include <QRect>
#include <QVariant>

Q_DECLARE_METATYPE(QWebEnginePage*)
Q_DECLARE_METATYPE(enum QWebEnginePage::Feature)
Q_DECLARE_METATYPE(enum QWebEnginePage::FileSelectionMode)
Q_DECLARE_METATYPE(enum QWebEnginePage::FindFlag)
Q_DECLARE_METATYPE(QWebEnginePage::FindFlags)
Q_DECLARE_METATYPE(enum QWebEnginePage::JavaScriptConsoleMessageLevel)
Q_DECLARE_METATYPE(enum QWebEnginePage::NavigationType)
Q_DECLARE_METATYPE(enum QWebEnginePage::PermissionPolicy)
Q_DECLARE_METATYPE(enum QWebEnginePage::RenderProcessTerminationStatus)
Q_DECLARE_METATYPE(enum QWebEnginePage::WebAction)
Q_DECLARE_METATYPE(enum QWebEnginePage::WebWindowType)

void setupQWebEnginePageProto(QScriptEngine *engine);
QScriptValue constructQWebEnginePage(QScriptContext *context, QScriptEngine *engine);

class QWebEnginePageProto : public QObject, public QScriptable
{
  Q_OBJECT

  Q_PROPERTY (bool audioMuted         READ isAudioMuted       WRITE setAudioMuted)
  Q_PROPERTY (QColor backgroundColor  READ backgroundColor    WRITE setBackgroundColor)
  Q_PROPERTY (QSizeF contentsSize     READ contentsSize)
  Q_PROPERTY (bool hasSelection       READ hasSelection)
  Q_PROPERTY (QIcon icon              READ icon)
  Q_PROPERTY (QUrl iconUrl            READ iconUrl)
  Q_PROPERTY (bool recentlyAudible    READ recentlyAudible)
  Q_PROPERTY (QUrl requestedUrl       READ requestedUrl)
  Q_PROPERTY (QPointF scrollPosition  READ scrollPosition)
  Q_PROPERTY (QString selectedText    READ selectedText)
  Q_PROPERTY (QString title           READ title)
  Q_PROPERTY (QUrl url                READ url                WRITE setUrl)
  Q_PROPERTY (double zoomFactor       READ zoomFactor         WRITE setZoomFactor)

  public:
    QWebEnginePageProto(QObject *parent = 0);
    QWebEnginePageProto(QWebEngineProfile *profile, QObject *parent);
    ~QWebEnginePageProto();

    Q_INVOKABLE QAction *action(QWebEnginePage::WebAction action) const;
    Q_INVOKABLE QColor backgroundColor() const;
    Q_INVOKABLE QSizeF contentsSize() const;
    Q_INVOKABLE const QWebEngineContextMenuData& contextMenuData() const;
    Q_INVOKABLE QMenu *createStandardContextMenu();
    Q_INVOKABLE void download(const QUrl & url, const QString & filename = QString());
    Q_INVOKABLE void findText(const QString & subString, QWebEnginePage::FindFlag options = QWebEnginePage::FindFlag());
    Q_INVOKABLE void findText(const QString & subString, QWebEnginePage::FindFlag options, std::function<void(bool)> resultCallback);
    Q_INVOKABLE bool hasSelection() const;
    Q_INVOKABLE QWebEngineHistory *history() const;
    Q_INVOKABLE QIcon icon() const;
    Q_INVOKABLE QUrl iconUrl() const;
    Q_INVOKABLE bool isAudioMuted() const;
    Q_INVOKABLE void print(QPrinter *printer, std::function<void(bool)> resultCallback);
    Q_INVOKABLE QWebEngineProfile *profile() const;
    Q_INVOKABLE bool recentlyAudible() const;
    Q_INVOKABLE void replaceMisspelledWord(const QString & replacement);
    Q_INVOKABLE QUrl requestedUrl() const;
    Q_INVOKABLE void save(const QString & filePath, QWebEngineDownloadItem::SavePageFormat format = QWebEngineDownloadItem::MimeHtmlSaveFormat) const;
    Q_INVOKABLE QWebEngineScriptCollection& scripts();
    Q_INVOKABLE QPointF scrollPosition() const;
    Q_INVOKABLE QString selectedText() const;
    Q_INVOKABLE void setAudioMuted(bool muted);
    Q_INVOKABLE void setBackgroundColor(const QColor & color);
    Q_INVOKABLE void setContent(const QByteArray & data, const QString & mimeType = QString(), const QUrl & baseUrl = QUrl());
    Q_INVOKABLE void setFeaturePermission(const QUrl & securityOrigin, QWebEnginePage::Feature feature, QWebEnginePage::PermissionPolicy policy);
    Q_INVOKABLE void setHtml(const QString & html, const QUrl & baseUrl = QUrl());
    Q_INVOKABLE void setUrl(const QUrl & url);
    Q_INVOKABLE void setView(QWidget * view);
    Q_INVOKABLE void setWebChannel(QWebChannel * channel, int worldId);
    Q_INVOKABLE void setWebChannel(QWebChannel * channel);
    Q_INVOKABLE void setZoomFactor(qreal factor);
    Q_INVOKABLE QWebEngineSettings *settings() const;
    Q_INVOKABLE QString title() const;
    Q_INVOKABLE void toHtml(std::function<void(QString)> resultCallback) const;
    Q_INVOKABLE void toPlainText(std::function<void(QString)> resultCallback) const;
    Q_INVOKABLE virtual void triggerAction(QWebEnginePage::WebAction action, bool checked = false);
    Q_INVOKABLE QUrl url() const;
    Q_INVOKABLE QWidget *view() const;
    Q_INVOKABLE QWebChannel *webChannel() const;
    Q_INVOKABLE qreal zoomFactor() const;

    // Reimplemented Public Functions
    Q_INVOKABLE bool event(QEvent * ev);

    signals:
      void audioMutedChanged(bool muted);
      void authenticationRequired(const QUrl & requestUrl, QAuthenticator * authenticator);
      void contentsSizeChanged(const QSizeF & size);
      void featurePermissionRequestCanceled(const QUrl & securityOrigin, QWebEnginePage::Feature feature);
      void featurePermissionRequested(const QUrl & securityOrigin, QWebEnginePage::Feature feature);
      void fullScreenRequested(QWebEngineFullScreenRequest request);
      void geometryChangeRequested(const QRect & geom);
      void iconChanged(const QIcon & icon);
      void iconUrlChanged(const QUrl & url);
      void linkHovered(const QString & url);
      void loadFinished(bool ok);
      void loadProgress(int progess);
      void loadStarted();
      void pdfPrintingFinished(const QString & filePath, bool success);
      void proxyAuthenticationRequired(const QUrl & requestUrl, QAuthenticator * authenticator, const QString & proxyHost);
      void recentlyAudibleChanged(bool recentlyAudible);
      void renderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode);
      void scrollPositionChanged(const QPointF & position);
      void selectionChagned();
      void titleChanged(const QString & title);
      void urlChanged(const QUrl & url);
      void windowCloseRequested();
};

#endif
