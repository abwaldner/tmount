
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QApplication>

#include "Listener.h"

static const QString // ".conf" file items.
  MountCmdItm  = "Mount_command"   , MountTOItm  = "Mount_timeout"   ,
  UnmntCmdItm  = "Unmount_command" , UnmntTOItm  = "Unmount_timeout" ,
  EjectCmdItm  = "Eject_command"   , EjectTOItm  = "Eject_timeout"   ,
  UnlckCmdItm  = "Unlock_command"  , UnlckTOItm  = "Unlock_timeout"  ,
  LockCmdItm   = "Lock_command"    , LockTOItm   = "Lock_timeout"    ,
  AddImCmdItm  = "Add_Img_command" , AddImTOItm  = "Add_Img_timeout" ,
  HideDevsItm  = "Hidden_devices"  , MntNewItm   = "Mount_new"       ,
  MntMediaItm  = "Mount_media"     , MntStartItm = "Mount_on_start"  ,
  AutoEjectItm = "Auto_eject"    ;

// Defaults.

static const int // arbitrary values
  DefTO =    5 , // 5 s for run the external program.
  MaxTO = 3600 ; // 1 h

static const QString
  MDfl = "udevil mount" , UDfl = "udevil umount" , EDfl = "eject" ;

static const QString // Icons.
  MountP = ":/icons/mount.png" , UnmntP = ":/icons/unmount.png" ,
  EjectP = ":/icons/eject.png" , UnlckP = ":/icons/unlock.png"  ,
  LockP  = ":/icons/lock.png"  , AddImP = ":/icons/fsimg.png"   ,
  ExitP  = ":/icons/exit.png"  , ConfP  = ":/icons/config.png"  ,
  AboutP = ":/icons/info.png"  , TMntP  = ":/icons/tmount.png"  ;

static CPtr // Tooltips.
  MountTtp = QT_TRANSLATE_NOOP ( "LOpts" ,
    "The device node name (/dev/...) will be<br/>"
    "appended to the end of given command line." ) ,
  UnmntTtp = QT_TRANSLATE_NOOP ( "LOpts" ,
    "The mountpoint pathname (/media/...) will be<br/>"
    "appended to the end of given command line." ) ,
  LockTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "The mapping name will be appended<br/>"
    "to the end of given command line." ) ,
  HideTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "Type a space-separated list of regexps, b.e.<br/>"
    "'/dev/sd[ab].* /dev/sdc1' (restart required)." ) ,
  AddImTtp = QT_TRANSLATE_NOOP ( "LOpts" ,
    "The selected file name will be appended<br/>"
    "to the end of given command line." ) ,
  TimeTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "Timeout in secs for given command,<br/> 0 means no timeout." ) ;

static CPtr // Labels.
  MountLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "&Mount"     ) ,
  UnmntLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "&Unmount"   ) ,
  EjectLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "&Eject"     ) ,
  UnlckLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "Unloc&k"    ) ,
  LockLbl      = QT_TRANSLATE_NOOP ( "LOpts" , "&Lock"      ) ,
  AddImLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "Add &Image" ) ,
  HideLbl      = QT_TRANSLATE_NOOP ( "LOpts" , "&Hidden devices"       ) ,
  MntNewLbl    = QT_TRANSLATE_NOOP ( "LOpts" , "Mount &new devices"    ) ,
  MntMediaLbl  = QT_TRANSLATE_NOOP ( "LOpts" , "Mount inser&ted media" ) ,
  MntStartLbl  = QT_TRANSLATE_NOOP ( "LOpts" , "Mount on &start"       ) ,
  AutoEjectLbl = QT_TRANSLATE_NOOP ( "LOpts" , "Autoe&ject media"      ) ;

static QFrame * HLine ( ) {
  QFrame * F = new QFrame ; F -> setFrameShape ( QFrame :: HLine ) ;
  return F ;
}// NFrame

struct LOpts :: Item {
  QString    CFKey   ;
  QVariant   Val     ;
  CPtr       Label   ;
  CPtr       ToolTip ;
  QWidget  * Editor  ;
  loKey      Pix     ;
  loKey      Lnk     ;
} ;

LOpts :: Item LOpts :: ITbl [ ] = {
  { "" , "" , NULL , NULL , NULL , kNoKey , kNoKey } ,
  { MountCmdItm  , MDfl  , MountLbl     , MountTtp , NULL , kMountPix ,
    kMountTO     } ,
  { UnmntCmdItm  , UDfl  , UnmntLbl     , UnmntTtp , NULL , kUnmntPix ,
    kUnmntTO     } ,
  { EjectCmdItm  , EDfl  , EjectLbl     , MountTtp , NULL , kEjectPix ,
    kEjectTO     } ,
  { UnlckCmdItm  , ""    , UnlckLbl     , MountTtp , NULL , kUnlckPix ,
    kUnlckTO     } ,
  { LockCmdItm   , ""    , LockLbl      , LockTtp  , NULL , kLockPix  ,
    kLockTO      } ,
  { AddImCmdItm  , ""    , AddImLbl     , AddImTtp , NULL , kAddImPix ,
    kAddImTO     } ,
  { MountTOItm   , DefTO , NULL         , TimeTtp  , NULL , kNoKey ,
    kNoKey } ,
  { UnmntTOItm   , DefTO , NULL         , TimeTtp  , NULL , kNoKey ,
    kNoKey } ,
  { EjectTOItm   , DefTO , NULL         , TimeTtp  , NULL , kNoKey ,
    kNoKey } ,
  { UnlckTOItm   , 0     , NULL         , TimeTtp  , NULL , kNoKey ,
    kNoKey } ,
  { LockTOItm    , 0     , NULL         , TimeTtp  , NULL , kNoKey ,
    kNoKey } ,
  { AddImTOItm   , DefTO , NULL         , TimeTtp  , NULL , kNoKey ,
    kNoKey } ,
  { HideDevsItm  , ""    , HideLbl      , HideTtp  , NULL , kNoKey ,
    kNoKey } ,
  { MntNewItm    , false , MntNewLbl    , NULL     , NULL , kNoKey ,
    kNoKey } ,
  { MntMediaItm  , false , MntMediaLbl  , NULL     , NULL , kNoKey ,
    kNoKey } ,
  { MntStartItm  , false , MntStartLbl  , NULL     , NULL , kNoKey ,
    kNoKey } ,
  { AutoEjectItm , true  , AutoEjectLbl , NULL     , NULL , kNoKey ,
    kNoKey } ,
  { "" , MountP , NULL  , NULL , NULL , kNoKey , kNoKey } ,
  { "" , UnmntP , NULL  , NULL , NULL , kNoKey , kNoKey } ,
  { "" , EjectP , NULL  , NULL , NULL , kNoKey , kNoKey } ,
  { "" , UnlckP , NULL  , NULL , NULL , kNoKey , kNoKey } ,
  { "" , LockP  , NULL  , NULL , NULL , kNoKey , kNoKey } ,
  { "" , AddImP , NULL  , NULL , NULL , kNoKey , kNoKey } ,
  { "" , ExitP  , NULL  , NULL , NULL , kNoKey , kNoKey } ,
  { "" , ConfP  , NULL  , NULL , NULL , kNoKey , kNoKey } ,
  { "" , AboutP , NULL  , NULL , NULL , kNoKey , kNoKey } ,
  { "" , TMntP  , NULL  , NULL , NULL , kNoKey , kNoKey } ,
} ;

LOpts :: LOpts ( QWidget * parent ) : QDialog ( parent ) {

  ITblSize = sizeof ( ITbl ) / sizeof ( Item ) ;

  Item * C ;

  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    C = & ITbl [ I ] ; QString K = C -> CFKey ;
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
      if ( C -> Editor ) {
        C -> Editor -> setToolTip ( tr ( C -> ToolTip ) ) ;
      }//fi
    }//fi
  }//done

  QGridLayout * Lay = new QGridLayout ;
  QLabel * Lbl ; QString Ttp ; QPixmap P ; int H , R = 0 ;

  { QString L ( 24 , 'M' ) ; // 24 is arbitrary.
    H = QLineEdit ( L ) . sizeHint ( ) . height ( ) ;
    Lay -> setColumnMinimumWidth (
      2  , QLabel ( L ) . sizeHint ( ) . width  ( ) ) ;
  }

  Lay -> addWidget ( new QLabel ( tr ( "Actions:"       ) ) , R    , 1 ) ;
  Lay -> addWidget ( new QLabel ( tr ( "Command lines:" ) ) , R    , 2 ) ;
  Lay -> addWidget ( new QLabel ( tr ( "Timeouts:"      ) ) , R ++ , 3 ) ;

  Lay -> addWidget ( HLine ( ) , R    , 0 , 1 , 2 ) ;
  Lay -> addWidget ( HLine ( ) , R    , 2 ) ;
  Lay -> addWidget ( HLine ( ) , R ++ , 3 ) ;

  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    C = & ITbl [ I ] ;
    if ( C -> Lnk ) {
      Ttp =  tr ( C -> ToolTip ) ;
      P . load ( toStr  ( C -> Pix ) ) ;
      Lbl =  new QLabel ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
      Lbl -> setToolTip ( Ttp ) ; Lay -> addWidget ( Lbl , R , 0 ) ;
      Lbl =  new QLabel ( tr ( C -> Label ) ) ;
      Lbl -> setBuddy   ( C -> Editor ) ;
      Lbl -> setToolTip ( Ttp ) ; Lay -> addWidget ( Lbl , R , 1 ) ;
      Lay -> addWidget  ( C -> Editor , R , 2 ) ;
      Lay -> addWidget  ( ITbl [ C -> Lnk ] . Editor , R ++ , 3 ) ;
    }//fi
  }//done

  Lay -> addWidget ( HLine ( ) , R ++ , 0 , 1 , -1 ) ;

  C = & ITbl [ kHideDevs ] ;
  Lbl =  new QLabel ( tr ( C -> Label ) ) ;
  Lbl -> setBuddy   ( C -> Editor ) ;
  Lbl -> setToolTip ( tr ( C -> ToolTip ) ) ;
  Lay -> addWidget  ( Lbl , R  , 1 ) ;
  Lay -> addWidget  ( C -> Editor  , R ++ , 2 ) ;

  Lay -> addWidget ( HLine ( ) , R ++ , 0 , 1 , -1 ) ;

  Lay -> addWidget ( ITbl [ kMntStart  ] . Editor , R ++ , 2 ) ;
  Lay -> addWidget ( new  QLabel ( tr  ( "Handling:" ) ) , R , 1 ) ;
  Lay -> addWidget ( ITbl [ kMntNew    ] . Editor , R ++ , 2 ) ;
  Lay -> addWidget ( ITbl [ kMntMedia  ] . Editor , R ++ , 2 ) ;
  Lay -> addWidget ( ITbl [ kAutoEject ] . Editor , R ++ , 2 ) ;

  Lay -> addWidget ( HLine ( ) , R ++ , 0 , 1 , -1 ) ;

  QPushButton * pbOk   = new QPushButton ( tr ( "&Ok"     ) ) ;
  QPushButton * pbCanc = new QPushButton ( tr ( "&Cancel" ) ) ;
  connect ( pbOk   , SIGNAL ( clicked ( ) ) , SLOT ( accept ( ) ) ) ;
  connect ( pbCanc , SIGNAL ( clicked ( ) ) , SLOT ( reject ( ) ) ) ;
  Lay -> addWidget ( pbOk , R , 1 ) ; Lay -> addWidget ( pbCanc , R , 2 ) ;

  setLayout ( Lay ) ; setWindowIcon ( QIcon ( ConfP ) ) ;
  setWindowTitle ( TPref + tr ( "Settings" ) ) ;

}// LOpts

int LOpts :: exec ( ) {
  int R = QDialog :: exec ( ) ; bool A = R == QDialog :: Accepted ;
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
    Item * C = & ITbl [ I ] ; QString K = C -> CFKey ;
    if ( ! K . isEmpty ( ) ) { Conf . setValue ( K , C -> Val ) ; }//fi
  }//done
}// ~LOpts

QString LOpts :: toStr ( loKey K ) {
  QString S = "" ;
  if ( K && K < ITblSize ) { S = ITbl [ K ] . Val . toString ( ) ; }//fi
  return S ;
}// LOpts :: toStr

int LOpts :: toInt ( loKey K ) {
  int I = -1 ;
  if ( K && K < ITblSize ) { I = ITbl [ K ] . Val . toInt    ( ) ; }//fi
  return I ;
}// LOpts :: toStr

bool LOpts :: toBool ( loKey K ) {
  bool B = false ;
  if ( K && K < ITblSize ) { B = ITbl [ K ] . Val . toBool   ( ) ; }//fi
  return B ;
}// LOpts :: toStr

OptList LOpts :: GetAll ( ) {
  OptList L ;
  for ( int I = 1 ; I < ITblSize ; ++ I ) {
    Item * C = & ITbl [ I ] ; QString K = C -> CFKey ;
    if ( ! K . isEmpty ( ) ) { L << OptPair ( K , C -> Val . toString ( ) ) ;
    }//fi
  }//done
  return L ;
}// LOpts :: GetAll


//eof LOpts.cpp
