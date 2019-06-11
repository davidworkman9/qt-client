/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which(including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __QWEBENGINEPAGEPROTO_H__
#define __QWEBENGINEPAGEPROTO_H__

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QString>
#include <QSizeF>
#include <QStringList>
#include <QtScript>
#include <QUndoStack>
#include <QVariant>
#include <QWebEnginePage>
#include <QWebEngineHistory>
#include <QWebEngineSettings>
#include <QWebEngineDownloadItem>
#include <QWidget>

Q_DECLARE_METATYPE(QWebEnginePage*)

void setupQWebEnginePageProto(QScriptEngine *engine);
QScriptValue constructQWebEnginePage(QScriptContext *context, QScriptEngine *engine);

class QWebEnginePageProto : public QObject, public QScriptable
{
  Q_OBJECT

  public:
    QWebEnginePageProto(QObject *parent);
    ~QWebEnginePageProto();

    Q_INVOKABLE QAction                        *action(QWebEnginePage::WebAction action) const;
    Q_INVOKABLE QMenu                          *createStandardContextMenu();
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
    Q_INVOKABLE void                            download(const QUrl &url, const QString &filename = QString());
#endif
    Q_INVOKABLE QWebEngineHistory              *history() const;
    Q_INVOKABLE void                            load(const QUrl &url);
    //Q_INVOKABLE void                          load(const QWebEngineHttpRequest &request); TODO: Expose QWebEngineHttpRequest
    Q_INVOKABLE void                            print(QPrinter *printer, const QWebEngineCallback<bool> &resultCallback);
    Q_INVOKABLE void                            printToPdf(const QString &filePath, const QPageLayout &pageLayout = QPageLayout(QPageSize(QPageSize::Letter), QPageLayout::Portrait, QMarginsF()));
    Q_INVOKABLE void                            printToPdf(const QWebEngineCallback<const QByteArray &> &resultCallback, const QPageLayout &pageLayout = QPageLayout(QPageSize(QPageSize::Letter), QPageLayout::Portrait, QMarginsF()));
    Q_INVOKABLE QWebEngineProfile              *profile() const;
    Q_INVOKABLE void 	                        replaceMisspelledWord(const QString &replacement);
    Q_INVOKABLE void                            runJavaScript(const QString &scriptSource);
    Q_INVOKABLE void                            runJavaScript(const QString &scriptSource, quint32 worldId);
    Q_INVOKABLE void                            runJavaScript(const QString &scriptSource, const QWebEngineCallback<const QVariant &> &resultCallback);
    Q_INVOKABLE void                            runJavaScript(const QString &scriptSource, quint32 worldId, const QWebEngineCallback<const QVariant &> &resultCallback);
    Q_INVOKABLE void                            save(const QString &filePath, QWebEngineDownloadItem::SavePageFormat format = QWebEngineDownloadItem::MimeHtmlSaveFormat) const;
    Q_INVOKABLE void                            setContent(const QByteArray &data, const QString &mimeType = QString(), const QUrl &baseUrl = QUrl());
    Q_INVOKABLE void                            setFeaturePermission(const QUrl &securityOrigin, QWebEnginePage::Feature feature, QWebEnginePage::PermissionPolicy policy);
    Q_INVOKABLE void                            setHtml(const QString &html, const QUrl &baseUrl = QUrl());
    Q_INVOKABLE void                            setView(QWidget * view);
    Q_INVOKABLE void                            setWebChannel(QWebChannel *channel, uint worldId);
    Q_INVOKABLE void                            setWebChannel(QWebChannel *channel);
    Q_INVOKABLE QWebEngineSettings             *settings() const;
    Q_INVOKABLE void                            toHtml(const QWebEngineCallback<const QString &> &resultCallback) const;
    Q_INVOKABLE void                            toPlainText(const QWebEngineCallback<const QString &> &resultCallback) const;
    Q_INVOKABLE virtual void                    triggerAction(QWebEnginePage::WebAction action, bool checked = false);
    Q_INVOKABLE QWidget                        *view() const;
    Q_INVOKABLE QWebChannel                    *webChannel() const;
#if QT_VERSION >= QT_VERSION_CHECK(5,11,0)
    Q_INVOKABLE QWebEnginePage                 *devToolsPage() const;
    Q_INVOKABLE QWebEnginePage                 *inspectedPage() const;
    Q_INVOKABLE void                            setDevToolsPage(QWebEnginePage *devToolsPage);
    Q_INVOKABLE void                            setInspectedPage(QWebEnginePage *page);
#endif
    //Reimplemented Public Functions
    virtual bool 	event(QEvent *e);

};

#endif
