/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef BUILDCRMGROUPS_H
#define BUILDCRMGROUPS_H

#include "guiclient.h"
#include "xwidget.h"
#include <parameter.h>

#include "crmGroups.h"

#include "ui_buildCRMGroups.h"

class buildCRMGroups : public XWidget, public Ui::buildCRMGroups
{
    Q_OBJECT

public:
    buildCRMGroups(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::Window);
    ~buildCRMGroups();

public slots:
    virtual void sTypeSelected();
    virtual void sHandleButton();
    virtual void sGenerateGroupRecords();
    virtual void buildParameters();
    virtual void addCharacteristics();
    virtual void addDynamicFilters();

protected slots:
    virtual void languageChange();

private:
    static QMap<int, struct GroupMap*> _grpMap;
    GroupMap *_elem;

    QList<QVariant> _charidstext;
    QList<QVariant> _charidslist;
    QList<QVariant> _charidsdate;
    QStringList _charList;
    QStringList _filterList;
    QList<QVariant> _filterParams;

};

#endif // BUILDCRMGROUPS_H
