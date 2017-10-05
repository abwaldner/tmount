
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#ifndef QUNIXAPP_H
#define QUNIXPPP_H

#include <QApplication>

class QUnixApp : public QApplication { Q_OBJECT
  public :
    explicit QUnixApp ( int ArgC , char * ArgV [ ] ) ; virtual ~QUnixApp ( ) ;
    int AddSig ( int Sig ) ; // 0 on success, -1 on error
    static const int
      SigHUP , SigINT , SigQUIT , SigTERM , SigUSR1 , SigUSR2 , SigALRM ;
    static unsigned sleep ( unsigned T ) ;
  private :
    Q_DISABLE_COPY ( QUnixApp )
    static  int  SockPair [ 2 ] ;
    static  void USHandler    ( int Sig  ) ;
  signals : void UnixSignal   ( int Sig  ) ;
  private slots : void Caught ( int sock ) ;
} ; // QUnixApp

#endif // QUNIXAPP_H

//eof QUnixApp.h
