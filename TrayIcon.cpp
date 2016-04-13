
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#include <QMenu>
#include <QApplication>

#include "TrayIcon.h"

TrayIcon :: TrayIcon ( Listener * parent ) : QSystemTrayIcon ( parent ) {

  Lstnr = parent ;

  QMenu * CMenu = new QMenu ;
  CMenu -> addAction ( QIcon ( ":/icons/info.png"   ) , tr ( "About"    ) ,
                                    Lstnr          , SLOT ( About ( ) ) ) ;
  CMenu -> addAction ( QIcon ( ":/icons/config.png" ) , tr ( "Settings" ) ,
                                    & Lstnr -> Opt , SLOT ( exec  ( ) ) ) ;
  CMenu -> addSeparator ( ) ;
  CMenu -> addAction ( QIcon ( ":/icons/exit.png" ) ,
                         tr ( "Quit"     ) , qApp   , SLOT ( quit  ( ) ) ) ;
  setContextMenu  ( CMenu ) ;

  setIcon ( QIcon ( ":/icons/tmount.png" ) ) ;
  setToolTip ( tr ( "Removable devices and media." ) ) ;

  connect (
    this , SIGNAL ( activated ( QSystemTrayIcon :: ActivationReason ) ) ,
    this , SLOT   ( Activated ( QSystemTrayIcon :: ActivationReason ) ) ) ;

  show ( ) ;

}// TrayIcon

TrayIcon :: ~TrayIcon ( ) { }// ~TrayIcon

void TrayIcon :: Activated ( QSystemTrayIcon :: ActivationReason reason ) {
  if ( reason == QSystemTrayIcon :: Trigger ) {
    if ( Lstnr ->
           findChildren < ActPtr > ( QRegExp ( "^." ) ) . isEmpty ( ) ) {
      showMessage ( "Devices not found." , "" ) ;
    } else { Lstnr -> exec ( geometry ( ) . center ( ) ) ;
    }//fi
  }//fi
}// TrayIcon :: Activated


//eof TrayIcon.cpp
