
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#include <QTranslator>

#include "defs.h"
#include "TrayIcon.h"
#include "QUnixApp.h"

static const int TrayTimeout = 30 ; // wait (sec) for systray available.
static const int AlarmRC = QUnixApp :: SigALRM + 128 ;

int main ( int ArgC , char * ArgV [ ] ) {

  QUnixApp App ( ArgC , ArgV ) ;
  App . setApplicationName    ( APPLICATION  ) ;
  App . setOrganizationName   ( ORGANIZATION ) ;
  App . setApplicationVersion ( VERSION      ) ;
  App . setQuitOnLastWindowClosed ( false  ) ;
  QTranslator Tr ;
  Tr . load ( APPLICATION "_" + QLocale :: system ( ) . name ( ) ,
                                                        APP_TRANSLATIONS ) ;
  App . installTranslator ( & Tr ) ;

  Listener L ; TrayIcon * TI = new TrayIcon ( & L ) ;
  QObject :: connect ( & App , SIGNAL ( UnixSignal ( int ) ) ,
                       TI    , SLOT   ( USigCaught ( int ) ) ) ;
  App . AddSig ( QUnixApp :: SigTERM ) ;
  App . AddSig ( QUnixApp :: SigINT  ) ;
  App . AddSig ( QUnixApp :: SigHUP  ) ;
  App . AddSig ( QUnixApp :: SigQUIT ) ;

  int Ret = AlarmRC , Time = TrayTimeout ;
  while ( ! QSystemTrayIcon :: isSystemTrayAvailable ( ) && Time -- ) {
    QUnixApp :: sleep ( 1 ) ;
  }//done

  if ( QSystemTrayIcon :: isSystemTrayAvailable ( ) ) {
    TI -> show ( ) ; Ret  = App . exec ( ) ;
  }//fi

  return Ret ;

}// main

//eof tmount.cpp
