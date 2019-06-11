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
#include "qwebengineviewproto.h"

#include <QWebEngineHistory>
#include <QWebEngineSettings>
#include <QWebEngineView>

void setupQWebEngineViewProto(QScriptEngine *engine)
{

  QScriptValue constructor = engine->newFunction(constructQWebEngineView);
  QScriptValue metaObject = engine->newQMetaObject(&QWebEngineView::staticMetaObject, constructor);
  engine->globalObject().setProperty("QWebEngineView", metaObject);

  QScriptValue proto = engine->newQObject(new QWebEngineViewProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QWebEngineView*>(), proto);

}

QScriptValue constructQWebEngineView(QScriptContext * context, QScriptEngine  *engine)
{
  QWebEngineView *obj = nullptr;
  if (context->argumentCount() == 1)
    obj = new QWebEngineView(qobject_cast<QWidget*>(context->argument(0).toQObject()));
  else
    obj = new QWebEngineView();
  return engine->newQObject(obj);
}

QWebEngineViewProto::QWebEngineViewProto(QObject *parent)
    : QObject(parent)
{
}
QWebEngineViewProto::~QWebEngineViewProto()
{
}

void QWebEngineViewProto::findText(const QString &subString, QWebEnginePage::FindFlags options)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->findText(subString, options);
}

QWebEngineHistory* QWebEngineViewProto::history() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->history();
  return nullptr;
}

void QWebEngineViewProto::load(const QUrl &url)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->load(url);
}

QWebEnginePage* QWebEngineViewProto::page() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->page();
  return nullptr;
}

QAction* QWebEngineViewProto::pageAction(QWebEnginePage::WebAction action)  const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->pageAction(action);
  return nullptr;
}

void QWebEngineViewProto::setContent(const QByteArray &data, const QString &mimeType, const QUrl &baseUrl)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->setContent(data, mimeType, baseUrl);
}

void QWebEngineViewProto::setHtml(const QString &html, const QUrl &baseUrl)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->setHtml(html, baseUrl);
}

void QWebEngineViewProto::setPage(QWebEnginePage *page)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->setPage(page);
}

QWebEngineSettings* QWebEngineViewProto::settings() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->settings();
  return nullptr;
}

void QWebEngineViewProto::setZoomFactor(qreal factor)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->setZoomFactor(factor);
}

void QWebEngineViewProto::triggerPageAction(QWebEnginePage::WebAction action, bool checked)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->triggerPageAction(action, checked);
}

qreal QWebEngineViewProto::zoomFactor() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->zoomFactor();
  return qreal();
}

// Reimplemented Public Functions.
QVariant QWebEngineViewProto::inputMethodQuery(Qt::InputMethodQuery property) const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->inputMethodQuery(property);
  return QVariant();
}

QSize QWebEngineViewProto::sizeHint() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->sizeHint();
  return QSize();
}
