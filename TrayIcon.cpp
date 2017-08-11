
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#include <QMenu>
#include <QMessageBox>
#include <QApplication>

#include "TrayIcon.h"
#include "defs.h"

TrayIcon :: TrayIcon ( Listener * parent ) : QSystemTrayIcon ( parent ) {

  Lstnr = parent ; LOpts & Cnf = Lstnr -> Opt ;

  QMenu * CMenu = new QMenu ;
  CMenu -> addAction ( QIcon ( Cnf . toStr ( kAboutPix ) ) ,
                       tr ( "About"     ) , this  , SLOT ( About    ( ) ) ) ;
  CMenu -> addAction ( QIcon ( Cnf . toStr ( kConfPix  ) ) ,
                       tr ( "Settings"  ) , & Cnf , SLOT ( exec     ( ) ) ) ;
  CMenu -> addSeparator (  ) ;
  CMenu -> addAction ( QIcon ( Cnf . toStr ( kAddImgPix  ) ) ,
                       tr ( "Add image" ) , Lstnr , SLOT ( AddImage ( ) ) ) ;
  CMenu -> addSeparator (  ) ;
  CMenu -> addAction ( QIcon ( Cnf . toStr ( kExitPix  ) ) ,
                       tr ( "Quit"      ) , qApp  , SLOT ( quit     ( ) ) ) ;
  setContextMenu ( CMenu ) ;

  setIcon  ( QIcon ( Cnf . toStr ( kTMntPix ) ) ) ;
  setToolTip ( tr ( "Removable devices and media." ) ) ;

  connect (
    this , SIGNAL ( activated ( QSystemTrayIcon :: ActivationReason ) ) ,
    this , SLOT   ( Activated ( QSystemTrayIcon :: ActivationReason ) ) ) ;

  show ( ) ;

}// TrayIcon

TrayIcon :: ~TrayIcon ( ) { }// ~TrayIcon

void TrayIcon :: Activated ( QSystemTrayIcon :: ActivationReason reason ) {
  if ( reason == QSystemTrayIcon :: Trigger ) {
    if ( Lstnr -> DevList ( ) . isEmpty ( ) ) {
      showMessage ( tr ( "Devices not found." ) , "" ) ;
    } else { Lstnr -> exec ( geometry ( ) . center ( ) ) ;
    }//fi
  }//fi
}// TrayIcon :: Activated

void TrayIcon :: About ( ) {
  QMessageBox :: about ( Lstnr , tr ( "About" ) ,
                         "<center>" + qApp -> applicationName    ( ) +
                         " v. "     + qApp -> applicationVersion ( ) +
                         tr ( " - block devices mounter/unmounter<br/>" ) +
                         COPYRYGHT + tr ( "<br/>License: "  ) + LICENSE ) ;
}// TrayIcon :: About

//eof TrayIcon.cpp
