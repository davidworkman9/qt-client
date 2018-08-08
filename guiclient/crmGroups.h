/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef CRMGROUPS_H
#define CRMGROUPS_H

#include "xwidget.h"
#include "ui_crmGroups.h"
#include "crmacctcluster.h"
#include "xcombobox.h"

struct GroupMap
{
  int     groupId;
  QString title;
  QString priv;
  QString table;
  QString itemTable;
  QString search;
  CRMAcctLineEdit::CRMAcctSubtype subtype;
  QString source;
  XComboBox::XComboBoxTypes groupcombo;

  GroupMap(int id, QString _title, QString _priv, QString _table, QString _itemTable, 
              QString _search, CRMAcctLineEdit::CRMAcctSubtype _subtype, QString _source,
              XComboBox::XComboBoxTypes _groupcombo) 
  {
    groupId    = id;
    title      = _title;
    priv       = _priv;
    table      = _table;
    itemTable  = _itemTable;
    search     = _search;
    subtype    = _subtype;
    source     = _source;
    groupcombo = _groupcombo;
  }
};  

class crmGroups : public XWidget, public Ui::crmGroups
{
    Q_OBJECT

public:
    crmGroups(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::Window);
    ~crmGroups();

//  if you add to this then add to the GroupMap[] in the .cpp
    enum GroupTypes {Account, Customer, Prospect, Employee, Contact, Address};

    static QMap<int, struct GroupMap*> &groupMap();
    static QMap<int, struct GroupMap*> _grpMap;

public slots:
    virtual SetResponse set(const ParameterList & pParams );
    virtual void sDelete();
    virtual void sNew();
    virtual void sEdit();
    virtual void sView();
    virtual void sFillList();

protected slots:
    virtual void languageChange();

private:
  static bool addToMap(int id, QString _title, QString _priv, QString _table, QString _itemTable, 
                               QString _search, CRMAcctLineEdit::CRMAcctSubtype _subtype, 
                               QString _source, XComboBox::XComboBoxTypes _groupcombo);

  GroupMap *_elem;

};

#endif // CRMGROUPS_H
