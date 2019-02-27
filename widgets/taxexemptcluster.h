/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef TAXEXEMPTCLUSTER_H
#define TAXEXEMPTCLUSTER_H

#include "xcombobox.h"
#include "taxIntegration.h"

class XTUPLEWIDGETS_EXPORT TaxExemptCluster : public XComboBox
{
  Q_OBJECT

  Q_PROPERTY(bool silent READ silent WRITE setSilent)

  public:
    TaxExemptCluster(QWidget* = 0, const char* = 0);
    ~TaxExemptCluster();

    Q_INVOKABLE bool silent();
    Q_INVOKABLE void populate(QStringList = QStringList());

  public slots:
    void setSilent(bool);
    void setCode(const QString&);
    void sPopulateTaxExempt(QJsonObject, QString);

  private:
    bool            _silent;
    QString         _code;
    TaxIntegration* _tax;
};

#endif
