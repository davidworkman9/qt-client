/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef ADDRESSMERGE_H
#define ADDRESSMERGE_H

#include "guiclient.h"
#include "xwidget.h"
#include <parameter.h>

#include "ui_addressMerge.h"

class addressMerge : public XWidget, public Ui::addressMerge
{
    Q_OBJECT

public:
    addressMerge(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::Window);
    ~addressMerge();

protected slots:
    virtual void languageChange();

    virtual void sAdd();
    virtual void sAddrDelete();
    virtual void sAddrEdit();
    virtual void sAddrView();
    virtual void sSrcAddrEdit();
    virtual void sSrcAddrView();
    virtual void sFillList();
    virtual void sHandleProcess();
    virtual void sPopulateAddrMenu(QMenu *);
    virtual void sPopulateSrcMenu(QMenu *, QTreeWidgetItem *, int);
    virtual void sPopulateSources();
    virtual void sPopulateTarget();
    virtual void sProcess();
    virtual void sSelect(bool);
    virtual void sSelectCol();
    virtual void sSetTarget();
    virtual void sDeselect(int);
    virtual void sDeselectCntct();
    virtual void sDeselectSource();
    virtual void closeEvent( QCloseEvent * pEvent );

private:
    int         _selectCol;
    QStringList _colMap;

};

#endif // ADDRESSMERGE_H
