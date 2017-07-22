
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

static const int NoTimeout = -1 ;
  // For interactive external program, defined by QProcess.

static QKeyEvent
  EP ( QEvent :: KeyPress   , Qt :: Key_Enter , Qt :: NoModifier ) ,
  ER ( QEvent :: KeyRelease , Qt :: Key_Enter , Qt :: NoModifier ) ;

Listener :: Listener ( QWidget * parent ) : QMenu ( parent ) {

  MIcon = QIcon ( Opt . toStr ( kMountPix ) ) ;
  UIcon = QIcon ( Opt . toStr ( kUnmntPix ) ) ;
  EIcon = QIcon ( Opt . toStr ( kEjectPix ) ) ;
  DIcon = QIcon ( Opt . toStr ( kUnlckPix ) ) ;
  LIcon = QIcon ( Opt . toStr ( kLockPix  ) ) ;
  Env = QProcessEnvironment :: systemEnvironment ( ) ;
  Suppl = reqNoAct ;

  UdevEnum En ( & UdevContext ) ;
  En . MatchSubsys ( Subsys_Block ) ; En . ScanDevs ( ) ;
  foreach ( UdevPair P , En . GetList ( ) ) {
    UdevDev Dev ( & UdevContext , P . first ) ;
    AddDevice ( Dev , Opt . toBool ( kMntStart ) ) ;
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

void Listener :: exec (const QPoint & Loc ) {

  ActList T = FindActs ( CurrDev ) ;
  if ( ! T . isEmpty ( ) ) { setActiveAction ( T . first ( ) ) ; }//fi

  ActPtr Act = QMenu :: exec ( Loc ) ;

  if ( Act ) {

    QString P = Act -> objectName ( ) ; // Primary key.
    CurrDev = P . section ( ' ' , 0 , 0 ) ; // SysPath.
    UdevDev Dev ( & UdevContext , CurrDev ) ;
    QString N = Dev . DevNode ( ) ;
    P = Mounts :: DecodeIFS ( P . section ( ' ' , 1 , 1 ) ) ;
      // Now mountpoint/dm-name if any.
    bool C = Dev . Property ( FS_TYPE  ) == TYPE_LUKS , // It's container.
         M = P   . isEmpty  ( ) ; // Mount or unlock required.
    int R = 0 ; // Return code for command.

    if ( ! ( M && Suppl ) ) { // Currently Suppl may be Eject or Remove.
      loKey K , T ;
      if ( M ) { K = C ? kUnlckCmd : kMountCmd ; T = C ? kUnlckTO : kMountTO ;
      } else {   K = C ? kLockCmd  : kUnmntCmd ; T = C ? kLockTO  : kUnmntTO ;
      }//fi
      R = ExecCmd ( Opt . toStr ( K ) , M ? N : P , Opt . toInt ( T ) ) ;
    }//fi

    if ( R ) { SetActions ( Dev ) ; // workaround for setChecked ()
    } else {

      MInfo . RefreshMountInfo ( ) ;
      QString E = Opt . toStr ( kEjectCmd ) ;

      if ( // Eject or Autoeject requested, is applicable and configured.
        ( ( ! M && Opt . toBool ( kAutoEject ) ) || Suppl == reqEject ) &&
        Ejectable ( Dev ) && ! E . isEmpty ( ) &&
        MPoints ( Dev ) . isEmpty ( ) // Overcaution.
      ) { ExecCmd ( E , N , Opt . toInt ( kEjectTO ) ) ;
      }//fi

    }//fi

  }//fi

  Suppl = reqNoAct ;

}// Listener :: exec

void Listener :: DeviceAction ( int socket ) { ( void ) socket ;

  UdevDev Dev ( UMonitor ) ; QString DAct = Dev . Action ( ) ;
  if ( DAct == ACT_add ) { AddDevice ( Dev , Opt . toBool ( kMntNew ) ) ;
  } else if ( DAct == ACT_remove ) { RemoveDevice ( Dev ) ;
  } else if ( DAct == ACT_change ) {
    bool M  = Opt . toBool ( kMntMedia ) &&
                Dev . SysAttr ( SA_Rem ) . toUInt ( ) ;
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

  QString N = Dev . DevNode ( ) , H = Opt . toStr ( kHideDevs ) ;
  bool  C = Dev . Property ( FS_TYPE  ) == TYPE_LUKS ;
          // It's container, FS_USAGE is "crypto"
  bool  T = Dev . Property ( FS_USAGE ) == USAGE_filesystem || C ;
  foreach ( QString R , H . split ( ' ' , QString :: SkipEmptyParts ) ) {
    T = T && ! QRegExp ( R ) . exactMatch ( N ) ; // Not disabled by config.
  }//done

  if ( T ) { // is target device

    CurrDev = Dev . SysPath ( ) ;
    DevList += CurrDev ;
    DevList . removeDuplicates ( ) ; // overcaution

    if ( ! C && TryMount && MPoints ( Dev ) . isEmpty ( ) ) {
      ExecCmd ( Opt . toStr ( kMountCmd ) , N , Opt . toInt ( kMountTO ) ) ;
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
    ExecCmd ( Opt . toStr ( kUnmntCmd ) , Mounts :: DecodeIFS ( M ) ,
              Opt . toInt ( kUnmntTO  ) ) ;
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

  int R = -1 ;

  if ( Cmd . trimmed ( ) . isEmpty ( ) ) {

    QMessageBox :: critical ( NULL , TPref + tr ( "Error" ) ,
                              tr ( "Action disabled by configuration." ) ) ;

  } else {

    foreach ( OptPair P , Opt . GetAll ( ) ) {
      Env . insert ( "TMOUNT_" + P . first , P . second ) ;
    }//done
    QProcess P ;
    P . setStandardInputFile  ( "/dev/null" ) ;
    P . setStandardOutputFile ( "/dev/null" ) ;
    P . setProcessEnvironment ( Env ) ;
    QString A = Arg ; int T = Timeout ? Timeout * 1000 : NoTimeout ;
    QString C = Cmd + " \"" + A . replace ( '"' , "\"\"\"" ) + '"' ;
    P . start ( C , QIODevice :: ReadOnly ) ;

    if ( ! P . waitForStarted ( StartTimeout ) ) {
      QMessageBox :: critical ( NULL , TPref + tr ( "Error" ) ,
                                tr ( "Can't execute" ) + " '" + C + "'" ) ;
    } else if ( ! P . waitForFinished ( T  ) ) {
      QMessageBox :: critical ( NULL , TPref + tr ( "Error" ) ,
                                "'" + C + "' " + tr ( "crashed." ) ) ;
    } else if ( ( R = P . exitCode ( ) ) ) {
      QStringList M = QTextCodec :: codecForLocale ( ) ->
                        toUnicode ( P . readAllStandardError ( ) ) .
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
    ExecCmd ( Opt . toStr ( kAddImCmd ) , F , Opt . toInt ( kAddImTO ) ) ;
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

bool Listener :: Ejectable ( UdevDev & Dev ) {
  return Dev . SysAttr ( SA_Events ) . contains ( Events_Eject ) ;
}// Listener :: Ejectable

void Listener :: contextMenuEvent ( QContextMenuEvent * event ) {

  ActPtr Act = activeAction ( ) , R ; QMenu S ;
  QString P = Act -> objectName ( ) ; // Primary key.
  UdevDev Dev ( & UdevContext , P . section ( ' ' , 0 , 0 ) ) ;
  bool C = Dev . Property ( FS_TYPE  ) == TYPE_LUKS  , // It's container.
       M = P . section ( ' ' , 1 , 1 ) . isEmpty ( ) ; // Not mounted.

  R = S . addAction ( Act -> icon ( ) ,
                      M ? ( C ? tr ( "Unlock" ) : tr ( "Mount"   ) )
                        : ( C ? tr ( "Lock"   ) : tr ( "Unmount" ) ) ) ;
  R -> setData ( ( uint ) reqNoAct ) ; S . setActiveAction ( R ) ;

  if ( ! Opt . toStr ( kEjectCmd ) . isEmpty ( ) && Ejectable ( Dev ) ) {
    R = S . addAction ( EIcon , tr ( "Eject" ) ) ;
    R -> setData ( ( uint ) reqEject ) ; S . setActiveAction ( R ) ;
  }//fi

  R = S . exec ( event -> globalPos ( ) ) ;  setActiveAction ( Act ) ;
  if ( R ) {
    Suppl = ( ActReq ) ( R -> data ( ) . toUInt ( ) ) ;
    keyPressEvent ( & EP ) ; keyPressEvent ( & ER ) ;
  }//fi

}// Listener :: contextMenuEvent

//eof Listener.cpp
