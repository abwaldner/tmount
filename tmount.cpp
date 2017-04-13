
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#include <QApplication>
#include <QTranslator>
#include <QLocale>

#include <unistd.h>

#include "defs.h"
#include "Listener.h"
#include "TrayIcon.h"

static const int TrayTimeout = 30 ; // wait 30 sec for systray available.

int main ( int ArgC , char * ArgV [ ] ) {

  QApplication App  ( ArgC , ArgV ) ;
  App . setApplicationName    ( APPLICATION  ) ;
  App . setOrganizationName   ( ORGANIZATION ) ;
  App . setApplicationVersion ( VERSION      ) ;
  App . setQuitOnLastWindowClosed ( false  ) ;
  QTranslator Tr ;
  Tr . load ( APPLICATION "_" + QLocale :: system ( ) . name ( ) ,
                                                          APP_TRANSLATIONS ) ;
  App . installTranslator ( & Tr ) ;

  Listener L ;

  int Ret = 2 , Time = TrayTimeout ;
  while ( ! QSystemTrayIcon :: isSystemTrayAvailable ( ) && Time -- ) {
    sleep ( 1 ) ;
  }//done

  if ( QSystemTrayIcon :: isSystemTrayAvailable ( ) ) {
    new TrayIcon ( & L ) ; Ret = App . exec ( ) ;
  }//fi

  return Ret ;

}// main

//eof tmount.cpp
