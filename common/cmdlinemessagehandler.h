/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2019 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __CMDLINEMESSAGEHANDLER_H__
#define __CMDLINEMESSAGEHANDLER_H__

#include "xabstractmessagehandler.h"

class CmdLineMessageHandlerPrivate;

class CmdLineMessageHandler : public XAbstractMessageHandler
{
  public:
    CmdLineMessageHandler(QObject *parent = 0);
    virtual ~CmdLineMessageHandler();

    virtual QMessageBox::StandardButton question(const QString &question, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

  protected:
    virtual void handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation);

  private:
    CmdLineMessageHandlerPrivate *_p;
};

#endif
