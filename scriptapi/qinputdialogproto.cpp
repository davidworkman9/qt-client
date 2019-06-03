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
#include "qinputdialogproto.h"

static QScriptValue InputDialogOptionToScriptValue(QScriptEngine *engine, 
                                                   const enum QInputDialog::InputDialogOption &p)
{
  return QScriptValue(engine, (int)p);
}

static void InputDialogOptionFromScriptValue(const QScriptValue &obj, 
                                             enum QInputDialog::InputDialogOption &p)
{
  p = (enum QInputDialog::InputDialogOption)obj.toInt32();
}

static QScriptValue InputDialogOptionsToScriptValue(QScriptEngine *engine, 
                                                    const QInputDialog::InputDialogOptions &p)
{
  return QScriptValue(engine, (int)p);
}

static void InputDialogOptionsFromScriptValue(const QScriptValue &obj, 
                                              QInputDialog::InputDialogOptions &p)
{
  p = (QInputDialog::InputDialogOptions)obj.toInt32();
}

static QScriptValue InputModeToScriptValue(QScriptEngine *engine,
                                           const enum QInputDialog::InputMode &p)
{
  return QScriptValue(engine, (int)p);
}

static void InputModeFromScriptValue(const QScriptValue &obj, enum QInputDialog::InputMode &p)
{
  p = (enum QInputDialog::InputMode)obj.toInt32();
}

static QScriptValue scriptGetDouble(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);
  if (context->argumentCount() == 3)
  {
    return
    QInputDialog::getDouble(qscriptvalue_cast<QWidget*>(context->argument(0)),
                            qscriptvalue_cast<QString>(context->argument(1)),
                            qscriptvalue_cast<QString>(context->argument(2)));
  }
  else if (context->argumentCount() == 4)
  {
    return
    QInputDialog::getDouble(qscriptvalue_cast<QWidget*>(context->argument(0)),
                            qscriptvalue_cast<QString>(context->argument(1)),
                            qscriptvalue_cast<QString>(context->argument(2)),
                            qscriptvalue_cast<double>(context->argument(3)));
  }
  else if (context->argumentCount() == 5)
  {
    return
    QInputDialog::getDouble(qscriptvalue_cast<QWidget*>(context->argument(0)),
                            qscriptvalue_cast<QString>(context->argument(1)),
                            qscriptvalue_cast<QString>(context->argument(2)),
                            qscriptvalue_cast<double>(context->argument(3)),
                            qscriptvalue_cast<double>(context->argument(4)));
  }
  else if (context->argumentCount() == 6)
  {
    return
    QInputDialog::getDouble(qscriptvalue_cast<QWidget*>(context->argument(0)),
                            qscriptvalue_cast<QString>(context->argument(1)),
                            qscriptvalue_cast<QString>(context->argument(2)),
                            qscriptvalue_cast<double>(context->argument(3)),
                            qscriptvalue_cast<double>(context->argument(4)),
                            qscriptvalue_cast<double>(context->argument(5)));
  }
  else if (context->argumentCount() == 7)
  {
    return
    QInputDialog::getDouble(qscriptvalue_cast<QWidget*>(context->argument(0)),
                            qscriptvalue_cast<QString>(context->argument(1)),
                            qscriptvalue_cast<QString>(context->argument(2)),
                            qscriptvalue_cast<double>(context->argument(3)),
                            qscriptvalue_cast<double>(context->argument(4)),
                            qscriptvalue_cast<double>(context->argument(5)),
                            qscriptvalue_cast<int>(context->argument(6)));
  }
  else if (context->argumentCount() == 8)
  {
    return
    QInputDialog::getDouble(qscriptvalue_cast<QWidget*>(context->argument(0)),
                            qscriptvalue_cast<QString>(context->argument(1)),
                            qscriptvalue_cast<QString>(context->argument(2)),
                            qscriptvalue_cast<double>(context->argument(3)),
                            qscriptvalue_cast<double>(context->argument(4)),
                            qscriptvalue_cast<double>(context->argument(5)),
                            qscriptvalue_cast<int>(context->argument(6)),
                            (bool*)context->argument(7).toQObject());
  }
  else if (context->argumentCount() == 9)
  {
    return
    QInputDialog::getDouble(qscriptvalue_cast<QWidget*>(context->argument(0)),
                            qscriptvalue_cast<QString>(context->argument(1)),
                            qscriptvalue_cast<QString>(context->argument(2)),
                            qscriptvalue_cast<double>(context->argument(3)),
                            qscriptvalue_cast<double>(context->argument(4)),
                            qscriptvalue_cast<double>(context->argument(5)),
                            qscriptvalue_cast<int>(context->argument(6)),
                            (bool*)context->argument(7).toQObject(),
                            (Qt::WindowFlags)qscriptvalue_cast<int>(context->argument(8)));
  }
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
  else if (context->argumentCount() == 10)
  {
    return
    QInputDialog::getDouble(qscriptvalue_cast<QWidget*>(context->argument(0)),
                            qscriptvalue_cast<QString>(context->argument(1)),
                            qscriptvalue_cast<QString>(context->argument(2)),
                            qscriptvalue_cast<double>(context->argument(3)),
                            qscriptvalue_cast<double>(context->argument(4)),
                            qscriptvalue_cast<double>(context->argument(5)),
                            qscriptvalue_cast<int>(context->argument(6)),
                            (bool*)context->argument(7).toQObject(),
                            (Qt::WindowFlags)qscriptvalue_cast<int>(context->argument(8)),
                            qscriptvalue_cast<double>(context->argument(9)));
  }
#endif
  else
  {
    context->throwError(QScriptContext::UnknownError,
                        "Could not find an appropriate getDouble function");
  }
  return QScriptValue();
}

static QScriptValue scriptGetInt(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);
  if (context->argumentCount() == 3)
  {
    return
    QInputDialog::getInt(qscriptvalue_cast<QWidget*>(context->argument(0)),
                         qscriptvalue_cast<QString>(context->argument(1)),
                         qscriptvalue_cast<QString>(context->argument(2)));
  }
  if (context->argumentCount() == 4)
  {
    return
    QInputDialog::getInt(qscriptvalue_cast<QWidget*>(context->argument(0)),
                         qscriptvalue_cast<QString>(context->argument(1)),
                         qscriptvalue_cast<QString>(context->argument(2)),
                         qscriptvalue_cast<int>(context->argument(3)));
  }
  else if (context->argumentCount() == 5)
  {
    return
    QInputDialog::getInt(qscriptvalue_cast<QWidget*>(context->argument(0)),
                         qscriptvalue_cast<QString>(context->argument(1)),
                         qscriptvalue_cast<QString>(context->argument(2)),
                         qscriptvalue_cast<int>(context->argument(3)),
                         qscriptvalue_cast<int>(context->argument(4)));
  }
  else if (context->argumentCount() == 6)
  {
    return
    QInputDialog::getInt(qscriptvalue_cast<QWidget*>(context->argument(0)),
                         qscriptvalue_cast<QString>(context->argument(1)),
                         qscriptvalue_cast<QString>(context->argument(2)),
                         qscriptvalue_cast<int>(context->argument(3)),
                         qscriptvalue_cast<int>(context->argument(4)),
                         qscriptvalue_cast<int>(context->argument(5)));
  }
  else if (context->argumentCount() == 7)
  {
    return 
    QInputDialog::getInt(qscriptvalue_cast<QWidget*>(context->argument(0)),
                         qscriptvalue_cast<QString>(context->argument(1)),
                         qscriptvalue_cast<QString>(context->argument(2)),
                         qscriptvalue_cast<int>(context->argument(3)),
                         qscriptvalue_cast<int>(context->argument(4)),
                         qscriptvalue_cast<int>(context->argument(5)),
                         qscriptvalue_cast<int>(context->argument(6)));
  }
  else if (context->argumentCount() == 8)
  {
    return
    QInputDialog::getInt(qscriptvalue_cast<QWidget*>(context->argument(0)),
                         qscriptvalue_cast<QString>(context->argument(1)),
                         qscriptvalue_cast<QString>(context->argument(2)),
                         qscriptvalue_cast<int>(context->argument(3)),
                         qscriptvalue_cast<int>(context->argument(4)),
                         qscriptvalue_cast<int>(context->argument(5)),
                         qscriptvalue_cast<int>(context->argument(6)),
                         (bool*)context->argument(7).toQObject());
  }
  else if (context->argumentCount() == 9)
  {
    return
    QInputDialog::getInt(qscriptvalue_cast<QWidget*>(context->argument(0)),
                         qscriptvalue_cast<QString>(context->argument(1)),
                         qscriptvalue_cast<QString>(context->argument(2)),
                         qscriptvalue_cast<int>(context->argument(3)),
                         qscriptvalue_cast<int>(context->argument(4)),
                         qscriptvalue_cast<int>(context->argument(5)),
                         qscriptvalue_cast<int>(context->argument(6)),
                         (bool*)context->argument(7).toQObject(),
                         (Qt::WindowFlags)qscriptvalue_cast<int>(context->argument(8)));
  }
  else
  {
    context->throwError(QScriptContext::UnknownError,
                        "Could not find an appropriate getInt function");
  }
  return QScriptValue();
}

static QScriptValue scriptGetItem(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);

  if (context->argumentCount() == 4)
  {
    return
    QInputDialog::getItem(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          qscriptvalue_cast<QStringList>(context->argument(3)));
  }
  else if (context->argumentCount() == 5)
  {
    return
    QInputDialog::getItem(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          qscriptvalue_cast<QStringList>(context->argument(3)),
                          qscriptvalue_cast<int>(context->argument(4)));
  }
  else if (context->argumentCount() == 6)
  {
    return
    QInputDialog::getItem(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          qscriptvalue_cast<QStringList>(context->argument(3)),
                          qscriptvalue_cast<int>(context->argument(4)),
                          qscriptvalue_cast<bool>(context->argument(5)));
  }
  else if (context->argumentCount() == 7)
  {
    return
    QInputDialog::getItem(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          qscriptvalue_cast<QStringList>(context->argument(3)),
                          qscriptvalue_cast<int>(context->argument(4)),
                          qscriptvalue_cast<bool>(context->argument(5)),
                          (bool*)context->argument(6).toQObject());
  }
  else if (context->argumentCount() == 8)
  {
    return
    QInputDialog::getItem(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          qscriptvalue_cast<QStringList>(context->argument(3)),
                          qscriptvalue_cast<int>(context->argument(4)),
                          qscriptvalue_cast<bool>(context->argument(5)),
                          (bool*)context->argument(6).toQObject(),
                          (Qt::WindowFlags)qscriptvalue_cast<int>(context->argument(7)));
  }
  else if (context->argumentCount() == 9)
  {
    return
    QInputDialog::getItem(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          qscriptvalue_cast<QStringList>(context->argument(3)),
                          qscriptvalue_cast<int>(context->argument(4)),
                          qscriptvalue_cast<bool>(context->argument(5)),
                          (bool*)context->argument(6).toQObject(),
                          (Qt::WindowFlags)qscriptvalue_cast<int>(context->argument(7)),
                          (Qt::InputMethodHints)qscriptvalue_cast<int>(context->argument(8)));
  }
  else
  {
    context->throwError(QScriptContext::UnknownError,
                        "Could not find an appropriate getItem function");
  }
  return QScriptValue();
}

static QScriptValue scriptGetMultiLineText(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);

  if (context->argumentCount() == 3)
  {
    return
    QInputDialog::getMultiLineText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                                   qscriptvalue_cast<QString>(context->argument(1)),
                                   qscriptvalue_cast<QString>(context->argument(2)));
  }
  else if (context->argumentCount() == 4)
  {
    return
    QInputDialog::getMultiLineText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                                   qscriptvalue_cast<QString>(context->argument(1)),
                                   qscriptvalue_cast<QString>(context->argument(2)),
                                   qscriptvalue_cast<QString>(context->argument(3)));
  }
  else if (context->argumentCount() == 5)
  {
    return
    QInputDialog::getMultiLineText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                                   qscriptvalue_cast<QString>(context->argument(1)),
                                   qscriptvalue_cast<QString>(context->argument(2)),
                                   qscriptvalue_cast<QString>(context->argument(3)),
                                   (bool*)context->argument(4).toQObject());
  }
  else if (context->argumentCount() == 6)
  {
    return
    QInputDialog::getMultiLineText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                                   qscriptvalue_cast<QString>(context->argument(1)),
                                   qscriptvalue_cast<QString>(context->argument(2)),
                                   qscriptvalue_cast<QString>(context->argument(3)),
                                   (bool*)context->argument(4).toQObject(),
                                   (Qt::WindowFlags)qscriptvalue_cast<int>(context->argument(5)));
  }
  else if (context->argumentCount() == 7)
  {
    return
    QInputDialog::getMultiLineText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                                   qscriptvalue_cast<QString>(context->argument(1)),
                                   qscriptvalue_cast<QString>(context->argument(2)),
                                   qscriptvalue_cast<QString>(context->argument(3)),
                                   (bool*)context->argument(4).toQObject(),
                                   (Qt::WindowFlags)qscriptvalue_cast<int>(context->argument(5)),
                                   (Qt::InputMethodHints)qscriptvalue_cast<int>(context->argument(6)));
  }
  else
  {
    context->throwError(QScriptContext::UnknownError,
                        "Could not find an appropriate getMultiLineText function");
  }
  return QScriptValue();
}

static QScriptValue scriptGetText(QScriptContext *context, QScriptEngine *engine)
{
  Q_UNUSED(engine);

  if (context->argumentCount() == 3)
  {
    return
    QInputDialog::getText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)));
  }
  else if (context->argumentCount() == 4)
  {
    return
    QInputDialog::getText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          (QLineEdit::EchoMode)qscriptvalue_cast<int>(context->argument(3)));
  }
  else if (context->argumentCount() == 5)
  {
    return
    QInputDialog::getText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          (QLineEdit::EchoMode)qscriptvalue_cast<int>(context->argument(3)),
                          qscriptvalue_cast<QString>(context->argument(4)));
  }
  else if (context->argumentCount() == 6)
  {
    return
    QInputDialog::getText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          (QLineEdit::EchoMode)qscriptvalue_cast<int>(context->argument(3)),
                          qscriptvalue_cast<QString>(context->argument(4)),
                          (bool*)context->argument(5).toQObject());
  }
  else if (context->argumentCount() == 7)
  {
    return
    QInputDialog::getText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          (QLineEdit::EchoMode)qscriptvalue_cast<int>(context->argument(3)),
                          qscriptvalue_cast<QString>(context->argument(4)),
                          (bool*)context->argument(5).toQObject(),
                          (Qt::WindowFlags)qscriptvalue_cast<int>(context->argument(6)));
  }
  else if (context->argumentCount() == 8)
  {
    return
    QInputDialog::getText(qscriptvalue_cast<QWidget*>(context->argument(0)),
                          qscriptvalue_cast<QString>(context->argument(1)),
                          qscriptvalue_cast<QString>(context->argument(2)),
                          (QLineEdit::EchoMode)qscriptvalue_cast<int>(context->argument(3)),
                          qscriptvalue_cast<QString>(context->argument(4)),
                          (bool*)context->argument(5).toQObject(),
                          (Qt::WindowFlags)qscriptvalue_cast<int>(context->argument(6)),
                          (Qt::InputMethodHints)qscriptvalue_cast<int>(context->argument(7)));
  }
  
  else
  {
    context->throwError(QScriptContext::UnknownError,
                        "Could not find an appropriate getText function");
  }
  return QScriptValue();
}

void setupQInputDialogProto(QScriptEngine *engine)
{
  if (!engine->globalObject().property("QInputDialog").isObject())
  {
    QScriptValue proto = engine->newQObject(new QInputDialogProto(engine));
    engine->setDefaultPrototype(qMetaTypeId<QInputDialog*>(), proto);
    QScriptValue constructor = engine->newFunction(constructQInputDialog, proto);
    QScriptValue metaObject = engine->newQMetaObject(&QInputDialog::staticMetaObject, constructor);
    engine->globalObject().setProperty("QInputDialog", metaObject);

    metaObject.setProperty("getDouble", engine->newFunction(scriptGetDouble), STATICPROPFLAGS);
    metaObject.setProperty("getInt", engine->newFunction(scriptGetInt), STATICPROPFLAGS);
    metaObject.setProperty("getItem", engine->newFunction(scriptGetItem), STATICPROPFLAGS);
    metaObject.setProperty("getMultiLineText",
                           engine->newFunction(scriptGetMultiLineText), STATICPROPFLAGS);
    metaObject.setProperty("getText", engine->newFunction(scriptGetText), STATICPROPFLAGS);

    qScriptRegisterMetaType(engine, InputDialogOptionToScriptValue,
                            InputDialogOptionFromScriptValue);
    qScriptRegisterMetaType(engine, InputDialogOptionsToScriptValue,
                            InputDialogOptionsFromScriptValue);
    metaObject.setProperty("NoButtons",
                           QScriptValue(engine, QInputDialog::NoButtons), ENUMPROPFLAGS);
    metaObject.setProperty("UseListViewForComboBoxItems",
                           QScriptValue(engine, QInputDialog::UseListViewForComboBoxItems),
                           ENUMPROPFLAGS);
    metaObject.setProperty("UsePlainTextEditForTextInput",
                           QScriptValue(engine, QInputDialog::UsePlainTextEditForTextInput),
                           ENUMPROPFLAGS);

    qScriptRegisterMetaType(engine, InputModeToScriptValue, InputModeFromScriptValue);
    metaObject.setProperty("TextInput",
                           QScriptValue(engine, QInputDialog::TextInput), ENUMPROPFLAGS);
    metaObject.setProperty("IntInput",
                           QScriptValue(engine, QInputDialog::IntInput), ENUMPROPFLAGS);
    metaObject.setProperty("DoubleInput",
                           QScriptValue(engine, QInputDialog::DoubleInput), ENUMPROPFLAGS);
  }
}

QScriptValue constructQInputDialog(QScriptContext *context,
  QScriptEngine  *engine)
{
  QInputDialog *obj = 0;
  if (context->argumentCount() == 2)
    obj = new QInputDialog(qscriptvalue_cast<QWidget*>(context->argument(0)),
                           (Qt::WindowFlags)(context->argument(1).toInt32()));
  else if (context->argumentCount() == 1)
    obj = new QInputDialog(qscriptvalue_cast<QWidget*>(context->argument(0)));
  else if (context->argumentCount() == 0)
    obj = new QInputDialog();
  else
    context->throwError(QScriptContext::UnknownError,
                        "Could not find an appropriate QInputDialog constructor");
  return engine->toScriptValue(obj);
}

QInputDialogProto::QInputDialogProto(QObject *parent) : QObject(parent)
{
}

QString QInputDialogProto::cancelButtonText() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->cancelButtonText();
  return QString();
}

QStringList QInputDialogProto::comboBoxItems() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->comboBoxItems();
  return QStringList();
}

void QInputDialogProto::done(int results)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->done(results);
}

int QInputDialogProto::doubleDecimals() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->doubleDecimals();
  return 0;
}

double QInputDialogProto::doubleMaximum() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->doubleMaximum();
  return double();
}

double QInputDialogProto::doubleMinimum() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->doubleMinimum();
  return double();
}

#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
double QInputDialogProto::doubleStep() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->doubleStep();
  return double();
}
#endif

double QInputDialogProto::doubleValue() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->doubleValue();
  return double();
}

int QInputDialogProto::inputMode() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->inputMode();
  return 0;
}

int QInputDialogProto::intMaximum() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->intMaximum();
  return 0;
}

int QInputDialogProto::intMinimum() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->intMinimum();
  return 0;
}

int QInputDialogProto::intStep() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->intStep();
  return 0;
}

int QInputDialogProto::intValue() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->intValue();
  return 0;
}

bool QInputDialogProto::isComboBoxEditable() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->isComboBoxEditable();
  return false;
}

QString QInputDialogProto::labelText() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->labelText();
  return QString();
}

QSize QInputDialogProto::minimumSizeHint() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->minimumSizeHint();
  return QSize();
}

QString QInputDialogProto::okButtonText() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->okButtonText();
  return QString();
}

void QInputDialogProto::open(QObject *receiver, const char *member)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->open(receiver, member);
}

int QInputDialogProto::options() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->options();
  return 0;
}

void QInputDialogProto::setCancelButtonText(const QString &text)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setCancelButtonText(text);
}

void QInputDialogProto::setComboBoxEditable(bool editable)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setComboBoxEditable(editable);
}

void QInputDialogProto::setComboBoxItems(const QStringList &items)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setComboBoxItems(items);
}

void QInputDialogProto::setDoubleDecimals(int decimals)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setDoubleDecimals(decimals);
}

void QInputDialogProto::setDoubleMaximum(double max)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setDoubleMaximum(max);
}

void QInputDialogProto::setDoubleMinimum(double min)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setDoubleMinimum(min);
}

void QInputDialogProto::setDoubleRange(double min, double max)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setDoubleRange(min, max);
}

#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
void QInputDialogProto::setDoubleStep(double step)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setDoubleStep(step);
}
#endif

void QInputDialogProto::setDoubleValue(double value)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setDoubleValue(value);
}

void QInputDialogProto::setInputMode(int mode)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setInputMode((QInputDialog::InputMode) mode);
}

void QInputDialogProto::setIntMaximum(int max)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setIntMaximum(max);
}

void QInputDialogProto::setIntMinimum(int min)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setIntMinimum(min);
}

void QInputDialogProto::setIntRange(int min, int max)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setIntRange(min, max);
}

void QInputDialogProto::setIntStep(int step)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setIntStep(step);
}

void QInputDialogProto::setIntValue(int value)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setIntValue(value);
}

void QInputDialogProto::setLabelText(const QString &text)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setLabelText(text);
}

void QInputDialogProto::setOkButtonText(const QString &text)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setOkButtonText(text);
}

void QInputDialogProto::setOption(int option, bool on)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setOption((QInputDialog::InputDialogOption) option, on);
}

void QInputDialogProto::setOptions(int options)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setOptions((QInputDialog::InputDialogOption) options);
}

void QInputDialogProto::setTextEchoMode(int mode)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setTextEchoMode((QLineEdit::EchoMode) mode);
}

void QInputDialogProto::setTextValue(const QString &text)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setTextValue(text);
}

void QInputDialogProto::setVisible(bool visible)
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    item->setVisible(visible);
}

QSize QInputDialogProto::sizeHint() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->sizeHint();
  return QSize();
}

bool QInputDialogProto::testOption(int option) const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->testOption((QInputDialog::InputDialogOption) option);
  return false;
}

int QInputDialogProto::textEchoMode() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->textEchoMode();
  return 0;
}

QString QInputDialogProto::textValue() const
{
  QInputDialog *item = qscriptvalue_cast<QInputDialog*>(thisObject());
  if (item)
    return item->textValue();
  return QString();
}

