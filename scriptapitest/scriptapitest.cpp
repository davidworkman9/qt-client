/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QString>
#include <QScriptEngine>

#include <parameter.h>

#include "metrics.h"
#include "login2.h"

#include "setupscriptapi.h"
#include "scriptapitestresults.h"

extern void xTupleMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

const QString Copyright("Copyright © 1999-2017");
const QString Version("0.0.1");
const QString Build("xTuple Script API Test %1 %2");
const QString ConnAppName("scriptapitest");

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setOrganizationDomain("xTuple.com");
  app.setOrganizationName("xTuple");
  app.setApplicationName(ConnAppName);
  app.setApplicationVersion(Version);
  QCoreApplication::addLibraryPath(QString("."));

  QCommandLineParser parser;
  parser.setApplicationDescription("Test the contents of scriptapi subdirectory");

  parser.addOption(QCommandLineOption("databaseURL", "e.g. PSQL://host:port/database", "databaseURL"));
  parser.addOption(QCommandLineOption("password",    "database user password",         "password"));
  parser.addOption(QCommandLineOption("enhancedAuth", "Use Enhanced Authentication"));
  parser.addOption(QCommandLineOption("evaluation", "This is an evaluatio database"));
  parser.addOption(QCommandLineOption(QStringList() << "h" << "host",      "database server host", "hostname"));
  parser.addOption(QCommandLineOption(QStringList() << "p" << "port",      "database server port", "port"));
  parser.addOption(QCommandLineOption(QStringList() << "d" << "database",         "database name", "database"));
  parser.addOption(QCommandLineOption(QStringList() << "U" << "username", "application user name", "username"));

  parser.process(app);

  QString username     = parser.value("username");
  QString databaseURL  = parser.value("databaseURL");
  QString passwd       = parser.value("password");
  bool    enhancedAuth = parser.isSet("enhancedAuth");
  bool    evaluation   = parser.isSet("evaluation");

  ParameterList params;
  params.append("copyright",       Copyright);
  params.append("version",         Version);
  params.append("build",           Build.arg(__DATE__).arg(__TIME__));
  params.append("applicationName", ConnAppName);
  params.append("setSearchPath",   true);

  if (! username.isEmpty())
    params.append("username", username);

  if (! passwd.isEmpty())
    params.append("password", passwd);

  if (! databaseURL.isEmpty())
    params.append("databaseURL", databaseURL);

  if (enhancedAuth)
    params.append("enhancedAuth");

  if (evaluation)
    params.append("evaluation");

  if (!databaseURL.isEmpty() && !username.isEmpty() && !passwd.isEmpty())
    params.append("login");

  login2 newdlg(0, "", true);
  newdlg.set(params);

  if(newdlg.result() != QDialog::Accepted)
  {
    if (newdlg.exec() == QDialog::Rejected)
      return -1;

    databaseURL = newdlg._databaseURL;
    username    = newdlg.username();
    passwd      = newdlg.password();
  }

  Preferences   preferences;
  QScriptEngine engine;
  setupScriptApi(&engine, &preferences);

  QObject::connect(&app, SIGNAL(aboutToQuit()), &app, SLOT(closeAllWindows()));

  ScriptApiTestResults w(&engine);
  w.show();
  app.exec();

  return 0;
}
