/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "buildCRMGroups.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QCloseEvent>

#include <metasql.h>
#include "characteristic.h"
#include "mqlutil.h"
#include "crmGroups.h"
#include "parameterwidget.h"
#include "errorReporter.h"

QMap<int,  struct GroupMap*> buildCRMGroups::_grpMap;

buildCRMGroups::buildCRMGroups(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : XWidget(parent, name, fl)
{
  setupUi(this);

  if (_grpMap.isEmpty())
  {
    crmGroups::groupMap();
    _grpMap = crmGroups::_grpMap;
  }

// Build CRM Group Type combo
  QMapIterator<int, GroupMap*> i(_grpMap);
  while (i.hasNext())
  {
    i.next();
    _elem = _grpMap.value(i.key());
    _type->append(_elem->groupId, _elem->title);
  }

  connect(_type, SIGNAL(newID(int)), this, SLOT(sTypeSelected()));
  connect(_params, SIGNAL(updated()), this, SLOT(sHandleButton()));
  connect(_generate, SIGNAL(clicked()), this, SLOT(sGenerateGroupRecords()));
  connect(_addGroup, SIGNAL(clicked()), this, SLOT(sAddGroup()));

  _params->append(tr("Street Pattern"), "addr_street_pattern", ParameterWidget::Text);
  _params->append(tr("City Pattern"), "addr_city_pattern", ParameterWidget::Text);
  _params->append(tr("State Pattern"), "addr_state_pattern", ParameterWidget::Text);
  _params->append(tr("Postal Code Pattern"), "addr_postalcode_pattern", ParameterWidget::Text);
  _params->append(tr("Country Pattern"), "addr_country_pattern", ParameterWidget::Text);

  sTypeSelected();
}

buildCRMGroups::~buildCRMGroups()
{
    // no need to delete child widgets, Qt does it all for us
}

void buildCRMGroups::languageChange()
{
    retranslateUi(this);
}

void buildCRMGroups::sTypeSelected()
{
  _elem = _grpMap.value(_type->id());
  _targetGrp->setType(_elem->groupcombo);
  _addGroup->setEnabled(_privileges->check(_elem->priv));
  buildParameters();
}

void buildCRMGroups::buildParameters()
{
  QStringList extras;
  int row;

  _params->clearFilters();

  // Remove extra parameters first (if they exist) then add back if the type applies
  extras << tr("Sales Rep") << tr("Customer Types") << tr("Item Group")
         << tr("Item") << tr("Open Quotes") << tr("Customer Type Pattern");

  extras << _charList << _filterList;

  for (int i = 0; i < extras.size(); i++)
  {
    row = _params->paramIndex(extras.at(i));
    if (row != -1)
      _params->removeParam(extras.at(i));
  }

  switch (_type->id())
  {
  case crmGroups::Customer:
    {
      _params->appendComboBox(extras.at(0), "salesrep_id", XComboBox::SalesReps);
      _params->appendComboBox(extras.at(1), "custtype_id", XComboBox::CustomerTypes);
      _params->append(extras.at(5),         "custtype_code_pattern", ParameterWidget::Text);
      _params->appendComboBox(extras.at(2), "itemGroup", XComboBox::ItemGroups);
      _params->append(extras.at(3),         "item_id", ParameterWidget::Item);
    }
    break;
  case crmGroups::Prospect:
    {
      _params->appendComboBox(extras.at(0), "salesrep_id", XComboBox::SalesReps);
      _params->appendComboBox(extras.at(2), "itemGroup", XComboBox::ItemGroups);
      _params->append(extras.at(3),         "item_id", ParameterWidget::Item);
      _params->appendComboBox(extras.at(4), "hasQuotes", ParameterWidget::Exists);
    }
    break;
  case crmGroups::Contact:
    {
      _params->appendComboBox(extras.at(2), "itemGroup", XComboBox::ItemGroups);
      _params->append(extras.at(3), "item_id", ParameterWidget::Item);
    }
    break;
  default:
    // nothing to add
    break;
  }

  addCharacteristics();
  addDynamicFilters();
}

void buildCRMGroups::addCharacteristics()
{
  XSqlQuery chars;
  QString column;
  QString name;

  _charList.clear();
  _charidstext.clear();
  _charidslist.clear();
  _charidsdate.clear();

  chars.prepare("SELECT char_id, char_name, char_type "
                        "FROM char "
                        " JOIN charuse ON char_id = charuse_char_id"
                        " JOIN source ON charuse_target_type = source_charass"
                        "             AND source_name = :source"
                        " WHERE char_search "
                        " ORDER BY char_name;" );
  chars.bindValue(":source", _elem->source);
  chars.exec();
  if (chars.size() <= 0)
  {
    qWarning() << "Could not find any characteristics matching" << _elem->source;
  }
  while (chars.next())
  {
    characteristic::Type chartype = (characteristic::Type)chars.value("char_type").toInt();
    column = QString("char%1").arg(chars.value("char_id").toString());
    name = chars.value("char_name").toString();
    if (chartype == characteristic::Date)
      _charList << name + " " + QApplication::translate("display", "Start Date", 0)
                << name + " " + QApplication::translate("display", "End Date", 0);
    else
      _charList << name;

    if (chartype == characteristic::Text)
    {
      _charidstext.append(chars.value("char_id").toInt());
      _params->append(name, column, ParameterWidget::Text);
    }
    else if (chartype == characteristic::Number)
    {
      _charidstext.append(chars.value("char_id").toInt());
      _params->append(name, column, ParameterWidget::Numeric);
    }
    else if (chartype == characteristic::List)
    {
      _charidslist.append(chars.value("char_id").toInt());
      QString sql = QString("SELECT charopt_value, charopt_value"
                    "  FROM charopt"
                    " WHERE charopt_char_id = %1"
                    " ORDER BY charopt_order, charopt_value;")
          .arg(chars.value("char_id").toInt());
      _params->append(name, column, ParameterWidget::Multiselect, QVariant(), false, sql);
    }
    else if (chartype == characteristic::Date)
    {
      _charidsdate.append(chars.value("char_id").toInt());
      QString start = QApplication::translate("display", "Start Date", 0);
      QString end = QApplication::translate("display", "End Date", 0);
      _params->append(name + " " + start, column + "startDate", ParameterWidget::Date);
      _params->append(name + " " + end, column + "endDate", ParameterWidget::Date);
    }
  }
}

void buildCRMGroups::addDynamicFilters()
{
  XSqlQuery filters;
  QString column;
  QString name;

  _filterList.clear();
  _filterParams.clear();

  filters.prepare("SELECT dynamicfilter_id, dynamicfilter_name"
                  " FROM dynamicfilter "
                  " WHERE dynamicfilter_object = :object");
  filters.bindValue(":object", _elem->title);
  filters.exec();
  while (filters.next())
  {
    _filterParams.append(filters.value("dynamicfilter_id").toInt());
    name = filters.value("dynamicfilter_name").toString();
    _filterList << name;
    column = QString("filter%1").arg(filters.value("dynamicfilter_id").toString());
    _params->append(name, column, ParameterWidget::Exists);
  }
}

void buildCRMGroups::sHandleButton()
{
  _generate->setEnabled(_targetGrp->isValid() && _params->parameters().length() > 0);
}

void buildCRMGroups::sAddGroup()
{
  ParameterList params;
  params.append("groupType", _elem->groupId);

  crmGroups* newdlg = new crmGroups();
  newdlg->set(params);
  if (newdlg->exec() != XDialog::Rejected)
    _targetGrp->populate();
}

void buildCRMGroups::sGenerateGroupRecords()
{
  XSqlQuery query;
  ParameterList params;
  QString sql;
  QString idField;
  int inserted;

  QString msg = tr("Are you sure you want to update the %1 %2 Group with the results of this selection?")
                .arg(_targetGrp->code()).arg(_elem->title);
  if (QMessageBox::question(this, tr("Generate Group Items"),
                        msg, QMessageBox::Yes | QMessageBox::No,
                             QMessageBox::No) == QMessageBox::No)
    return;

  sql = "SELECT source_table, source_key_field, source_charass "
        "FROM source "
        "WHERE source_name = <? value('source_name') ?>;" ;
  params.append("source_name", _elem->source);
  MetaSQLQuery srcq(sql);
  query = srcq.toQuery(params);
  if (query.first())
  {
    params.append("source_table", query.value("source_table").toString());
    params.append("source_id", query.value("source_key_field").toString());
    idField = query.value("source_key_field").toString();
    params.append("charass", query.value("source_charass").toString());
  }

  _params->appendValue(params);
  params.append(_elem->title, true);
  params.append("groupsitem", _elem->itemTable);
  params.append("groups_id", _targetGrp->id());

  // Handle characteristics
  QVariant param;
  bool valid;
  QString column;
  QStringList clauses;

  // Put together the list of text and date based charids used to build joins
  params.append("char_id_text_list", _charidstext);
  params.append("char_id_list_list", _charidslist);
  params.append("char_id_date_list", _charidsdate);

  // Handle text based sections of clause
  foreach (QVariant columnid, _charidstext)
  {
    column = QString("char%1").arg(columnid.toString());
    param = params.value(column, &valid);
    if (valid)
      clauses.append(QString("charass_alias%1.charass_value ~* '%2'").arg(columnid.toString()).arg(param.toString()));
  }
  // Handle text based sections of clause
  foreach (QVariant columnid, _charidslist)
  {
    column = QString("char%1").arg(columnid.toString());
    param = params.value(column, &valid);
    if (valid)
    {
      QStringList list;
      for (int j = 0; j < param.toStringList().count(); j++)
        list << param.toString();

      clauses.append(QString("charass_alias%1.charass_value IN ('%2') ").arg(columnid.toString()).arg(list.join("','")));
    }
  }
  // Handle date based sections of clause
  foreach (QVariant columnid, _charidsdate)
  {
    // Look for start date
    column = QString("char%1startDate").arg(columnid.toString());
    param = params.value(column, &valid);
    if (valid)
      clauses.append(QString("charass_alias%1.charass_value::date >= '%2'").arg(columnid.toString()).arg(param.toString()));

    // Look for end date
    column = QString("char%1endDate").arg(columnid.toString());
    param = params.value(column, &valid);
    if (valid)
      clauses.append(QString("charass_alias%1.charass_value::date <= '%2'").arg(columnid.toString()).arg(param.toString()));
  }
  if (clauses.count())
    params.append("charClause", clauses.join(" AND ").prepend(" AND "));

  // Handle Dynamic Filters (convert exists Parameter to literal WHERE clause)
  QStringList filters;

  foreach (QVariant filterid, _filterParams)
  {
    column = QString("filter%1").arg(filterid.toString());
    param = params.value(column, &valid);
    if (valid)
    {
      query.prepare("SELECT dynamicfilter_filter FROM dynamicfilter WHERE dynamicfilter_id = :filterid;");
      query.bindValue(":filterid",  filterid);
      query.exec();
      if (query.first())
        filters.append(QString("%1 IN (%2)")
                  .arg(idField)
                  .arg(query.value("dynamicfilter_filter").toString()));
    }
  }
  if (filters.count())
    params.append("filterClause", filters.join(" AND ").prepend(" AND "));

  XSqlQuery rollback;
  rollback.prepare("ROLLBACK;");

  XSqlQuery begin("BEGIN;");

  sql = "DELETE FROM <? literal('groupsitem') ?> "
                "WHERE (groupsitem_groups_id=<? value('groups_id') ?>); " ;
  MetaSQLQuery delq(sql);
  query = delq.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting %1 Group Items").arg(_elem->title),
                                  query, __FILE__, __LINE__))
  {
    rollback.exec();
    return;
  }

  MetaSQLQuery genq = mqlLoad("buildGroups", "generateItems");
  query = genq.toQuery(params);
  if (query.first())
    inserted = query.value("groupsinserted").toInt();
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Generating %1 Group Items").arg(_elem->title),
                                  query, __FILE__, __LINE__))
  {
    rollback.exec();
    return;
  }

  XSqlQuery commit("COMMIT;");
  QMessageBox::information(this, tr("Generate Group Items"),
                        tr("Group %1 has been successfully populated with %2 %3s")
                           .arg(_targetGrp->code())
                           .arg(inserted)
                           .arg(_elem->title) 
                          );
}
