/*
* This file is part of the xTuple ERP: PostBooks Edition, a free and
* open source Enterprise Resource Planning software suite,
* Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
* It is licensed to you under the Common Public Attribution License
* version 1.0, the full text of which (including xTuple-specific Exhibits)
* is available at www.xtuple.com/CPAL.  By using this software, you agree
* to be bound by its terms.
*/

#include "selectionwidget.h"


#define DEBUG false

/**
* @class SelectionWidget

* @brief Widget used to pick items from one list and add them to another.

* The selection widget incorporates 2 XTreeWidgets, one as a source (or available)
* and one as a destination (or selected). Each tree will be populated from a different query
* but the data should be compatible with the fields on both trees otherwise moving items back
* and forth doesn't make sense.
*
* The widget was designed to be adaptable and thus requires some configuration first.
* The following must be done before the widget will function properly:
*
* <UL>
* <LI>Tree Setup:<BR>
* The internal trees are exposed via the getAvail() and getSel() functions. Once you
* have pointers to the trees, they must be configured as is usual for XTreeWidgets.
*
* <LI>Equality Columns:<BR>
* Since each of the trees may present different attributes of the underlying data, there needs
* to be a way to determine equality between items on different trees. A list of name-value 
* pairs must be passed to the setEqualityColumns() function. Each pair will contain the XTreeWidget
* column names to be considered equivalent. When the trees first load, any items in the "available" 
* tree that are already present in the "selected" tree (based on the equality columns) will be
* filtered out of the available list.
*
* <LI>Select and Cancel Buttons:<BR>
* Select and cancel buttons can be shown or hidden depending on your use case with the 
* hideCancel(), showCancel() and hideSelect(), showSelect(). These buttons don't do anything other
* than emit a clicked signal when clicked. They are shown by default. 
* </UL>
*
* The widget can either be automatic or handled manually depending on the situation.
* <UL>
* <LI>Manual:<BR>
* With the above setup in place, the widget will function properly and track selections.
* Various signals are emitted by the widget when buttons are clicked and before/after items
* are moved in either direction. At any point you can retrieve a list of changes from the 
* getAdded() and getRemoved() functions. The getAdded function will return a list of items that
* are currently in the "selected" tree that were not there when the widget first loaded. 
* getRemoved() returns items that were in the "selected" tree when the widget first loaded and
* are no longer there. Between the accessor functions and the signals, you can process the
* changes in whatever way suits your needs.
* 
* <LI>Automatic:<BR>
* The widget can automatically build and execute SQL queries to add and remove items from
* the underlying table. In order to do that, the following must be configured:
* <UL>
* <LI>Set Modification Table:<BR>
* The table where new items will be added (and from which removed items will be deleted) must
* be passed to setModifyTableName(). Ideally this will be the table from which the "selected"
* tree was populated though this is not necessary. The only requirement is that calling id()
* or altId() on the XTreeWidget item will return an id that is sufficient to identify a single
* record in the table passed to setModifyTableName().
*
* <LI>Set Insert Criteria:<BR>
* In order to insert records into an arbitrary table, a list of column names and values to insert
* must be provided. A parameter list must be passed to setAddConstraints(). This list will contain
* one entry for each column to be populated and must, at a minimum, contain entries for each 
* required column in the table passed to setModifyTableName(). In each entry, the parameter name 
* must be the table column name. The parameter value can either be a literal value, or the name of
* a column in the "selected" XTreeWidget. If the parameter value matches a column name, the 
* contents of that column will be inserted, otherwise the parameter value will be inserted 
* directly.
*
* <LI>Set Delete Criteria:<BR>
* In order to delete records from an arbitrary table, a column name must be passed to
* setRemoveByIdTableColName() to be used as a key to identify a specific record. The widget will
* create a delete query which will remove records if the value returned by the XTreeWidgetItem's
* id() or altId() function matches the value in the column provided. id() is used by default 
* and can be changed by calling the setRemoveByAltId() function.
*</UL>
*</UL>
*
*@todo <UL><LI>add a way to add a new item directly to the "selected" tree.
<LI> replace queries with metasql
</UL>
*/

SelectionWidget::SelectionWidget(QWidget *parent, Qt::WindowFlags fl)
  : QWidget(parent, fl)
{
  setupUi(this);

  _modifyTableName = "";
  _removeByIdTableColName = "";
  _removeByAltId = false;
  _parentInTrans = false;
  connect(_add, SIGNAL(clicked()), this, SLOT(sAdd()));
  connect(_remove, SIGNAL(clicked()), this, SLOT(sRemove()));
  connect(_addAll, SIGNAL(clicked()), this, SLOT(sAddAll()));
  connect(_removeAll, SIGNAL(clicked()), this, SLOT(sRemoveAll()));
  connect(_select, SIGNAL(clicked()), this, SLOT(sSelect()));
  connect(_cancel, SIGNAL(clicked()), this, SLOT(sCancel()));
  //connect both signals so population order doesn't matter
  connect(_avail, SIGNAL(populated()), this, SLOT(sFilterDuplicates()));
  connect(_sel, SIGNAL(populated()), this, SLOT(sFilterDuplicates()));
}

SelectionWidget::~SelectionWidget()
{
}

/**@brief Emits selectClicked() signal*/
void SelectionWidget::sSelect() { emit selectClicked(); }
/**@brief Emits cancelClicked() signal*/
void SelectionWidget::sCancel() { emit cancelClicked(); }
/**@brief Hides the cancel button*/
void SelectionWidget::hideCancel() { _cancel->hide(); }
/**@brief Shows the cancel button*/
void SelectionWidget::showCancel() { _cancel->show(); }
/**@brief Hides the select button*/
void SelectionWidget::hideSelect() { _select->hide(); }
/**@brief Shows the select button*/
void SelectionWidget::showSelect() { _select->show(); }

/**@brief Moves selected items from the "available" tree to the "selected" tree.
*
* Emits addClickedBefore() and addClickedAfter() before and after moving an item respectively.
* Emits itemAdded() after each item is moved
*/
void SelectionWidget::sAdd()
{
  emit addClickedBefore();
  foreach(XTreeWidgetItem *xtitem, _avail->selectedItems())
  {
    move(xtitem, true);
    emit itemAdded(xtitem->id());
  }
  emit addClickedAfter();
}

/**@brief Moves selected items from the "selected" tree to the "available" tree
*
* Emits removeClickedBefore() and removeClickedAfter() before and after moving an item
* respectively. Emits itemRemoved() after each item is moved.
*/
void SelectionWidget::sRemove()
{
  emit removeClickedBefore();
  foreach(XTreeWidgetItem *xtitem, _sel->selectedItems())
  {
    move(xtitem, false);
    emit itemRemoved(xtitem->id());
  }
  emit removeClickedAfter();
}

/**@brief Moves all items from the "available" tree to the "selected" tree.
*
* Emits addAllClickedBefore() and addAllClickedAfter() before and after moving an item
* respectively. Emits itemAdded() after each item is moved.
*/
void SelectionWidget::sAddAll()
{
  emit addAllClickedBefore();
  XTreeWidgetItem *xtitem;
  while ((xtitem = dynamic_cast<XTreeWidgetItem*>(_avail->takeTopLevelItem(0))))
  {
    move(xtitem, true);
    emit itemAdded(xtitem->id());
  }
  emit addAllClickedAfter();
}

/**@brief Moves all items from the "selected" tree to the "available" tree
*
* Emits removeAllClickedBefore() and removeAllClickedAfter() before and after moving an item
* respectively. Emits itemRemoved() after each item is moved.
*/
void SelectionWidget::sRemoveAll()
{
  emit removeAllClickedBefore();
  XTreeWidgetItem *xtitem;
  while ((xtitem = dynamic_cast<XTreeWidgetItem*>(_sel->takeTopLevelItem(0))))
  {
    move(xtitem, false);
    emit itemRemoved(xtitem->id());
  }
  emit removeAllClickedAfter();
}


/**@brief Moves an item from one tree to another based on the value of pAdd
*
* Based on the value of pAdd, one tree is designated as a source, the other as a destination.
* Likewise _added and _removed are designated as either a check list or an append list.
* pXtitem is removed from the source tree. If pXtitem is NOT in the check list, it means this is
* a new change we must track. pXtitem is then added to the append list. If pXtitem IS present in 
* the check list, it means the item was previously moved, and this move is merely undoing that
* action. Therefore, pXtitem is only removed from the check list and not added to the append list.
* Finally, pXtitem is added to the destination tree.
* The key idea here is that depending on pAdd, the append and check lists will switch places. So
* the list we add an item to in one direction, will be the list we check against when going in
* the opposite direction.
* @param pXtitem XTreeWidgetItem to be moved from one tree to another.
* @param pAdd True when moving from "available" tree to "selected" tree, false otherwise.
*/
void SelectionWidget::move(XTreeWidgetItem *pXtitem, bool pAdd)
{
  XTreeWidget *sourceTree;
  XTreeWidget *destTree;
  QList<XTreeWidgetItem*> *checkList;
  QList<XTreeWidgetItem*> *appendList;

  if (pAdd)
  {
    sourceTree = _avail;
    destTree = _sel;
    checkList = &_removed;
    appendList = &_added;
  }
  else
  {
    sourceTree = _sel;
    destTree = _avail;
    checkList = &_added;
    appendList = &_removed;
  }
  
  sourceTree->takeTopLevelItem(sourceTree->indexOfTopLevelItem(pXtitem));
  if (!checkList->removeOne(pXtitem))
  {
    appendList->append(pXtitem);
  }
  destTree->addTopLevelItem(pXtitem);
}

/**@brief Determines if 2 items are equal based on configured equality criteria.
*
* This function checks the _equalityColumns list. For each name value pair entry, it checks
* the contents of the corresponding columns in each tree item and returns true only if they 
* are the same for all entries in the list
*/
bool SelectionWidget::isSameItem(XTreeWidgetItem *pXtitem1, XTreeWidgetItem *pXtitem2) const
{
  int index = 0;
  while (index < _equalityColumns.count())
  {
    if (pXtitem1->rawValue(_equalityColumns.name(index)) !=
        pXtitem2->rawValue(_equalityColumns.value(index).toString()))
    {
      return false;
    }
    index++;
  }
  return true;
}

/**@brief Removes items from the "available" tree that are already in the "selected" tree. 
*
* This function iterates through the items in the "available" tree and for each, uses the 
* isSameItem() function to find any items in the "selected" tree that match. If it finds any,
* the item is removed from the "available" tree.
*/
void SelectionWidget::sFilterDuplicates()
{
  //maybe a hash might be better here...
  for (int availIndex = _avail->topLevelItemCount() - 1; availIndex >= 0; availIndex--)
  {
    for (int selIndex = 0; selIndex < _sel->topLevelItemCount(); selIndex++)
    {
      if (isSameItem(_avail->topLevelItem(availIndex), _sel->topLevelItem(selIndex)))
      {
        _avail->takeTopLevelItem(availIndex);
        break;
      }
    }
  }
}

/**@brief Builds and executes an INSERT query for each item in the _added list. 
*
* This function builds INSERT statements using _modifyTableName and _addConstraints in order
* to create flexible queries for any table needed. Values to be inserted are bound to 
* "parameterN" for N = 0 to addConstraints.count() - 1. Returns the query executed to
* allow for error reporting outside of a transaction block.<BR>
* The entries in _addConstraints will determine how the query is built. For each entry in 
* _addConstraints, the parameter name specifies the column where the value will be inserted. the
* parameter value will either be a column name in the "available" tree or a literal value. If the
* parameter value matches a column name, the value in that column will be inserted, otherwise the
* parameter value itself will be inserted directly.
* @param [out] outQry returns the last executed query before either detecting an error or completing. 
*/
int SelectionWidget::execAddQuery(XSqlQuery &outQry)
{
  QSqlQuery qry;
  QString qryString = QString("INSERT INTO %1 (").arg(_modifyTableName);
  QString valString = ") VALUES (";
  for (int i = 0; i < _addConstraints.count(); i++)
  {
    qryString.append(_addConstraints.name(i) + ", ");
    valString.append(QString(":parameter%1").arg(i) + ", ");
  }
  qryString.chop(2);//remove the ", " from the last iteration.
  valString.chop(2);
  qryString.append(valString);
  qryString.append(");");
  qry.prepare(qryString);
  
  //build lists of values for each constraint
  QVariantList valueList;
  QVariant valToInsert;
  
  for (int i = 0; i < _addConstraints.count(); i++)
  {
    foreach(XTreeWidgetItem *xtitem, _added)
    {
      valToInsert = xtitem->rawValue(_addConstraints.value(i).toString());
      if (valToInsert.isNull())
      {
        valToInsert = _addConstraints.value(i);
      }
      valueList.append(valToInsert);
    }
    //bind value list for batch execution
    qry.bindValue(QString(":parameter%1").arg(i), valueList);
    valueList.clear();
  }

  if (DEBUG)
  {
    qDebug() << "Add query string:" << qryString;
    QMapIterator<QString, QVariant> bindings(qry.boundValues());
    while (bindings.hasNext())
    {
      bindings.next();
      qDebug() << bindings.key() << "\t" << bindings.value();
    }
  }

  qry.execBatch();
  outQry = qry;
  if (qry.lastError().type() != QSqlError::NoError)
  {
    return(-1);
  } 
  return(0);
}

/**@brief Builds and executes a DELETE query to remove all items in the _removed list
*
* This function builds a DELETE statement using _modifyTableName and _removeByIdTableColName in
* order to create flexible queries for any table needed. Ids to be deleted are bound to 
* "parameterN" for N = 0 to _removed.count() - 1 Returns the last query executed to
* allow for error reporting outside of a transaction block.<BR>
* The delete statement will use the value returned by calling id() on each item in the _removed
* list to identify which records to remove. If the _removeByAltId flag is set to true, the 
* statement will instead use the result of calling altId().
* @param [out] outQry returns the executed query.*/
int SelectionWidget::execRemoveQuery(XSqlQuery &outQry)
{
  XSqlQuery qry;
  QString qryString = QString("DELETE FROM %1 WHERE %2 IN (").arg(_modifyTableName)
                                                             .arg(_removeByIdTableColName);
  for (int i = 0; i < _removed.count(); i++)
  {
    qryString.append(QString(":parameter%1").arg(i) + ", ");
  }
  qryString.chop(2);//remove the ", " from the last iteration.
  qryString.append(");");
  qry.prepare(qryString);
  int counter = 0;
  foreach(XTreeWidgetItem *xtitem, _removed)
  {
    int remove = _removeByAltId ? xtitem->altId() : xtitem->id();
    qry.bindValue(QString(":parameter%1").arg(counter), remove);
    counter++;
  }
  if (DEBUG)
  {
    qDebug() << "Remove query string:" << qryString;
    QMapIterator<QString, QVariant> bindings(qry.boundValues());
    while (bindings.hasNext())
    {
      bindings.next();
      qDebug() << bindings.key() << "\t" << bindings.value();
    }
  }
  qry.exec();
  outQry = qry;
  return (qry.lastError().type() != QSqlError::NoError) ? -1 : 0;
}

/**@brief Calls functions to execute the appropriate INSERT and DELETE queries
*
* This function will call execAddQuery() and execRemoveQuery() then clear the _added and 
* _removed lists. If the _parentInTrans flag is false, the calls to these functions will be 
* wrapped in a transaction block to ensure atomicity. It is left to the caller to ensure
* that the _parentInTrans flag is set appropriately.
*/
int SelectionWidget::sCommitChanges()
{
  XSqlQuery outQry;
  XSqlQuery qry;
  XSqlQuery rollback;
  if (!_parentInTrans)
  {
    rollback.prepare("ROLLBACK;");
    qry.exec("BEGIN;");
  }
  if ((!_modifyTableName.isEmpty()) && (!_addConstraints.isEmpty()) && (!_added.isEmpty()))
  {
    if (execAddQuery(outQry) == -1)
    {
      if (!_parentInTrans) { rollback.exec(); }
      ErrorReporter::error(QtCriticalMsg, this, tr("Error Adding Selected items."),
                           outQry, __FILE__, __LINE__);
      return -1;
    }
  }
  if ((!_modifyTableName.isEmpty()) && (!_removeByIdTableColName.isEmpty()) && (!_removed.isEmpty()))
  {
    if (execRemoveQuery(outQry) == -1)
    {
      if (!_parentInTrans) { rollback.exec(); }
      ErrorReporter::error(QtCriticalMsg, this, tr("Error Removing Selected items."),
                           outQry, __FILE__, __LINE__);
      return -1;
    }
  }
  if (!_parentInTrans) { qry.exec("COMMIT;"); }
  _added.clear();
  _removed.clear();
  return 0;
}

// script exposure ////////////////////////////////////////////////////////////
QScriptValue SelectionWidgetToScriptValue(QScriptEngine *engine, SelectionWidget *const &obj)
{
  return engine->newQObject(obj);
}
void SelectionWidgetFromScriptValue(const QScriptValue &obj, SelectionWidget* &item)
{
  item = qobject_cast<SelectionWidget*>(obj.toQObject());
}
void setupSelectionWidget(QScriptEngine *engine)
{
  qScriptRegisterMetaType(engine, SelectionWidgetToScriptValue, SelectionWidgetFromScriptValue);
  if (!engine->globalObject().property("SelectionWidget").isObject())
  {
      QScriptValue ctor = engine->newObject();
      QScriptValue meta = engine->newQMetaObject(&SelectionWidget::staticMetaObject, ctor);

      engine->globalObject().setProperty("SelectionWidget", meta, 
                                          QScriptValue::ReadOnly | QScriptValue::Undeletable);
  }
}
