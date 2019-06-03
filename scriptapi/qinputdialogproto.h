/*
* This file is part of the xTuple ERP: PostBooks Edition, a free and
* open source Enterprise Resource Planning software suite,
* Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
* It is licensed to you under the Common Public Attribution License
* version 1.0, the full text of which(including xTuple-specific Exhibits)
* is available at www.xtuple.com/CPAL.  By using this software, you agree
* to be bound by its terms.
*/

#ifndef __QINPUTDIALOGPROTO_H__
#define __QINPUTDIALOGPROTO_H__

#include <QInputDialog>
#include <QObject>
#include <QtScript>

Q_DECLARE_METATYPE(QInputDialog*)
void setupQInputDialogProto(QScriptEngine *engine);
QScriptValue constructQInputDialog(QScriptContext *context, QScriptEngine *engine);

class QInputDialogProto : public QObject, public QScriptable
{
  Q_OBJECT

public:
  QInputDialogProto(QObject *parent);
  Q_INVOKABLE QString        cancelButtonText() const;
  Q_INVOKABLE QStringList    comboBoxItems() const;
  Q_INVOKABLE virtual void   done(int result);
  Q_INVOKABLE int            doubleDecimals() const;
  Q_INVOKABLE double         doubleMaximum() const;
  Q_INVOKABLE double         doubleMinimum() const;
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
  Q_INVOKABLE double         doubleStep() const;
#endif
  Q_INVOKABLE double         doubleValue() const;
  Q_INVOKABLE int            inputMode() const;
  Q_INVOKABLE int            intMaximum() const;
  Q_INVOKABLE int            intMinimum() const;
  Q_INVOKABLE int            intStep() const;
  Q_INVOKABLE int            intValue() const;
  Q_INVOKABLE bool           isComboBoxEditable() const;
  Q_INVOKABLE QString        labelText() const;
  Q_INVOKABLE virtual QSize  minimumSizeHint() const;
  Q_INVOKABLE QString        okButtonText() const;
  Q_INVOKABLE void           open(QObject *receiver, const char *member);
  Q_INVOKABLE int            options() const;
  Q_INVOKABLE void           setCancelButtonText(const QString &text);
  Q_INVOKABLE void           setComboBoxEditable(bool editable);
  Q_INVOKABLE void           setComboBoxItems(const QStringList &items);
  Q_INVOKABLE void           setDoubleDecimals(int decimals);
  Q_INVOKABLE void           setDoubleMaximum(double max);
  Q_INVOKABLE void           setDoubleMinimum(double min);
  Q_INVOKABLE void           setDoubleRange(double min, double max);
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
  Q_INVOKABLE void           setDoubleStep(double step);
#endif
  Q_INVOKABLE void           setDoubleValue(double value);
  Q_INVOKABLE void           setInputMode(int mode);
  Q_INVOKABLE void           setIntMaximum(int max);
  Q_INVOKABLE void           setIntMinimum(int min);
  Q_INVOKABLE void           setIntRange(int min, int max);
  Q_INVOKABLE void           setIntStep(int step);
  Q_INVOKABLE void           setIntValue(int value);
  Q_INVOKABLE void           setLabelText(const QString &text);
  Q_INVOKABLE void           setOkButtonText(const QString &text);
  Q_INVOKABLE void           setOption(int option, bool on = true);
  Q_INVOKABLE void           setOptions(int options);
  Q_INVOKABLE void           setTextEchoMode(int mode);
  Q_INVOKABLE void           setTextValue(const QString &text);
  Q_INVOKABLE virtual void   setVisible(bool visible);
  Q_INVOKABLE virtual QSize  sizeHint() const;
  Q_INVOKABLE bool           testOption(int option) const;
  Q_INVOKABLE int            textEchoMode() const;
  Q_INVOKABLE QString        textValue() const;

signals:
  void doubleValueChanged(double value);
  void doubleValueSelected(double value);
  void intValueChanged(int value);
  void intValueSelected(int value);
  void textValueChanged(const QString &text);
  void textValueSelected(const QString &text);
};

Q_DECLARE_METATYPE(enum QInputDialog::InputDialogOption);
Q_DECLARE_METATYPE(QInputDialog::InputDialogOptions);
Q_DECLARE_METATYPE(enum QInputDialog::InputMode);
#endif