/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which(including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "scriptapi_internal.h"
#include "qwebenginepageproto.h"

#include <QMenu>
#include <QPageLayout>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QUndoStack>
#include <QVariant>
#include <QWebEnginePage>
#include <QWidget>

void setupQWebEnginePageProto(QScriptEngine *engine)
{

  QScriptValue constructor = engine->newFunction(constructQWebEnginePage);
  QScriptValue metaObject = engine->newQMetaObject(&QWebEnginePage::staticMetaObject, constructor);
  engine->globalObject().setProperty("QWebEnginePage",  metaObject);

  QScriptValue proto = engine->newQObject(new QWebEnginePageProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QWebEnginePage*>(), proto);

}

QScriptValue constructQWebEnginePage(QScriptContext * context, QScriptEngine  *engine)
{
  QWebEnginePage *obj = nullptr;
  if (context->argumentCount() == 1)
    obj = new QWebEnginePage(qobject_cast<QWidget*>(context->argument(0).toQObject()));
  else
    obj = new QWebEnginePage();
  return engine->newQObject(obj);
}

QWebEnginePageProto::QWebEnginePageProto(QObject *parent)
    : QObject(parent)
{
}
QWebEnginePageProto::~QWebEnginePageProto()
{
}

QAction* QWebEnginePageProto::action(QWebEnginePage::WebAction action) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->action(action);
  return nullptr;
}

QMenu* QWebEnginePageProto::createStandardContextMenu()
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->createStandardContextMenu();
  return nullptr;
}

#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
void QWebEnginePageProto::download(const QUrl &url, const QString &filename)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->download(url, filename);
}
#endif

QWebEngineHistory* QWebEnginePageProto::history() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->history();
  return nullptr;
}

void QWebEnginePageProto::load(const QUrl &url)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->load(url);
}

void QWebEnginePageProto::print(QPrinter *printer, const QWebEngineCallback<bool> &resultCallback)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->print(printer, resultCallback);
}

void QWebEnginePageProto::printToPdf(const QString &filePath, const QPageLayout &pageLayout)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
      item->printToPdf(filePath, pageLayout);
}

void QWebEnginePageProto::printToPdf(const QWebEngineCallback<const QByteArray &> &resultCallback, const QPageLayout &pageLayout)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
      item->printToPdf(resultCallback, pageLayout);
}

QWebEngineProfile* QWebEnginePageProto::profile() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->profile();
  return nullptr;
}

void QWebEnginePageProto::replaceMisspelledWord(const QString &replacement)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->replaceMisspelledWord(replacement);
}

void QWebEnginePageProto::runJavaScript(const QString &scriptSource)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->runJavaScript(scriptSource);
}

void QWebEnginePageProto::runJavaScript(const QString &scriptSource, quint32 worldId)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->runJavaScript(scriptSource, worldId);
}

void QWebEnginePageProto::runJavaScript(const QString &scriptSource, const QWebEngineCallback<const QVariant &> &resultCallback)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->runJavaScript(scriptSource, resultCallback);
}

void QWebEnginePageProto::runJavaScript(const QString &scriptSource, quint32 worldId, const QWebEngineCallback<const QVariant &> &resultCallback)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->runJavaScript(scriptSource, worldId, resultCallback);
}

void QWebEnginePageProto::save(const QString &filePath, QWebEngineDownloadItem::SavePageFormat format) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->save(filePath, format);
}

void QWebEnginePageProto::setContent(const QByteArray &data, const QString &mimeType, const QUrl &baseUrl)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setContent(data, mimeType, baseUrl);
}

void QWebEnginePageProto::setFeaturePermission(const QUrl &securityOrigin, QWebEnginePage::Feature feature, QWebEnginePage::PermissionPolicy policy)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setFeaturePermission(securityOrigin, feature, policy);
}

void QWebEnginePageProto::setHtml(const QString &html, const QUrl &baseUrl)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setHtml(html, baseUrl);
}

void QWebEnginePageProto::setView(QWidget * view)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setView(view);
}

void QWebEnginePageProto::setWebChannel(QWebChannel *channel, uint worldId)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setWebChannel(channel, worldId);
}

void QWebEnginePageProto::setWebChannel(QWebChannel *channel)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setWebChannel(channel);
}

QWebEngineSettings* QWebEnginePageProto::settings() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->settings();
  return nullptr;
}

void QWebEnginePageProto::toHtml(const QWebEngineCallback<const QString &> &resultCallback) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->toHtml(resultCallback);
}

void QWebEnginePageProto::toPlainText(const QWebEngineCallback<const QString &> &resultCallback) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->toPlainText(resultCallback);
}

void QWebEnginePageProto::triggerAction(QWebEnginePage::WebAction action, bool checked)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->triggerAction(action, checked);
}

QWidget* QWebEnginePageProto::view() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->view();
  return nullptr;
}

QWebChannel* QWebEnginePageProto::webChannel() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->webChannel();
  return nullptr;
}

#if QT_VERSION >= QT_VERSION_CHECK(5,11,0)
QWebEnginePage* QWebEnginePageProto::devToolsPage() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->devToolsPage();
  return nullptr;
}

QWebEnginePage* QWebEnginePageProto::inspectedPage() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->inspectedPage();
  return nullptr;
}

void QWebEnginePageProto::setDevToolsPage(QWebEnginePage *devToolsPage)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setDevToolsPage(devToolsPage);
}

void QWebEnginePageProto::setInspectedPage(QWebEnginePage *page)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setInspectedPage(page);
}
#endif

bool QWebEnginePageProto::event(QEvent *e)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->event(e);
  return false;
}
