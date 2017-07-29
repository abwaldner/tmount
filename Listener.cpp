
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
  FS_TYPE  = "ID_FS_TYPE"  , DM_NAME  = "DM_NAME"     ,
  DEV_TYPE = "DEVTYPE"     , DEV_BUS  = "ID_BUS"      ;

static CPtr // Property values.
  TYPE_LUKS = "crypto_LUKS" , TYPE_DISK = "disk" ,
  TYPE_PART = "partition"   , BUS_USB   = "usb"  ,
  USAGE_filesystem = "filesystem" ;

static const QString // UEvent actions.
  ACT_add = "add" , ACT_remove = "remove" , ACT_change = "change" ;

static CPtr // These constants are defined by "sysfs".
  Subsys_Block = "block"  , SA_Rem  = "removable" ,
  SA_Events    = "events" , SA_Size = "size"      ,
  Events_Eject = "eject_request" ;

static const int NoTimeout = -1 ;
  // For interactive external program, defined by QProcess.

static QKeyEvent
  EntrPrs ( QEvent :: KeyPress   , Qt :: Key_Enter , Qt :: NoModifier ) ,
  EntrRls ( QEvent :: KeyRelease , Qt :: Key_Enter , Qt :: NoModifier ) ;

Listener :: Listener ( QWidget * parent ) : QMenu ( parent ) {

  MIcon = QIcon ( Opt . toStr ( kMountPix  ) ) ;
  UIcon = QIcon ( Opt . toStr ( kUnmntPix  ) ) ;
  EIcon = QIcon ( Opt . toStr ( kEjectPix  ) ) ;
  RIcon = QIcon ( Opt . toStr ( kRemovePix ) ) ;
  DIcon = QIcon ( Opt . toStr ( kUnlockPix ) ) ;
  LIcon = QIcon ( Opt . toStr ( kLockPix   ) ) ;
  Env   = QProcessEnvironment :: systemEnvironment ( ) ;
  Suppl = reqNoAct ;

  UdevEnum En ( UdevContext ) ;
  En . MatchSubsys ( Subsys_Block ) ; En . ScanDevs ( ) ;
  foreach ( const UdevPair  P , En . GetList ( ) ) {
    UdevDev Dev ( UdevContext , P  . first ) ;
    AddDevice ( Dev , Opt . toBool ( kMntStart ) ) ;
  }//done

  UMonitor  = new UdevMon ( UdevContext ) ;
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

void Listener :: exec ( const QPoint & Loc ) {

  { const ActList AL = FindActs ( CurrDev ) ;
    if ( ! AL . isEmpty ( ) ) { setActiveAction ( AL . first ( ) ) ; }//fi
  }

  ActPtr Act = QMenu :: exec ( Loc ) ;

  if ( Act ) {

    QString AN = Act -> objectName ( ) ; // Primary key.
    CurrDev = AN . section ( ' ' , 0 , 0 ) ; // SysPath.
    UdevDev Dev ( UdevContext , CurrDev ) ;
    QString Node = Dev . DevNode ( ) ;
    const QString MPt = Mounts :: DecodeIFS ( AN . section ( ' ' , 1 , 1 ) ) ;
      // Mountpoint/dm-name if any.
    bool Cont = isLUKS ( Dev ) ,      // It's container.
         MoM  = ! MPt . isEmpty ( ) ; // Mounted or mapped.
    int RC = 0 ; // Return code for command.

    if ( MoM || ! Suppl ) { // Currently Suppl may be Eject or Remove.

      loKey C , T ;
      if ( MoM ) {
        C = Cont ? kLockCmd   : kUnmntCmd ; T = Cont ? kLockTO   : kUnmntTO ;
      } else {
        C = Cont ? kUnlockCmd : kMountCmd ; T = Cont ? kUnlockTO : kMountTO ;
      }//fi

      RC = ExecCmd ( Opt . toStr ( C ) , MoM ? MPt : Node ,
                     Opt . toInt ( T ) ) ;

    }//fi

    UdevDev WD ( WDisk ( Dev ) ) ; // whole disk.

    { const bool A = MoM && Opt . toBool ( kAutoEject ) && Ejectable ( WD ) ;
     if ( A && ! Suppl ) { Suppl = reqEject ; }//fi
    }

    if ( RC ) { SetActions ( Dev ) ; // workaround for setChecked ()
    } else if ( Suppl ) {

      QStringList Msg ; MInfo . RefreshMountInfo ( ) ;

      { QStringList L ;
        if ( ! isPart ( Dev ) ) { L << Dev . SysPath ( ) ;
        } else { Node = WD . DevNode ( ) ; L << Parts ( WD ) ;
        }//fi
        foreach ( const QString P , L ) {
          UdevDev D ( UdevContext , P ) ;
          foreach ( const QString M , MPoints ( D ) ) {
            Msg <<  D . DevNode ( ) + tr ( " mounted on " ) + M ;
          }//done
          foreach ( const QString M , D . Holders ( ) ) {
            Msg <<  D . DevNode ( ) + tr ( " mapped on "  ) + M ;
          }//done
        }//done
      }

      if ( ! Msg . isEmpty ( ) ) {
        QMessageBox :: critical (
          NULL , TPref + tr ( "Error" ) ,
          tr ( "Device "  ) + Node +
          tr ( " in use:" ) + "\n" + Msg . join ( "\n" )
        ) ;
      } else {

        QString Cmd ; int TO ;
        if ( Suppl == reqEject ) {
          Cmd = Opt . toStr ( kEjectCmd ) ;
          TO  = Opt . toInt ( kEjectTO  ) ;
        } else {
          Cmd = Opt . toStr ( kRemoveCmd ) ;
          TO  = Opt . toInt ( kRemoveTO  ) ;
        }//fi

        ExecCmd ( Cmd , Node , TO ) ;

      }//fi

    }//fi

  }//fi

  Suppl = reqNoAct ;

}// Listener :: exec

void Listener :: DeviceAction ( int socket ) { ( void ) socket ;

  UdevDev Dev ( * UMonitor ) ;
  const QString DAct = Dev . Action ( ) ;

  if ( DAct == ACT_add ) { AddDevice ( Dev , Opt . toBool ( kMntNew ) ) ;
  } else if ( DAct == ACT_remove ) { RemoveDevice ( Dev ) ;
  } else if ( DAct == ACT_change ) {
    const bool M  = Opt . toBool ( kMntMedia ) &&
                      Dev . SysAttr ( SA_Rem ) . toUInt ( ) ;
    if ( ! AddDevice ( Dev , M ) ) { RemoveDevice ( Dev ) ; }//fi
  }//fi

}// Listener :: DeviceAction

void Listener :: MountAction ( int socket ) { ( void ) socket ;

  MInfo . RefreshMountInfo ( ) ;

  foreach ( const QString P , DevList ) {
    UdevDev Dev ( UdevContext , P ) ; SetActions ( Dev ) ;
  }//done

}// Listener :: MountAction

bool Listener :: AddDevice ( UdevDev & Dev , bool TryMount ) {

  const QString Node = Dev . DevNode ( ) ;
  const bool Cont = isLUKS ( Dev ) ; // It's container, FS_USAGE is "crypto".
  bool Targ = Dev . Property ( FS_USAGE ) == USAGE_filesystem || Cont ;
  const QStringList Hide = Opt . toStr ( kHideDevs ) .
                             split ( ' ' , QString :: SkipEmptyParts ) ;

  foreach ( const QString R , Hide ) {
    Targ = Targ && ! QRegExp ( R ) . exactMatch ( Node ) ;
  }//done // Not disabled by config.

  if ( Targ ) { // is target device

    CurrDev = Dev . SysPath ( ) ; DevList += CurrDev ;
    DevList . removeDuplicates ( ) ; // overcaution

    if ( ! Cont && TryMount && MPoints ( Dev ) . isEmpty ( ) ) {

      ExecCmd ( Opt . toStr ( kMountCmd ) , Node ,
                Opt . toInt ( kMountTO  ) ) ;

    }//fi // LUKS containers are never automatically unlocked.

    SetActions ( Dev ) ;

  }//fi

  return Targ ;

}// Listener :: AddDevice

void Listener :: RemoveDevice ( UdevDev & Dev ) {

  QString SP = Dev . SysPath ( ) ;
  DevList . removeOne ( SP ) ;

  foreach ( const ActPtr Act , FindActs ( SP ) ) {
    removeAction ( Act ) ; delete Act ;
  }//done

  // Desperate attempt.
  foreach ( const QString M , MPoints ( Dev ) ) {
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

  const QString Node = Dev . DevNode  ( ) ,  SPth = Dev . SysPath ( ) ,
                FST  = Dev . Property ( FS_TYPE ) ;
  const qulonglong Cap = Dev . SysAttr ( SA_Size ) . toULongLong ( ) / 2 ;
    // Capacity in KiB, sysfs uses 512-bytes units.

  QString Lbl = Dev . Property ( FS_LABEL ) ;
  Lbl = Lbl . isEmpty ( ) ? tr ( "(no label)" ) : '[' + Lbl + ']' ;
  Lbl = Node . mid ( 5 ) + ' ' + FST + ',' + Lbl + ',' + ToHum ( Cap ) ;

  const bool Cont = FST == TYPE_LUKS ; // It's container.

  QStringList MPt = Cont ? DM_Maps ( Dev ) : MPoints ( Dev ) ;

  const bool MoM = ! MPt . isEmpty ( ) ; // Mounted or mapped.

  QIcon * Ico ;

  if ( MoM ) { Ico = Cont ? & LIcon : & UIcon ; Lbl += tr ( " on " ) ;
  } else     { Ico = Cont ? & DIcon : & MIcon ; MPt << "" ;
  }//fi

  ActList AL = FindActs ( SPth ) ; const int Sz = MPt . size ( ) ;

  while ( AL . size ( ) > Sz ) {
    const ActPtr Act = AL . takeLast ( ) ;
    removeAction ( Act ) ; delete Act ;
  }//done
  while ( AL . size ( ) < Sz ) {
    const ActPtr Act = addAction ( "" ) ;
    AL += Act ; Act -> setCheckable ( true ) ;
  }//done

  foreach ( const QString MP , MPt ) {
    ActPtr Act = AL . takeFirst ( ) ; Act -> setIcon ( * Ico ) ;
    Act -> setText ( Lbl + MP ) ; Act -> setChecked ( MoM ) ;
    Act -> setObjectName ( ( SPth + ' ' + MP ) . trimmed ( ) ) ;
  }//done

}// Listener :: SetActions

int Listener :: ExecCmd ( const QString & Cmd ,
                          const QString & Arg , int Timeout ) {

  int RC = -1 ; // Return code.

  if ( Cmd . trimmed ( ) . isEmpty ( ) ) {

    QMessageBox :: critical ( NULL , TPref + tr ( "Error" ) ,
                              tr ( "Action disabled by configuration." ) ) ;

  } else {

    foreach ( const OptPair P , Opt . GetAll ( ) ) {
      Env . insert ( "TMOUNT_" + P . first , P . second ) ;
    }//done

    QProcess Proc ;
    Proc . setStandardInputFile  ( "/dev/null" ) ;
    Proc . setStandardOutputFile ( "/dev/null" ) ;
    Proc . setProcessEnvironment ( Env ) ;

    const int TO = Timeout ? Timeout * 1000 : NoTimeout ;
    QString A = Arg ;
    const QString C = Cmd + " \"" + A . replace ( '"' , "\"\"\"" ) + '"' ;

    Proc . start ( C , QIODevice :: ReadOnly ) ;

    if ( ! Proc . waitForStarted ( StartTO ) ) {
      QMessageBox :: critical ( NULL , TPref + tr ( "Error" ) ,
                                tr ( "Can't execute" ) + " '" + C + "'" ) ;
    } else if ( ! Proc . waitForFinished ( TO ) ) {
      QMessageBox :: critical ( NULL , TPref + tr ( "Error" ) ,
                                "'" + C + "' " + tr ( "crashed." ) ) ;
    } else if ( ( RC = Proc . exitCode ( ) ) ) {
      QStringList Msg = QTextCodec :: codecForLocale ( ) ->
                          toUnicode ( Proc . readAllStandardError ( ) ) .
                            split ( '\n' , QString ::  SkipEmptyParts ) ;
      Msg . removeDuplicates ( ) ;
      QMessageBox :: warning ( NULL , TPref + tr ( "Warning" ) ,
                               Msg . join ( "\n" ) ) ;
    }//fi

  }//fi

  return RC ;

}// Listener :: ExecCmd

void Listener :: AddImage ( ) {
  const QString F = QFileDialog :: getOpenFileName ( NULL ,
                      TPref + tr ( "Select filesystem image" ) , "" ,
                      tr ( "Images (*.img *.iso);;All files (*)" ) ) ;
  if ( ! F . isEmpty ( ) ) {
    ExecCmd ( Opt . toStr ( kAddImgCmd ) , F , Opt . toInt ( kAddImgTO ) ) ;
  }//fi
}// Listener :: AddImage

QStringList Listener :: MPoints ( UdevDev & Dev ) {
  return MInfo . MPoints ( Dev . DevNum ( ) ) ;
}// Listener :: MPoints

QStringList Listener :: DM_Maps ( UdevDev & Dev ) {
  QStringList M ;
  foreach ( QString H , Dev . Holders ( ) ) {
    QFile F ( Dev . SysPath ( ) + "/holders/" + H + "/dm/name" ) ;
    if ( F . open ( QFile :: ReadOnly ) ) {
      H = QTextStream ( & F ) . readLine ( ) ;
      M << Mounts :: EncodeIFS ( H ) ; // (It may contain "\").
    }//fi
  }//done
  return M ;
}// Listener :: DM_Maps

QStringList Listener :: Parts ( UdevDev & Dev ) {
  QStringList L ; UdevEnum En ( UdevContext ) ; En . MatchParent ( Dev ) ;
  En . MatchProperty ( DEV_TYPE , TYPE_PART ) ; En . ScanDevs ( ) ;
  foreach ( UdevPair const P , En . GetList ( ) ) { L << P . first  ; }//done
  return L ;
}// Listener :: Parts

UdevDev & Listener :: WDisk ( UdevDev & Dev ) {
  return * new UdevDev (
     isPart ( Dev ) ? Dev . FindParent ( Subsys_Block , TYPE_DISK ) : Dev
  ) ;
}// Listener :: WDisk

bool Listener :: Ejectable ( UdevDev & Dev ) {
  return Dev . SysAttr ( SA_Events ) . contains ( Events_Eject ) ;
}// Listener :: Ejectable

bool Listener :: isLUKS ( UdevDev & Dev ) {
  return Dev . Property ( FS_TYPE ) == TYPE_LUKS ;
}// Listener :: isLUKS

bool Listener :: isPart ( UdevDev & Dev ) {
  return Dev . DevType ( ) == TYPE_PART ;
}// Listener :: isLUKS

void Listener :: contextMenuEvent ( QContextMenuEvent * event ) {

  QMenu SMn ; // Supplementary actions menu.
  const ActPtr Act = activeAction ( ) ;
  const QString AN = Act -> objectName ( ) ; // Primary key.

  UdevDev Dev ( UdevContext , AN . section ( ' ' , 0 , 0 ) ) ;
  UdevDev WD  ( WDisk ( Dev ) ) ; // Whole disk.

  const bool MoM  = ! AN . section ( ' ' , 1 , 1 ) . isEmpty ( ) ;
  const bool Cont = isLUKS ( Dev ) ; // It's container.

  ActPtr SA ;

  { const QString Txt = MoM
                          ? ( Cont ? tr ( "Lock"   ) : tr ( "Unmount" ) )
                          : ( Cont ? tr ( "Unlock" ) : tr ( "Mount"   ) ) ;
    SA = SMn . addAction ( Act -> icon ( ) , Txt ) ;
    SA -> setCheckable ( true ) ;
    SA -> setChecked ( Act -> isChecked ( ) ) ;
    SA -> setData ( ( uint ) reqNoAct ) ; SMn . setActiveAction ( SA ) ;
  }

  if ( ! Opt . toStr ( kEjectCmd ) . isEmpty ( ) && Ejectable ( WD ) ) {
    SA = SMn . addAction ( EIcon , tr ( "Eject" ) ) ;
    SA -> setData ( ( uint ) reqEject ) ; SMn . setActiveAction ( SA ) ;
  }//fi

  if ( ! Opt . toStr ( kRemoveCmd ) . isEmpty ( ) &&
       WD . Property ( DEV_BUS ) == BUS_USB ) {
    SA = SMn . addAction ( RIcon , tr ( "Remove" ) ) ;
    SA -> setData ( ( uint ) reqRemove ) ; SMn . setActiveAction ( SA ) ;
  }//fi

  SA = SMn . exec ( event -> globalPos ( ) ) ; setActiveAction ( Act ) ;

  if ( SA ) {
    Suppl = ( ActReq ) ( SA -> data ( ) . toUInt ( ) ) ;
    keyPressEvent ( & EntrPrs ) ; keyPressEvent ( & EntrRls ) ;
  }//fi

}// Listener :: contextMenuEvent

//eof Listener.cpp
