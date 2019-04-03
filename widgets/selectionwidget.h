/*
* This file is part of the xTuple ERP: PostBooks Edition, a free and
* open source Enterprise Resource Planning software suite,
* Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
* It is licensed to you under the Common Public Attribution License
* version 1.0, the full text of which (including xTuple-specific Exhibits)
* is available at www.xtuple.com/CPAL.  By using this software, you agree
* to be bound by its terms.
*/

#ifndef SELECTIONWIDGET_H
#define SELECTIONWIDGET_H

#include "widgets.h"
#include <parameter.h>
#include <errorReporter.h>
#include "xsqlquery.h"
#include "ui_selectionwidget.h"


class SelectionWidget : public QWidget, public Ui::SelectionWidget
{
  Q_OBJECT

public:


  SelectionWidget(QWidget* parent = 0, Qt::WindowFlags fl = 0);
  ~SelectionWidget();

  Q_INVOKABLE inline XTreeWidget  *getAvail() const { return _avail; }
  Q_INVOKABLE inline XTreeWidget  *getSel() const { return _sel; }
  Q_INVOKABLE inline QList<XTreeWidgetItem*> getAdded() const { return _added; }
  Q_INVOKABLE inline QList<XTreeWidgetItem*> getRemoved() const { return _removed; }
  Q_INVOKABLE inline QString getModifyTableName() const { return _modifyTableName; }
  Q_INVOKABLE inline ParameterList getAddConstraints() const { return _addConstraints; }
  Q_INVOKABLE inline ParameterList getEqualityColumns() const { return _equalityColumns; }
  Q_INVOKABLE inline QString  getRemoveByIdTableColName() const { return _removeByIdTableColName; }
  Q_INVOKABLE inline bool  getRemoveByAltId() const { return _removeByAltId; }
  Q_INVOKABLE inline bool  isParentInTrans() const { return _parentInTrans; }

  Q_INVOKABLE void hideCancel();
  Q_INVOKABLE void showCancel();
  Q_INVOKABLE void hideSelect();
  Q_INVOKABLE void showSelect();
  Q_INVOKABLE void setModifyTableName(QString name) { _modifyTableName = name; }
  Q_INVOKABLE void setAddConstraints(ParameterList constraints) { _addConstraints = constraints; }
  Q_INVOKABLE void setEqualityColumns(ParameterList columns) { _equalityColumns = columns; }
  Q_INVOKABLE void setRemoveByIdTableColName(QString name)  { _removeByIdTableColName = name; }
  Q_INVOKABLE void setRemoveByAltId(bool useAlt = true) { _removeByAltId = useAlt; }
  Q_INVOKABLE void setParentInTrans(bool parInTrans = true) { _parentInTrans = parInTrans; }

  


  public slots:
  virtual void sAdd();
  virtual void sRemove();
  virtual void sAddAll();
  virtual void sRemoveAll();
  virtual void sSelect();
  virtual void sCancel();
  virtual int  sCommitChanges();

signals:
  void addClickedBefore();
  void addClickedAfter();
  void removeClickedBefore();
  void removeClickedAfter();
  void addAllClickedBefore();
  void addAllClickedAfter();
  void removeAllClickedBefore();
  void removeAllClickedAfter();
  void selectClicked();
  void cancelClicked();
  void itemAdded(int id);
  void itemRemoved(int id);

protected slots:
  void sFilterDuplicates();

protected:
  QList<XTreeWidgetItem*> _added;
  QList<XTreeWidgetItem*> _removed;
  QString _modifyTableName;
  QString _removeByIdTableColName;
  ParameterList _addConstraints;
  ParameterList _equalityColumns;
  bool _removeByAltId;
  bool _parentInTrans;
  void move(XTreeWidgetItem *pXtitem, bool pAdd);
  bool isSameItem(XTreeWidgetItem *pXtitem1, XTreeWidgetItem *pXtitem2) const;
  int execAddQuery(XSqlQuery &outQry);
  int execRemoveQuery(XSqlQuery &outQry);
};

void setupSelectionWidget(QScriptEngine *engine);

#endif // SELECTIONWIDGET_H
