
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2021.

#include <QMenu>
#include <QMessageBox>

#include "TrayIcon.h"
#include "QUnixApp.h"
#include "defs.h"

TrayIcon :: TrayIcon ( Listener * parent ) : QSystemTrayIcon ( parent ) {

  Lstnr = parent ;  const LOpts & Cnf = Lstnr -> Opt ;
  Desk  = QApplication :: desktop ( ) ;

  CMenu . addAction ( QIcon ( Cnf . toStr ( kAboutPix ) ) ,
                      tr  ( "About"     ) , this  , SLOT ( About    ( ) ) ) ;
  CMenu . addAction ( QIcon ( Cnf . toStr ( kConfPix  ) ) ,
                      tr  ( "Settings"  ) , & Cnf , SLOT ( exec     ( ) ) ) ;
  CMenu . addSeparator (  ) ;
  CMenu . addAction ( QIcon ( Cnf . toStr ( kAddImgPix  ) ) ,
                      tr  ( "Add image" ) , Lstnr , SLOT ( AddImage ( ) ) ) ;
  CMenu . addSeparator (  ) ;
  ToggleAct = CMenu . addAction (   ""    , this  , SLOT ( Toggle   ( ) ) ) ;
  CMenu . addAction ( QIcon ( Cnf . toStr ( kExitPix  ) ) ,
                      tr  ( "Quit"      ) , qApp  , SLOT ( quit     ( ) ) ) ;

  setIcon  ( QIcon ( Cnf . toStr ( kTMntPix ) ) ) ;
  setToolTip ( tr ( "Removable devices and media." ) ) ;

  connect (
    this , SIGNAL ( activated ( QSystemTrayIcon :: ActivationReason ) ) ,
    this , SLOT   ( Activated ( QSystemTrayIcon :: ActivationReason ) ) ) ;

}// TrayIcon

TrayIcon :: ~TrayIcon ( ) { }// ~TrayIcon

void TrayIcon :: Activated ( ActivationReason reason ) {
  const bool Vis = isVisible ( ) , TAv = TrayAvail ( ) ;
  const QPoint Loc = Vis && TAv ?
    geometry ( ) . center ( ) : Desk -> availableGeometry ( ) . center ( ) ;
  if ( reason == Trigger ) {
    bool H = false ;
    foreach ( const ActPtr A , Lstnr -> actions ( ) ) {
      H = H || A -> isVisible ( ) ;
    }//done
    if ( H ) { Lstnr -> exec ( Loc ) ;
    } else {
      const QString Msg = tr ( "Devices not found." ) ;
      if ( Vis ) { showMessage ( Msg , "" ) ;
      } else { QMessageBox :: about ( Lstnr , nullptr , Msg ) ;
      }//fi
    }//fi
  } else if ( reason == Context ) {
    ToggleAct ->
      setText ( Vis ? tr ( "Hide icon" ) : tr ( "Show in tray" ) ) ;
    ToggleAct -> setEnabled ( TAv ) ;
    CMenu . popup ( Loc ) ;
  }//fi
}// TrayIcon :: Activated

void TrayIcon :: USigCaught ( int Sig ) {
  if ( Sig == QUnixApp :: SigHUP || Sig == QUnixApp :: SigTERM ||
       Sig == QUnixApp :: SigINT || Sig == QUnixApp :: SigQUIT  ) {
    qApp -> exit ( Sig + 128 ) ;
  } else if ( Sig == QUnixApp :: SigUSR1 ) { emit activated ( Trigger ) ;
  } else if ( Sig == QUnixApp :: SigUSR2 ) { emit activated ( Context ) ;
  }//fi
}// TrayIcon :: USigCaught

void TrayIcon :: Toggle ( ) { setVisible ( ! isVisible ( ) ) ;
}// TrayIcon :: Toggle

void TrayIcon :: About ( ) const {
  QMessageBox :: about ( Lstnr , tr ( "About" ) ,
                         "<center>" + qApp -> applicationName    ( ) +
                         " v. "     + qApp -> applicationVersion ( ) +
                         tr ( " - block devices mounter/unmounter<br/>" ) +
                         COPYRYGHT + tr ( "<br/>License: "  ) + LICENSE ) ;
}// TrayIcon :: About

//eof TrayIcon.cpp
