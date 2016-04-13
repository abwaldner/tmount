
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#include <QProcess>
#include <QSocketNotifier>
#include <QMessageBox>
#include <QTextCodec>
#include <QApplication>

#include "Listener.h"
#include "defs.h"

static const char * FS_USAGE = "ID_FS_USAGE" ;
static const char * FS_LABEL = "ID_FS_LABEL" ;
static const char * FS_TYPE  = "ID_FS_TYPE"  ;
static const char * USAGE_filesystem = "filesystem" ;

static const char * SA_events    = "events"  ;
static const char * events_Eject = "eject_request"  ;

static const int StartTimeout = 2000 ; // 2 s for start of external program.
static const int ExecTimeout  = 5000 ; // 5 s for run the external program.

Listener :: Listener ( QWidget * parent ) : QMenu ( parent ) {

  MIcon = QIcon ( ":/icons/mount.png"   ) ;
  UIcon = QIcon ( ":/icons/unmount.png" ) ;
  UdevEnum En ( & UdevContext ) ;
  En . MatchSubsys ( "block"  ) ; En . ScanDevs ( ) ;
  foreach ( UdevPair P , En . GetList ( ) ) {
    UdevDev Dev ( & UdevContext , P . first ) ;
    AddDevice ( Dev , Opt . MountStart ( ) ) ;
  }//done

  UMonitor  = new UdevMon ( & UdevContext ) ;
  UMonitor -> AddMatch ( "block" , NULL ) ;
  UMonitor -> EnableReceiving ( ) ;
  QSocketNotifier * Ntfr ;
  Ntfr = new QSocketNotifier (
               UMonitor -> GetFD ( ) , QSocketNotifier :: Read , this ) ;
  connect ( Ntfr , SIGNAL ( activated    ( int ) ) ,
            this , SLOT   ( DeviceAction ( int ) ) ) ;

  Ntfr = new QSocketNotifier (
               MMonitor .  GetFD ( ) , QSocketNotifier :: Exception , this ) ;
  connect ( Ntfr , SIGNAL ( activated   ( int ) ) ,
            this , SLOT   ( MountAction ( int ) ) ) ;

}// Listener

Listener :: ~Listener ( ) { delete UMonitor ; }// ~Listener

ActPtr Listener :: exec ( const QPoint & Loc , ActPtr At ) {

  ActList T = FindActs ( CurrDev ) ;
  if ( ! T . isEmpty ( ) ) { setActiveAction ( T . first ( ) ) ; }//fi

  ActPtr Act = QMenu :: exec ( Loc , At ) ;

  if ( Act ) {

    QString P = Act -> objectName ( ) ;
    CurrDev = P . section ( ' ' , 0 , 0 ) ;
    UdevDev Dev ( & UdevContext , CurrDev ) ;
    QString N = Dev . DevNode ( ) ;
    P = Mounts :: DecodeIFS ( P . section ( ' ' , 1 , 1 ) ) ;
    bool M = P . isEmpty ( ) ; // Mount required.
    int R ;

    R = ExecCmd ( M ? Opt . MountCmd   ( ) + " \"" + N + '"'
                    : Opt . UnmountCmd ( ) + " \"" + P + '"' ) ;
    if ( R ) { SetActions ( Dev ) ; }//fi // workaround for setChecked ()

    MInfo . RefreshMountInfo   ( ) ;
    QString E = Opt . EjectCmd ( ) ;
    if ( ! M && ! R && ! E . isEmpty ( ) &&
                MInfo . MPoints ( N ) . isEmpty ( ) &&
                Dev . SysAttr ( SA_events ) . contains ( events_Eject ) ) {
      ExecCmd ( E + " \"" + N + '"' ) ;
    }//fi

  }//fi

  return Act ;

}// Listener :: ShowDevices

void Listener :: DeviceAction ( int socket ) { ( void ) socket ;

  UdevDev Dev ( UMonitor ) ; QString DAct = Dev . Action ( ) ;

  if ( DAct == "add" ) { AddDevice ( Dev , Opt . MountNew ( ) ) ;
  } else if ( DAct == "remove" ) { RemoveDevice ( Dev ) ;
  } else if ( DAct == "change" ) {
    if ( !  AddDevice ( Dev , Opt . MountMedia ( ) ) ) {
      RemoveDevice ( Dev ) ;
    }//fi
  }//fi

}// Listener :: DeviceAction

void Listener :: MountAction ( int socket ) { ( void ) socket ;

  MInfo . RefreshMountInfo ( ) ;

  foreach ( QString P , DevList ) {
    UdevDev Dev ( & UdevContext , P ) ; SetActions ( Dev ) ;
  }//done

}// Listener :: MountAction

bool Listener :: AddDevice ( UdevDev & Dev , bool TryMount ) {

  QString N = Dev . DevNode ( ) ;
  bool    T = Dev . Property ( FS_USAGE ) == USAGE_filesystem &&
                ! Opt . HideDevs ( ) . contains (
                    QRegExp ( "(^| )" + QRegExp :: escape ( N ) + "( |$)" ) ) ;

  if ( T ) { // is target device

    CurrDev = Dev . SysPath ( ) ;
    DevList += CurrDev ;
    DevList . removeDuplicates ( ) ; // overcaution

    if ( TryMount && MInfo . MPoints ( N ) . isEmpty ( ) ) {
      ExecCmd ( Opt . MountCmd ( ) + " \"" + N + '"' ) ;
    }//fi

    SetActions ( Dev ) ;

  }//fi

  return T ;

}// Listener :: AddDevice

void Listener :: RemoveDevice  ( UdevDev & Dev ) {

  QString P = Dev . SysPath ( ) ; DevList . removeOne ( P ) ;

  foreach ( ActPtr Act , FindActs ( P ) ) { removeAction ( Act ) ; delete Act ;
  }//done

  foreach ( QString M , MInfo . MPoints ( Dev . DevNode ( ) ) ) {
    ExecCmd ( Opt . UnmountCmd ( ) +
                      " \"" + Mounts :: DecodeIFS ( M ) + '"' ) ;
  }//done

}// Listener :: RemoveDevice

ActList Listener :: FindActs ( const QString & Name ) {
  return findChildren < ActPtr > (
           QRegExp ( '^' + QRegExp :: escape ( Name ) + "( |$)" ) ) ;
}// Listener :: FindAct

void Listener :: SetActions ( UdevDev & Dev ) {

  QIcon * I = & MIcon ;
  QString N = Dev . DevNode ( ) , L = Dev . Property ( FS_LABEL ) ,
          P = Dev . SysPath ( ) ;
  L = N . mid ( 5 ) + ' ' + Dev . Property ( FS_TYPE  ) + ',' +
                    ( L . isNull ( ) ? tr ( "(no label)" ) : '[' + L + ']' ) ;

  ActList A = FindActs  ( P ) ;
  QStringList M = MInfo . MPoints ( N ) ; bool U = M . isEmpty ( ) ;

  if ( U ) { M += "" ; } else { I = & UIcon ; L += tr ( " on " ) ; }//fi

  int S = M . size ( ) ;
  while ( A . size ( ) > S ) {
    ActPtr Act = A . takeLast ( ) ; removeAction ( Act ) ; delete Act ;
  }//done
  while ( A . size ( ) < S ) {
    ActPtr Act = addAction ( "" ) ; A += Act ; Act -> setCheckable ( true ) ;
  }//done

  foreach ( QString MP , M ) {
    ActPtr Act = A . takeFirst ( ) ; Act -> setIcon ( * I ) ;
    Act -> setText ( L + MP ) ; Act -> setChecked ( ! U ) ;
    Act -> setObjectName ( ( P + ' ' + MP ) . trimmed ( ) ) ;
  }//done

}// Listener :: SetActions

int Listener :: ExecCmd ( const QString & Cmd ) {

  QProcess Pr ; int R = -1 ;
  Pr . setStandardInputFile  ( "/dev/null" ) ;
  Pr . setStandardOutputFile ( "/dev/null" ) ;
  Pr . start ( Cmd , QIODevice :: ReadOnly ) ;

  if ( ! Pr . waitForStarted ( StartTimeout ) ) {
    QMessageBox :: critical ( NULL , tr ( "Error"  ) ,
                                tr ( "Can't execute" ) + " '" + Cmd + "'" ) ;
  } else if ( ! Pr . waitForFinished ( ExecTimeout ) ) {
    QMessageBox :: critical ( NULL , tr ( "Error"  ) ,
                                "'" + Cmd + "' " + tr ( "crashed." ) ) ;
  } else if ( ( R = Pr . exitCode ( ) ) ) {
    QMessageBox :: warning ( NULL , tr ( "Warning" ) ,
                     QTextCodec :: codecForLocale () ->
                       toUnicode ( Pr . readAllStandardError ( ) ) ) ;
  }//fi

  return R ;

}// Listener :: ExecCmd

void Listener :: About ( ) {
  QMessageBox :: about ( NULL , tr ( "About" ) , QString ( "<center>" ) +
    qApp -> applicationName ( ) + " v. " + qApp -> applicationVersion ( ) +
    tr ( " - block devices mounter/unmounter<br/>" ) + COPYRYGHT +
    tr ( "<br/>License: " ) + LICENSE ) ;
}// Listener :: About

//eof Listener.cpp
