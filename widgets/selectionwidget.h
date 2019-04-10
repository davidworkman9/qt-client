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
  /**@brief Returns the "available" tree*/
  Q_INVOKABLE inline XTreeWidget  *getAvail() const { return _avail; }
  /**@brief Returns the "selected" tree*/
  Q_INVOKABLE inline XTreeWidget  *getSel() const { return _sel; }
  /**@brief Returns the _added list*/
  Q_INVOKABLE inline QList<XTreeWidgetItem*> getAdded() const { return _added; }
  /**@brief Returns the _removed list*/
  Q_INVOKABLE inline QList<XTreeWidgetItem*> getRemoved() const { return _removed; }
  Q_INVOKABLE inline QString getModifyTableName() const { return _modifyTableName; }
  Q_INVOKABLE inline ParameterList getAddConstraints() const { return _addConstraints; }
  Q_INVOKABLE inline ParameterList getEqualityColumns() const { return _equalityColumns; }
  Q_INVOKABLE inline QString  getRemoveByIdTableColName() const { return _removeByIdTableColName; }
  Q_INVOKABLE inline bool  getRemoveByAltId() const { return _removeByAltId; }
  Q_INVOKABLE inline bool  isParentInTrans() const { return _parentInTrans; }

  /**@brief Hides the cancel button*/
  Q_INVOKABLE void hideCancel();
  /**@brief Shows the cancel button*/
  Q_INVOKABLE void showCancel();
  /**@brief Hides the select button*/
  Q_INVOKABLE void hideSelect();
  /**@brief Shows the select button*/
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
  QList<XTreeWidgetItem*> _added; /**<@brief List of items added to "available" tree since loading*/
  QList<XTreeWidgetItem*> _removed; /**<@brief List of items removed from the "selected" tree since loading*/
  QString _modifyTableName; /**<@brief Name of table where items will be added or removed*/
  QString _removeByIdTableColName; /**<@brief Column name used as a key by the delete statement to remove records*/
  ParameterList _addConstraints; /**<@brief Name value pairs indicating column names and values to insert */
  ParameterList _equalityColumns; /**<@brief Name Value pairs indicating pairs of columns to be compared for equality*/
  bool _removeByAltId; /**<@brief When true, delete statements use the item's altId instead of id.*/
  bool _parentInTrans; /**<@brief When true, insert and delete statements will not be wrapped in a transaction block.*/
  void move(XTreeWidgetItem *pXtitem, bool pAdd);
  bool isSameItem(XTreeWidgetItem *pXtitem1, XTreeWidgetItem *pXtitem2) const;
  int execAddQuery(XSqlQuery &outQry);
  int execRemoveQuery(XSqlQuery &outQry);
};

void setupSelectionWidget(QScriptEngine *engine);

#endif // SELECTIONWIDGET_H
