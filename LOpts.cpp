
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2018.

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

#include "Listener.h"

static const QString // ".conf" file items.
  MountCmdItm   = "Mount_command"   , UnmntCmdItm   = "Unmount_command"  ,
  EjectCmdItm   = "Eject_command"   , RemoveCmdItm  = "Remove_command"   ,
  UnlockCmdItm  = "Unlock_command"  , LockCmdItm    = "Lock_command"     ,
  AddImgCmdItm  = "Add_Img_command" ,
  MountTOItm    = "Mount_timeout"   , UnmntTOItm    = "Unmount_timeout"  ,
  EjectTOItm    = "Eject_timeout"   , RemoveTOItm   = "Remove_timeout"   ,
  UnlockTOItm   = "Unlock_timeout"  , LockTOItm     = "Lock_timeout"     ,
  AddImgTOItm   = "Add_Img_timeout" ,
  HideDevsItm   = "Hidden_devices"  , ForceDevsItm  = "Forced_devices"   ,
  VerboseItm    = "Verbose"         ,
  MntNewItm     = "Mount_new"       , MntMediaItm   = "Mount_media"      ,
  AutoEjectItm  = "Auto_eject"      , MntStartItm   = "Mount_on_start"   ,
  MountShowItm  = "Mount_show"      , UnmntShowItm  = "Unmount_show"     ,
  EjectShowItm  = "Eject_show"      , RemoveShowItm = "Remove_show"      ,
  UnlockShowItm = "Unlock_show"     , LockShowItm   = "Lock_show"        ,
  AddImgShowItm = "Add_Img_show"    ,
  NewShowItm    = "Mount_new_show"  , MediaShowItm  = "Mount_media_show" ,
  AutoEjShowItm = "Auto_eject_show" , StartShowItm  = "Mount_start_show" ;

// Defaults.

static const int // arbitrary values
  DefTO =    5 , // 5 s for run the external program.
  MaxTO = 3600 ; // 1 h

static const QString
  MDfl = "udevil mount" , UDfl = "udevil umount" , EDfl = "eject" ,
  RDfl = "sh -c \"echo You can remove ${0}\"" ;

static const QString // Icons.
  MountP  = ":/icons/mount.png"  , UnmntP  = ":/icons/unmount.png" ,
  EjectP  = ":/icons/eject.png"  , RemoveP = ":/icons/remove.png"  ,
  UnlockP = ":/icons/unlock.png" , LockP   = ":/icons/lock.png"    ,
  UnrecP  = ":/icons/unrec.png"  , AddImgP = ":/icons/fsimg.png"   ,
  ExitP   = ":/icons/exit.png"   , ConfigP = ":/icons/config.png"  ,
  AboutP  = ":/icons/info.png"   , TMntP   = ":/icons/tmount.png"  ;

static CPtr // Tooltips.
  TtpStyle  = "<p style='white-space:pre'>" ,
  MountTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    This command applies to unmounted devices.<br/>"
    "    The device node name (/dev/...) will be<br/>"
    "appended to the end of given command line." ) ,
  UnmntTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    This command applies to mounted devices.<br/>"
    "    The mountpoint pathname (/media/...) will be<br/>"
    "appended to the end of given command line." ) ,
  EjectTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    This command is available from the additional<br/>"
    "(right-button clicked on device item) menu for<br/>"
    "ejectable media and also applies if<br/>"
    "the \"Autoeject media\" option is enabled.<br/>"
    "    The whole disk node name for the given device<br/>"
    "(b.e. /dev/sdd for /dev/sdd[0-9]*, /dev/sr0 for<br/>"
    "/dev/sr0) will be appended to the end of given<br/>"
    "command line." ) ,
  RemoveTtp = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    This command is available from the additional<br/>"
    "(right-button clicked on device item) menu for<br/>"
    "hotplugged devices.<br/>"
    "    The whole disk node name for the given device<br/>"
    "(b.e. /dev/sdd for /dev/sdd[0-9]*, /dev/sr0 for<br/>"
    "/dev/sr0) will be appended to the end of given<br/>"
    "command line." ) ,
  UnlockTtp = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    This command applies to unmapped LUKS<br/>"
    "containers.<br/>"
    "    The device node name (/dev/...) will be<br/>"
    "appended to the end of given command line." ) ,
  LockTtp   = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    This command applies to mapped LUKS<br/>"
    "containers.<br/>"
    "    The mapping name will be appended to<br/>"
    "the end of given command line." ) ,
  HideTtp   = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    Type here the extended regexp,<br/>"
    "b.e. '/dev/sd([ab].*|c1)'." ) ,
  ForceTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    Type here the extended regexp,<br/>"
    "b.e. '/dev/sr[01]'." ) ,
  AddImgTtp = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    This command is available from the context<br/>"
    "menu of the tray icon.<br/>"
    "    The selected file name will be appended<br/>"
    "to the end of given command line." ) ,
  TimeTtp   = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    Timeout in secs for given command,<br/>"
    "0 means no timeout." ) ,
  ShowTtp   = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    Specifies whether to display the output<br/>"
    "of the command when it succeeds." ) ;

static CPtr // Labels.
  MountLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "&Mount"     ) ,
  UnmntLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "&Unmount"   ) ,
  EjectLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "&Eject"     ) ,
  RemoveLbl    = QT_TRANSLATE_NOOP ( "LOpts" , "&Remove"    ) ,
  UnlockLbl    = QT_TRANSLATE_NOOP ( "LOpts" , "Unloc&k"    ) ,
  LockLbl      = QT_TRANSLATE_NOOP ( "LOpts" , "&Lock"      ) ,
  AddImgLbl    = QT_TRANSLATE_NOOP ( "LOpts" , "Add &Image" ) ,
  HideLbl      = QT_TRANSLATE_NOOP ( "LOpts" , "&Hidden devices"       ) ,
  ForceLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "Show &anyway"          ) ,
  VerboseLbl   = QT_TRANSLATE_NOOP ( "LOpts" , "&Verbose listing"      ) ,
  MntNewLbl    = QT_TRANSLATE_NOOP ( "LOpts" , "Mount &new devices"    ) ,
  MntMediaLbl  = QT_TRANSLATE_NOOP ( "LOpts" , "Mount inser&ted media" ) ,
  AutoEjectLbl = QT_TRANSLATE_NOOP ( "LOpts" , "Autoe&ject media"      ) ,
  MntStartLbl  = QT_TRANSLATE_NOOP ( "LOpts" , "Mount on &start"       ) ;

inline QFrame * HLine ( ) {
  QFrame * F = new QFrame ; F -> setFrameShape ( QFrame :: HLine ) ;
  return F ;
}// HLine

struct LOpts :: Item {
  QString    CFKey   ; // Config file key.
  QVariant   Val     ;
  CPtr       Label   ;
  CPtr       ToolTip ;
  QWidget  * Editor  ;
  loKey      Pix     ;
  loKey      TOLnk   ; // Command -> Timeout link.
  loKey      ShowLnk ; // -> Show flag link.
  int        Grp     ; // Group in the dialog window.
} ;

LOpts :: Item LOpts :: ITbl [ ] = {
  { "" , "" , NULL , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { MountCmdItm   , MDfl   , MountLbl     , MountTtp  , NULL , kMountPix  ,
    kMountTO      , kMountShow  , 1 } ,
  { UnmntCmdItm   , UDfl   , UnmntLbl     , UnmntTtp  , NULL , kUnmntPix  ,
    kUnmntTO      , kUnmntShow  , 1 } ,
  { EjectCmdItm   , EDfl   , EjectLbl     , EjectTtp  , NULL , kEjectPix  ,
    kEjectTO      , kEjectShow  , 1 } ,
  { RemoveCmdItm  , RDfl   , RemoveLbl    , RemoveTtp , NULL , kRemovePix ,
    kRemoveTO     , kRemoveShow , 1 } ,
  { UnlockCmdItm  , ""     , UnlockLbl    , UnlockTtp , NULL , kUnlockPix ,
    kUnlockTO     , kUnlockShow , 1 } ,
  { LockCmdItm    , ""     , LockLbl      , LockTtp   , NULL , kLockPix   ,
    kLockTO       , kLockShow   , 1 } ,
  { AddImgCmdItm  , MDfl   , AddImgLbl    , AddImgTtp , NULL , kAddImgPix ,
    kAddImgTO     , kAddImgShow , 1 } ,
  { MountTOItm    , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey , 0 } ,
  { UnmntTOItm    , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey , 0 } ,
  { EjectTOItm    , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey , 0 } ,
  { RemoveTOItm   , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey , 0 } ,
  { UnlockTOItm   , 0      , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey , 0 } ,
  { LockTOItm     , 0      , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey , 0 } ,
  { AddImgTOItm   , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey , 0 } ,
  { HideDevsItm   , ""     , HideLbl      , HideTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey , 3 } ,
  { ForceDevsItm  , ""     , ForceLbl     , ForceTtp  , NULL , kNoKey ,
    kNoKey        , kNoKey , 3 } ,
  { VerboseItm    , false  , VerboseLbl   , NULL      , NULL , kNoKey ,
    kNoKey        , kNoKey , 4 } ,
  { MntNewItm     , false  , MntNewLbl    , NULL      , NULL , kNoKey ,
    kNoKey        , kNewShow    , 2 } ,
  { MntMediaItm   , false  , MntMediaLbl  , NULL      , NULL , kNoKey ,
    kNoKey        , kMediaShow  , 2 } ,
  { AutoEjectItm  , true   , AutoEjectLbl , NULL      , NULL , kNoKey ,
    kNoKey        , kAutoEjShow , 2 } ,
  { MntStartItm   , false  , MntStartLbl  , NULL      , NULL , kNoKey ,
    kNoKey        , kStartShow  , 2 } ,
  { "" , MountP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , UnmntP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , EjectP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , RemoveP  , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , UnlockP  , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , LockP    , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , UnrecP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , AddImgP  , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , ExitP    , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , ConfigP  , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , AboutP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { "" , TMntP    , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey , 0 } ,
  { MountShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { UnmntShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { EjectShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { RemoveShowItm , true   , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { UnlockShowItm , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { LockShowItm   , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { AddImgShowItm , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { NewShowItm    , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { MediaShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { AutoEjShowItm , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
  { StartShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey , 0  } ,
} ;

const int LOpts :: ITblSize =
                     sizeof ( LOpts :: ITbl ) / sizeof ( LOpts :: Item ) ;

LOpts :: LOpts ( QWidget * parent ) : QDialog ( parent ) {

  static const Qt :: Alignment HC = Qt :: AlignHCenter  ;
  static const Qt :: Alignment VC = Qt :: AlignVCenter ;

  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    Item & C = ITbl [ I ] ; const QString K = C . CFKey ;
    if ( ! K . isEmpty ( ) ) {
      const QVariant :: Type T = C . Val . type ( ) ;
      if ( Conf . contains ( K ) ) {
        QVariant NV = Conf . value ( K ) ;
        if ( NV . convert ( T ) ) { C . Val = NV ; }//fi
      }//fi
      Conf . setValue ( K , C . Val ) ;
      if ( T == QVariant :: String ) {
        C . Editor = new QLineEdit ( C . Val . toString ( ) ) ;
      } else if ( T == QVariant :: Int ) {
        QSpinBox * S = new QSpinBox ; S -> setMaximum ( MaxTO ) ;
        C . Editor = S ; S -> setValue ( C . Val . toInt ( ) ) ;
      } else if ( T == QVariant :: Bool ) {
        QCheckBox * B = new QCheckBox ( tr ( C . Label ) ) ;
        C . Editor = B ; B -> setChecked ( C . Val . toBool ( ) ) ;
      }//fi
      if ( C . Editor && C . ToolTip ) {
        C . Editor -> setToolTip ( TtpStyle + tr ( C . ToolTip ) ) ;
      }//fi
    }//fi
  }//done

  QGridLayout * Lay = new QGridLayout ; QPixmap P ; int H , R = 0 ;

  { const QString L ( 24 , 'M' ) ; // 24 is arbitrary.
    H = QLineEdit ( L ) . sizeHint ( ) . height ( ) ;
    Lay -> setColumnMinimumWidth (
      2  , QLabel ( L ) . sizeHint ( ) . width  ( ) ) ;
  }

  Lay -> addWidget ( new QLabel ( tr ( "Actions:"       ) ) , R    , 1 ) ;
  Lay -> addWidget ( new QLabel ( tr ( "Command lines:" ) ) , R    , 2 ) ;
  Lay -> addWidget ( new QLabel ( tr ( "Timeouts:"      ) ) , R    , 3 ) ;
  Lay -> addWidget ( new QLabel ( tr ( "Show:"          ) ) , R ++ , 4 ) ;

  Lay -> addWidget ( HLine ( ) , R    , 0 , 1 , 2 ) ;
  Lay -> addWidget ( HLine ( ) , R    , 2 ) ;
  Lay -> addWidget ( HLine ( ) , R    , 3 ) ;
  Lay -> addWidget ( HLine ( ) , R ++ , 4 ) ;

  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    const Item & C = ITbl [ I ] ;
    if ( C . Grp == 1 ) {
      const QString Ttp = tr ( C . ToolTip ) ;
      QLabel * Lbl = new QLabel ( ) ; P . load ( toStr ( C . Pix ) ) ;
      Lbl -> setPixmap  ( P . scaled ( H , H ) ) ;
      Lbl -> setToolTip ( TtpStyle + Ttp ) ;
      Lay -> addWidget  ( Lbl , R , 0 ) ;
      Lbl =  new QLabel ( tr ( C . Label ) ) ;
      Lbl -> setBuddy   ( C . Editor ) ;
      Lbl -> setToolTip ( TtpStyle + Ttp ) ;
      Lay -> addWidget  ( Lbl , R , 1 ) ;
      Lay -> addWidget  ( C . Editor , R , 2 ) ;
      Lay -> addWidget  ( ITbl [ C . TOLnk   ] . Editor , R    , 3 ) ;
      Lay -> addWidget  ( ITbl [ C . ShowLnk ] . Editor , R ++ , 4 , HC ) ;
    }//fi
  }//done

  Lay -> addWidget ( HLine ( ) , R    , 0 , 1 , 4 ) ;
  Lay -> addWidget ( HLine ( ) , R ++ , 4 ) ;

  Lay -> addWidget (
           new QLabel ( tr ( "Handling:" ) ) , R , 1 , 4 , 1 , VC ) ;

  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    const Item & C = ITbl [ I ] ;
    if ( C . Grp == 2 ) {
      Lay -> addWidget ( C . Editor , R , 2 ) ;
      Lay -> addWidget ( ITbl [ C . ShowLnk ] . Editor , R ++ , 4 , HC ) ;
    }//fi
  }//done

  Lay -> addWidget ( HLine ( ) , R ++ , 0 , 1 , -1 ) ;

  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    const Item & C = ITbl [ I ] ;
    if ( C . Grp == 3 ) {
      QLabel * Lbl = new QLabel ( tr ( C . Label ) ) ;
      Lbl -> setBuddy   ( C . Editor ) ;
      Lbl -> setToolTip ( TtpStyle + tr ( C . ToolTip ) ) ;
      Lay -> addWidget  ( Lbl , R  , 1 ) ;
      Lay -> addWidget  ( C . Editor  , R ++ , 2 ) ;
    }//fi
  }//done

  Lay -> addWidget ( ITbl [ kVerbose ] . Editor , R ++ , 2 ) ;

  Lay -> addWidget ( HLine ( ) , R ++ , 0 , 1 , -1 ) ;

  QPushButton * pbOk   = new QPushButton ( tr ( "&Ok"     ) ) ;
  QPushButton * pbCanc = new QPushButton ( tr ( "&Cancel" ) ) ;
  connect ( pbOk   , SIGNAL ( clicked ( ) ) , SLOT ( accept ( ) ) ) ;
  connect ( pbCanc , SIGNAL ( clicked ( ) ) , SLOT ( reject ( ) ) ) ;
  Lay -> addWidget ( pbOk   , R , 0 , 1 , 2 , HC ) ;
  Lay -> addWidget ( pbCanc , R , 3 , 1 , 2 , HC ) ;

  setLayout ( Lay ) ; setWindowIcon ( QIcon ( TMntP ) ) ;
  setWindowTitle ( AppName ( ) + " - " + tr ( "Settings" ) ) ;

}// LOpts

int LOpts :: exec ( ) {
  int R = QDialog :: exec ( ) ; const bool A = R == QDialog :: Accepted ;
  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    Item & C = ITbl [ I ] ;
    if ( C . Editor ) {
      const QVariant :: Type T = C . Val . type ( ) ;
      if ( T == QVariant :: String ) {
        QLineEdit * L = qobject_cast < QLineEdit * > ( C . Editor ) ;
        if ( A ) { C . Val = L -> text ( ) ;
        } else { L -> setText ( C . Val . toString ( ) ) ;
        }//fi
      } else if ( T == QVariant :: Int ) {
        QSpinBox * S = qobject_cast < QSpinBox * > ( C . Editor ) ;
        if ( A ) { C . Val = S -> value ( ) ;
        } else { S -> setValue ( C . Val . toInt ( ) ) ;
        }//fi
      } else if ( T == QVariant :: Bool ) {
        QCheckBox * B = qobject_cast < QCheckBox * > ( C . Editor ) ;
        if ( A ) { C . Val = B -> isChecked ( ) ;
        } else { B -> setChecked ( C . Val . toBool ( ) ) ;
        }//fi
      }//fi
      if ( A ) { Conf . setValue ( C . CFKey , C . Val ) ; }//fi
    }//fi
  }//done
  if ( A ) { emit Changed ( ) ; }//fi
  return R ;
}// LOpts :: exec

LOpts :: ~LOpts ( ) { }// ~LOpts

QString LOpts :: toStr ( loKey K ) const {
  return K && K < ITblSize ? ITbl [ K ] . Val . toString ( ) .
                                                trimmed  ( ) : "" ;
}// LOpts :: toStr

int LOpts :: toInt ( loKey K ) const {
  return K && K < ITblSize ? ITbl [ K ] . Val . toInt    ( ) : -1 ;
}// LOpts :: toStr

bool LOpts :: toBool ( loKey K ) const {
  return K && K < ITblSize ? ITbl [ K ] . Val . toBool   ( ) : false ;
}// LOpts :: toStr

OptList LOpts :: GetAll ( ) const {
  OptList L ;
  foreach ( const QString K , Conf . allKeys ( ) ) {
    L << OptPair ( K , Conf . value ( K ) . toString ( ) ) ;
  }//done
  return L ;
}// LOpts :: GetAll

QString LOpts :: AppName ( ) const { return Conf . applicationName ( ) ;
}// LOpts :: AppName

//eof LOpts.cpp
