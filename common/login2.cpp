/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "login2.h"

#include <QAction>
#include <QCursor>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSplashScreen>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStringList>
#include <QDesktopServices>
#include <QTimeZone>
#include <QUrl>
#include <QVariant>

#include "dbtools.h"
#include "errorReporter.h"
#include "qmd5.h"
#include "storedProcErrorLookup.h"
#include "xsqlquery.h"
#include "xtsettings.h"

#include "splashconst.h"
#include "cmdlinemessagehandler.h"
#include "guimessagehandler.h"

#define DEBUG false

class login2Private {
  public:
    bool _captive;
    bool _multipleConnections;
    bool _setSearchPath;
    QSplashScreen *_splash;
    QString _cUsername;
    QString _cPassword;
    QString _connAppName;
    QString _earliest;
    QString _latest;
    XAbstractMessageHandler* _handler;

    login2Private() :
      _captive(false),
      _multipleConnections(false),
      _setSearchPath(false),
      _splash(0),
      _handler(0)
    {
      _connAppName = "xTuple ERP (unknown)";
    };
};

login2::login2(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, modal ? (fl | Qt::Dialog) : fl)
{
  setObjectName(name);
  Q_INIT_RESOURCE(xTupleCommon);
  setupUi(this);
  _d = new login2Private();

  _recent = _buttonBox->addButton(tr("Recent"), QDialogButtonBox::ActionRole);
  _recent->setEnabled(false);
  _buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Login"));

  connect(_buttonBox, SIGNAL(accepted()), this, SLOT(sLogin()));
  connect(_buttonBox, SIGNAL(helpRequested()), this, SLOT(sOpenHelp()));
  connect(_server,               SIGNAL(editingFinished()), this, SLOT(sChangeURL()));
  connect(_database->lineEdit(), SIGNAL(editingFinished()), this, SLOT(sChangeURL()));
  connect(_port,                 SIGNAL(editingFinished()), this, SLOT(sChangeURL()));

  GuiMessageHandler *g = new GuiMessageHandler(this);
  g->setDestination(QtWarningMsg, this);
  g->setDestination(QtCriticalMsg, this);
  _d->_handler = g;

  _password->setEchoMode(QLineEdit::Password);

  _databaseURL = xtsettingsValue("/xTuple/_databaseURL", "pgsql://:5432/demo").toString();

  adjustSize();
}

login2::~login2()
{
  // no need to delete child widgets, Qt does it all for us
}

void login2::languageChange()
{
  retranslateUi(this);
}

int login2::set(const ParameterList &pParams, QSplashScreen *pSplash)
{
  _d->_splash = pSplash;

  QVariant param;
  bool     valid;

  param = pParams.value("username", &valid);
  if (valid)
  {
    _username->setText(param.toString());
    _password->setFocus();
    _d->_captive = true;
  }
  else
  {
    _username->setFocus();
  }

  param = pParams.value("password", &valid);
  if (valid)
  {
    _password->setText(param.toString());
    _d->_captive = true;
  }

  param = pParams.value("copyright", &valid);
  if (valid)
    _copyrightLit->setText(param.toString());

  param = pParams.value("version", &valid);
  if (valid)
    _versionLit->setText(tr("Version ") + param.toString());

  param = pParams.value("build", &valid);
  if (valid)
    _build->setText(param.toString());

  param = pParams.value("name", &valid);
  if (valid)
    _nameLit->setText(param.toString());

  param = pParams.value("earliest", &valid);
  if (valid)
    _d->_earliest = param.toString();

  param = pParams.value("latest", &valid);
  if (valid)
    _d->_latest = param.toString();

  param = pParams.value("databaseURL", &valid);
  if (valid)
    _databaseURL = param.toString();

  populateDatabaseInfo();

  param = pParams.value("multipleConnections", &valid);
  if (valid)
    _d->_multipleConnections = true;

  param = pParams.value("setSearchPath", &valid);
  if (valid)
    _d->_setSearchPath = true;

  param = pParams.value("applicationName", &valid);
  if (valid) {
    _d->_connAppName = param.toString().trimmed();
  }

  if(pParams.inList("cmd"))
  {
    delete _d->_handler;
    _d->_handler = new CmdLineMessageHandler(this);
  }

  if(pParams.inList("login"))
    sLogin();

  sChangeURL();
  updateRecentOptions();
  updateRecentOptionsActions();

  return 0;
}

void login2::sChangeURL()
{
  buildDatabaseURL(_databaseURL, "psql", _server->text(), _database->lineEdit()->text(), _port->text());

  _databaseURL.replace("https://", "");
  _databaseURL.replace("http://", "");
  populateDatabaseInfo();
}

void login2::sOpenHelp()
{
  QUrl url("http://www.xtuple.com/how-do-I-login-to-xTuple-PostBooks");
  QDesktopServices::openUrl(url);
}

void login2::sLogin()
{
  QSqlDatabase db;

  sChangeURL();

  QString hostName = _server->text();
  QString dbName   = _database->lineEdit()->text();
  QString port     = _port->text();

  if (_d->_splash)
  {
    _d->_splash->show();
    _d->_splash->raise();
    _d->_splash->showMessage(tr("Initializing the Database Connector"), SplashTextAlignment, SplashTextColor);
    qApp->processEvents();
  }

  if (_d->_multipleConnections)
    db = QSqlDatabase::addDatabase("QPSQL7", dbName);
  else
    db = QSqlDatabase::addDatabase("QPSQL7");
  if (!db.isValid())
  {
    if (_d->_splash)
      _d->_splash->hide();

    _d->_handler->message(QtWarningMsg, tr("<p>A connection could not be established with "
                                       "the specified Database as the Proper Database "
                                       "Drivers have not been installed. Contact your "
                                       "Systems Administator."));

    return;
  }

  if(hostName.isEmpty() || dbName.isEmpty())
  {
    if (_d->_splash)
      _d->_splash->hide();

    _d->_handler->message(QtWarningMsg, tr("<p>One or more connection options are missing. "
                                           "Please check that you have specified the host "
                                           "name, database name, and any other required "
                                           "options.") );

    return;
  }

  db.setDatabaseName(dbName);
  db.setHostName(hostName);
  db.setPort(port.toInt());

  _d->_cUsername = _username->text().trimmed();
  _d->_cPassword = _password->text().trimmed();

  setCursor(QCursor(Qt::WaitCursor));
  qApp->processEvents();

  if (_d->_splash)
  {
    _d->_splash->showMessage(tr("Connecting to the Database"), SplashTextAlignment, SplashTextColor);
    qApp->processEvents();
  }

  db.setUserName(_d->_cUsername);

  QRegExp xtuplecloud(".*\\.xtuplecloud\\.com");
  QRegExp xtuple(".*\\.xtuple\\.com");

  bool isCloud = xtuplecloud.exactMatch(hostName);
  bool isXtuple = xtuple.exactMatch(hostName);
  QString salt;

  if(isCloud || isXtuple)
  {
    salt = "j3H44uadEI#8#kSmkh#H%JSLAKDOHImklhdfsn3#432?%^kjasdjla3uy989apa3uipoweurw-03235##+=-lhkhdNOHA?%@mxncvbwoiwerNKLJHwe278NH28shNeGc";
  }
  else
  {
    salt = "xTuple";
  }

  QStringList connectionOptions;
  QStringList password;

  connectionOptions << QString("requiressl=1;application_name='%1'").arg(_d->_connAppName)
                    << "requiressl=1"
                    << QString("application_name='%1'").arg(_d->_connAppName)
                    << "";
  password << QMd5(QString(_d->_cPassword + salt  + _d->_cUsername))
           << _d->_cPassword
           << QMd5(QString(_d->_cPassword + "OpenMFG" + _d->_cUsername)) ;      // this must be last
  int passwordidx = 0; // not declared in for () because we need it later
  foreach (QString p, password)
  {
    foreach (QString options, connectionOptions)
    {
      if (DEBUG)
        qDebug() << options << passwordidx;
      db.setConnectOptions(options);
      db.setPassword(password.at(passwordidx));
      if (db.open())
        goto connected;
    }
    passwordidx++;
  }

connected:
  if (db.isOpen() && !_d->_earliest.isEmpty() && !_d->_latest.isEmpty())
  {
    XSqlQuery checkVersion;   // include earliest in the range but exclude latest
    checkVersion.prepare("SELECT compareVersion(:earliest) <= 0"
                         "   AND compareVersion(:latest)   > 0 AS ok,"
                         "       version() AS version;");
    checkVersion.bindValue(":earliest", _d->_earliest);
    checkVersion.bindValue(":latest",   _d->_latest);
    checkVersion.exec();
    if (checkVersion.first() && ! checkVersion.value("ok").toBool() &&
        _d->_handler->question(tr("<p>The database server is at version %1 but "
                                "xTuple ERP only supports from %2 up to but "
                                "not including %3.</p><p>Continue anyway?</p>")
                               .arg(checkVersion.value("version").toString(),
                                    _d->_earliest, _d->_latest),
                           QMessageBox::Yes | QMessageBox::No,
                           QMessageBox::No) == QMessageBox::No) {
      if (_d->_splash) {
        _d->_splash->hide();
      }
      setCursor(QCursor(Qt::ArrowCursor));
      db.close();
      if (! _d->_captive) {
        _username->setText("");
        _username->setFocus();
      } else
        _password->setFocus();

      _password->setText("");
      return;
    } else if (checkVersion.lastError().type() != QSqlError::NoError) {
      _d->_handler->message(QtCriticalMsg, checkVersion.lastError().text());

      if (_d->_splash)
        _d->_splash->hide();
      db.close();
      if (! _d->_captive) {
        _username->setText("");
        _username->setFocus();
      } else
        _password->setFocus();

      _password->setText("");
      return;
    }
  }

   // if connected using OpenMFG enhanced auth, remangle the password
  if (db.isOpen() && passwordidx >= (password.size() - 1))
    XSqlQuery chgpass(QString("ALTER USER \"%1\" WITH PASSWORD '%2'")
                      .arg(_d->_cUsername, password.at(0)));

  if (! db.isOpen())
  {
    if (_d->_splash)
      _d->_splash->hide();

    setCursor(QCursor(Qt::ArrowCursor));

    _d->_handler->message(QtCriticalMsg, tr("<p>Sorry, can't connect to the specified xTuple ERP server:</p>"
                                            "<pre>%1</pre>" ).arg(db.lastError().text()));
    _password->setFocus();
    _password->setText("");
    return;
  }

  if (_d->_splash)
  {
    _d->_splash->showMessage(tr("Logging into the Database"), SplashTextAlignment, SplashTextColor);
    qApp->processEvents();
  }

  QString errormsg;
  XSqlQuery checkSchema("SELECT COUNT(*) AS tablecount"
                        "  FROM pg_class AS c"
                        "  JOIN pg_namespace AS n ON n.oid = c.relnamespace"
                        "  WHERE n.nspname = 'public'"
                        "    AND c.relname IN ('metric', 'cohead', 'bankaccnt');"
                      );
  if (! checkSchema.first() || checkSchema.value("tablecount").toInt() != 3)
  {
    errormsg = tr("<p>'%1' does not appear to be a valid xTuple database.</p>"
                  "<p>Please check the database name and try again.</p>"
                 ).arg(dbName);
  }
  else
  {
    XSqlQuery login;
    login.prepare("SELECT login(:setpath) AS result, CURRENT_USER AS user;");
    login.bindValue(":setpath", _d->_setSearchPath);

    if (login.exec() && login.first())
    {
      _user = login.value("user").toString();
      int result = login.value("result").toInt();
      if (result < 0)
      {
        errormsg = storedProcErrorLookup("login", result);
      }
      else
      {
        XSqlQuery time;
        time.prepare("SET TIME ZONE :timezone;");
        time.bindValue(":timezone", QString(QTimeZone::systemTimeZoneId()));
        time.exec();
        if (time.lastError().type() != QSqlError::NoError)
        {
          errormsg = tr("Could not set the time zone: %1")
                       .arg(time.lastError().databaseText());
        }
        else if (login.exec("SELECT getEffectiveXtUser() AS user;") &&
                 login.first() &&
                 ! login.value("user").toString().isEmpty())
        {
          _user = login.value("user").toString();
        } // no error check - older databases don't have getEffectiveXtUser
      }
    }
    else if (login.lastError().type() != QSqlError::NoError)
    {
      errormsg = tr("Could not set up database session properly: %1")
                   .arg(login.lastError().databaseText());
    }
  }

  if (! errormsg.isEmpty())
  {
    setCursor(QCursor(Qt::ArrowCursor));
    if (_d->_splash)
      _d->_splash->hide();
    _d->_handler->message(QtCriticalMsg, errormsg);
    return;
  }

  updateRecentOptions();
  accept();
}

void login2::populateDatabaseInfo()
{
  QString protocol;
  QString hostName;
  QString dbName;
  QString port;

  parseDatabaseURL(_databaseURL, protocol, hostName, dbName, port);
  _server->setText(hostName);
  _database->lineEdit()->setText(dbName);
  _port->setText(port);
}

QString login2::username()
{
  return _d->_cUsername;
}

QString login2::password()
{
  return _d->_cPassword;
}

void login2::setLogo(const QImage & img)
{
  if(img.isNull())
    _logo->setPixmap(QPixmap(":/login/images/splashXTuple.png"));
  else
    _logo->setPixmap(QPixmap::fromImage(img));
}

void login2::updateRecentOptions()
{
  QStringList list = xtsettingsValue("/xTuple/_recentOptionsList").toStringList();
  _recent->setEnabled(list.size());
  list.removeAll(_databaseURL);
  list.prepend(_databaseURL);

  xtsettingsSetValue("/xTuple/_recentOptionsList", list);
  xtsettingsSetValue("/xTuple/_databaseURL", _databaseURL);
}

void login2::updateRecentOptionsActions()
{
  QMenu * recentMenu = new QMenu;
  QStringList list = xtsettingsValue("/xTuple/_recentOptionsList").toStringList();
  if (! list.isEmpty())
  {
    list.takeFirst();

    QString protocol;
    QString hostName;
    QString dbName;
    QString port;

    _recent->setEnabled(true);
    QAction *act;
    for (int i = 0; i < list.size() && i < 10; ++i)
    {
      act = new QAction(list.value(i).remove("psql://"), this);
      connect(act, SIGNAL(triggered()), this, SLOT(selectRecentOptions()));
      recentMenu->addAction(act);
      parseDatabaseURL(list.value(i), protocol, hostName, dbName, port);
      if (_database->findText(dbName) == -1)
        _database->addItem(dbName);
    }

    recentMenu->addSeparator();

    act = new QAction(tr("Clear &Menu"), this);
    act->setObjectName(QLatin1String("__xt_action_clear_menu_"));
    connect(act, SIGNAL(triggered()), this, SLOT(clearRecentOptions()));
    recentMenu->addAction(act);
  }
  _recent->setEnabled(! list.isEmpty());
  _recent->setMenu(recentMenu);
}

void login2::selectRecentOptions()
{
  if (const QAction *action = qobject_cast<const QAction *>(sender()))
  {
    _databaseURL = action->iconText().prepend("psql://");
    populateDatabaseInfo();
    updateRecentOptions();
    updateRecentOptionsActions();
  }
}

void login2::clearRecentOptions()
{
    QStringList list;
    xtsettingsSetValue("/xTuple/_recentOptionsList", list);
    updateRecentOptionsActions();
}
