
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2019.

#include <QSocketNotifier>

#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "QUnixApp.h"

const int QUnixApp :: SigHUP  = SIGHUP  ;
const int QUnixApp :: SigINT  = SIGINT  ;
const int QUnixApp :: SigQUIT = SIGQUIT ;
const int QUnixApp :: SigTERM = SIGTERM ;
const int QUnixApp :: SigUSR1 = SIGUSR1 ;
const int QUnixApp :: SigUSR2 = SIGUSR2 ;
const int QUnixApp :: SigALRM = SIGALRM ;

unsigned QUnixApp :: sleep ( unsigned T ) { return :: sleep ( T ) ;
}// QUnixApp :: sleep

QUnixApp :: QUnixApp ( int ArgC , char * ArgV [ ] ) :
                       QApplication ( ArgC , ArgV ) {
  :: socketpair ( AF_UNIX , SOCK_STREAM , 0 , SockPair ) ;
  QSocketNotifier * Ntfr = new QSocketNotifier (
                      SockPair [ 1 ] , QSocketNotifier :: Read , this ) ;
  connect ( Ntfr , SIGNAL ( activated ( int ) ) ,
            this , SLOT   ( Caught    ( int ) ) ) ;
}// QUnixApp

QUnixApp :: ~QUnixApp ( ) {
  :: close ( SockPair [ 0 ] ) ; :: close ( SockPair [ 1 ] ) ;
}// ~QUnixApp

int QUnixApp :: AddSig ( int Sig ) const {
  struct sigaction SA ;
  SA . sa_handler = USHandler ; SA . sa_flags = SA_RESTART ;
  :: sigemptyset ( & SA . sa_mask ) ;
  return :: sigaction ( Sig  , & SA , nullptr ) ;
}// QUnixApp :: AddSig

void QUnixApp :: USHandler ( int Sig ) {
  ssize_t N = :: write ( SockPair [ 0 ] , & Sig , sizeof ( Sig ) ) ;
  ( void ) N ;
}// QUnixApp :: USHandler

void QUnixApp :: Caught ( int sock ) {
  int Sig ;
  ssize_t N = :: read ( sock , & Sig , sizeof ( Sig ) ) ; ( void ) N ;
  emit UnixSignal ( Sig ) ;
}// QUnixApp :: Caught

int QUnixApp :: SockPair [ 2 ] = { -1 , -1 } ;

//eof QUnixApp.cpp
