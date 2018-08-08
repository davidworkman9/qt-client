/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef SCRIPTAPITESTRESULTS_H
#define SCRIPTAPITESTRESULTS_H

#include <QMainWindow>
#include "ui_scriptapitestresults.h"

class QScriptEngine;
class ScriptApiTestResultsPrivate;

class ScriptApiTestResults : public QMainWindow, Ui::ScriptApiTestResults
{
    Q_OBJECT
  public:
    explicit ScriptApiTestResults(QScriptEngine *engine = NULL, QWidget *parent = NULL);
             ~ScriptApiTestResults();

  signals:

  public slots:
    virtual void sRunTests();
  private slots:


private:
    ScriptApiTestResultsPrivate *_data;
};

#endif // SCRIPTAPITESTRESULTS_H
