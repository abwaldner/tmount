
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#include <QProcess>
#include <QSocketNotifier>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QApplication>

#include "Listener.h"

//   These constants are hardcoded in "udev"
// and "util-linux"(in libblkid) packages.

static CPtr // Property names.
  FS_USAGE = "ID_FS_USAGE" , FS_LABEL = "ID_FS_LABEL" ,
  FS_TYPE  = "ID_FS_TYPE"  , DM_NAME  = "DM_NAME"     ;

static const QString // Property values.
  USAGE_filesystem = "filesystem" , TYPE_LUKS = "crypto_LUKS" ;

static const QString // UEvent actions.
  ACT_add = "add" , ACT_remove = "remove" , ACT_change = "change" ;

static CPtr // These constants are defined by "sysfs".
  Subsys_Block = "block"  , SA_Rem  = "removable" ,
  SA_Events    = "events" , SA_Size = "size" ;
static const QString Events_Eject = "eject_request" ;

Listener :: Listener ( QWidget * parent ) : QMenu ( parent ) {

  MIcon = QIcon ( Opt . MountIcon ( ) ) ;
  UIcon = QIcon ( Opt . UnmntIcon ( ) ) ;
  DIcon = QIcon ( Opt . UnlckIcon ( ) ) ;
  LIcon = QIcon ( Opt . LockIcon  ( ) ) ;

  UdevEnum En ( & UdevContext ) ;
  En . MatchSubsys ( Subsys_Block ) ; En . ScanDevs ( ) ;
  foreach ( UdevPair P , En . GetList ( ) ) {
    UdevDev Dev ( & UdevContext , P . first ) ;
    AddDevice ( Dev , Opt . MntStart  ( ) ) ;
  }//done

  UMonitor  = new UdevMon ( & UdevContext ) ;
  UMonitor -> AddMatch ( Subsys_Block , NULL ) ;
  UMonitor -> EnableReceiving ( ) ;

  QSocketNotifier * Ntfr ;

  Ntfr = new QSocketNotifier ( UMonitor -> GetFD ( ) ,
                                 QSocketNotifier :: Read      , this ) ;
  connect ( Ntfr , SIGNAL ( activated    ( int ) ) ,
            this , SLOT   ( DeviceAction ( int ) ) ) ;

  Ntfr = new QSocketNotifier ( MMonitor  . GetFD ( ) ,
                                 QSocketNotifier :: Exception , this ) ;
  connect ( Ntfr , SIGNAL ( activated    ( int ) ) ,
            this , SLOT   ( MountAction  ( int ) ) ) ;

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
    bool C = Dev . Property ( FS_TYPE  ) == TYPE_LUKS , // It's container.
         M = P   . isEmpty  ( ) ; // Mount or unlock required.

    int T ; QString  Cmd , Arg = M ? N : P ;
    if ( M ) {
      if ( C ) { Cmd = Opt . UnlckCmd ( ) ; T = Opt . UnlckTO ( ) ;
      } else {   Cmd = Opt . MountCmd ( ) ; T = Opt . MountTO ( ) ;
      }//fi
    } else {
      if ( C ) { Cmd = Opt . LockCmd  ( ) ; T = Opt . LockTO  ( ) ;
      } else {   Cmd = Opt . UnmntCmd ( ) ; T = Opt . UnmntTO ( ) ;
      }//fi
    }//fi

    int R = ExecCmd ( Cmd , Arg , T ) ;

    if ( R ) { SetActions ( Dev ) ; }//fi // workaround for setChecked ()

    MInfo . RefreshMountInfo   ( ) ;
    QString E = Opt . EjectCmd ( ) ;
    if ( ! M && ! R && ! E . isEmpty ( ) &&
           MPoints ( Dev ) . isEmpty ( ) &&
           Dev . SysAttr ( SA_Events ) . contains ( Events_Eject ) ) {
      ExecCmd  ( E , N , Opt . EjectTO ( ) ) ;
    }//fi

  }//fi

  return Act ;

}// Listener :: exec

void Listener :: DeviceAction ( int socket ) { ( void ) socket ;

  UdevDev Dev ( UMonitor ) ; QString DAct = Dev . Action ( ) ;
  if ( DAct == ACT_add ) { AddDevice ( Dev , Opt . MntNew ( ) ) ;
  } else if ( DAct == ACT_remove ) { RemoveDevice ( Dev ) ;
  } else if ( DAct == ACT_change ) {
    bool M  = Opt . MntMedia ( ) && Dev . SysAttr ( SA_Rem  ) . toUInt ( ) ;
    if ( ! AddDevice ( Dev , M ) ) { RemoveDevice ( Dev ) ; }//fi
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
  bool  C = Dev . Property ( FS_TYPE  ) == TYPE_LUKS ;
          // It's container, FS_USAGE is "crypto"
  bool  T = Dev . Property ( FS_USAGE ) == USAGE_filesystem || C ;
  foreach ( QString R  , Opt . HideDevs ( ) ) {
    T = T && ! QRegExp ( R ) . exactMatch ( N ) ; // Not disabled by config.
  }//done

  if ( T ) { // is target device

    CurrDev = Dev . SysPath ( ) ;
    DevList += CurrDev ;
    DevList . removeDuplicates ( ) ; // overcaution

    if ( ! C && TryMount && MPoints ( Dev ) . isEmpty ( ) ) {
      ExecCmd ( Opt . MountCmd  ( ) , N , Opt . MountTO ( ) ) ;
    }//fi // LUKS containers are never automatically unlocked.

    SetActions ( Dev ) ;

  }//fi

  return T ;

}// Listener :: AddDevice

void Listener :: RemoveDevice ( UdevDev & Dev ) {

  QString P = Dev . SysPath ( ) ; DevList . removeOne ( P ) ;

  foreach ( ActPtr Act , FindActs ( P ) ) {
    removeAction ( Act ) ; delete Act ;
  }//done

  // Desperate attempt.
  foreach ( QString M , MPoints ( Dev ) ) {
    ExecCmd ( Opt . UnmntCmd ( ) , Mounts :: DecodeIFS ( M ) ,
              Opt . UnmntTO  ( ) ) ;
  }//done

  if ( ! Dev . Property ( DM_NAME ) . isEmpty ( ) ) {
    MountAction ( -1 ) ; // workaround for LUKS.
  }//fi

}// Listener :: RemoveDevice

ActList Listener :: FindActs ( const QString & Name ) {
  return findChildren < ActPtr > (
           QRegExp ( '^' + QRegExp :: escape ( Name ) + "( |$)" ) ) ;
}// Listener :: FindAct

QString Listener :: ToHum ( qulonglong KB ) {
  const int K = 1024 ; QString S = "" ;
  uint R = KB % K ; qulonglong Q = KB / K ;
  if ( R ) { S = QString :: number ( R ) + "K " + S ; }//fi
  R = Q % K ; Q /= K ;
  if ( R ) { S = QString :: number ( R ) + "M " + S ; }//fi
  R = Q % K ; Q /= K ;
  if ( R ) { S = QString :: number ( R ) + "G " + S ; }//fi
  if ( Q ) { S = QString :: number ( Q ) + "T " + S ; }//fi
  return S . trimmed ( ) ;
}// Listener :: ToHum

void Listener :: SetActions ( UdevDev & Dev ) {

  QString N = Dev . DevNode ( ) , L = Dev . Property ( FS_LABEL ) ,
          P = Dev . SysPath ( ) , T = Dev . Property ( FS_TYPE  ) ;
  qulonglong  C = Dev . SysAttr ( SA_Size ) . toULongLong ( ) / 2 ;
    // sysfs uses 512-bytes units.

  L = N . mid ( 5 ) + ' ' + T + ',' +
        ( L . isNull ( ) ? tr ( "(no label)" ) : '[' + L + ']' ) +
        ',' + ToHum  ( C ) ;

  QStringList M ; QIcon * I ;
  bool K = T == TYPE_LUKS ;

  if ( K ) { M = MapDevs ( Dev ) ; I = & LIcon ;
  } else {   M = MPoints ( Dev ) ; I = & UIcon ;
  }//fi

  bool U = M . isEmpty ( ) ;

  if ( U ) { M << "" ; I = K ? & DIcon : & MIcon ;
  } else { L += tr ( " on " ) ;
  }//fi

  ActList A = FindActs ( P ) ;

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

int Listener :: ExecCmd ( const QString & Cmd ,
                          const QString & Arg , int Timeout ) {

  int R = -1 ; QString A = Arg ;

  if ( Cmd . trimmed ( ) . isEmpty ( ) ) {

    QMessageBox :: critical ( NULL , TPref + tr ( "Error" ) ,
                              tr ( "Action disabled by configuration." ) ) ;

  } else {

    QProcess Pr ;
    Pr . setStandardInputFile  ( "/dev/null" ) ;
    Pr . setStandardOutputFile ( "/dev/null" ) ;
    QString  C = Cmd + " \"" + A . replace ( '"' , "\"\"\"" ) + '"' ;
    Pr . start ( C , QIODevice :: ReadOnly ) ;

    if ( ! Pr . waitForStarted ( StartTimeout ) ) {
      QMessageBox :: critical ( NULL , TPref + tr ( "Error" ) ,
                                tr ( "Can't execute" ) + " '" + C + "'" ) ;
    } else if ( ! Pr . waitForFinished ( Timeout ) ) {
      QMessageBox :: critical ( NULL , TPref + tr ( "Error" ) ,
                                "'" + C + "' " + tr ( "crashed." ) ) ;
    } else if ( ( R = Pr . exitCode ( ) ) ) {
      QStringList M = QTextCodec :: codecForLocale ( ) ->
                        toUnicode ( Pr . readAllStandardError ( ) ) .
                          split ( '\n' , QString ::  SkipEmptyParts ) ;
      M . removeDuplicates ( ) ;
      QMessageBox :: warning ( NULL , TPref + tr ( "Warning" ) ,
                               M . join ( "\n" ) ) ;
    }//fi

  }//fi

  return R ;

}// Listener :: ExecCmd

void Listener :: AddImage ( ) {
  QString F = QFileDialog :: getOpenFileName ( NULL ,
                TPref + tr ( "Select filesystem image" ) , "" ,
                tr ( "Images (*.img *.iso);;All files (*)" ) ) ;
  if ( ! F . isEmpty ( ) ) {
    ExecCmd ( Opt . AddImCmd ( ) , F , Opt . AddImTO ( ) ) ;
  }//fi
}// Listener :: AddImage

QStringList Listener :: MPoints ( UdevDev & Dev ) {
  return MInfo . MPoints ( Dev . DevNum ( ) ) ;
}// Listener :: MPoints

QStringList Listener :: MapDevs ( UdevDev & Dev ) {
  QStringList M ;
  foreach ( QString H , Dev . Holders ( ) ) {
    QFile F ( Dev . SysPath ( ) + "/holders/" + H + "/dm/name" ) ;
    if ( F . open ( QFile :: ReadOnly ) ) {
      H = QTextStream ( & F ) . readLine ( ) ;
      M << Mounts :: EncodeIFS ( H ) ; // (It may contain "\").
    }
  }//done
  return M ;
}// Listener :: MapDevs

//eof Listener.cpp
