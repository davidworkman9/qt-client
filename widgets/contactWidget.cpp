/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include <QDebug>
#include <QDesktopServices>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlError>
#include <QUrl>
#include <QVBoxLayout>
#include <QtScript>

#include <metasql.h>

#include "xsqlquery.h"

#include "contactemail.h"
#include "contactwidget.h"
#include "errorReporter.h"

#define FAKEEMAILID -999

void ContactWidget::init()
{
    _ignoreSignals = false;

    _titleSingular = tr("Contact");
    _titlePlural = tr("Contacts");
    _query = "SELECT cntct.* "
             "FROM cntct()";

    _layoutDone = false;
    _minimalLayout = false;
    _searchAcctId = -1;

    _list = new QPushButton(tr("..."), this);
    _list->setObjectName("_list");
    _list->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  #ifndef Q_OS_MAC
    _list->setMaximumWidth(25);
  #else
    _list->setMinimumWidth(60);
    _list->setMinimumHeight(32);
  #endif

    _grid->removeWidget(_label);	// will be reinserted
    _grid->removeWidget(_description);
    delete _description;
    _description = 0;

    _grid->setMargin(0);
    _grid->setSpacing(2);

    _nameBox		= new QHBoxLayout;
    _nameBox->setSpacing(2);
    _initialsBox	= new QHBoxLayout;
    _initialsBox->setSpacing(2);
    _titleBox		= new QHBoxLayout;
    _titleBox->setSpacing(2);

    _change             = new XLineEdit(this);
    _numberLit		= new QLabel(tr("Number:"), this);
    _numberLit->setObjectName("_numberLit");
    _number		= new XLineEdit(this, "_number");

    _change->hide();
    _number->hide();
    _numberLit->hide();

    _nameLit		= new QLabel(tr("Name:"), this);
    _nameLit->setObjectName("_nameLit");
    _honorific		= new XComboBox(this, "_honorific");
    _first		= new XLineEdit(this, "_first");
    _middle		= new XLineEdit(this, "_middle");
    _middle->setFixedWidth(20);
    _last		= new XLineEdit(this, "_last");
    _suffix		= new XLineEdit(this, "_suffix");
    _suffix->setFixedWidth(30);
    _initialsLit	= new QLabel(tr("Initials:"), this);
    _initialsLit->setObjectName("_initialsLit");
    _initials		= new XLineEdit(this, "_initials");
    _initials->setFixedWidth(_initials->size().width() / 3);
    _titleLit		= new QLabel(tr("Job Title:"), this);
    _titleLit->setObjectName("_titleLit");
    _title		= new XLineEdit(this, "_title");

    _mapper		= new XDataWidgetMapper(this);

    //_nameBox->addWidget(_nameLit,	0);
    _nameBox->addWidget(_honorific,	0);
    _nameBox->addWidget(_first,		1);
    _nameBox->addWidget(_middle,	0);
    _nameBox->addWidget(_last,		2);
    _nameBox->addWidget(_suffix,	0);
    _nameBox->addWidget(_list,		0, Qt::AlignRight);

    //_initialsBox->addWidget(_initialsLit, 0);
    _initialsBox->addWidget(_initials,	  0);
    _initialsBox->addStretch(0);

    //_titleBox->addWidget(_titleLit,	0);
    _titleBox->addWidget(_title,	2);

    _buttonBox 		= new QGridLayout;
    _active		= new QCheckBox(tr("Active"), this);
    _active->setObjectName("_active");
    _owner              = new UsernameCluster(this, "_owner");

    _buttonBox->addWidget(_owner, 	0, 2, Qt::AlignTop);

    _phoneGrid 		= new QGridLayout;

    QRegExp rx("^([0-9a-zA-Z]+[-._+&amp;])*[0-9a-zA-Z]+@([-0-9a-zA-Z]+[.])+[a-zA-Z]{2,6}$");
    QValidator *validator = new QRegExpValidator(rx, this);

    _emailBox		= new QHBoxLayout;
    _emailBox->setSpacing(2);
    _emailLit		= new QLabel(tr("E-Mail:"), this);
    _emailLit->setObjectName("_emailLit");
    _email		= new XComboBox(this, "_email");
    _email->setMinimumWidth(110);
    _email->setEditable(true);
    _email->setValidator(validator);
    _email->lineEdit()->installEventFilter(this);
    _email->insertEditor(XComboBox::Adhoc, this, SLOT(sEditEmailList()),
                         "MaintainPersonalContacts MaintainAllContacts");
    _emailopt           = new QCheckBox(tr("Opt In"), this);
    _emailopt->setObjectName("_emailopt");

    if (_x_metrics)
      _emailopt->setChecked(_x_metrics->boolean("DefaultEmailOptIn"));

    _emailBox->addWidget(_email, 2);
    _emailBox->addWidget(_emailopt, 0);
  
    _webaddrLit		= new QLabel(tr("Web:"), this);
    _webaddrLit->setObjectName("_webaddrLit");
    _webaddr		= new XLineEdit(this, "_webaddr");
    _address		= new AddressCluster(this, "_address");

    //So we can manipulate just the line edit
    QLineEdit* emailEdit = _email->lineEdit();

#if defined Q_OS_MAC
    _honorific->setMinimumHeight(26);
    _first->setMinimumHeight(22);
    _middle->setMinimumHeight(22);
    _last->setMinimumHeight(22);
    _suffix->setMinimumHeight(22);
#endif

    QPalette p = _email->palette();
    p.setColor(QPalette::Text, Qt::blue);
    emailEdit->setPalette(p);
    _webaddr->setPalette(p);

    QFont newFont = _email->font();
    newFont.setUnderline(true);
    emailEdit->setFont(newFont);
    _webaddr->setFont(newFont);

    _numberLit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    _label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    _nameLit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    _titleLit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    _emailLit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    _initialsLit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    _webaddrLit->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    _honorific->setEditable(true);
    _honorific->setType(XComboBox::Honorifics);

    _owner->setLabel(tr("Owner:"));

    layout();

    connect(_list,	SIGNAL(clicked()),	this, SLOT(sEllipses()));

    connect(_honorific,	SIGNAL(newID(int)),		     this, SIGNAL(changed()));
    connect(_first,	SIGNAL(textChanged(const QString&)), this, SIGNAL(changed()));
    connect(_middle,	SIGNAL(textChanged(const QString&)), this, SIGNAL(changed()));
    connect(_last,	SIGNAL(textChanged(const QString&)), this, SIGNAL(changed()));
    connect(_suffix,	SIGNAL(textChanged(const QString&)), this, SIGNAL(changed()));
    connect(_initials,	SIGNAL(textChanged(const QString&)), this, SIGNAL(changed()));
    connect(_title,	SIGNAL(textChanged(const QString&)), this, SIGNAL(changed()));
    connect(_email,	SIGNAL(currentTextChanged(const QString&)), this, SIGNAL(changed()));
    connect(_webaddr,	SIGNAL(textChanged(const QString&)), this, SIGNAL(changed()));
    connect(_address,	SIGNAL(changed()),                   this, SIGNAL(changed()));
    connect(this,       SIGNAL(changed()),                   this, SLOT(setChanged()));

    connect(_honorific,	SIGNAL(newID(int)),  this, SLOT(sCheck()));
    connect(_first,	SIGNAL(editingFinished()), this, SLOT(sCheck()));
    connect(_middle,	SIGNAL(editingFinished()), this, SLOT(sCheck()));
    connect(_last,	SIGNAL(editingFinished()), this, SLOT(sCheck()));
    connect(_suffix,	SIGNAL(editingFinished()), this, SLOT(sCheck()));
    connect(_initials,	SIGNAL(editingFinished()), this, SLOT(sCheck()));
    connect(_title,	SIGNAL(editingFinished()), this, SLOT(sCheck()));
    connect(emailEdit,	SIGNAL(editingFinished()), this, SLOT(sCheck()));
    connect(_webaddr,	SIGNAL(editingFinished()), this, SLOT(sCheck()));
    connect(_address,	SIGNAL(changed()),   this, SLOT(sCheck()));

    connect(_webaddr,   SIGNAL(doubleClicked()), this, SLOT(sLaunchWebaddr()));

    setListVisible(true);

    connect(_first, SIGNAL(editingFinished()), this, SLOT(findDuplicates()));
    connect(_last, SIGNAL(editingFinished()), this, SLOT(findDuplicates()));

    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(_honorific);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setLabel("");
    silentSetId(-1);
    setOwnerVisible(false);
    _mode = Edit;
}

ContactWidget::ContactWidget(QWidget* pParent, const char* pName) :
    VirtualCluster(pParent, pName)
{
    init();
}

void ContactWidget::findDuplicates()
{
  if (_first->text().isEmpty() && _last->text().isEmpty())
    return;

  QString msg;
  XSqlQuery r;

  r.prepare(  "SELECT cntct_id, COALESCE(crmacctcntctass_crmacct_id,0) AS crmacct_id "
              "FROM cntct "
              " JOIN crmacctcntctass ON (crmacctcntctass_cntct_id=cntct_id) "
              "WHERE cntct_first_name ~~* :first "
              " AND  cntct_last_name ~~* :last; ");
  r.bindValue(":first", _first->text());
  r.bindValue(":last", _last->text());
  r.exec();
  if (r.size() == 1)
  {
    r.first();

    QString msgpart;
    if (_searchAcctId > 0 && r.value("crmacct_id").toInt() == 0)
      msgpart = QString(" not associated with any Account");
    else if (_searchAcctId == r.value("crmacct_id").toInt())
      msgpart = QString(" on the current Account");
    else if (_searchAcctId > 0)
      msgpart = QString(" associated with another Account");
    msg = tr("A contact exists with the same first and last name %1. "
                  "Would you like to use the existing contact?")
                  .arg(msgpart);

    if (QMessageBox::question(this, tr("Existing Contact"), msg,
                             QMessageBox::Yes | QMessageBox::Default,
                             QMessageBox::No  | QMessageBox::Escape) == QMessageBox::Yes)
    {
      r.first();
      setId(r.value("cntct_id").toInt());
    }
  }
  else if (r.size() > 1)
  {
    if (_searchAcctId > 0)
    {
      int cnt = 0;
      int cntctid = 0;
      while (r.next())
      {
        if (r.value("crmacct_id").toInt() == _searchAcctId)
        {
          cnt += 1;
          cntctid = r.value("cntct_id").toInt();
        }
      }
      if (cnt == 1)
      {
         msg = tr("A contact exists with the same first and last name "
                  "on the current Account. "
                  "Would you like to use the existing contact?");
        if (QMessageBox::question(this, tr("Existing Contact"), msg,
                             QMessageBox::Yes | QMessageBox::Default,
                             QMessageBox::No  | QMessageBox::Escape) == QMessageBox::Yes)
          setId(cntctid);
        return;
      }
      if (cnt == 0)
        _searchAcctId = -1;
    }

    QString msg = tr("Multple contacts exist with the same first and last name");
    msg += tr(". Would you like to view all the existing contacts?");

    if (QMessageBox::question(this, tr("Existing Contacts"), msg,
                             QMessageBox::Yes | QMessageBox::Default,
                             QMessageBox::No  | QMessageBox::Escape) == QMessageBox::Yes)
    {
      ContactSearch* newdlg = new ContactSearch(this);
      if (newdlg)
      {
        newdlg->_searchFirst->setChecked(true);
        newdlg->_searchLast->setChecked(true);
        newdlg->_search->setText(_first->text() + " " + _last->text());
        newdlg->sFillList();
	      int id = newdlg->exec();
	      setId(id);
      }
      else
	       QMessageBox::critical(this, tr("A System Error Occurred at %1::%2.")
			      .arg(__FILE__)
				    .arg(__LINE__),
			    tr("Could not instantiate a Search Dialog"));
    }
  }
  else if (r.lastError().type() != QSqlError::NoError)
    QMessageBox::critical(this, tr("A System Error Occurred at %1::%2.")
                                        .arg(__FILE__)
                                        .arg(__LINE__),
                                r.lastError().databaseText());

}

void ContactWidget::setChanged()
{
  _changed=true;
}

void ContactWidget::setId(const int pId, const QString&)
{
    if (pId == _id)
	return;
    else
    {
	silentSetId(pId);
	emit newId(pId);
    }
}

void ContactWidget::silentSetId(const int pId)
{
  disconnect(_first, SIGNAL(editingFinished()), this, SLOT(findDuplicates()));
  disconnect(_last, SIGNAL(editingFinished()), this, SLOT(findDuplicates()));
  if (pId == -1 || pId == 0)
  {
    _id = -1;
    _valid = false;
    connect(_first, SIGNAL(editingFinished()), this, SLOT(findDuplicates()));
    connect(_last, SIGNAL(editingFinished()), this, SLOT(findDuplicates()));
    clear();
    return;
  }
  else if (pId == _id)
    return;
  else
  {
      XSqlQuery idQ;
      _query.replace("cntct()","cntct"); // Switch to non-restrictive table
      idQ.prepare(_query + " WHERE cntct_id = :id;");
      _query.replace("FROM cntct", "FROM cntct()"); // Switch back to restrictive table function
      idQ.bindValue(":id", pId);
      idQ.exec();
      if (idQ.first())
      {
          _ignoreSignals = true;

          _id = pId;

          _valid = true;
          _number->setText(idQ.value("cntct_number").toString());
          _honorific->setEditText(idQ.value("cntct_honorific").toString());
          _first->setText(idQ.value("cntct_first_name").toString());
          _middle->setText(idQ.value("cntct_middle").toString());
          _last->setText(idQ.value("cntct_last_name").toString());
          _suffix->setText(idQ.value("cntct_suffix").toString());
          _initials->setText(idQ.value("cntct_initials").toString());
          _title->setText(idQ.value("cntct_title").toString());
          _emailCache=idQ.value("cntct_email").toString();
          _email->append(FAKEEMAILID, idQ.value("cntct_email").toString()); // TODO: this is rotten
          _emailopt->setChecked(idQ.value("cntct_email_optin").toBool());
          _webaddr->setText(idQ.value("cntct_webaddr").toString());
          _address->setId(idQ.value("cntct_addr_id").toInt());
          _active->setChecked(idQ.value("cntct_active").toBool());
          _notes = idQ.value("cntct_notes").toString();
          _owner->setUsername(idQ.value("cntct_owner_username").toString());

          sBuildPhones();

          fillEmail();

          if (_mapper->model())
          { 	 
            _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_number)),       _number->text());
            _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_honorific)),    _honorific->currentText());
            _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_first)),        _first->text());
            _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_last)),         _last->text());
            _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_initials)),     _initials->text());
            _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(this)),          _number->text());
            _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_title)),        _title->text());
            _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_email)),        _email->currentText());
            _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_webaddr)),      _webaddr->text());
           }

          _ignoreSignals = false;
      }
      else if (idQ.lastError().type() != QSqlError::NoError)
          QMessageBox::critical(this, tr("A System Error Occurred at %1::%2.")
                                        .arg(__FILE__)
                                        .arg(__LINE__),
                                idQ.lastError().databaseText());
    _valid = true;
  }

  _changed=false;
  // _parsed = true;
}

void ContactWidget::setListVisible(bool p)
{
  _first->disconnect();
  _middle->disconnect();
  _last->disconnect();
  _suffix->disconnect();
  _first->disconnect();
  _middle->disconnect();
  _last->disconnect();
  _suffix->disconnect();

  _list->setVisible(p);
  if (p)
  {
    connect(_first, SIGNAL(requestSearch()),	this, SLOT(sSearch()));
    connect(_middle, SIGNAL(requestSearch()),	this, SLOT(sSearch()));
    connect(_last, SIGNAL(requestSearch()),	this, SLOT(sSearch()));
    connect(_suffix, SIGNAL(requestSearch()),	this, SLOT(sSearch()));
    connect(_first, SIGNAL(requestList()),	this, SLOT(sList()));
    connect(_middle, SIGNAL(requestList()),	this, SLOT(sList()));
    connect(_last, SIGNAL(requestList()),	this, SLOT(sList()));
    connect(_suffix, SIGNAL(requestList()),	this, SLOT(sList()));
  }
}

void ContactWidget::setNumber(QString p)
{
  if (p == _number->text())
    return;

  _number->setText(p);
  XSqlQuery q;
  q.prepare("SELECT cntct_id "
              "FROM cntct "
              "WHERE (cntct_number=:number);");
  q.bindValue(":number", p);
  q.exec();
  if (q.first())
  {
    if (q.value("cntct_id").toInt() != _id)
      silentSetId(q.value("cntct_id").toInt());
  }
  else
    _number->setText(p);
}

void ContactWidget::clear()
{
  _id = -1;
  _crmacctid = -1;
  _valid = false;

  _honorific->clearEditText();
  _first->clear();
  _middle->clear();
  _last->clear();
  _suffix->clear();
  _initials->clear();
  _title->clear();
  _email->clear();
  
  if (_x_metrics)
    _emailopt->setChecked(_x_metrics->boolean("DefaultEmailOptIn"));

  _webaddr->clear();
  _address->clear();
  _active->setChecked(true);
  _notes = "";
  _number->clear();
}

QString ContactWidget::name() const
{
  QString name;

  if (!_first->text().isEmpty())
    name = _first->text();

  if (!_middle->text().isEmpty())
  {
    if (!name.isEmpty())
      name += " ";
    name += _middle->text();
  }

  if (!_last->text().isEmpty())
  {
    if (!name.isEmpty())
      name += " ";
    name += _last->text();
  }

  return name;
}

void ContactWidget::setDataWidgetMap(XDataWidgetMapper* m)
{
  m->addMapping(this          ,  _fieldNameNumber,        "number", "defaultText");
  m->addMapping(_change      ,  _fieldNameChange,         "text",   "defaultText");
  m->addMapping(_active      ,  _fieldNameActive);
  m->addMapping(_first       ,  _fieldNameFirst,          "text",   "defaultText");
  m->addMapping(_middle      ,  _fieldNameMiddle,         "text",   "defaultText");
  m->addMapping(_last        ,  _fieldNameLast,           "text",   "defaultText");
  m->addMapping(_suffix      ,  _fieldNameSuffix,         "text",   "defaultText");
  m->addMapping(_initials    ,  _fieldNameInitials,       "text",   "defaultText");
  m->addMapping(_title       ,  _fieldNameTitle,          "text",   "defaultText");
  m->addMapping(_email       ,  _fieldNameEmailAddress,   "text",   "defaultText");
  m->addMapping(_webaddr     ,  _fieldNameWebAddress,     "text",   "defaultText");
  _honorific->setFieldName(_fieldNameHonorific);
  _honorific->setDataWidgetMap(m);
  _address->setFieldNameAddrChange(_fieldNameAddrChange);
  _address->setFieldNameNumber(_fieldNameAddrNumber);
  _address->setFieldNameLine1(_fieldNameLine1);
  _address->setFieldNameLine2(_fieldNameLine2);
  _address->setFieldNameLine3(_fieldNameLine3);
  _address->setFieldNameCity(_fieldNameCity);
  _address->setFieldNameState(_fieldNameState);
  _address->setFieldNamePostalCode(_fieldNamePostalCode);
  _address->setFieldNameCountry(_fieldNameCountry);
  _address->setDataWidgetMap(m);
  _mapper=m;
}


void ContactWidget::setName(int, const QString& p)
{
  int firstSpace = p.indexOf(" ");
  if (firstSpace > 0)
  {
    setFirst(p.left(firstSpace));
    setLast(p.right(p.length() - firstSpace - 1));
  }
  else
    setLast(p);
}

/*
  returns  -1 if error
  	   -2 if address is shared
	  -10 if unclear if intent is to overwrite or create new contact
 */
int ContactWidget::save(AddressCluster::SaveFlags flag)
{
  int addrSave = _address->save(flag);

  QString phonelist = sBuildPhoneJson();

  if (addrSave < 0)
    return addrSave;

  XSqlQuery datamodQ;
  datamodQ.prepare("SELECT COALESCE(saveCntct(:cntct_id,:cntct_number,:addr_id,"
		   ":honorific,:first,:middle,:last,:suffix,:initials,"
		   ":active,:phone,:email,:webaddr,"
                   ":notes,:title,:flag,:owner, :emailopt),0) AS result;");
  datamodQ.bindValue(":cntct_number", _number->text());
  datamodQ.bindValue(":cntct_id",  id());
  datamodQ.bindValue(":honorific", _honorific->currentText());
  if (_address->id() > 0)
    datamodQ.bindValue(":addr_id", _address->id());
  datamodQ.bindValue(":first",	   _first->text());
  datamodQ.bindValue(":middle",	   _middle->text());
  datamodQ.bindValue(":last",	   _last->text());
  datamodQ.bindValue(":suffix",	   _suffix->text());
  datamodQ.bindValue(":initials",  _initials->text());
  datamodQ.bindValue(":title",	   _title->text());
  datamodQ.bindValue(":phone",	   phonelist);
  datamodQ.bindValue(":email",	   _email->currentText());
  datamodQ.bindValue(":webaddr",   _webaddr->text());
  datamodQ.bindValue(":notes",	   _notes);
  datamodQ.bindValue(":owner",     _owner->username());
  datamodQ.bindValue(":emailopt",  _emailopt->isChecked());
  if (flag == AddressCluster::CHECK)
    datamodQ.bindValue(":flag", QString("CHECK"));
  else if (flag == AddressCluster::CHANGEALL)
    datamodQ.bindValue(":flag", QString("CHANGEALL"));
  else if (flag == AddressCluster::CHANGEONE)
    datamodQ.bindValue(":flag", QString("CHANGEONE"));
  else
    return -1;
  datamodQ.bindValue(":active",    QVariant(_active->isChecked()));
  datamodQ.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Saving Contact"),
                           datamodQ, __FILE__, __LINE__))
    return -99;
  else if (datamodQ.first())
  {
    if (datamodQ.value("result").toInt() == 0)
      return 0;
    else if (datamodQ.value("result").toInt() > 0)
      if (_mapper->model())
      {
        _id=datamodQ.value("result").toInt();
        _changed=false;
      }
      else
        silentSetId(datamodQ.value("result").toInt());
    else if (datamodQ.value("result").toInt() == -10)
      return -10;
  }
  else
    return -1;
  return id();
}

void ContactWidget::setAccount(const int p)
{
  if (_crmacctid != p)
  {
    _crmacctid = p;
    _changed = true;
  }
}

void ContactWidget::setOwnerVisible(const bool vis)
{
  _owner->setVisible(vis);
  layout();
}

void ContactWidget::setOwnerEnabled(const bool vis)
{
  _owner->setEnabled(vis);
}

void ContactWidget::setNumberVisible(const bool vis)
{
  _number->setVisible(vis);
  _numberLit->setVisible(vis);
  layout();
}

void ContactWidget::setAddressVisible(const bool vis)
{
  _address->setVisible(vis);
  layout();
}

void ContactWidget::setActiveVisible(const bool p)
{
  _active->setVisible(p);
  layout();
}

void ContactWidget::setEmailVisible(const bool p)
{
  _emailLit->setVisible(p);
  _email->setVisible(p);
  _emailopt->setVisible(p);
  // layout();
}

void ContactWidget::setInitialsVisible(const bool p)
{
  _initialsLit->setVisible(p);
  _initials->setVisible(p);
  layout();
}

void ContactWidget::setPhonesVisible(const bool p)
{
  // TODO Loop through _phoneGrid children and set visibility
  _phoneGrid->setEnabled(p);
  layout();
}

void ContactWidget::setWebaddrVisible(const bool p)
{
  _webaddrLit->setVisible(p);
  _webaddr->setVisible(p);
  // layout();
}

void ContactWidget::setMinimalLayout(const bool p)
{

  // always visible
  _label->setVisible(true);
  _nameLit->setVisible(true);
  _honorific->setVisible(true);
  _first->setVisible(true);
  _middle->setVisible(true);
  _last->setVisible(true);
  _title->setVisible(true);
  _emailLit->setVisible(true);
  _email->setVisible(true);
  _emailopt->setVisible(true);

  // at the mercy of minimal layout
  _initialsLit->setVisible(! p);
  _initials->setVisible(! p);
  _active->setVisible(! p);
  _webaddrLit->setVisible(! p);
  _webaddr->setVisible(! p);
  _address->setVisible(! p);

  _minimalLayout = p;
  layout();
}

void ContactWidget::layout()
{
  QLayout* pwl = 0;
  if(parentWidget())
    pwl = parentWidget()->layout();
  QLayout* currLayout = (_layoutDone) ? _grid : pwl;

  if (_layoutDone)
  {
    currLayout->removeItem(_nameBox);
    currLayout->removeItem(_initialsBox);
    currLayout->removeItem(_buttonBox);
    currLayout->removeItem(_titleBox);
  }

  if (currLayout)
  {
    currLayout->removeWidget(_label);
    currLayout->removeItem(_nameBox);
    currLayout->removeItem(_buttonBox);
    currLayout->removeItem(_titleBox);
    currLayout->removeWidget(_emailLit);
    currLayout->removeItem(_emailBox);
    currLayout->removeWidget(_webaddrLit);
    currLayout->removeWidget(_webaddr);
    currLayout->removeWidget(_address);
    currLayout->removeItem(_phoneGrid);
  }

  _grid->addWidget(_label,	0, 0, 1, -1);
  //_grid->addItem(_numberBox,    1, 0, 1, -1);  Possible implement at a later time
  _grid->addWidget(_nameLit,    1, 0, 1, 1);
  _grid->addItem(_nameBox,	1, 1, 1, -1);
  _grid->addWidget(_initialsLit,2, 0, 1, 1);
  _grid->addItem(_initialsBox,  2, 1, 1, -1);
  _grid->addWidget(_active,     2, 2, 1, -1);
  _grid->addItem(_buttonBox,	3, 0, 1, -1);
  _grid->addWidget(_titleLit,   4, 0, 1, 1);
  _grid->addItem(_titleBox,	4, 1, 1, -1);

  if (_minimalLayout)
  {
    _grid->setColumnStretch(0, 0);
    _grid->setColumnStretch(1, 1);
    _grid->setColumnStretch(2, 0);
    _grid->setColumnStretch(3, 1);
    _grid->setColumnStretch(4, 0);
    _grid->setColumnStretch(5, 2);

    _grid->addWidget(_emailLit, 5, 0);
    _grid->addItem(_emailBox,	5, 1, 1, 2);
    _grid->addWidget(_webaddrLit, 5, 3);
    _grid->addWidget(_webaddr,    5, 4);
    _grid->addWidget(_address,    6, 1); //, -1, -1);
  }
  else if (_address->isVisibleTo(this))
  {
    _grid->setColumnStretch(0, 0);
    _grid->setColumnStretch(1, 1);
    _grid->setColumnStretch(2, 3);

    _grid->addWidget(_address,		5, 2, 5, -1);
    _grid->addWidget(_emailLit,         5, 0);
    _grid->addItem(_emailBox,	        5, 1, 1, 1);
    _grid->addWidget(_webaddrLit,	6, 0);
    _grid->addWidget(_webaddr,		6, 1);
    _grid->addItem(_phoneGrid,          7, 0, -1, 2);
  }
  else
  {
    _grid->setColumnStretch(0, 0);
    _grid->setColumnStretch(1, 1);
    _grid->setColumnStretch(2, 0);
    _grid->setColumnStretch(3, 2);
    _grid->setColumnStretch(4, 0);
    _grid->setColumnStretch(5, 2);

    _grid->addWidget(_webaddrLit,	6, 4);
    _grid->addWidget(_webaddr,		5, 5);
    _grid->addWidget(_emailLit,         7, 0);
    _grid->addItem(_emailBox,	        7, 1, 1, 2);
    _grid->addWidget(_address,		8, 0, 1, -1);
    _grid->addItem(_phoneGrid,          9, 0, -1, 2);
  }

#if defined Q_OS_MAC
  setMinimumSize(_grid->columnCount() * 50, _grid->rowCount() * 11);
#endif
  _layoutDone = true;
}

void ContactWidget::setSearchAcct(const int p)
{
  _searchAcctId = p;
  addressWidget()->setSearchAcct(p);
}

void ContactWidget::check()
{
  QString oldNumber = _number->text();
  XSqlQuery tx;
  tx.exec("BEGIN;");
  int result=save();
  tx.exec("ROLLBACK;");
  _number->setText(oldNumber);
  if (result == -2)
  {
    int answer = 2;	// Cancel
    answer = QMessageBox::question(this, tr("Question Saving Address"),
                tr("There are multiple Contacts sharing this Address.\n"
                   "What would you like to do?"),
                tr("Change This One"),
                tr("Change Address for All"),
                0, 0);
     if (answer==0)
     {
       _address->setAddrChange(QString("CHANGEONE"));
       _change->setText("CHANGEONE");
     }
     else if (answer==1)
     {
       _address->setAddrChange(QString("CHANGEALL"));
       _change->setText("CHANGEALL");
     }
  }
  else if (result == -10)
  {
    int answer;
    answer = QMessageBox::question(this,
                tr("Question Saving %1").arg(label()),
                tr("<p>Would you like to update the existing Contact or "
                   "create a new one?"),
                tr("Create New"),
                tr("Change Existing"),
                0, 0);
    if (0 == answer)
      _change->setText("CHANGEONE");
    else if (1 == answer)
      _change->setText("CHANGEALL");

    // Make sure the mapper is aware of this change
    if (_mapper && _mapper->model() &&
        (_mapper->model()->data(_mapper->model()->index(_mapper->currentIndex(),
                                                        _mapper->mappedSection(_change))).toString() != _change->text()))
        _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),
                                                          _mapper->mappedSection(_change)),
                                  _change->text());
  }
  else if (result < 0)
  {
    QMessageBox::critical(this, tr("Error"),tr("There was an error checking this Contact (%1).").arg(result));
    return;
  }
}

void ContactWidget::sCheck()
{
  if(_ignoreSignals)
    return;

  if ((! _honorific->isVisibleTo(this) || _honorific->currentText().simplified().isEmpty()) &&
      (! _first->isVisibleTo(this)   || _first->text().simplified().isEmpty()) &&
      (! _middle->isVisibleTo(this)   || _middle->text().simplified().isEmpty()) &&
      (! _last->isVisibleTo(this)    || _last->text().simplified().isEmpty()) &&
      (! _suffix->isVisibleTo(this)   || _suffix->text().simplified().isEmpty()) &&
      (! _initials->isVisibleTo(this) || _initials->text().simplified().isEmpty()) &&
      (! _title->isVisibleTo(this)   || _title->text().simplified().isEmpty()) &&
      (! _email->isVisibleTo(this)   || _email->currentText().simplified().isEmpty()) &&
      (! _webaddr->isVisibleTo(this) || _webaddr->text().simplified().isEmpty()) &&
      (! _address->isVisibleTo(this) || _address->id() <= 0))
    setId(-1);
}

void ContactWidget::sEllipses()
{
    if (_x_preferences && _x_preferences->value("DefaultEllipsesAction") == "search")
	sSearch();
    else
	sList();
}

void ContactWidget::sInfo()
{
    /*
    ContactInfo* newdlg = new ContactInfo(this, "ContactInfo", true);
    if (newdlg)
    {
	int id = newdlg->exec();
	setId(id);
    }
    else
    */
	QMessageBox::critical(this, tr("A System Error Occurred at %1::%2.")
				      .arg(__FILE__)
				      .arg(__LINE__),
			      tr("%1::sInfo() not yet defined").arg(metaObject()->className()));
}

void ContactWidget::sList()
{
  ContactList* newdlg = new ContactList(this);
  if (newdlg)
  {
    ParameterList params;
    params.append("searchAcctId", _searchAcctId);
    newdlg->set(params);
    int id = newdlg->exec();
    if (id > 0)
      setId(id);
  }
  else
    QMessageBox::critical(this, tr("A System Error Occurred at %1::%2.")
                          .arg(__FILE__)
                          .arg(__LINE__),
                          tr("Could not instantiate a List Dialog"));
}

void ContactWidget::sSearch()
{
    ContactSearch* newdlg = new ContactSearch(this);
    if (newdlg)
    {
        ParameterList params;
        params.append("searchAcctId", _searchAcctId);
        newdlg->set(params);
	int id = newdlg->exec();
        if (id > 0)
	  setId(id);
    }
    else
	QMessageBox::critical(this, tr("A System Error Occurred at %1::%2.")
				      .arg(__FILE__)
				      .arg(__LINE__),
			      tr("Could not instantiate a Search Dialog"));
}

void ContactWidget::setChange(QString p)
{
  _change->setText(p);

  if (_mapper->model() &&
      _mapper->model()->data(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_change))).toString() != _change->text())
    _mapper->model()->setData(_mapper->model()->index(_mapper->currentIndex(),_mapper->mappedSection(_change)), _change->text());
}

void ContactWidget::sLaunchEmail()
{
  QString extUrl = QString(_email->currentText().toLower());
  if (!_subjText.isEmpty() ||
      !_bodyText.isEmpty())
    extUrl.append("?");

  if (!_subjText.isEmpty())
  {
    extUrl.append(_subjText.prepend("subject="));
    if (!_bodyText.isEmpty())
      extUrl.append("&");
  }

  if (!_bodyText.isEmpty())
    extUrl.append(_bodyText.prepend("body="));

  QDesktopServices::openUrl(QUrl("mailto:" + extUrl));
}

void ContactWidget::sLaunchWebaddr()
{
  QUrl tmpurl(_webaddr->text());

  if (QDesktopServices::openUrl(tmpurl))
    return;

  if(tmpurl.scheme().isEmpty())
  {
    tmpurl.setScheme("http");
    if (QDesktopServices::openUrl(tmpurl))
      return;
  }

  if (tmpurl.scheme() == "http" &&
      tmpurl.host().isEmpty() && ! tmpurl.path().isEmpty())
  {
    tmpurl.setHost(tmpurl.path());
    tmpurl.setPath("");
    if (QDesktopServices::openUrl(tmpurl))
      return;
  }

  if (tmpurl.scheme() == "http" && ! tmpurl.host().startsWith("www."))
  {
    tmpurl.setHost("www." + tmpurl.host());
    if (QDesktopServices::openUrl(tmpurl))
      return;
  }

  qWarning("%s::sLaunchWebaddr() could not open %s",
           qPrintable(objectName()), qPrintable(tmpurl.toString()));
}

///////////////////////////////////////////////////////////////////////////////

ContactList::ContactList(QWidget* pParent, const char* pName, bool, Qt::WindowFlags)
  : VirtualList(pParent, 0)
{
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(tr("Contacts"));

  _searchAcctId = -1;

  if (!pName)
      setObjectName("ContactList");
  else
      setObjectName(pName);

  _searchAcct = new QCheckBox();
  _dialogLyt->addWidget(_searchAcct);
  _searchAcct->hide();

  _listTab->setColumnCount(0);

  _listTab->addColumn(tr("First Name"),   80, Qt::AlignLeft, true, "cntct_first_name");
  _listTab->addColumn(tr("Last Name"),    -1, Qt::AlignLeft, true, "cntct_last_name");
  _listTab->addColumn(tr("Account"), 100, Qt::AlignLeft, true, "crmacct_name");
}

void ContactList::set(const ParameterList &pParams)
{
  QVariant param;
  bool     valid;

  param = pParams.value("searchAcctId", &valid);
  if (valid)
  {
    _searchAcctId = param.toInt();
    XSqlQuery crmacct;
    crmacct.prepare("SELECT crmacct_number || ' - ' || crmacct_name AS account "
                    "FROM crmacct "
                    "WHERE (crmacct_id = :crmacct_id);");
    crmacct.bindValue(":crmacct_id", _searchAcctId);
    crmacct.exec();
    if (crmacct.first())
    {
      _searchAcct->setChecked(true);
      _searchAcct->setText(tr("Only contacts for %1").arg(crmacct.value("account").toString()));
      connect(_searchAcct, SIGNAL(toggled(bool)), this, SLOT(sFillList()));
    }
  }

  _searchAcct->setVisible(_searchAcctId > 0);
}

void ContactList::sFillList()
{
  MetaSQLQuery mql("SELECT cntct.*, crmacct_name "
                   "  FROM cntct()"
                   "  LEFT OUTER JOIN crmacctcntctass ON (crmacctcntctass_cntct_id=cntct_id) "
                   "  LEFT OUTER JOIN crmacct ON (crmacctcntctass_crmacct_id = crmacct_id)"
                   " WHERE cntct_active"
                   "<? if exists('crmacctid') ?>"
                   " AND ( (crmacct_id=<? value('crmacctid') ?>) OR (crmacct_parent_id=<? value('crmacctid') ?>) )"
                   "<? endif ?>"
                   " ORDER BY cntct_last_name, cntct_first_name;");
  ParameterList params;
  if (_searchAcct->isChecked())
    params.append("crmacctid",  _searchAcctId);

  XSqlQuery query = mql.toQuery(params);
  if (ErrorReporter::error(QtCriticalMsg, this, tr("List Error"),
                           query, __FILE__, __LINE__))
    return;

  if (query.size() < 1)	// no rows found with limit so try without
  {
    ParameterList noparams;
    mql.toQuery(noparams);
    if (ErrorReporter::error(QtCriticalMsg, this, tr("List Error"),
                             query, __FILE__, __LINE__))
      return;
  }

  _listTab->populate(query);
}

/* do this differently than VirtualList::sSearch(QString&):
   look for characters that match the beginning of first or last name,
   not just the first field
*/
void ContactList::sSearch(const QString& pTarget)
{
  XTreeWidgetItem *target = 0;
  for (int i = 0; i < _listTab->topLevelItemCount(); i++)
  {
    target = _listTab->topLevelItem(i);
    if (target == NULL ||
        target->text(0).startsWith(pTarget.toUpper(), Qt::CaseInsensitive) ||
        target->text(1).startsWith(pTarget.toUpper(), Qt::CaseInsensitive) ||
        target->text(2).startsWith(pTarget.toUpper(), Qt::CaseInsensitive) ||
        target->text(3).startsWith(pTarget.toUpper(), Qt::CaseInsensitive))
      break;
  }

  if (target)
  {
    _listTab->setCurrentItem(target);
    _listTab->scrollToItem(target);
  }
}

///////////////////////////////////////////////////////////////////////////

ContactSearch::ContactSearch(QWidget* pParent, Qt::WindowFlags pFlags)
    : VirtualSearch(pParent, pFlags)
{
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(tr("Contacts"));

  // remove the stuff we won't use
  disconnect(_searchNumber,	SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  disconnect(_searchName,	SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  disconnect(_searchDescrip,	SIGNAL(toggled(bool)), this, SLOT(sFillList()));

  selectorsLyt->removeWidget(_searchNumber);
  selectorsLyt->removeWidget(_searchName);
  selectorsLyt->removeWidget(_searchNumber);

  delete _searchNumber;
  delete _searchName;
  delete _searchDescrip;
  _searchNumber = _searchName = _searchDescrip = 0;

  _listTab->setColumnCount(0);

  _searchFirst	= new XCheckBox(tr("Search First Name"),this);
  _searchLast		= new XCheckBox(tr("Search Last Name"),this);
  _searchCRMAcct	= new XCheckBox(tr("Search Account"),this);
  _searchTitle	= new XCheckBox(tr("Search Title"),this);
  _searchPhones	= new XCheckBox(tr("Search Phone Numbers"),this);
  _searchEmail	= new XCheckBox(tr("Search Email Address"),this);
  _searchWebAddr	= new XCheckBox(tr("Search Web Address"),this);
  _searchInactive	= new XCheckBox(tr("Show Inactive Contacts"),this);

  _searchFirst->setObjectName("_searchName");
  _searchLast->setObjectName("_searchLast");
  _searchCRMAcct->setObjectName("_searchCRMAcct");
  _searchTitle->setObjectName("_searchTitle");
  _searchPhones->setObjectName("_searchPhones");
  _searchEmail->setObjectName("_searchEmail");
  _searchWebAddr->setObjectName("_searchWebAddr");
  _searchInactive->setObjectName("_searchInactive");

  selectorsLyt->addWidget(_searchFirst,    0, 0);
  selectorsLyt->addWidget(_searchLast,     1, 0);
  selectorsLyt->addWidget(_searchCRMAcct,  2, 0);
  selectorsLyt->addWidget(_searchTitle,    3, 0);
  selectorsLyt->addWidget(_searchPhones,   0, 1);
  selectorsLyt->addWidget(_searchEmail,    1, 1);
  selectorsLyt->addWidget(_searchWebAddr,  2, 1);
  selectorsLyt->addWidget(_searchInactive, 3, 1);

  _searchAcct = new QCheckBox();
  _dialogLyt->addWidget(_searchAcct);
  _searchAcct->hide();

  connect(_searchFirst,    SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_searchLast,     SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_searchCRMAcct,  SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_searchTitle,    SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_searchPhones,   SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_searchEmail,    SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_searchWebAddr,  SIGNAL(toggled(bool)), this, SLOT(sFillList()));
  connect(_searchInactive, SIGNAL(toggled(bool)), this, SLOT(sFillList()));

  _listTab->addColumn(tr("First Name"),     80, Qt::AlignLeft, true, "cntct_first_name");
  _listTab->addColumn(tr("Last Name"),      -1, Qt::AlignLeft, true, "cntct_last_name");
  _listTab->addColumn(tr("Account"),       100, Qt::AlignLeft, true, "crmacct_name");
  _listTab->addColumn(tr("Title"),         100, Qt::AlignLeft, true, "cntct_title");
  _listTab->addColumn(tr("Phone"),	    75, Qt::AlignLeft, true, "contact_phone");
  _listTab->addColumn(tr("Alt. Phone"),     75, Qt::AlignLeft, true, "contact_phone2");
  _listTab->addColumn(tr("Fax"),	    75, Qt::AlignLeft, true, "contact_fax");
  _listTab->addColumn(tr("Email Address"), 100, Qt::AlignLeft, true, "cntct_email");
  _listTab->addColumn(tr("Web Address"),   100, Qt::AlignLeft, true, "cntct_webaddr");

  setObjectName("contactSearch");
}

void ContactSearch::set(const ParameterList &pParams)
{
  QVariant param;
  bool     valid;

  param = pParams.value("searchAcctId", &valid);
  if (valid)
  {
    _searchAcctId = param.toInt();
    if (_searchAcctId > 0)
    {
      XSqlQuery crmacct;
      crmacct.prepare("SELECT crmacct_number || ' - ' || crmacct_name AS account "
                      "FROM crmacct "
                      "WHERE (crmacct_id = :crmacct_id);");
      crmacct.bindValue(":crmacct_id", _searchAcctId);
      crmacct.exec();
      if (crmacct.first())
      {
        _searchAcct->setChecked(true);
        _searchAcct->setText(tr("Only contacts for %1").arg(crmacct.value("account").toString()));
        connect(_searchAcct, SIGNAL(toggled(bool)), this, SLOT(sFillList()));
      }
    }
  }
  _searchAcct->setVisible(_searchAcctId > 0);
}

void ContactSearch::sFillList()
{
  if (_search->text().isEmpty() ||
      (!_searchFirst->isChecked()    && !_searchLast->isChecked() &&
       !_searchCRMAcct->isChecked()  && !_searchTitle->isChecked() &&
       !_searchPhones->isChecked()   &&
       !_searchEmail->isChecked()    && !_searchWebAddr->isChecked() ))
    return;

  MetaSQLQuery mql("SELECT cntct.*, crmacct_name, "
                   " getcontactphone(cntct_id, 'Office') AS contact_phone, "
                   " getcontactphone(cntct_id, 'Mobile') AS contact_phone2, "
                   " getcontactphone(cntct_id, 'Fax') AS contact_fax "
                   "  FROM cntct()"
                   "  LEFT OUTER JOIN crmacctcntctass ON (crmacctcntctass_cntct_id=cntct_id)"
                   "  LEFT OUTER JOIN crmacct ON (crmacctcntctass_crmacct_id = crmacct_id)"
                   " WHERE true"
                   "<? if exists('crmacctid') ?>"
                   "  AND ( (crmacct_id=<? value('crmacctid') ?>) OR (crmacct_parent_id=<? value('crmacctid') ?>) )"
                   "<? endif ?>"
                   "<? if not exists('searchInactive') ?> "
                   "  AND cntct_active "
                   "<? endif ?>"
                   "<? if reExists('search[FLCTPEW]') ?> "
                   "  AND ("
                   "  <? if exists('searchFirst') ?> "
                   "     COALESCE(TRIM(cntct_first_name),'') || ' ' "
                   "  <? else ?>"
                   "    '\n' "
                   "  <? endif ?>"
                   "  <? if exists('searchLast') ?> "
                   "     || COALESCE(TRIM(cntct_last_name),'') || '\n' "
                   "  <? endif ?>"
                   "  <? if exists('searchCRMAcct') ?> "
                   "     || COALESCE(crmacct_name,'') || '\n' "
                   "  <? endif ?>"
                   "  <? if exists('searchTitle') ?> "
                   "     || COALESCE(cntct_title,'') || '\n' "
                   "  <? endif ?>"
                   "  <? if exists('searchPhones') ?> "
                   "    || phonejson(cntct_id) || '\n' "
                   "  <? endif ?>"
                   "  <? if exists('searchEmail') ?> "
                   "     || COALESCE(cntct_email,'') || '\n' "
                   "  <? endif ?>"
                   "  <? if exists('searchWebAddr') ?> "
                   "     || COALESCE(cntct_webaddr,'') || '\n' "
                   "  <? endif ?>"
                   "  ~* <? value('searchText') ?> ) "
                   "<? endif ?>"
                   "ORDER BY cntct_last_name, cntct_first_name, crmacct_number;");

  ParameterList params;
  if (_searchAcct->isChecked())
    params.append("crmacctid", _searchAcctId);

  if (_searchFirst->isChecked())
    params.append("searchFirst");
  if (_searchLast->isChecked())
    params.append("searchLast");
  if (_searchCRMAcct->isChecked())
    params.append("searchCRMAcct");
  if (_searchTitle->isChecked())
    params.append("searchTitle");
  if (_searchPhones->isChecked())
    params.append("searchPhones");
  if (_searchEmail->isChecked())
    params.append("searchEmail");
  if (_searchWebAddr->isChecked())
    params.append("searchWebAddr");
  if (_searchInactive->isChecked())
    params.append("searchInactive");

  params.append("searchText", _search->text());

  XSqlQuery query = mql.toQuery(params);
  _listTab->populate(query);
  ErrorReporter::error(QtCriticalMsg, this, tr("Search Error"),
                       query, __FILE__, __LINE__);
}

void ContactWidget::setMode(Mode p)
{
  if (p == _mode)
    return;
  bool enabled = (p == Edit);

  QList<QWidget *> widgets = findChildren<QWidget *>();
  for (int i = 0; i < widgets.size(); i++)
    widgets.at(i)->setEnabled(enabled);
  if (p == Select)
    _list->setEnabled(true);
}

void ContactWidget::setEmailSubjectText(const QString text)
{
  _subjText = text;
}

void ContactWidget::setEmailBodyText(const QString text)
{
  _bodyText = text;
}

void ContactWidget::fillEmail()
{
  XSqlQuery qry;
  qry.prepare("SELECT cntcteml_id AS id, cntcteml_email AS email "
              "FROM cntcteml "
              "WHERE (cntcteml_cntct_id=:cntct_id) "
              "ORDER BY email, id;");
  qry.bindValue(":cntct_id", _id);
  qry.exec();
  ErrorReporter::error(QtCriticalMsg, this, tr("Getting Email Details"),
                         qry, __FILE__, __LINE__);

  QString curremail = _email->currentText().toLower();

  if(qry.first())
  {
   _email->populate(qry);
  }

  if (curremail.isEmpty())
      curremail = _emailCache;
  if (! curremail.isEmpty())
  {
      if (_email->findText(curremail) >= 0)
          _email->setText(curremail);
      else
          _email->append(FAKEEMAILID, curremail);
  }
}

void ContactWidget::sEditEmailList()
{
  int emailId = _email->id();
  ParameterList params;
  params.append("cntct_id", _id);

  contactEmail newdlg(this, "", true);
  newdlg.set(params);
  int selected = newdlg.exec();
  fillEmail();
  if (selected)
    _email->setId(selected);
  else
    _email->setId(emailId);
}

bool ContactWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != _email->lineEdit())
        return QObject::eventFilter(obj, event);

    switch (event->type()) {
    case QEvent::MouseButtonDblClick: {;
        sLaunchEmail();
        return true;
    }
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}

void ContactWidget::sBuildPhones()
{
  XSqlQuery getp;
  _rowId = -1;
  cmap.clear();
  getp.prepare("SELECT cntctphone_crmrole_id, cntctphone_phone "
               "FROM cntctphone "
               "JOIN crmrole ON crmrole_id=cntctphone_crmrole_id "
               "WHERE cntctphone_cntct_id=:cntctid "
               "ORDER BY crmrole_sort;");
  getp.bindValue(":cntctid", _id);
  getp.exec();
  ErrorReporter::error(QtCriticalMsg, this, tr("Getting Phone Details"),
                         getp, __FILE__, __LINE__);

//  if (_email->isEnabled())
//  {
    _add = new QPushButton(tr("+"), this);
    _add->setMaximumWidth(30);
    _add->setObjectName(QString("_phoneAdd"));
    connect(_add, SIGNAL(clicked()), this, SLOT(sAddNewPhoneRow()));
//  }

  while (getp.next())
  {
    sAddNewPhoneRow();
    XComboBox *cRole   = findChild<XComboBox*>(QString("_phRole%1").arg(_rowId));
    XLineEdit *cNumber = findChild<XLineEdit*>(QString("_contactPhone%1").arg(_rowId));
    cRole->setId(getp.value("cntctphone_crmrole_id").toInt());
    cNumber->setText(getp.value("cntctphone_phone").toString());
  }

  while (cmap.size() < 3)  // Pad phone list for aesthetics
  {
    sAddNewPhoneRow();
  }
}

void ContactWidget::sAddNewPhoneRow()
{
  _rowId++;
  bool enb = _email->isEnabled();

  QPushButton *rem = new QPushButton(tr("-"), this);
  rem->setMaximumWidth(30);
  XLineEdit   *ph  = new XLineEdit(this, qPrintable(QString("_contactPhone%1").arg(_rowId)));
  ph->setMinimumWidth(110);
  XComboBox   *cb  = new XComboBox(this, qPrintable(QString("_phRole%1").arg(_rowId)));

  cb->setObjectName(QString("_phRole%1").arg(_rowId));
  cb->setType(XComboBox::CRMRolePhone);
  ph->setObjectName(QString("_contactPhone%1").arg(_rowId));

  cb->setEnabled(enb);
  ph->setEnabled(enb);

  rem->setObjectName(QString("_contactRem%1").arg(_rowId));
  connect(rem, SIGNAL(clicked()), this, SLOT(sRemovePhone()));

  _phoneGrid->addWidget(cb,  _rowId, 0);
  _phoneGrid->addWidget(ph,  _rowId, 1);
  if (enb)
  {
    _phoneGrid->addWidget(rem, _rowId, 2);
    _phoneGrid->addWidget(_add, _rowId, 3);
  }

  cb->show();
  ph->show();
  rem->show();
  _add->show();

  cmap[rem->objectName()] = _rowId;
}

void ContactWidget::sRemovePhone()
{
  XSqlQuery remq;
  QString rem = qobject_cast<QPushButton*>(sender())->objectName();
  int i = cmap[rem];

  XComboBox *oldRole = findChild<XComboBox*>(QString("_phRole%1").arg(cmap[rem]));
  XLineEdit *oldNumb = findChild<XLineEdit*>(QString("_contactPhone%1").arg(cmap[rem]));
  QPushButton *oldRem = findChild<QPushButton*>(QString("_contactRem%1").arg(cmap[rem]));

  remq.prepare("DELETE FROM cntctphone "
               "WHERE cntctphone_crmrole_id=:cntctphone_crmrole_id "
               " AND  cntctphone_cntct_id=:cntctphone_cntct_id "
               " AND  cntctphone_phone=:cntctphone_phone;");
  remq.bindValue(":cntctphone_crmrole_id", oldRole->id());
  remq.bindValue(":cntctphone_cntct_id", _id);
  remq.bindValue(":cntctphone_phone", oldNumb->text());
  remq.exec();
  ErrorReporter::error(QtCriticalMsg, this, tr("Removing Contact Phone Details"),
                         remq, __FILE__, __LINE__);

  delete oldNumb;
  delete oldRole;
  delete oldRem;

  cmap.remove(rem);

  if (i == _rowId)
    _rowId--;
  _phoneGrid->addWidget(_add, _rowId, 3);
  _add->show();
}

QString ContactWidget::sBuildPhoneJson()
{
  QJsonObject phonesObject;
  QJsonArray phoneArray;

  foreach (int i, cmap) {
    XComboBox *cRole   = findChild<XComboBox*>(QString("_phRole%1").arg(i));
    XLineEdit *cNumber = findChild<XLineEdit*>(QString("_contactPhone%1").arg(i));

    if (!cRole->isValid() || cNumber->text().trimmed().isEmpty())
      continue;

    QJsonObject phoneObject;
    phoneObject.insert("role", cRole->code());
    phoneObject.insert("number", cNumber->text());
    phoneArray.push_back(phoneObject);
  }
  phonesObject.insert("phones", phoneArray);

  QJsonDocument doc(phonesObject);

  return doc.toJson();
}

// script api //////////////////////////////////////////////////////////////////

void setupContactWidget(QScriptEngine *engine)
{
  if (! engine->globalObject().property("ContactWidget").isObject())
  {
    QScriptValue ctor = engine->newObject(); //engine->newFunction(scriptconstructor);
    QScriptValue meta = engine->newQMetaObject(&ContactWidget::staticMetaObject, ctor);

    engine->globalObject().setProperty("ContactWidget", meta,
                                       QScriptValue::ReadOnly | QScriptValue::Undeletable);
  }
}
