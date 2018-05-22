
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
    "    Type here a extended regexp,<br/>"
    "b.e. '/dev/sd([ab].*|c1)' (restart required)." ) ,
  ForceTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "    Type here a extended regexp,<br/>"
    "b.e. '/dev/sr[01]' (restart required)." ) ,
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
} ;

LOpts :: Item LOpts :: ITbl [ ] = {
  { "" , "" , NULL , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { MountCmdItm   , MDfl   , MountLbl     , MountTtp  , NULL , kMountPix  ,
    kMountTO      , kMountShow  } ,
  { UnmntCmdItm   , UDfl   , UnmntLbl     , UnmntTtp  , NULL , kUnmntPix  ,
    kUnmntTO      , kUnmntShow  } ,
  { EjectCmdItm   , EDfl   , EjectLbl     , EjectTtp  , NULL , kEjectPix  ,
    kEjectTO      , kEjectShow  } ,
  { RemoveCmdItm  , RDfl   , RemoveLbl    , RemoveTtp , NULL , kRemovePix ,
    kRemoveTO     , kRemoveShow } ,
  { UnlockCmdItm  , ""     , UnlockLbl    , UnlockTtp , NULL , kUnlockPix ,
    kUnlockTO     , kUnlockShow } ,
  { LockCmdItm    , ""     , LockLbl      , LockTtp   , NULL , kLockPix   ,
    kLockTO       , kLockShow   } ,
  { AddImgCmdItm  , MDfl   , AddImgLbl    , AddImgTtp , NULL , kAddImgPix ,
    kAddImgTO     , kAddImgShow } ,
  { MountTOItm    , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey } ,
  { UnmntTOItm    , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey } ,
  { EjectTOItm    , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey } ,
  { RemoveTOItm   , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey } ,
  { UnlockTOItm   , 0      , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey } ,
  { LockTOItm     , 0      , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey } ,
  { AddImgTOItm   , DefTO  , NULL         , TimeTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey } ,
  { HideDevsItm   , ""     , HideLbl      , HideTtp   , NULL , kNoKey ,
    kNoKey        , kNoKey } ,
  { ForceDevsItm  , ""     , ForceLbl     , ForceTtp  , NULL , kNoKey ,
    kNoKey        , kNoKey } ,
  { MntNewItm     , false  , MntNewLbl    , NULL      , NULL , kNoKey ,
    kNoKey        , kNewShow    } ,
  { MntMediaItm   , false  , MntMediaLbl  , NULL      , NULL , kNoKey ,
    kNoKey        , kMediaShow  } ,
  { AutoEjectItm  , true   , AutoEjectLbl , NULL      , NULL , kNoKey ,
    kNoKey        , kAutoEjShow } ,
  { MntStartItm   , false  , MntStartLbl  , NULL      , NULL , kNoKey ,
    kNoKey        , kStartShow  } ,
  { "" , MountP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , UnmntP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , EjectP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , RemoveP  , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , UnlockP  , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , LockP    , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , UnrecP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , AddImgP  , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , ExitP    , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , ConfigP  , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , AboutP   , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { "" , TMntP    , NULL   , NULL , NULL , kNoKey , kNoKey , kNoKey } ,
  { MountShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { UnmntShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { EjectShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { RemoveShowItm , true   , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { UnlockShowItm , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { LockShowItm   , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { AddImgShowItm , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { NewShowItm    , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { MediaShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { AutoEjShowItm , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
  { StartShowItm  , false  , NULL , ShowTtp , NULL , kNoKey , kNoKey ,
    kNoKey } ,
} ;

const int LOpts :: ITblSize =
                     sizeof ( LOpts :: ITbl ) / sizeof ( LOpts :: Item ) ;

LOpts :: LOpts ( QWidget * parent ) : QDialog ( parent ) {

  static const Qt :: Alignment HC = Qt :: AlignHCenter  ;
  static const Qt :: Alignment VC = Qt :: AlignVCenter ;

  Item * C ;

  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    C = & ITbl [ I ] ; const QString K = C -> CFKey ;
    if ( ! K . isEmpty ( ) ) {
      QVariant :: Type T = C -> Val . type ( ) ;
      if ( Conf . contains ( K ) ) {
        C -> Val = Conf . value ( K ) ; C -> Val . convert ( T ) ;
      }//fi
      if ( T == QVariant :: String ) {
        C -> Editor = new QLineEdit ( C -> Val . toString ( ) ) ;
      } else if ( T == QVariant :: Int ) {
        QSpinBox * S = new QSpinBox ; S -> setMaximum ( MaxTO ) ;
        C -> Editor = S ; S -> setValue ( C -> Val . toInt ( ) ) ;
      } else if ( T == QVariant :: Bool ) {
        QCheckBox * B = new QCheckBox ( tr ( C -> Label ) ) ;
        C -> Editor = B ; B -> setChecked ( C -> Val . toBool ( ) ) ;
      }//fi
      if ( C -> Editor && C -> ToolTip ) {
        C -> Editor -> setToolTip ( TtpStyle + tr ( C -> ToolTip ) ) ;
      }//fi
    }//fi
  }//done

  QGridLayout * Lay = new QGridLayout ;
  QLabel * Lbl ; QString Ttp ; QPixmap P ; int H , R = 0 ;

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
    C = & ITbl [ I ] ;
    if ( C -> TOLnk ) {
      Ttp = tr ( C -> ToolTip ) ;
      P . load ( toStr  ( C -> Pix ) ) ;
      Lbl =  new QLabel ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
      Lbl -> setToolTip ( TtpStyle  + Ttp ) ;
      Lay -> addWidget  ( Lbl , R , 0 ) ;
      Lbl =  new QLabel ( tr ( C -> Label ) ) ;
      Lbl -> setBuddy   ( C -> Editor ) ;
      Lbl -> setToolTip ( TtpStyle  + Ttp ) ;
      Lay -> addWidget  ( Lbl , R , 1 ) ;
      Lay -> addWidget  ( C -> Editor , R , 2 ) ;
      Lay -> addWidget  ( ITbl [ C -> TOLnk   ] . Editor , R    , 3 ) ;
      Lay -> addWidget  ( ITbl [ C -> ShowLnk ] . Editor , R ++ , 4 , HC ) ;
    }//fi
  }//done

  Lay -> addWidget ( HLine ( ) , R    , 0 , 1 , 4 ) ;
  Lay -> addWidget ( HLine ( ) , R ++ , 4 ) ;

  Lay -> addWidget (
           new QLabel ( tr ( "Handling:" ) ) , R , 1 , 4 , 1 , VC ) ;

  C = & ITbl [ kMntNew    ] ;
  Lay -> addWidget ( C -> Editor , R , 2 ) ;
  Lay -> addWidget ( ITbl [ C -> ShowLnk ] . Editor , R ++ , 4 , HC ) ;
  C = & ITbl [ kMntMedia  ] ;
  Lay -> addWidget ( C -> Editor , R , 2 ) ;
  Lay -> addWidget ( ITbl [ C -> ShowLnk ] . Editor , R ++ , 4 , HC ) ;
  C = & ITbl [ kAutoEject ] ;
  Lay -> addWidget ( C -> Editor , R , 2 ) ;
  Lay -> addWidget ( ITbl [ C -> ShowLnk ] . Editor , R ++ , 4 , HC ) ;
  C = & ITbl [ kMntStart ] ;
  Lay -> addWidget ( C -> Editor , R , 2 ) ;
  Lay -> addWidget ( ITbl [ C -> ShowLnk ] . Editor , R ++ , 4 , HC ) ;

  Lay -> addWidget ( HLine ( ) , R ++ , 0 , 1 , -1 ) ;

  C = & ITbl [ kHideDevs ] ;
  Lbl =  new QLabel ( tr ( C -> Label ) ) ;
  Lbl -> setBuddy   ( C -> Editor ) ;
  Lbl -> setToolTip ( TtpStyle + tr ( C -> ToolTip ) ) ;
  Lay -> addWidget  ( Lbl , R  , 1 ) ;
  Lay -> addWidget  ( C -> Editor  , R ++ , 2 ) ;
  C = & ITbl [ kForceDevs ] ;
  Lbl =  new QLabel ( tr ( C -> Label ) ) ;
  Lbl -> setBuddy   ( C -> Editor ) ;
  Lbl -> setToolTip ( TtpStyle + tr ( C -> ToolTip ) ) ;
  Lay -> addWidget  ( Lbl , R  , 1 ) ;
  Lay -> addWidget  ( C -> Editor  , R ++ , 2 ) ;

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
    Item * C = & ITbl [ I ] ;
    if ( C -> Editor ) {
      QVariant :: Type T = C -> Val . type ( ) ;
      if ( T == QVariant :: String ) {
        QLineEdit * L = qobject_cast < QLineEdit * > ( C -> Editor ) ;
        if ( A ) { C -> Val = L -> text ( ) ;
        } else { L -> setText ( C -> Val . toString ( ) ) ;
        }//fi
      } else if ( T == QVariant :: Int ) {
        QSpinBox * S = qobject_cast < QSpinBox * > ( C -> Editor ) ;
        if ( A ) { C -> Val = S -> value ( ) ;
        } else { S -> setValue ( C -> Val . toInt ( ) ) ;
        }//fi
      } else if ( T == QVariant :: Bool ) {
        QCheckBox * B = qobject_cast < QCheckBox * > ( C -> Editor ) ;
        if ( A ) { C -> Val = B -> isChecked ( ) ;
        } else { B -> setChecked ( C -> Val . toBool ( ) ) ;
        }//fi
      }//fi
    }//fi
  }//done
  return R ;
}// LOpts :: exec

LOpts :: ~LOpts ( ) {
  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    Item * C = & ITbl [ I ] ; const QString K = C -> CFKey ;
    if ( ! K . isEmpty ( ) ) { Conf . setValue ( K , C -> Val ) ; }//fi
  }//done
}// ~LOpts

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
  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    Item * C = & ITbl [ I ] ; const QString K = C -> CFKey ;
    if ( ! K . isEmpty ( ) ) { L << OptPair ( K , C -> Val . toString ( ) ) ;
    }//fi
  }//done
  return L ;
}// LOpts :: GetAll

QString LOpts :: AppName ( ) const { return Conf . applicationName ( ) ;
}// LOpts :: AppName

//eof LOpts.cpp
