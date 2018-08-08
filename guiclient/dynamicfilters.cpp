/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "dynamicfilters.h"

#include <QVariant>
#include <QMessageBox>
#include <parameter.h>
#include "dynamicfilter.h"
#include "guiclient.h"

dynamicfilters::dynamicfilters(QWidget* parent, const char* name, Qt::WindowFlags fl)
  : XWidget(parent, name, fl)
{
  setupUi(this);


  // signals and slots connections
  connect(_filters, SIGNAL(valid(bool)), _delete, SLOT(setEnabled(bool)));
  connect(_filters, SIGNAL(itemSelected(int)), _edit, SLOT(animateClick()));
  connect(_filters, SIGNAL(valid(bool)), _edit, SLOT(setEnabled(bool)));
  connect(_new, SIGNAL(clicked()), this, SLOT(sNew()));
  connect(_edit, SIGNAL(clicked()), this, SLOT(sEdit()));
  connect(_delete, SIGNAL(clicked()), this, SLOT(sDelete()));
  connect(_close, SIGNAL(clicked()), this, SLOT(close()));

  _filters->addColumn(tr("Name"),        _itemColumn, Qt::AlignLeft, true, "dynamicfilter_name" );
  _filters->addColumn(tr("Description"), -1,          Qt::AlignLeft, true, "dynamicfilter_descrip" );

  sFillList();
}

dynamicfilters::~dynamicfilters()
{
  // no need to delete child widgets, Qt does it all for us
}

void dynamicfilters::languageChange()
{
  retranslateUi(this);
}

void dynamicfilters::sNew()
{
  ParameterList params;
  params.append("mode", "new");

  dynamicfilter newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void dynamicfilters::sEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("filter_id", _filters->id());

  dynamicfilter newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void dynamicfilters::sDelete()
{
  XSqlQuery dynamicfiltersDelete;
  dynamicfiltersDelete.prepare( "DELETE FROM dynamicfilter "
             "WHERE (dynamicfilter_id=:dynamicfilter_id);" );
  dynamicfiltersDelete.bindValue(":dynamicfilter_id", _filters->id());
  dynamicfiltersDelete.exec();

  sFillList();
}

void dynamicfilters::sFillList()
{
  _filters->populate( "SELECT dynamicfilter_id, dynamicfilter_name, dynamicfilter_descrip "
                   "FROM dynamicfilter "
                   "ORDER BY dynamicfilter_name;" );
}
