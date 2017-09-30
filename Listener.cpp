
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#include <QSocketNotifier>
#include <QMessageBox>
#include <QTextCodec>
#include <QFile>
#include <QFileDialog>

#include "Listener.h"

//   These constants are hardcoded in "udev"
// and "util-linux"(in libblkid) packages.

static CPtr // Property names.
  FS_USAGE = "ID_FS_USAGE" , FS_LABEL = "ID_FS_LABEL" ,
  FS_TYPE  = "ID_FS_TYPE"  , DM_NAME  = "DM_NAME"     ,
  DEV_TYPE = "DEVTYPE"     , DEV_BUS  = "ID_BUS"      ;

static CPtr // Property values.
  TYPE_LUKS = "crypto_LUKS" , TYPE_DISK = "disk"      ,
  USAGE_FS  = "filesystem"  , TYPE_PART = "partition" ;
static const QRegExp HOTPLUG ( "usb|pcmcia|ieee1394|firewire|mmc|ccw" ) ;

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

static const QString :: SplitBehavior SEP = QString :: SkipEmptyParts ;

inline QString sect ( const QString & S , int K ) {
  return S . section ( ' ' , K , K ) ;
}// sect

Listener :: Listener ( QWidget * parent ) :
                         QMenu ( parent ) , UMonitor ( UdevContext ) {

  TIcon = QIcon ( Opt . toStr ( kTMntPix   ) ) ;
  MIcon = QIcon ( Opt . toStr ( kMountPix  ) ) ;
  UIcon = QIcon ( Opt . toStr ( kUnmntPix  ) ) ;
  EIcon = QIcon ( Opt . toStr ( kEjectPix  ) ) ;
  RIcon = QIcon ( Opt . toStr ( kRemovePix ) ) ;
  DIcon = QIcon ( Opt . toStr ( kUnlockPix ) ) ;
  LIcon = QIcon ( Opt . toStr ( kLockPix   ) ) ;

  setWindowIcon ( TIcon ) ; TPref = Opt . AppName ( ) + " - " ;

  Env   = QProcessEnvironment :: systemEnvironment ( ) ;
  Suppl = reqNoAct ;

  UdevEnum En ( UdevContext ) ;
  En . MatchSubsys ( Subsys_Block ) ; En . ScanDevs ( ) ;
  foreach ( const UdevPair  P , En . GetList ( ) ) {
    UdevDev Dev ( UdevContext , P  . first ) ;
    AddDevice ( Dev , Opt . toBool ( kMntStart ) , false ) ;
  }//done

  UMonitor . AddMatch ( Subsys_Block , NULL ) ;
  UMonitor . EnableReceiving ( ) ;

  connect ( & UMonitor , SIGNAL ( Changed      ( ) ) ,
            this       , SLOT   ( DeviceAction ( ) ) ) ;

  connect ( & MInfo    , SIGNAL ( Changed      ( ) ) ,
            this       , SLOT   ( MountAction  ( ) ) ) ;

}// Listener

Listener :: ~Listener ( ) { }// ~Listener

void Listener :: exec ( const QPoint & Loc ) {

  { const ActList AL = FindActs ( CurrDev ) ;
    if ( ! AL . isEmpty ( ) ) { setActiveAction ( AL . first ( ) ) ; }//fi
  }

  ActPtr Act = QMenu :: exec ( Loc ) ;

  if ( Act ) {

    const QString AN = Act -> objectName ( ) ; // Primary key.
    CurrDev = sect ( AN , 0 ) ; // SysPath.
    UdevDev Dev ( UdevContext , CurrDev ) ;
    QString Node = Dev . DevNode ( ) ;
    const QString MP = Mounts :: DecodeIFS ( sect ( AN , 1 ) ) ;
      // Mountpoint/dm-name if any.
    const bool Cont = isLUKS ( Dev ) ,     // It's container.
               MoM  = ! MP . isEmpty ( ) ; // It's mounted or mapped.
    int RC = 0 ; // Return code for command.

    if ( MoM || ! Suppl ) { // Currently Suppl may be Eject or Remove.

      loKey C , T , S ;
      if ( MoM ) {
        if ( Cont ) { C = kLockCmd   ; T = kLockTO   ; S = kLockShow   ;
        }    else   { C = kUnmntCmd  ; T = kUnmntTO  ; S = kUnmntShow  ;
        }//fi
      } else {
        if ( Cont ) { C = kUnlockCmd ; T = kUnlockTO ; S = kUnlockShow ;
        }    else   { C = kMountCmd  ; T = kMountTO  ; S = kMountShow  ;
        }//fi
      }//fi

      RC = ExecCmd ( Opt . toStr ( C ) , MoM ? MP : Node ,
                     Opt . toInt ( T ) , Opt . toBool ( S ) ) ;

    }//fi

    UdevDev WD ( WDisk ( Dev ) ) ; // whole disk.
    bool Show = Opt . toBool ( kEjectShow ) ;

    { const bool A =
        ! Opt . toStr  ( kEjectCmd  ) . trimmed ( ) . isEmpty ( ) &&
          Opt . toBool ( kAutoEject ) && Ejectable ( WD ) ;
      if ( MoM && A && ! Suppl ) {
        Suppl = reqEject ; Show = Opt . toBool ( kAutoEjShow ) ;
      }//fi
    }

    if ( RC ) { SetActions ( Dev ) ; // workaround for setChecked ()
    } else if ( Suppl ) {

      QStringList Msg ; MInfo . RefreshMountInfo ( ) ;

      { QStringList L ;
        if ( ! isPart ( Dev ) ) { L << Dev . SysPath ( ) ;
        } else { Node = WD . DevNode ( ) ; L << Parts ( WD ) ;
        }//fi
        foreach ( const QString P , L ) {
          UdevDev D ( UdevContext , P ) ; QString N = D . DevNode ( ) ;
          foreach ( const QString M , MPoints ( D ) ) {
            Msg << N + tr ( " mounted on " ) + sect ( M , 0 ) ;
          }//done
          foreach ( const QString M , Holders ( D ) ) {
            Msg << N + tr ( " mapped on "  ) + M ;
          }//done
        }//done
      }

      if ( ! Msg . isEmpty ( ) ) {

        QMessageBox :: critical (
          this , TPref + tr ( "Error" ) ,
          tr ( "Device "  ) + Node +
          tr ( " in use:" ) + "\n" + Msg . join ( "\n" )
        ) ;

      } else {

        QString Cmd ; int TO ;
        if ( Suppl == reqEject ) {
          Cmd = Opt . toStr ( kEjectCmd ) ;
          TO  = Opt . toInt ( kEjectTO  ) ;
        } else {
          Cmd  = Opt . toStr  ( kRemoveCmd  ) ;
          TO   = Opt . toInt  ( kRemoveTO   ) ;
          Show = Opt . toBool ( kRemoveShow ) ;
        }//fi

        ExecCmd ( Cmd , Node , TO , Show ) ;

      }//fi

    }//fi

  }//fi

  Suppl = reqNoAct ;

}// Listener :: exec

void Listener :: DeviceAction ( ) {

  UdevDev Dev ( UMonitor ) ;
  const QString DAct = Dev . Action ( ) ;

  if ( DAct == ACT_add ) {

    const bool M = Opt . toBool ( kMntNew  ) ,
               S = Opt . toBool ( kNewShow ) ;
    AddDevice ( Dev , M , S ) ;

  } else if ( DAct == ACT_remove ) { RemoveDevice ( Dev ) ;

  } else if ( DAct == ACT_change ) {

    const bool M = Opt . toBool  ( kMntMedia  ) &&
                   Dev . SysAttr ( SA_Rem     ) . toUInt ( ) ,
               S = Opt . toBool  ( kMediaShow ) ;
      //   The test of "removable" attribute prevent attempt to mount of
      // cryptsetup's dm-* devices.

    if ( ! AddDevice ( Dev , M , S ) ) { RemoveDevice ( Dev ) ; }//fi
      // The extracted media looks like an non-target device.

  }//fi

}// Listener :: DeviceAction

void Listener :: MountAction ( ) {

  MInfo . RefreshMountInfo ( ) ;

  foreach ( const QString P , DevList ( ) ) {
    UdevDev Dev ( UdevContext , P ) ;
    if ( ! isLUKS ( Dev ) ) { SetActions ( Dev ) ; }//fi
  }//done

}// Listener :: MountAction

QStringList Listener :: DevList ( ) const {
  QStringList L ;
  foreach ( const ActPtr A , findChildren < ActPtr > ( QRegExp ( "^." ) ) ) {
    L << sect ( A -> objectName ( ) , 0 ) ;
  }//fi
  L . removeDuplicates ( ) ;
  return L ;
}// Listener :: DevList

bool Listener :: AddDevice (
                   const UdevDev & Dev , bool TryMount , bool Show ) {

  const QString Node = Dev . DevNode ( ) ;
  const bool Cont = isLUKS ( Dev ) ; // It's container, FS_USAGE is "crypto".
  bool Targ = Dev . Property ( FS_USAGE ) == USAGE_FS || Cont ;
  const QStringList Hide = Opt . toStr ( kHideDevs ) . split ( ' ' , SEP ) ;

  foreach ( const QString R , Hide ) {
    Targ = Targ && ! QRegExp ( R ) . exactMatch ( Node ) ;
  }//done // Not disabled by config.

  if ( Targ ) { // is target device

    CurrDev = Dev . SysPath ( ) ;

    if ( ! Cont && TryMount && MPoints ( Dev ) . isEmpty ( ) ) {

      ExecCmd ( Opt . toStr ( kMountCmd ) , Node ,
                Opt . toInt ( kMountTO  ) , Show ) ;

    }//fi // LUKS containers are never automatically unlocked.

    SetActions ( Dev ) ;

  }//fi

  return Targ ;

}// Listener :: AddDevice

void Listener :: RemoveDevice ( const UdevDev & Dev ) {

  const QString SP = Dev . SysPath ( ) ,
                DN = Mounts :: EncodeIFS ( Dev . Property ( DM_NAME ) ) ;

  foreach ( const ActPtr Act , FindActs ( SP ) ) {
    removeAction ( Act ) ; delete Act ;
  }//done

  // Desperate attempt.
  foreach ( const QString M , MPoints  ( Dev ) ) {
    ExecCmd ( Opt . toStr ( kUnmntCmd  ) ,
              Mounts :: DecodeIFS ( sect ( M , 0 ) ) ,
              Opt . toInt ( kUnmntTO   ) , false ) ;
  }//done

  if ( ! DN . isEmpty ( ) ) {
    QRegExp RE ( "^[^ ]+ " + QRegExp :: escape ( DN ) + '$' ) ;
    foreach ( const ActPtr Act , findChildren < ActPtr > ( RE ) ) {
      UdevDev UD ( UdevContext , sect ( Act -> objectName ( ) , 0 ) ) ;
      SetActions ( UD ) ;
    }//done
  }//fi

}// Listener :: RemoveDevice

ActList Listener :: FindActs ( const QString & Key ) {
  QRegExp RE ( '^' + QRegExp :: escape ( Key ) + "( |$)" ) ;
  return findChildren < ActPtr > ( RE ) ;
}// Listener :: FindActs

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

void Listener :: SetActions ( const UdevDev & Dev ) {

  const QString Node = Dev . DevNode ( ) , SPth = Dev . SysPath ( ) ,
                FST  = Dev . Property ( FS_TYPE ) ;
  const qulonglong Cap = Dev . SysAttr ( SA_Size ) . toULongLong ( ) / 2 ;
    // Capacity in KiB, sysfs uses 512-bytes units.

  QString Lbl = Dev . Property ( FS_LABEL ) ;
  Lbl = Lbl . isEmpty ( ) ? tr ( "(no label)" ) : '[' + Lbl + ']' ;
  Lbl = Node . mid ( 5 ) + ' ' + FST + ',' + Lbl + ',' + ToHum ( Cap ) ;

  const  bool Cont = FST == TYPE_LUKS ; // It's container.
  QStringList MPts = Cont ? DM_Maps ( Dev ) : MPoints ( Dev ) ;
  const  bool MoM  = ! MPts . isEmpty ( ) ; // It's mounted or mapped.

  QIcon * Ico ;
  if  ( MoM ) { Ico = Cont ? & LIcon : & UIcon ; Lbl  += tr ( " on " ) ;
  } else      { Ico = Cont ? & DIcon : & MIcon ; MPts << "" ;
  }//fi

  ActList AL = FindActs ( SPth ) ; const int Sz = MPts . size ( ) ;

  while ( AL . size ( ) > Sz ) { // Remove redundant items.
    const ActPtr Act = AL . takeLast ( ) ;
    removeAction ( Act ) ; delete Act ;
  }//done
  while ( AL . size ( ) < Sz ) { // Add laking items.
    const ActPtr Act = addAction ( "" ) ;
    AL += Act ; Act -> setCheckable ( true ) ;
  }//done

  foreach ( const QString MP , MPts ) {
    QString M = sect ( MP , 0 ) ;
    ActPtr Act = AL . takeFirst ( ) ;
    Act -> setObjectName ( ( SPth + ' ' + M ) . trimmed ( ) ) ;
    Act -> setIcon ( * Ico ) ; Act -> setChecked ( MoM ) ;
    M += Cont && MoM ? " (" + sect ( MP , 1 ) + ")" : "" ;
    Act -> setText ( Lbl + M ) ;
  }//done

}// Listener :: SetActions

int Listener :: ExecCmd ( const QString & Cmd ,
                          const QString & Arg , int Timeout , bool Show ) {

  int RC = -1 ; // Return code.

  if ( Cmd . trimmed ( ) . isEmpty ( ) ) {

    QMessageBox :: critical ( this , TPref + tr ( "Error" ) ,
                              tr ( "Action disabled by configuration." ) ) ;

  } else {

    foreach ( const OptPair P , Opt . GetAll ( ) ) {
      Env . insert ( "TMOUNT_" + P . first , P . second ) ;
    }//done

    QProcess Proc ;
    Proc . setProcessEnvironment ( Env ) ;
    Proc . setStandardInputFile  ( "/dev/null" ) ;
    if ( ! Show ) { Proc . setStandardOutputFile ( "/dev/null" ) ; }//fi

    const int TO = Timeout ? Timeout * 1000 : NoTimeout ;
    const QString C = Cmd + " \"" +
                      QString ( Arg ) . replace ( '"' , "\"\"\"" ) + '"' ;

    Proc . start ( C , QIODevice :: ReadOnly ) ;

    if ( ! Proc . waitForStarted ( StartTO ) ) {

      QMessageBox :: critical ( this , TPref + tr ( "Error" ) ,
                                tr ( "Can't execute '" ) + C + "'" ) ;

    } else if ( ! Proc . waitForFinished ( TO ) ) {

      Proc . kill ( ) ;
      QMessageBox :: critical ( this , TPref + tr ( "Error" ) ,
                                "'" + C + tr ( "' crashed." ) ) ;

    } else if ( ( RC = Proc . exitCode ( ) ) ) {

      QStringList Msg = QTextCodec :: codecForLocale ( ) ->
                          toUnicode ( Proc . readAllStandardError ( ) ) .
                            split ( '\n' , SEP ) ;
      Msg . removeDuplicates ( ) ;
      QMessageBox :: warning ( this , TPref + tr ( "Warning" ) ,
                                      Msg . join ( "\n" ) ) ;

    } else if ( Show ) {

      QStringList Msg = QTextCodec :: codecForLocale ( ) ->
                          toUnicode ( Proc . readAllStandardOutput ( ) ) .
                            split ( '\n' , SEP ) ;
      Msg . removeDuplicates ( ) ;
      if ( Msg . isEmpty ( ) ) {
        Msg << "'" + C + tr ( "' - successful completion." ) ;
      }//fi
      QMessageBox :: information ( this , NULL , Msg . join ( "\n" ) ) ;

    }//fi

  }//fi

  return RC ;

}// Listener :: ExecCmd

void Listener :: AddImage ( ) {
  const QString Cmd = Opt . toStr ( kAddImgCmd ) . trimmed ( ) ;
  if ( Cmd . isEmpty ( ) ) {
    QMessageBox :: critical ( this , TPref + tr ( "Error" ) ,
                              tr ( "Action disabled by configuration." ) ) ;
  } else {
    const QString F = QFileDialog :: getOpenFileName ( this ,
                        TPref + tr ( "Select filesystem image" ) , "" ,
                        tr ( "Images (*.img *.iso);;All files (*)" ) ) ;
    if ( ! F . isEmpty ( ) ) {
      ExecCmd ( Cmd , F , Opt . toInt  ( kAddImgTO   ) ,
                          Opt . toBool ( kAddImgShow ) ) ;
    }//fi
  }//fi
}// Listener :: AddImage

QStringList Listener :: MPoints ( const UdevDev & Dev ) const {
  return MInfo . MPoints ( Dev . DevNum ( ) ) ;
}// Listener :: MPoints

QStringList Listener :: DM_Maps ( const UdevDev & Dev ) const {
  QStringList ML ;
  foreach ( const QString H , Holders ( Dev ) ) {
    QString M = sect ( H , 1 ) ;
    if ( ! M . isEmpty ( ) ) { ML << M + ' ' + sect ( H , 0 ) ; }//fi
  }//done
  return ML ;
}// Listener :: DM_Maps

QStringList Listener :: Holders ( const UdevDev & Dev ) const {
  QStringList SL ;
  foreach ( const QFileInfo I ,
            QDir ( Dev . SysPath ( ) + "/holders" ) .
              entryInfoList ( QDir :: Dirs | QDir :: NoDotAndDotDot ) ) {
    UdevDev D ( UdevContext , I . readLink ( ) ) ;
    SL << Mounts :: EncodeIFS ( D . DevNode  ( ) ) + ' ' +
          Mounts :: EncodeIFS ( D . Property ( DM_NAME ) ) ;
  }//done
  return SL ;
}// Listener :: Holders

QStringList Listener :: Parts ( const UdevDev & Dev ) const {
  QStringList L ; UdevEnum En ( UdevContext ) ; En . MatchParent ( Dev ) ;
  En . MatchProperty ( DEV_TYPE , TYPE_PART ) ; En . ScanDevs ( ) ;
  foreach ( UdevPair const P , En . GetList ( ) ) { L << P . first  ; }//done
  return L ;
}// Listener :: Parts

UdevDev & Listener :: WDisk ( const UdevDev & Dev ) {
  return * new UdevDev (
     isPart ( Dev ) ? Dev . FindParent ( Subsys_Block , TYPE_DISK ) : Dev
  ) ;
}// Listener :: WDisk

bool Listener :: Ejectable ( const UdevDev & Dev ) {
  return Dev . SysAttr ( SA_Events ) . contains ( Events_Eject ) ;
}// Listener :: Ejectable

bool Listener :: isLUKS ( const UdevDev & Dev ) {
  return Dev . Property ( FS_TYPE ) == TYPE_LUKS ;
}// Listener :: isLUKS

bool Listener :: isPart ( const UdevDev & Dev ) {
  return Dev . DevType ( ) == TYPE_PART ;
}// Listener :: isLUKS

void Listener :: contextMenuEvent ( QContextMenuEvent * event ) {

  QMenu SMn ; // Supplementary actions menu.
  const ActPtr Act = activeAction ( ) ;
  const QString AN = Act -> objectName ( ) ; // Primary key.

  UdevDev Dev ( UdevContext , sect ( AN , 0 ) ) ;
  UdevDev WD  ( WDisk ( Dev ) ) ; // Whole disk.

  const bool MoM  = ! sect ( AN  , 1 ) . isEmpty ( ) ; // Mounted or mapped.
  const bool Cont = isLUKS ( Dev ) ;                   // It's container.

  ActPtr SA ;

  { const QString Txt = MoM
                          ? ( Cont ? tr ( "Lock"   ) : tr ( "Unmount" ) )
                          : ( Cont ? tr ( "Unlock" ) : tr ( "Mount"   ) ) ;
    SA = SMn . addAction ( Act -> icon ( ) , Txt ) ;
    SA -> setCheckable ( true ) ; SA -> setChecked ( Act -> isChecked ( ) ) ;
    SA -> setData ( ( uint ) reqNoAct ) ; SMn . setActiveAction ( SA ) ;
  }

  if ( ! Opt . toStr ( kEjectCmd ) . trimmed ( ) . isEmpty ( ) &&
         Ejectable ( WD ) ) {
    SA = SMn . addAction ( EIcon , tr ( "Eject" ) ) ;
    SA -> setData ( ( uint ) reqEject ) ; SMn . setActiveAction ( SA ) ;
  }//fi

  if ( ! Opt . toStr ( kRemoveCmd ) . trimmed ( ) . isEmpty ( ) &&
         HOTPLUG . exactMatch ( WD . Property ( DEV_BUS ) ) ) {
    SA = SMn . addAction ( RIcon , tr ( "Remove" ) ) ;
    SA -> setData ( ( uint ) reqRemove ) ; SMn . setActiveAction ( SA ) ;
  }//fi

  SA = SMn . exec ( event -> globalPos ( ) ) ;

  if ( SA && findChildren < ActPtr > ( ) . contains ( Act ) ) {
    // "findchildren" is insufficient, but better than nothing.
    Suppl  = ( ActReq ) ( SA -> data ( ) . toUInt ( ) ) ;
    setActiveAction ( Act ) ;
    keyPressEvent ( & EntrPrs ) ; keyPressEvent ( & EntrRls ) ;
  }//fi

}// Listener :: contextMenuEvent

//eof Listener.cpp
