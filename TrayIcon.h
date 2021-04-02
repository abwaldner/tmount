
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2021.

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QDesktopWidget>

#include "Listener.h"

class TrayIcon : public QSystemTrayIcon { Q_OBJECT
  public : explicit TrayIcon ( Listener * parent ) ; virtual ~TrayIcon ( ) ;
  private slots :
    void Activated  ( QSystemTrayIcon :: ActivationReason reason ) ;
    void USigCaught ( int Sig ) ;
    void Toggle ( ) ;
    void About  ( ) const ;
  private :
    Listener * Lstnr ;
    QMenu CMenu ; ActPtr ToggleAct ; QDesktopWidget * Desk ;
} ; // TrayIcon

inline bool TrayAvail ( ) {
  return QSystemTrayIcon :: isSystemTrayAvailable ( ) ;
}// TrayAvail

#endif // TRAYICON_H

//eof TrayIcon.h
