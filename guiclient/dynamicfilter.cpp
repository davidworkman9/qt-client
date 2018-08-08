/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "dynamicfilter.h"

#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include <metasql.h>
#include "crmGroups.h"
#include "errorReporter.h"
#include "guiErrorCheck.h"

dynamicfilter::dynamicfilter(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : XDialog(parent, name, modal, fl)
{
  setupUi(this);

  connect(_buttonBox, SIGNAL(accepted()), this, SLOT(sSave()));
  connect(_check, SIGNAL(clicked()), this, SLOT(sCheckFilterValidity()));

  crmGroups::groupMap();
  GroupMap *_elem;
  // Build CRM Group Type combo
  QMapIterator<int, GroupMap*> i(crmGroups::_grpMap);
  while (i.hasNext())
  {
    i.next();
    _elem = crmGroups::_grpMap.value(i.key());
    _crmGroup->append(_elem->groupId, _elem->title, _elem->title);
  }

  rx.setPattern("\\b(ALTER|CREATE|TRUNCATE|DO|INSERT|UPDATE|DELETE|BEGIN|COMMIT|ROLLBACK)\\b");
  rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
  _isValid = false;
  _buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);

}

dynamicfilter::~dynamicfilter()
{
  // no need to delete child widgets, Qt does it all for us
}

void dynamicfilter::languageChange()
{
  retranslateUi(this);
}

enum SetResponse dynamicfilter::set(const ParameterList &pParams)
{
  XDialog::set(pParams);
  QVariant param;
  bool     valid;

  param = pParams.value("filter_id", &valid);
  if (valid)
  {
    _dynamicfilterid = param.toInt();
    _isValid = true;
    populate();
  }

  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "new")
      _mode = cNew;

    else if (param.toString() == "edit")
      _mode = cEdit;

    else if (param.toString() == "view")
    {
      _mode = cView;

      _name->setEnabled(false);
      _buttonBox->clear();
      _buttonBox->addButton(QDialogButtonBox::Close);
    }
  }

  return NoError;
}

void dynamicfilter::sCheckFilterValidity()
{
  QString sql;
  XSqlQuery query;
  ParameterList params;
  GroupMap *_elem;

  _elem = crmGroups::_grpMap.value(_crmGroup->id());
  sql = "SELECT source_table, source_key_field "
        "FROM source "
        "WHERE source_name = <? value('source_name') ?>;" ;
  params.append("source_name", _elem->source);
  MetaSQLQuery srcq(sql);
  query = srcq.toQuery(params);
  if (query.first())
  {
    params.append("source_table", query.value("source_table").toString());
    params.append("source_id", query.value("source_key_field").toString());
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Rerieving %1 Group Source").arg(_elem->title),
                                  query, __FILE__, __LINE__))
  {
    _isValid = false;
    return;
  }

  // Test the query for errors together with some SQL injection prevention
  
  QString cleanFilter =  _filter->toPlainText().trimmed();
  cleanFilter = cleanFilter.replace(rx, "");
  params.append("filter", cleanFilter);
  sql = "SELECT count(*) FROM <? literal('source_table') ?> "
        "WHERE <? literal('source_id') ?> "
        "IN (<? literal('filter') ?>);";
  MetaSQLQuery chkq(sql);
  query = chkq.toQuery(params);
  _isValid = true;

  if (ErrorReporter::error(QtCriticalMsg, this, tr("Invalid Filter SQL"),
                                  query, __FILE__, __LINE__))
    _isValid = false;

  _validCheck->setChecked(_isValid);
  _buttonBox->button(QDialogButtonBox::Save)->setEnabled(_isValid);

}

void dynamicfilter::sSave()
{
  XSqlQuery dynamicfilterSave;

  sCheckFilterValidity();

  QList<GuiErrorCheck> errors;
  errors << GuiErrorCheck(_name->text().trimmed().isEmpty(), _name,
                          tr("You must enter a valid Name for this Filter "))
         << GuiErrorCheck(_crmGroup->id() == -1, _crmGroup,
                          tr("You must select a CRM Group type"))
         << GuiErrorCheck(!_isValid, _check,
                          tr("You must validate your filter first"))
     ;

  if (_mode == cNew)
  {
    dynamicfilterSave.prepare( "SELECT dynamicfilter_id "
               "FROM dynamicfilter "
               "WHERE (dynamicfilter_name=:dynamicfilter_name);" );
    dynamicfilterSave.bindValue(":dynamicfilter_name", _name->text());
    dynamicfilterSave.exec();
    if (dynamicfilterSave.first())
      errors << GuiErrorCheck(true, _name,
                              tr( "A Filter has already been defined with the selected name.\n"
                                  "You may not create duplicate Filters." ) );
  }

  if (GuiErrorCheck::reportErrors(this, tr("Cannot Save Filter"), errors))
    return;

  if (_mode == cNew)
    dynamicfilterSave.prepare( "INSERT INTO dynamicfilter "
               "(dynamicfilter_name, dynamicfilter_descrip, dynamicfilter_object, dynamicfilter_filter) "
               "VALUES "
               "(:dynamicfilter_name, :dynamicfilter_descrip, :dynamicfilter_object, :dynamicfilter_filter); " );
  else if (_mode == cEdit)
    dynamicfilterSave.prepare( "UPDATE dynamicfilter "
               "SET dynamicfilter_name=:dynamicfilter_name, dynamicfilter_descrip=:dynamicfilter_descrip,"
               "    dynamicfilter_object=:dynamicfilter_object, dynamicfilter_filter=:dynamicfilter_filter "
               "WHERE dynamicfilter_id=:dynamicfilter_id " );

  dynamicfilterSave.bindValue(":dynamicfilter_id", _dynamicfilterid);
  dynamicfilterSave.bindValue(":dynamicfilter_name", _name->text());
  dynamicfilterSave.bindValue(":dynamicfilter_descrip", _descrip->text());
  dynamicfilterSave.bindValue(":dynamicfilter_object", _crmGroup->code());

  // SQL injection prevention
  QString cleanFilter =  _filter->toPlainText().trimmed();
  cleanFilter = cleanFilter.replace(rx, "");
  dynamicfilterSave.bindValue(":dynamicfilter_filter", cleanFilter);

  dynamicfilterSave.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Saving Dynamic Filter"),
                                dynamicfilterSave, __FILE__, __LINE__))
    return;

  accept();
}

void dynamicfilter::populate()
{
  XSqlQuery dynamicfilterpopulate;
  dynamicfilterpopulate.prepare( "SELECT * "
      	     "FROM dynamicfilter "
             "WHERE (dynamicfilter_id=:dynamicfilter_id);" );
  dynamicfilterpopulate.bindValue(":dynamicfilter_id", _dynamicfilterid);
  dynamicfilterpopulate.exec();
  if (dynamicfilterpopulate.first())
  {
    _name->setText(dynamicfilterpopulate.value("dynamicfilter_name").toString());
    _descrip->setText(dynamicfilterpopulate.value("dynamicfilter_descrip").toString());
    _crmGroup->setCode(dynamicfilterpopulate.value("dynamicfilter_object").toString());
    _filter->setText(dynamicfilterpopulate.value("dynamicfilter_filter").toString());
  
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Retrieving Dynamic Filter"),
                                dynamicfilterpopulate, __FILE__, __LINE__))
  {
    return;
  }

  sCheckFilterValidity();
}
