
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2022.

#include <QTranslator>
#include <QTextStream>

#include "defs.h"
#include "TrayIcon.h"
#include "QUnixApp.h"

static const QString WaitP = "-wait" , DefTW = "30" ;
  // Default argument - wait 30 sec for system tray available.
static const int AlarmRC = QUnixApp :: SigALRM + 128 ;

int main ( int ArgC , char * ArgV [ ] ) {

  QUnixApp App ( ArgC , ArgV ) ; QStringList Args = App . arguments ( ) ;
  QTranslator Tr ;
  Tr . load ( APPLICATION "_" + QLocale :: system ( ) . name ( ) ,
                                                          APP_TRANSLATIONS ) ;
  App . installTranslator ( & Tr ) ;
  App . setApplicationName    ( APPLICATION  ) ;
  App . setOrganizationName   ( ORGANIZATION ) ;
  App . setApplicationVersion ( VERSION      ) ;

  if ( Args . size ( ) == 1 ) {
    Args . append ( WaitP ) ; Args . append ( DefTW ) ;
  }//fi

  int RetCode = 1 , TimeWait = 0 ; bool Ok = Args . size ( ) == 3 ;

  if ( Ok ) {
    TimeWait = Args . at ( 2 ) .  toInt (  & Ok ) ;
    Ok = Ok && Args . at ( 1 ) == WaitP && TimeWait >= 0 ;
  }//fi

  if ( Ok ) {

    Listener Lstnr ; TrayIcon * TrIc = new TrayIcon ( & Lstnr ) ;

    QObject :: connect ( & App , SIGNAL ( UnixSignal ( int ) ) ,
                         TrIc  , SLOT   ( USigCaught ( int ) ) ) ;
    App . AddSig ( QUnixApp :: SigTERM ) ;
    App . AddSig ( QUnixApp :: SigINT  ) ;
    App . AddSig ( QUnixApp :: SigQUIT ) ;
    App . AddSig ( QUnixApp :: SigHUP  ) ;
    App . AddSig ( QUnixApp :: SigUSR1 ) ;
    App . AddSig ( QUnixApp :: SigUSR2 ) ;

    int Rest = TimeWait ;
    while ( ! TrayAvail ( ) && Rest -- ) { QUnixApp :: sleep ( 1 ) ; }//done
    if ( TimeWait ) { TrIc -> show ( ) ; }//fi

    if ( TrayAvail ( ) || ! TimeWait ) {
      App . setQuitOnLastWindowClosed ( false  ) ; RetCode = App . exec ( ) ;
    } else { RetCode = AlarmRC ;
    }//fi

  } else {

    QTextStream ( stderr , QIODevice :: WriteOnly ) << QObject :: tr (
      "Usage: tmount [-wait <n>]\n"
      "  where <n> - time (in sec.) to wait for system tray availability,\n"
      "  or 0 to work without systray.  Default is 30 sec.\n" ) ;

  }//fi

  return RetCode ;

}// main

//eof tmount.cpp
