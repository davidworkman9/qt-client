/*
* This file is part of the xTuple ERP: PostBooks Edition, a free and
* open source Enterprise Resource Planning software suite,
* Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
* It is licensed to you under the Common Public Attribution License
* version 1.0, the full text of which (including xTuple-specific Exhibits)
* is available at www.xtuple.com/CPAL.  By using this software, you agree
* to be bound by its terms.
*/

#include "qwebengineviewproto.h"

QScriptValue QWebEngineViewtoScriptValue(QScriptEngine *engine, QWebEngineView* const &item)
{
  return engine->newQObject(item);
}

void QWebEngineViewfromScriptValue(const QScriptValue &obj, QWebEngineView* &item)
{
  item = qobject_cast<QWebEngineView*>(obj.toQObject());
}

void setupQWebEngineViewProto(QScriptEngine *engine)
{
  qScriptRegisterMetaType(engine, QWebEngineViewtoScriptValue, QWebEngineViewfromScriptValue);

  QScriptValue proto = engine->newQObject(new QWebEngineViewProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QWebEngineView*>(), proto);

  QScriptValue constructor = engine->newFunction(constructQWebEngineView, proto);
  engine->globalObject().setProperty("QWebEngineView",  constructor);
}

QScriptValue constructQWebEngineView(QScriptContext * context, QScriptEngine  *engine)
{
  QWebEngineView *obj = 0;

  if (context->argumentCount() >= 1)
    obj = new QWebEngineView(qobject_cast<QWidget*>(context->argument(0).toQObject()));
  else
    obj = new QWebEngineView();
  return engine->toScriptValue(obj);
}

QWebEngineViewProto::QWebEngineViewProto(QObject *parent)
    : QObject(parent)
{
}

QWebEngineViewProto::~QWebEngineViewProto()
{}

void QWebEngineViewProto::findText(const QString & subString, QWebEnginePage::FindFlags options)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->findText(subString, options);
}

void QWebEngineViewProto::findText(const QString & subString, QWebEnginePage::FindFlags options, std::function<void(bool)> resultCallback)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->findText(subString, options, resultCallback);
}

bool QWebEngineViewProto::hasSelection() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->hasSelection();
  return false;
}

QWebEngineHistory* QWebEngineViewProto::history() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->history();
  return 0;
}

QIcon QWebEngineViewProto::icon() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->icon();
  return QIcon();
}

QUrl QWebEngineViewProto::iconUrl() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->iconUrl();
  return QUrl();
}

void QWebEngineViewProto::load(const QUrl & url)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->load(url);
}

void QWebEngineViewProto::load(const QWebEngineHttpRequest & request)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->load(request);
}

QWebEnginePage* QWebEngineViewProto::page() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->page();
  return 0;
}

QAction* QWebEngineViewProto::pageAction(QWebEnginePage::WebAction action) const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->pageAction(action);
  return 0;
}

QString QWebEngineViewProto::selectedText() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->selectedText();
  return QString();
}

void QWebEngineViewProto::setContent(const QByteArray & data, const QString & mimeType, const QUrl & baseUrl)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->setContent(data, mimeType, baseUrl);
}

void QWebEngineViewProto::setHtml(const QString & html, const QUrl & baseUrl)
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

void QWebEngineViewProto::setUrl(const QUrl & url)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->setUrl(url);
}

void QWebEngineViewProto::setZoomFactor(double factor)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->setZoomFactor(factor);
}

QWebEngineSettings* QWebEngineViewProto::settings() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->settings();
  return 0;
}

QSize QWebEngineViewProto::sizeHint() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->sizeHint();
  return QSize();
}

QString QWebEngineViewProto::title() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->title();
  return QString();
}

void QWebEngineViewProto::triggerPageAction(QWebEnginePage::WebAction action, bool checked)
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    item->triggerPageAction(action, checked);
}

QUrl QWebEngineViewProto::url() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->url();
  return QUrl();
}

double QWebEngineViewProto::zoomFactor() const
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->zoomFactor();
  return double();
}

void QWebEngineViewProto::back()
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->back();
}

void QWebEngineViewProto::forward()
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->forward();
}

void QWebEngineViewProto::reload()
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->reload();
}

void QWebEngineViewProto::stop()
{
  QWebEngineView *item = qscriptvalue_cast<QWebEngineView*>(thisObject());
  if (item)
    return item->stop();
}
