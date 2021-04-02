
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2021.

#include <QSocketNotifier>
#include <QMessageBox>
#include <QTextCodec>
#include <QFile>
#include <QFileDialog>

#include "Listener.h"
#include "PTName.h"

//   These constants are hardcoded in "udev"
// and "util-linux"(in libblkid) packages.

static CPtr // Property names.
  FS_USAGE = "ID_FS_USAGE"          , FS_TYPE  = "ID_FS_TYPE" ,
  FS_LABEL = "ID_FS_LABEL"          , DEV_BUS  = "ID_BUS"     ,
  CD_MEDIA = "ID_CDROM_MEDIA"       , CDROM    = "ID_CDROM"   ,
  TBL_TYPE = "ID_PART_TABLE_TYPE"   , ID_TYPE  = "ID_TYPE"    ,
  PRT_TYPE = "ID_PART_ENTRY_TYPE"   , DM_UUID  = "DM_UUID"    ,
  PRT_SCHM = "ID_PART_ENTRY_SCHEME" , DM_NAME  = "DM_NAME"    ,
  PRT_NAME = "ID_PART_ENTRY_NAME"   , DEV_TYPE = "DEVTYPE"    ,
  AUDIO_TR = "ID_CDROM_MEDIA_TRACK_COUNT_AUDIO" ,
  DATA_TR  = "ID_CDROM_MEDIA_TRACK_COUNT_DATA"  ;

static CPtr // Property values.
  TYPE_LUKS = "crypto_LUKS" , TYPE_DISK = "disk" ,
  USAGE_FS  = "filesystem"  , PART_PREF = "part" ,
  TYPE_PART = "partition"   ;
static const QRegExp HOTPLUG ( "usb|pcmcia|ieee1394|firewire|mmc|ccw" ) ;

static const QString // UEvent actions.
  ACT_add = "add" , ACT_remove = "remove" , ACT_change = "change" ;

static CPtr // These constants are defined by "sysfs".
  Subsys_Block = "block"    , SA_Rem    = "removable" ,
  SA_Events    = "events"   , SA_Size   = "size"      ,
  SD_Holders   = "/holders" , SD_Slaves = "/slaves"   ,
  SA_BackFile  = "loop/backing_file" ,
  Events_Eject = "eject_request"     ;

static const int NoTimeout = -1 ;
  // For interactive external program, defined by QProcess.

static QKeyEvent
  EntrPrs ( QEvent :: KeyPress   , Qt :: Key_Enter , Qt :: NoModifier ) ,
  EntrRls ( QEvent :: KeyRelease , Qt :: Key_Enter , Qt :: NoModifier ) ;

static const QString :: SplitBehavior SEP = QString :: SkipEmptyParts ;

static const QDir :: Filters Drs = QDir :: Dirs | QDir :: NoDotAndDotDot ;

inline QString sect ( const QString & S , int K , QChar Sep = ' ' ) {
  return S . section ( Sep , K , K ) ;
}// sect

Listener :: Listener ( QWidget * parent ) :
                         QMenu ( parent ) , UMonitor ( UdevContext ) {

  MIcon = QIcon ( Opt . toStr ( kMountPix  ) ) ;
  UIcon = QIcon ( Opt . toStr ( kUnmntPix  ) ) ;
  EIcon = QIcon ( Opt . toStr ( kEjectPix  ) ) ;
  RIcon = QIcon ( Opt . toStr ( kRemovePix ) ) ;
  DIcon = QIcon ( Opt . toStr ( kUnlockPix ) ) ;
  LIcon = QIcon ( Opt . toStr ( kLockPix   ) ) ;
  FIcon = QIcon ( Opt . toStr ( kUnrecFPix ) ) ;
  SIcon = QIcon ( Opt . toStr ( kUnrecSPix ) ) ;
  TIcon = QIcon ( Opt . toStr ( kTMntPix   ) ) ;

  setWindowIcon ( TIcon ) ; TPref = Opt . AppName ( ) + " - " ;

  Env   = QProcessEnvironment :: systemEnvironment ( ) ;
  Suppl = reqNoAct ;

  HDevs = QRegExp ( Opt . toStr ( kHideDevs  ) ) ;
  FDevs = QRegExp ( Opt . toStr ( kForceDevs ) ) ;
  Verb  = Opt . toBool ( kVerbose ) ;

  const bool TryMount = Opt . toBool ( kMntStart  ) ,
             Show     = Opt . toBool ( kStartShow ) ;

  UMonitor . AddMatch ( Subsys_Block , nullptr ) ;
  UMonitor . EnableReceiving ( ) ;

  connect ( & UMonitor , SIGNAL ( Changed      ( ) ) ,
            this       , SLOT   ( DeviceAction ( ) ) ) ;

  connect ( & MInfo    , SIGNAL ( Changed      ( ) ) ,
            this       , SLOT   ( MountAction  ( ) ) ) ;

  connect ( & Opt      , SIGNAL ( Changed      ( ) ) ,
            this       , SLOT   ( OptChanged   ( ) ) ) ;

  UdevEnum En ( UdevContext ) ;
  En . MatchSubsys ( Subsys_Block ) ; En . ScanDevs ( ) ;
  foreach ( const UdevPair P , En . GetList ( ) ) {
    SetupDevice ( UdevDev ( UdevContext , P . first ) , TryMount , Show ) ;
  }//done

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
    const UdevDev Dev ( UdevContext , CurrDev ) ;
    QString Node = Dev . DevNode ( ) ;
    const QString MP = Mounts :: DecodeIFS ( sect ( AN , 1 ) ) ;
      // Mountpoint/dm-name if any.
    const bool Cont = isLUKS ( Dev ) ,     // It's container.
               MoM  = ! MP . isEmpty ( ) ; // It's mounted or mapped.
    int RC = 0 ; // Return code for command.

    if ( MoM || ! Suppl ) {
      // Currently Suppl may be Eject, Remove or UnmountAll.

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

    const UdevDev WD ( WDisk ( Dev ) ) ; // whole disk.
    bool Show = Opt . toBool ( kEjectShow ) ;
    RefreshMInfo ( ) ;

    { const bool
        A = Opt . toBool ( kAutoEject ) ,
        D = Opt . toStr  ( kEjectCmd  ) . isEmpty ( ) ,
        L = ( MPoints ( Dev ) + Holders ( Dev ) ) . isEmpty ( ) ,
        S = Suppl == reqEject || Suppl == reqRemove ,
        E = Ejectable ( WD  ) ;
      if ( MoM && ( L || Suppl ) && A && ! D && E && ! S ) {
        Suppl = reqEject ; Show = Opt . toBool ( kAutoEjShow ) ;
      }//fi
    }

    if ( RC ) { SetActions ( Dev ) ; // workaround for setChecked ()

    } else if ( Suppl == reqUnmtAll ) {

      UnmntAll ( Dev , Opt . toBool ( kUnmntShow ) ) ;

    } else if ( Suppl ) { // Eject or Remove.

      QStringList Msg , NL ; Node = WD . DevNode ( ) ;

      foreach ( const QString P , Parts ( WD ) << WD . SysPath ( ) ) {
        const UdevDev D ( UdevContext , P ) ;
        const QString N = D . DevNode ( ) ; NL += N ;
        UnmntAll ( D , false ) ; RefreshMInfo ( ) ;
        foreach ( const QString M , MPoints ( D ) ) {
          Msg << N + tr ( " mounted on " ) + M ;
        }//done
        foreach ( const QString M , Holders ( D ) ) {
          Msg << N + tr ( " mapped on "  ) + M ;
        }//done
        UdevEnum En ( UdevContext ) ; En . MatchSubsys ( Subsys_Block ) ;
        En . MatchSysAttr ( SA_BackFile , N . toLatin1 ( ) ) ;
        En . ScanDevs ( ) ;
        foreach ( const UdevPair M , En . GetList ( ) ) {
          Msg << N + tr ( " mapped on " ) +
                       UdevDev ( UdevContext , M . first ) . DevNode ( ) ;
        }//done
      }//done

      const QStringList F ( "[1-9]*" ) ;
      foreach ( const QString P , QDir ( "/proc" ) . entryList ( F , Drs ) ) {
        const QDir D ( "/proc/" + P + "/fd" ) ;
        foreach ( const QFileInfo I , D . entryInfoList ( QDir :: Files ) ) {
          const QString N = I . symLinkTarget ( ) ;
          if ( NL . contains ( N ) ) { Msg << N + tr ( " used by PId " ) + P ;
          }//fi
        }//done
      }//done

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

  const UdevDev Dev ( UMonitor ) ;
  const QString DAct = Dev . Action ( ) ;

  if ( DAct == ACT_remove ) { RemoveDevice ( Dev ) ;

  } else if ( DAct == ACT_add ) {

    const bool M = Opt . toBool ( kMntNew  ) ,
               S = Opt . toBool ( kNewShow ) ;
    SetupDevice ( Dev , M , S ) ;

  } else if ( DAct == ACT_change ) {

    const bool M = Opt . toBool  ( kMntMedia  ) &&
                   Dev . SysAttr ( SA_Rem     ) . toUInt ( ) ,
               S = Opt . toBool  ( kMediaShow ) ;
      //   The test of "removable" attribute prevent attempt to mount of
      // cryptsetup's dm-* devices.

    SetupDevice ( Dev , M , S ) ;

  }//fi

}// Listener :: DeviceAction

void Listener :: MountAction ( ) {

  RefreshMInfo ( ) ;

  QStringList L ;
  foreach ( const ActPtr A , actions ( ) ) {
    L << sect ( A -> objectName ( ) , 0 ) ;
  }//done
  L . removeDuplicates ( ) ;
  foreach ( const QString P , L ) {
    const UdevDev Dev ( UdevContext , P ) ;
    if ( hasFS ( Dev ) ) { SetActions ( Dev ) ; }//fi
  }//done

}// Listener :: MountAction

void Listener :: OptChanged ( ) {

  HDevs = QRegExp ( Opt . toStr ( kHideDevs  ) ) ;
  FDevs = QRegExp ( Opt . toStr ( kForceDevs ) ) ;
  Verb  = Opt . toBool ( kVerbose ) ;

  QStringList L ;
  foreach ( const ActPtr A , actions ( ) ) {
    L << sect ( A -> objectName ( ) , 0 ) ;
  }//done
  L . removeDuplicates ( ) ;
  foreach ( const QString P , L ) {
    SetActions ( UdevDev ( UdevContext , P ) ) ;
  }//done

}// Listener :: OptChanged

bool Listener :: isTarget ( const UdevDev & Dev ) const {
  const QString Node = Dev . DevNode ( ) ;
  const bool Hide  = HDevs . exactMatch ( Node ) ,
             Force = FDevs . exactMatch ( Node ) ;
  return ( ( hasFS ( Dev ) || isLUKS ( Dev ) ) && ! Hide ) || Force ;
}// Listener :: isTarget

void Listener :: SetupDevice (
                   const UdevDev & Dev , bool TryMount , bool Show ) {

  CurrDev = Dev . SysPath ( ) ;

  if ( isTarget ( Dev ) && TryMount &&
       hasFS    ( Dev ) && MPoints ( Dev ) . isEmpty ( ) ) {

    ExecCmd ( Opt . toStr ( kMountCmd ) , Dev . DevNode ( ) ,
              Opt . toInt ( kMountTO  ) , Show ) ;

  }//fi // LUKS containers are never automatically unlocked.

  SetActions ( Dev ) ;

  if ( ! Dev . Property ( DM_NAME ) . isEmpty ( ) ) {
    foreach ( const QString P , Slaves ( Dev ) ) {
      SetActions ( UdevDev ( UdevContext , P ) ) ;
    }//done
  }//fi

}// Listener :: SetupDevice

void Listener :: RemoveDevice ( const UdevDev & Dev ) {

  const QString SP = Dev . SysPath ( ) ,
                DN = Mounts :: EncodeIFS ( Dev . Property ( DM_NAME ) ) ;

  foreach ( const ActPtr A , FindActs ( SP ) ) { delete A ; }//done

  UnmntAll ( Dev , false ) ; // Desperate attempt.

  if ( ! DN . isEmpty ( ) ) {
    const QRegExp RE ( "^[^ ]+ " + QRegExp :: escape ( DN ) + "$" ) ;
    foreach ( const ActPtr A , findChildren < ActPtr > ( RE ) ) {
      const UdevDev UD ( UdevContext , sect ( A -> objectName ( ) , 0 ) ) ;
      SetActions ( UD ) ;
    }//done
  }//fi

}// Listener :: RemoveDevice

int Listener :: UnmntAll ( const UdevDev & Dev , bool Show ) {
  //   This is workaround for "pumount" bug (the unmounting not the point
  // that the specified), and the precaution against infinite looping.
  int RC = 0 ; bool NE ; int PS = 0 ;
  do {
    RefreshMInfo ( ) ;
    const QStringList MP = MPoints ( Dev ) ;
    const int CS = MP . size ( ) ; NE = CS != PS ; PS = CS ;
    if ( NE ) {
      RC = ExecCmd ( Opt . toStr ( kUnmntCmd ) ,
                     Mounts :: DecodeIFS ( sect ( MP . first ( ) , 0 ) ) ,
                     Opt . toInt ( kUnmntTO ) , Show ) ;
    }//fi
  } while ( NE && ! RC && PS != 1 ) ;
  return RC ;
}// Listener :: UnmntAll

ActList Listener :: FindActs ( const QString & Key ) const {
  const QRegExp RE ( "^" + QRegExp :: escape ( Key ) + "( |$)" ) ;
  return findChildren < ActPtr > ( RE ) ;
}// Listener :: FindActs

QString Listener :: ToHum ( qulonglong KiB ) {
  const int Ki = 1024 ; QString S = "" ;
  uint R = KiB % Ki ; qulonglong Q = KiB / Ki ;
  if ( R ) { S = QString :: number ( R ) + "_Ki_" + S ; }//fi
  R = Q % Ki ; Q /= Ki ;
  if ( R ) { S = QString :: number ( R ) + "_Mi_" + S ; }//fi
  R = Q % Ki ; Q /= Ki ;
  if ( R ) { S = QString :: number ( R ) + "_Gi_" + S ; }//fi
  if ( Q ) { S = QString :: number ( Q ) + "_Ti_" + S ; }//fi
  S . chop ( 1 ) ;
  return S + "B" ;
}// Listener :: ToHum

void Listener :: SetActions ( const UdevDev & Dev ) {

  const bool Cont = isLUKS ( Dev ) , FSys = hasFS ( Dev ) ;
  QString Lbl = Dev . DevNode ( ) . section ( '/' , 2 ) + " " ;

  { QString DT = Dev . DevType ( ) , CT = "" ;
    if ( DT == TYPE_DISK ) {
      CT = Dev . Property ( ID_TYPE ) ;
      if ( CT . isEmpty ( ) ) {
        CT = sect ( Dev . Property ( DM_UUID ) , 0 , '-' ) . toLower ( ) ;
      }//fi
      if ( CT . isEmpty ( ) ) {
        CT = sect ( Dev . SysAttr ( SA_BackFile ) , -1 , '/' ) ;
      } else if ( CT . startsWith ( PART_PREF ) ) { DT = PART_PREF ;
      }//fi
    } else if ( DT == TYPE_PART ) { DT = PART_PREF ;
    }//fi
    if ( ! ( Cont || FSys ) || Verb ) {
      Lbl += "(" + ( CT . isEmpty ( ) ? DT : CT ) ;
      if ( DT == TYPE_DISK ) {
        CT = Dev . Property ( TBL_TYPE ) . toUpper ( ) ;
        if ( ! CT . isEmpty ( ) ) { Lbl += "," + CT + " pt" ; }//fi
        if ( ! Dev . Property ( AUDIO_TR ) . isEmpty ( ) ) { Lbl += ",audio" ;
        }//fi
        if ( ! Dev . Property ( DATA_TR  ) . isEmpty ( ) ) { Lbl += ",data"  ;
        }//fi
      } else if ( DT == PART_PREF ) {
        CT = Dev . Property ( PRT_SCHM ) ; // TBL_TYPE may be empty.
        if ( ! CT . isEmpty ( ) ) { Lbl += "," + CT ; }//fi
        CT = PTName ( CT , Dev . Property ( PRT_TYPE ) ) ;
        if ( ! CT . isEmpty ( ) ) { Lbl += "," + CT ; }//fi
        CT = Dev . Property ( PRT_NAME ) ; // Mac or GPT.
        if ( ! CT . isEmpty ( ) ) { Lbl += ",[" + CT + "]" ; }//fi
      }//fi
      Lbl += ")," ;
    }//fi
  }

  QStringList MPts = FSys ? MPoints ( Dev ) : DM_Maps ( Dev ) ;
  const  bool MoM  = ! MPts . isEmpty ( ) ; // It's mounted or mapped.

  if ( ! FSys || ! MoM || Verb ) {
    qulonglong Cap = Dev . SysAttr ( SA_Size ) . toULongLong ( ) / 2 ;
      // Capacity in KiB, sysfs uses 512-bytes units.
    if ( ! Dev . Property ( CDROM    ) . isEmpty ( ) &&
           Dev . Property ( CD_MEDIA ) . isEmpty ( )    ) { Cap = 0 ;
    }//fi // This is a workaround for "no media" on optical disks.
    Lbl += Cap ? ToHum ( Cap ) : tr ( "(no media)" ) ;
  }//fi

  { const QString FST = Dev . Property ( FS_TYPE ) ;
    if ( ! FST . isEmpty ( ) ) { Lbl += " " + FST ; }//fi
  } // fs type, luks, swap, etc...

  if ( FSys ) {
    QString L = Dev . Property ( FS_LABEL ) ;
#if ( QT_VERSION < QT_VERSION_CHECK ( 5 , 0 , 0 ) )
    L = QString :: fromLocal8Bit ( L . toLatin1 ( ) ) ;
#endif
    Lbl += L . isEmpty ( ) ? tr ( ",(no label)" ) : ",[" + L + "]" ;
    if ( MoM ) {
      QString F = Mounts :: DecodeIFS ( sect ( MPts . first ( ) , 0 ) ) ;
      Lbl += "," + ToHum ( Mounts :: GetStatFS ( F ) . TotalSize / 1024 ) ;
    }//fi
  }//fi

  QIcon * Ico ;
  if ( MoM ) {
    Lbl += tr ( " on " ) ;
    Ico = Cont ? & LIcon : ( FSys ? & UIcon : & SIcon ) ;
  } else {
    MPts << "" ; // see below.
    Ico = Cont ? & DIcon : ( FSys ? & MIcon : & FIcon ) ;
  }//fi

  const QString SPth = Dev . SysPath ( ) ;
  ActList AL = FindActs ( SPth ) ; const int Sz = MPts . size ( ) ;

  while ( AL . size ( ) > Sz ) { delete AL . takeLast ( ) ; }//done
    // Remove redundant items.

  while ( AL . size ( ) < Sz ) { AL += addAction ( "" ) ; }//done
    // Add laking items.

  ActPtr Aft = nullptr ;
  if ( ! Dev . Property ( DM_NAME ) . isEmpty ( ) && Sz == 1 ) {
    foreach ( const QString S , Slaves ( Dev ) ) {
      foreach ( const ActPtr N , FindActs ( S ) ) { Aft = N ; }//done
    }//done
  } else { Aft = AL . first ( ) ;
  }//fi

  const bool Targ = isTarget ( Dev ) ;

  foreach ( const QString M , MPts ) {
    QString P = sect ( M , 0 ) ; ActPtr B = nullptr ;
    const ActPtr A = AL . takeFirst ( ) ; A -> setIcon ( * Ico ) ;
    A -> setObjectName ( ( SPth + " " + P ) . trimmed ( ) ) ;
    A -> setCheckable ( true ) ; A -> setChecked ( MoM ) ;
    P += ! FSys && MoM ? " (" + sect ( M , 1 ) + ")" : "" ;
    A -> setText ( Lbl + P ) ; A -> setVisible ( Targ ) ;
    const ActList L = actions ( ) ; int I = L . indexOf ( Aft ) ;
    if ( 0 <= I && I < L . size ( ) - 2 ) { B = L . at ( I + 1 ) ; }//fi
    if ( B && B != A ) { removeAction ( A ) ; insertAction ( B , A ) ; }//fi
    Aft = A ;
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

    int STO = Opt . toInt ( kStartTO ) ;
    STO = STO > 0 ? STO * 1000 : NoTimeout ;
    const int RTO = Timeout > 0 ? Timeout * 1000 : NoTimeout ;
    const QString C = Cmd + " \"" +
                        QString ( Arg ) . replace ( '"' , "\"\"\"" ) + "\"" ;

    Proc . start ( C , QIODevice :: ReadOnly ) ;

    if ( ! Proc . waitForStarted ( STO ) ) {

      Proc . kill ( ) ;
      QMessageBox :: critical ( this , TPref + tr ( "Error" ) ,
                                tr ( "Can't execute '" ) + C + "'" ) ;

    } else if ( ! Proc . waitForFinished ( RTO ) ) {

      Proc . kill ( ) ;
      QMessageBox :: critical ( this , TPref + tr ( "Error" ) ,
                                "'" + C + tr ( "' crashed." ) ) ;
    } else if ( ( RC = Proc . exitCode ( ) ) ) {

      QStringList Msg = QTextCodec :: codecForLocale ( ) ->
                          toUnicode ( Proc . readAllStandardError ( ) ) .
                            split ( '\n' , SEP ) .
                              filter ( QRegExp ( "\\S" ) ) ;
      Msg . removeDuplicates ( ) ;
      if ( Msg . isEmpty ( ) ) {
        Msg << "'" + C + tr ( "' returns " ) + QString :: number ( RC ) ;
      }//fi
      QMessageBox :: warning ( this , TPref + tr ( "Warning" ) ,
                                        Msg . join ( "\n"  ) ) ;

    } else if ( Show ) {

      QStringList Msg = QTextCodec :: codecForLocale ( ) ->
                          toUnicode ( Proc . readAllStandardOutput ( ) ) .
                            split ( '\n' , SEP ) .
                              filter ( QRegExp ( "\\S" ) ) ;
      Msg . removeDuplicates ( ) ;
      if ( Msg . isEmpty ( ) ) {
        Msg << "'" + C + tr ( "' - successful completion." ) ;
      }//fi
      QMessageBox :: information ( this , nullptr , Msg . join ( "\n" ) ) ;

    }//fi

  }//fi

  return RC ;

}// Listener :: ExecCmd

void Listener :: AddImage ( ) {
  const QString Cmd = Opt . toStr ( kAddImgCmd ) ;
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

void Listener :: RefreshMInfo ( ) { MInfo . RefreshMountInfo ( ) ;
}// Listener :: RefreshMInfo

QStringList Listener :: MPoints ( const UdevDev & Dev ) const {
  return MInfo . MPoints ( Dev . DevNum ( ) ) ;
}// Listener :: MPoints

QStringList Listener :: DM_Maps ( const UdevDev & Dev ) const {
  QStringList L ;
  foreach ( const QString H , Holders ( Dev ) ) {
    const QString M = sect ( H , 1 ) ;
    if ( ! M . isEmpty ( ) ) { L << M + " " + sect ( H , 0 ) ; }//fi
  }//done
  return L ;
}// Listener :: DM_Maps

QStringList Listener :: Holders ( const UdevDev & Dev ) const {
  QStringList L ; const QDir SD ( Dev . SysPath ( ) + SD_Holders ) ;
  foreach ( const QFileInfo I , SD . entryInfoList ( Drs ) ) {
    const UdevDev D ( UdevContext , I . symLinkTarget ( ) ) ;
    L << Mounts :: EncodeIFS ( D . DevNode  ( ) ) + " " +
         Mounts :: EncodeIFS ( D . Property ( DM_NAME ) ) ;
  }//done
  return L ;
}// Listener :: Holders

QStringList Listener :: Slaves ( const UdevDev & Dev ) const {
  QStringList L ; const QDir SD ( Dev . SysPath ( ) + SD_Slaves ) ;
  foreach ( const QFileInfo I , SD . entryInfoList ( Drs ) ) {
    L << I . symLinkTarget ( ) ;
  }//done
  return L ;
}// Listener :: Slaves

QStringList Listener :: Parts ( const UdevDev & Dev ) const {
  QStringList L ; UdevEnum En ( UdevContext ) ; En . MatchParent ( Dev ) ;
  En . MatchProperty ( DEV_TYPE , TYPE_PART ) ; En . ScanDevs ( ) ;
  foreach ( const UdevPair P , En . GetList ( ) ) { L << P . first ; }//done
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
  return Dev . Property ( FS_TYPE  ) == TYPE_LUKS ;
}// Listener :: isLUKS

bool Listener :: hasFS  ( const UdevDev & Dev ) {
  return Dev . Property ( FS_USAGE ) == USAGE_FS  ;
}// Listener :: hasFS

bool Listener :: isPart ( const UdevDev & Dev ) {
  return Dev . DevType ( ) == TYPE_PART ;
}// Listener :: isLUKS

void Listener :: contextMenuEvent ( QContextMenuEvent * event ) {

  QMenu SMn ; ActPtr SA ; // Supplementary actions menu.
  const ActPtr Act = activeAction ( ) ;
  const QString AN = Act -> objectName ( ) ; // Primary key.
  const UdevDev Dev ( UdevContext , sect ( AN , 0 ) ) ;
  const bool MoM  = ! sect ( AN  , 1 ) . isEmpty ( ) ; // Mounted or mapped.
  const UdevDev WD  ( WDisk ( Dev ) ) ; // Whole disk.

  { const QString Txt =
      isLUKS ( Dev ) ? ( MoM ? tr ( "Lock"    ) : tr ( "Unlock" ) )
                     : ( MoM ? tr ( "Unmount" ) : tr ( "Mount"  ) ) ;
    SA = SMn . addAction ( Act -> icon ( ) , Txt ) ;
    SA -> setCheckable ( true ) ; SA -> setChecked ( Act -> isChecked ( ) ) ;
    SA -> setData ( reqNoAct ) ; SMn . setActiveAction ( SA ) ;
  }

  if ( MPoints ( Dev ) . size ( ) > 1 ) {
    SA = SMn . addAction ( Act -> icon ( ) , tr ( "Unmount all MP's" ) ) ;
    SA -> setCheckable ( true ) ; SA -> setChecked ( true ) ;
    SA -> setData ( reqUnmtAll ) ; SMn . setActiveAction ( SA ) ;
  }//fi

  if ( ! Opt . toStr ( kEjectCmd ) . isEmpty ( ) &&
         Ejectable ( WD ) ) {
    SA = SMn . addAction ( EIcon , tr ( "Eject" ) ) ;
    SA -> setData ( reqEject ) ; SMn . setActiveAction ( SA ) ;
  }//fi

  if ( ! Opt . toStr ( kRemoveCmd ) . isEmpty ( ) &&
         HOTPLUG . exactMatch ( WD . Property ( DEV_BUS ) ) ) {
    SA = SMn . addAction ( RIcon , tr ( "Remove" ) ) ;
    SA -> setData ( reqRemove ) ; SMn . setActiveAction ( SA ) ;
  }//fi

  SA = SMn . exec ( event -> globalPos ( ) ) ;

  if ( SA && actions ( ) . contains ( Act ) && Act -> isVisible ( ) ) {
    //   Don't reorder.  In case of device already removed or manually
    // ejected, this check is insufficient, but is better than nothing.
    Suppl  = static_cast < ActReq > ( SA -> data ( ) . toUInt ( ) ) ;
    setActiveAction ( Act ) ;
    keyPressEvent ( & EntrPrs ) ; keyPressEvent ( & EntrRls ) ;
  }//fi

}// Listener :: contextMenuEvent

//eof Listener.cpp
