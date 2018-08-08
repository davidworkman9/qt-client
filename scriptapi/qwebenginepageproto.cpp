/*
* This file is part of the xTuple ERP: PostBooks Edition, a free and
* open source Enterprise Resource Planning software suite,
* Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
* It is licensed to you under the Common Public Attribution License
* version 1.0, the full text of which (including xTuple-specific Exhibits)
* is available at www.xtuple.com/CPAL.  By using this software, you agree
* to be bound by its terms.
*/

#include "scriptapi_internal.h"
#include "qwebenginepageproto.h"

#include <QString>

QScriptValue QWebEnginePagetoScriptValue(QScriptEngine *engine, QWebEnginePage* const &item)
{
  return engine->newQObject(item);
}

void QWebEnginePagefromScriptValue(const QScriptValue &obj, QWebEnginePage* &item)
{
  item = qobject_cast<QWebEnginePage*>(obj.toQObject());
}

QScriptValue FeatureToScriptValue(QScriptEngine *engine, const enum QWebEnginePage::Feature &p)
{
    return QScriptValue(engine, (int)p);
}

void FeatureFromScriptValue(const QScriptValue &obj, enum QWebEnginePage::Feature &p)
{
    p = (enum QWebEnginePage::Feature)obj.toInt32();
}

QScriptValue FileSelectionModeToScriptValue(QScriptEngine *engine, const enum QWebEnginePage::FileSelectionMode &p)
{
    return QScriptValue(engine, (int)p);
}

void FileSelectionModeFromScriptValue(const QScriptValue &obj, enum QWebEnginePage::FileSelectionMode &p)
{
    p = (enum QWebEnginePage::FileSelectionMode)obj.toInt32();
}

QScriptValue FindFlagToScriptValue(QScriptEngine *engine, const enum QWebEnginePage::FindFlag &p)
{
    return QScriptValue(engine, (int)p);
}

void FindFlagFromScriptValue(const QScriptValue &obj, enum QWebEnginePage::FindFlag &p)
{
    p = (enum QWebEnginePage::FindFlag)obj.toInt32();
}

QScriptValue JavaScriptConsoleMessageLevelToScriptValue(QScriptEngine *engine, const enum QWebEnginePage::JavaScriptConsoleMessageLevel &p)
{
    return QScriptValue(engine, (int)p);
}

void JavaScriptConsoleMessageLevelFromScriptValue(const QScriptValue &obj, enum QWebEnginePage::JavaScriptConsoleMessageLevel &p)
{
    p = (enum QWebEnginePage::JavaScriptConsoleMessageLevel)obj.toInt32();
}

QScriptValue NavigationTypeToScriptValue(QScriptEngine *engine, const enum QWebEnginePage::NavigationType &p)
{
    return QScriptValue(engine, (int)p);
}

void NavigationTypeFromScriptValue(const QScriptValue &obj, enum QWebEnginePage::NavigationType &p)
{
    p = (enum QWebEnginePage::NavigationType)obj.toInt32();
}

QScriptValue PermissionPolicyToScriptValue(QScriptEngine *engine, const enum QWebEnginePage::PermissionPolicy &p)
{
    return QScriptValue(engine, (int)p);
}

void PermissionPolicyFromScriptValue(const QScriptValue &obj, enum QWebEnginePage::PermissionPolicy &p)
{
    p = (enum QWebEnginePage::PermissionPolicy)obj.toInt32();
}

QScriptValue RenderProcessTerminationStatusToScriptValue(QScriptEngine *engine, const enum QWebEnginePage::RenderProcessTerminationStatus &p)
{
    return QScriptValue(engine, (int)p);
}

void RenderProcessTerminationStatusFromScriptValue(const QScriptValue &obj, enum QWebEnginePage::RenderProcessTerminationStatus &p)
{
    p = (enum QWebEnginePage::RenderProcessTerminationStatus)obj.toInt32();
}

QScriptValue WebActionToScriptValue(QScriptEngine *engine, const enum QWebEnginePage::WebAction &p)
{
    return QScriptValue(engine, (int)p);
}

void WebActionFromScriptValue(const QScriptValue &obj, enum QWebEnginePage::WebAction &p)
{
    p = (enum QWebEnginePage::WebAction)obj.toInt32();
}

QScriptValue WebWindowTypeToScriptValue(QScriptEngine *engine, const enum QWebEnginePage::WebWindowType &p)
{
    return QScriptValue(engine, (int)p);
}

void WebWindowTypeFromScriptValue(const QScriptValue &obj, enum QWebEnginePage::WebWindowType &p)
{
    p = (enum QWebEnginePage::WebWindowType)obj.toInt32();
}

QScriptValue runJavaScriptForJs(QScriptContext* context, QScriptEngine* engine)
{
    QScriptValue result = engine->undefinedValue();
    QWebEnginePage qwep;

    if (context->argumentCount() == 1) {
        if (context->argument(0).isString())
            qwep.runJavaScript(context->argument(0).toString());
        else
           qFatal("No matching runJavaScript function");
    }

    if (context->argumentCount() == 2) {
        if (context->argument(0).isString() && context->argument(1).isNumber()) {
            qwep.runJavaScript(context->argument(0).toString(), context->argument(1).toInt32());
        } else if (context->argument(0).isString() && context->argument(1).isFunction()) {
            qwep.runJavaScript(context->argument(0).toString());
            qWarning() << "Lambdas and Callbacks are not currently supported";
        } else {
            qFatal("No matching runJavaScript function");
        }
    }

    if (context->argumentCount() == 3) {
        if (context->argument(0).isString() && context->argument(1).isNumber() && context->argument(2).isFunction()) {
            qwep.runJavaScript(context->argument(0).toString(), context->argument(1).toInt32());
            qWarning() << "Lambdas and Callbacks are not currently supported";
        } else {
            qFatal("No matching runJavaScript function");
        }
    }

    if (context->argumentCount() > 3) {
        qFatal("No matching runJavaScript function");
    }

    return result;
}

QScriptValue printToPdfForJs(QScriptContext* context, QScriptEngine* engine)
{
   qFatal("Function printToPdf not yet supported");
   return engine->undefinedValue();
}

QScriptValue loadForJs(QScriptContext* context, QScriptEngine* engine)
{
    qFatal("Function load not yet supported");
    return engine->undefinedValue();
}

void setupQWebEnginePageProto(QScriptEngine *engine)
{
  qScriptRegisterMetaType(engine, QWebEnginePagetoScriptValue, QWebEnginePagefromScriptValue);

  QScriptValue proto = engine->newQObject(new QWebEnginePageProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QWebEnginePage*>(), proto);

  QScriptValue constructor = engine->newFunction(constructQWebEnginePage, proto);
  engine->globalObject().setProperty("QWebEnginePage",  constructor);

  qScriptRegisterMetaType(engine, FeatureToScriptValue, FeatureFromScriptValue);
  constructor.setProperty("Geolocation", QScriptValue(engine, QWebEnginePage::Geolocation), ENUMPROPFLAGS);
  constructor.setProperty("MediaAudioCapture", QScriptValue(engine, QWebEnginePage::MediaAudioCapture), ENUMPROPFLAGS);
  constructor.setProperty("MediaVideoCapture", QScriptValue(engine, QWebEnginePage::MediaVideoCapture), ENUMPROPFLAGS);
  constructor.setProperty("MediaAudioVideoCapture", QScriptValue(engine, QWebEnginePage::MediaAudioVideoCapture), ENUMPROPFLAGS);
  constructor.setProperty("MouseLock", QScriptValue(engine, QWebEnginePage::MouseLock), ENUMPROPFLAGS);
  constructor.setProperty("DesktopVideoCapture", QScriptValue(engine, QWebEnginePage::DesktopVideoCapture), ENUMPROPFLAGS);
  constructor.setProperty("DesktopAudioVideoCapture", QScriptValue(engine, QWebEnginePage::DesktopAudioVideoCapture), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, FileSelectionModeToScriptValue, FileSelectionModeFromScriptValue);
  constructor.setProperty("FileSelectOpen", QScriptValue(engine, QWebEnginePage::FileSelectOpen), ENUMPROPFLAGS);
  constructor.setProperty("FileSelectOpenMultiple", QScriptValue(engine, QWebEnginePage::FileSelectOpenMultiple), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, FindFlagToScriptValue, FindFlagFromScriptValue);
  constructor.setProperty("FindBackward", QScriptValue(engine, QWebEnginePage::FindBackward), ENUMPROPFLAGS);
  constructor.setProperty("FindCaseSensitively", QScriptValue(engine, QWebEnginePage::FindCaseSensitively), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, JavaScriptConsoleMessageLevelToScriptValue, JavaScriptConsoleMessageLevelFromScriptValue);
  constructor.setProperty("InfoMessageLevel", QScriptValue(engine, QWebEnginePage::InfoMessageLevel), ENUMPROPFLAGS);
  constructor.setProperty("WarningMessageLevel", QScriptValue(engine, QWebEnginePage::WarningMessageLevel), ENUMPROPFLAGS);
  constructor.setProperty("ErrorMessageLevel", QScriptValue(engine, QWebEnginePage::ErrorMessageLevel), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, NavigationTypeToScriptValue, NavigationTypeFromScriptValue);
  constructor.setProperty("NavigationTypeLinkClicked", QScriptValue(engine, QWebEnginePage::NavigationTypeLinkClicked), ENUMPROPFLAGS);
  constructor.setProperty("NavigationTypeTyped", QScriptValue(engine, QWebEnginePage::NavigationTypeTyped), ENUMPROPFLAGS);
  constructor.setProperty("NavigationTypeFormSubmitted", QScriptValue(engine, QWebEnginePage::NavigationTypeFormSubmitted), ENUMPROPFLAGS);
  constructor.setProperty("NavigationTypeBackForward", QScriptValue(engine, QWebEnginePage::NavigationTypeBackForward), ENUMPROPFLAGS);
  constructor.setProperty("NavigationTypeReload", QScriptValue(engine, QWebEnginePage::NavigationTypeReload), ENUMPROPFLAGS);
  constructor.setProperty("NavigationTypeOther", QScriptValue(engine, QWebEnginePage::NavigationTypeOther), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, PermissionPolicyToScriptValue, PermissionPolicyFromScriptValue);
  constructor.setProperty("PermissionUnknown", QScriptValue(engine, QWebEnginePage::PermissionUnknown), ENUMPROPFLAGS);
  constructor.setProperty("PermissionGrantedByUser", QScriptValue(engine, QWebEnginePage::PermissionGrantedByUser), ENUMPROPFLAGS);
  constructor.setProperty("PermissionDeniedByUser", QScriptValue(engine, QWebEnginePage::PermissionDeniedByUser), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, RenderProcessTerminationStatusToScriptValue, RenderProcessTerminationStatusFromScriptValue);
  constructor.setProperty("NormalTerminationStatus", QScriptValue(engine, QWebEnginePage::NormalTerminationStatus), ENUMPROPFLAGS);
  constructor.setProperty("AbnormalTerminationStatus", QScriptValue(engine, QWebEnginePage::AbnormalTerminationStatus), ENUMPROPFLAGS);
  constructor.setProperty("CrashedTerminationStatus", QScriptValue(engine, QWebEnginePage::CrashedTerminationStatus), ENUMPROPFLAGS);
  constructor.setProperty("KilledTerminationStatus", QScriptValue(engine, QWebEnginePage::KilledTerminationStatus), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, WebActionToScriptValue, WebActionFromScriptValue);
  constructor.setProperty("NoWebAction", QScriptValue(engine, QWebEnginePage::NoWebAction), ENUMPROPFLAGS);
  constructor.setProperty("Back", QScriptValue(engine, QWebEnginePage::Back), ENUMPROPFLAGS);
  constructor.setProperty("Forward", QScriptValue(engine, QWebEnginePage::Forward), ENUMPROPFLAGS);
  constructor.setProperty("Stop", QScriptValue(engine, QWebEnginePage::Stop), ENUMPROPFLAGS);
  constructor.setProperty("Reload", QScriptValue(engine, QWebEnginePage::Reload), ENUMPROPFLAGS);
  constructor.setProperty("ReloadAndBypassCache", QScriptValue(engine, QWebEnginePage::ReloadAndBypassCache), ENUMPROPFLAGS);
  constructor.setProperty("Cut", QScriptValue(engine, QWebEnginePage::Cut), ENUMPROPFLAGS);
  constructor.setProperty("Copy", QScriptValue(engine, QWebEnginePage::Copy), ENUMPROPFLAGS);
  constructor.setProperty("Paste", QScriptValue(engine, QWebEnginePage::Paste), ENUMPROPFLAGS);
  constructor.setProperty("Undo", QScriptValue(engine, QWebEnginePage::Undo), ENUMPROPFLAGS);
  constructor.setProperty("Redo", QScriptValue(engine, QWebEnginePage::Redo), ENUMPROPFLAGS);
  constructor.setProperty("SelectAll", QScriptValue(engine, QWebEnginePage::SelectAll), ENUMPROPFLAGS);
  constructor.setProperty("PasteAndMatchStyle", QScriptValue(engine, QWebEnginePage::PasteAndMatchStyle), ENUMPROPFLAGS);
  constructor.setProperty("OpenLinkInThisWindow", QScriptValue(engine, QWebEnginePage::OpenLinkInThisWindow), ENUMPROPFLAGS);
  constructor.setProperty("OpenLinkInNewWindow", QScriptValue(engine, QWebEnginePage::OpenLinkInNewWindow), ENUMPROPFLAGS);
  constructor.setProperty("OpenLinkInNewTab", QScriptValue(engine, QWebEnginePage::OpenLinkInNewTab), ENUMPROPFLAGS);
  constructor.setProperty("OpenLinkInNewBackgroundTab", QScriptValue(engine, QWebEnginePage::OpenLinkInNewBackgroundTab), ENUMPROPFLAGS);
  constructor.setProperty("CopyLinkToClipboard", QScriptValue(engine, QWebEnginePage::CopyLinkToClipboard), ENUMPROPFLAGS);
  constructor.setProperty("CopyImageToClipboard", QScriptValue(engine, QWebEnginePage::CopyImageToClipboard), ENUMPROPFLAGS);
  constructor.setProperty("CopyImageUrlToClipboard", QScriptValue(engine, QWebEnginePage::CopyImageUrlToClipboard), ENUMPROPFLAGS);
  constructor.setProperty("CopyMediaUrlToClipboard", QScriptValue(engine, QWebEnginePage::CopyMediaUrlToClipboard), ENUMPROPFLAGS);
  constructor.setProperty("ToggleMediaControls", QScriptValue(engine, QWebEnginePage::ToggleMediaControls), ENUMPROPFLAGS);
  constructor.setProperty("ToggleMediaLoop", QScriptValue(engine, QWebEnginePage::ToggleMediaLoop), ENUMPROPFLAGS);
  constructor.setProperty("ToggleMediaPlayPause", QScriptValue(engine, QWebEnginePage::ToggleMediaPlayPause), ENUMPROPFLAGS);
  constructor.setProperty("ToggleMediaMute", QScriptValue(engine, QWebEnginePage::ToggleMediaMute), ENUMPROPFLAGS);
  constructor.setProperty("DownloadLinkToDisk", QScriptValue(engine, QWebEnginePage::DownloadLinkToDisk), ENUMPROPFLAGS);
  constructor.setProperty("DownloadImageToDisk", QScriptValue(engine, QWebEnginePage::DownloadImageToDisk), ENUMPROPFLAGS);
  constructor.setProperty("DownloadMediaToDisk", QScriptValue(engine, QWebEnginePage::DownloadMediaToDisk), ENUMPROPFLAGS);
  constructor.setProperty("InspectElement", QScriptValue(engine, QWebEnginePage::InspectElement), ENUMPROPFLAGS);
  constructor.setProperty("ExitFullScreen", QScriptValue(engine, QWebEnginePage::ExitFullScreen), ENUMPROPFLAGS);
  constructor.setProperty("RequestClose", QScriptValue(engine, QWebEnginePage::RequestClose), ENUMPROPFLAGS);
  constructor.setProperty("Unselect", QScriptValue(engine, QWebEnginePage::Unselect), ENUMPROPFLAGS);
  constructor.setProperty("SavePage", QScriptValue(engine, QWebEnginePage::SavePage), ENUMPROPFLAGS);
  constructor.setProperty("ViewSource", QScriptValue(engine, QWebEnginePage::ViewSource), ENUMPROPFLAGS);
  constructor.setProperty("ToggleBold", QScriptValue(engine, QWebEnginePage::ToggleBold), ENUMPROPFLAGS);
  constructor.setProperty("ToggleItalic", QScriptValue(engine, QWebEnginePage::ToggleItalic), ENUMPROPFLAGS);
  constructor.setProperty("ToggleUnderline", QScriptValue(engine, QWebEnginePage::ToggleUnderline), ENUMPROPFLAGS);
  constructor.setProperty("ToggleStrikethrough", QScriptValue(engine, QWebEnginePage::ToggleStrikethrough), ENUMPROPFLAGS);
  constructor.setProperty("AlignLeft", QScriptValue(engine, QWebEnginePage::AlignLeft), ENUMPROPFLAGS);
  constructor.setProperty("AlignCenter", QScriptValue(engine, QWebEnginePage::AlignCenter), ENUMPROPFLAGS);
  constructor.setProperty("AlignRight", QScriptValue(engine, QWebEnginePage::AlignRight), ENUMPROPFLAGS);
  constructor.setProperty("AlignJustified", QScriptValue(engine, QWebEnginePage::AlignJustified), ENUMPROPFLAGS);
  constructor.setProperty("Indent", QScriptValue(engine, QWebEnginePage::Indent), ENUMPROPFLAGS);
  constructor.setProperty("Outdent", QScriptValue(engine, QWebEnginePage::Outdent), ENUMPROPFLAGS);
  constructor.setProperty("InsertOrderedList", QScriptValue(engine, QWebEnginePage::InsertOrderedList), ENUMPROPFLAGS);
  constructor.setProperty("InsertUnorderedList", QScriptValue(engine, QWebEnginePage::InsertUnorderedList), ENUMPROPFLAGS);

  qScriptRegisterMetaType(engine, WebWindowTypeToScriptValue, WebWindowTypeFromScriptValue);
  constructor.setProperty("WebBrowserWindow", QScriptValue(engine, QWebEnginePage::WebBrowserWindow), ENUMPROPFLAGS);
  constructor.setProperty("WebBrowserTab", QScriptValue(engine, QWebEnginePage::WebBrowserTab), ENUMPROPFLAGS);
  constructor.setProperty("WebDialog", QScriptValue(engine, QWebEnginePage::WebDialog), ENUMPROPFLAGS);
  constructor.setProperty("WebBrowserBackgroundTab", QScriptValue(engine, QWebEnginePage::WebBrowserBackgroundTab), ENUMPROPFLAGS);

  QScriptValue runJavaScript = engine->newFunction(runJavaScriptForJs);
  constructor.setProperty("runJavaScript", runJavaScript);

  QScriptValue printToPdf = engine->newFunction(printToPdfForJs);
  constructor.setProperty("printToPdf", printToPdf);

  QScriptValue load = engine->newFunction(loadForJs);
  constructor.setProperty("load", load);
}

QScriptValue constructQWebEnginePage(QScriptContext * context, QScriptEngine  *engine)
{
  QWebEnginePage *obj = 0;
  if (context->argumentCount() >= 1)
    obj = new QWebEnginePage(qobject_cast<QObject*>(context->argument(0).toQObject()));
  else
    obj = new QWebEnginePage();
  return engine->toScriptValue(obj);
}

QWebEnginePageProto::QWebEnginePageProto(QObject * parent)
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
  return 0;
}

QColor QWebEnginePageProto::backgroundColor() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->backgroundColor();
  return QColor();
}

QSizeF QWebEnginePageProto::contentsSize() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->contentsSize();
  return QSizeF();
}

const QWebEngineContextMenuData& QWebEnginePageProto::contextMenuData() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->contextMenuData();

  QWebEngineContextMenuData results = QWebEngineContextMenuData();
  return results;
}

QMenu* QWebEnginePageProto::createStandardContextMenu()
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->createStandardContextMenu();
  return 0;
}

void QWebEnginePageProto::download(const QUrl & url, const QString & filename)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->download(url, filename);
}

void QWebEnginePageProto::findText(const QString & subString, QWebEnginePage::FindFlag options)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->findText(subString, options);
}

void QWebEnginePageProto::findText(const QString & subString, QWebEnginePage::FindFlag options, std::function<void(bool)> resultCallback)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->findText(subString, options, resultCallback);
}

bool QWebEnginePageProto::hasSelection() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->hasSelection();
  return false;
}

QWebEngineHistory* QWebEnginePageProto::history() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->history();
  return 0;
}

QIcon QWebEnginePageProto::icon() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->icon();
  return QIcon();
}

QUrl QWebEnginePageProto::iconUrl() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->iconUrl();
  return QUrl();
}

bool QWebEnginePageProto::isAudioMuted() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->isAudioMuted();
  return false;
}

void QWebEnginePageProto::print(QPrinter *printer, std::function<void(bool)> resultCallback)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->print(printer, resultCallback);
}

QWebEngineProfile* QWebEnginePageProto::profile() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->profile();
  return 0;
}

bool QWebEnginePageProto::recentlyAudible() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->recentlyAudible();
  return false;
}

void QWebEnginePageProto::replaceMisspelledWord(const QString & replacement)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->replaceMisspelledWord(replacement);
}

QUrl QWebEnginePageProto::requestedUrl() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->requestedUrl();
  return QUrl();
}

void QWebEnginePageProto::save(const QString & filePath, QWebEngineDownloadItem::SavePageFormat format) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->save(filePath, format);
}

QWebEngineScriptCollection& QWebEnginePageProto::scripts()
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->scripts();
  else
    qFatal("No matching function scripts");
}

QPointF QWebEnginePageProto::scrollPosition() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->scrollPosition();
  return QPointF();
}

QString QWebEnginePageProto::selectedText() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->selectedText();
  return QString();
}

void QWebEnginePageProto::setAudioMuted(bool muted)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setAudioMuted(muted);
}

void QWebEnginePageProto::setBackgroundColor(const QColor & color)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setBackgroundColor(color);
}

void QWebEnginePageProto::setContent(const QByteArray & data, const QString & mimeType, const QUrl & baseUrl)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setContent(data, mimeType, baseUrl);
}

void QWebEnginePageProto::setFeaturePermission(const QUrl & securityOrigin, QWebEnginePage::Feature feature, QWebEnginePage::PermissionPolicy policy)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setFeaturePermission(securityOrigin, feature, policy);
}

void QWebEnginePageProto::setHtml(const QString & html, const QUrl & baseUrl)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setHtml(html, baseUrl);
}

void QWebEnginePageProto::setUrl(const QUrl & url)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setUrl(url);
}

void QWebEnginePageProto::setView(QWidget * view)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setView(view);
}

void QWebEnginePageProto::setWebChannel(QWebChannel * channel, int worldId)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setWebChannel(channel, worldId);
}

void QWebEnginePageProto::setWebChannel(QWebChannel * channel)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setWebChannel(channel);
}

void QWebEnginePageProto::setZoomFactor(qreal factor)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->setZoomFactor(factor);
}

QWebEngineSettings* QWebEnginePageProto::settings() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->settings();
  return 0;
}

QString QWebEnginePageProto::title() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->title();
  return QString();
}

void QWebEnginePageProto::toHtml(std::function<void(QString)> resultCallback) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->toHtml(resultCallback);
}

void QWebEnginePageProto::toPlainText(std::function<void(QString)> resultCallback) const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    item->toPlainText(resultCallback);
}

void QWebEnginePageProto::triggerAction(QWebEnginePage::WebAction action, bool checked)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->triggerAction(action, checked);
}

QUrl QWebEnginePageProto::url() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->url();
  return QUrl();
}

QWidget* QWebEnginePageProto::view() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->view();
  return 0;
}

QWebChannel* QWebEnginePageProto::webChannel() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->webChannel();
  return 0;
}

qreal QWebEnginePageProto::zoomFactor() const
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->zoomFactor();
  return 0;
}

bool QWebEnginePageProto::event(QEvent * ev)
{
  QWebEnginePage *item = qscriptvalue_cast<QWebEnginePage*>(thisObject());
  if (item)
    return item->event(ev);
  return false;
}
