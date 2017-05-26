
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

#include "Listener.h"

class TrayIcon : public QSystemTrayIcon { Q_OBJECT
  public : explicit TrayIcon ( Listener * parent ) ; virtual ~TrayIcon ( ) ;
  private slots :
   void Activated ( QSystemTrayIcon :: ActivationReason reason ) ;
  private : Listener * Lstnr ;
} ; // TrayIcon

#endif // TRAYICON_H

//eof TrayIcon.h
