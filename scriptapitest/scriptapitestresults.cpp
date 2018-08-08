/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "scriptapitestresults.h"
#include "format.h"
#include "qtexteditproto.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QException>



class ScriptApiTestResultsPrivate : public QWidget {
public:
    QScriptEngine *engine;
    QMap <QString, QString> tests;
    const QString currDir = "./";
    QMap<QString, QString> fileData;

    ScriptApiTestResultsPrivate(QScriptEngine *pEngine) : engine(pEngine)
    {
        // get absolute path to current directory
        QString newPath = this->getAbsolutePathToJsTests(currDir);

        // get the file names and contents
        fileData = this->getTestFileNamesAndContent(newPath);

        // add the files to the tests list
        this->addFilesToTests(fileData);
    }

    QString getAbsolutePathToJsTests(QString pCurrDir)
    {
        QDir binDir(pCurrDir);
        QString currPath = binDir.absolutePath();

        // make sure it has the scriptapitest directory in it
        QString newPath = "";
        QString needle = "qt-client/scriptapitest";
        int hasScriptApiTest = currPath.indexOf(needle);
        if (hasScriptApiTest > -1)
        {
            // remove everything from the path after qt-client/scriptapitest
            newPath = currPath.replace(hasScriptApiTest + needle.length(), currPath.length(), "");

            // make sure last char is a fwd slash
            if (!newPath.endsWith("/")) {
                newPath.append("/");
            }

            // append jstests directory onto newPath
            newPath.append("jstests");
        } else {
            qDebug() << "ScriptsAPI Directory not found";
        }

        return newPath;
    }

    QMap<QString, QString> getTestFileNamesAndContent(QString pPath)
    {
        QString tmpKey;
        QString tmpFilename;
        QMap<QString, QString> results;

        // get the file names and contents
        QDirIterator it(pPath, QDirIterator::NoIteratorFlags);
        while (it.hasNext()) {
            QFile f(it.next());
            f.open(QIODevice::ReadOnly);

            // remove path and leave just the filename
            tmpFilename = f.fileName().replace(0, pPath.length() + 1, "");

            // only look at javascript files
            if (tmpFilename.indexOf(".js") > -1) {
                QTextStream in(&f);
                QString fileContents = in.readAll();

                tmpKey = tmpFilename;
                tmpKey.replace(tmpKey.indexOf(".js"), 3, "");

                results.insert(tmpKey, fileContents);
            }
            f.close();
        }

        return results;
    }

    void addFilesToTests(QMap<QString, QString> pFiles)
    {
        QMapIterator<QString, QString> fit(pFiles);
        while (fit.hasNext())
        {
            fit.next();
            tests[fit.key()] = fit.value();
        }
    }
};

ScriptApiTestResults::ScriptApiTestResults(QScriptEngine *engine, QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    _data = new ScriptApiTestResultsPrivate(engine);

    connect(reruntests, SIGNAL(clicked(bool)), this, SLOT(sRunTests()));

    sRunTests();
}

ScriptApiTestResults::~ScriptApiTestResults()
{
}

void ScriptApiTestResults::sRunTests()
{
    // Remove all the old tests results
    QList<QObject*> children = scrollAreaWidgetContents->children();

    foreach(QObject *obj, children)
        if (qobject_cast<QWidget*>(obj))
            obj->deleteLater();

    // get absolute path to current directory
    QString newPath = _data->getAbsolutePathToJsTests(_data->currDir);

    // get the file names and contents
    _data->fileData = _data->getTestFileNamesAndContent(newPath);

    // add the files to the tests list
    _data->addFilesToTests(_data->fileData);

    QMapIterator<QString, QString> i(_data->tests);
    while (i.hasNext())
    {
      i.next();
      QLabel    *label = new QLabel(i.key(), this);
      QTextEdit *edit  = new QTextEdit(this);
      edit->setMaximumHeight(20);
      QScriptValue result = _data->engine->evaluate(i.value(), i.key());
      if (_data->engine->hasUncaughtException())
      {
        edit->setTextColor(namedColor("error"));
        edit->setPlainText(result.toString());
      }
      else
      {
        edit->setTextColor(namedColor("altEmphasis"));
        edit->setPlainText("Passed");
      }
      int row = _resultsGrid->rowCount();
      _resultsGrid->addWidget(label, row, 0);
      _resultsGrid->addWidget(edit,  row, 1);
    }
}
