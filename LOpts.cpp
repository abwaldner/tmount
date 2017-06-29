
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
  MountCmdItm = "Mount_command"   , MountTOItm  = "Mount_timeout"   ,
  UnmntCmdItm = "Unmount_command" , UnmntTOItm  = "Unmount_timeout" ,
  EjectCmdItm = "Eject_command"   , EjectTOItm  = "Eject_timeout"   ,
  UnlckCmdItm = "Unlock_command"  , UnlckTOItm  = "Unlock_timeout"  ,
  LockCmdItm  = "Lock_command"    , LockTOItm   = "Lock_timeout"    ,
  AddImCmdItm = "Add_Img_command" , AddImTOItm  = "Add_Img_timeout" ,
  HideDevsItm = "Hidden_devices"  , MntNewItm   = "Mount_new"       ,
  MntMediaItm = "Mount_media"     , MntStartItm = "Mount_on_start"  ;

// Defaults.

static const int NoTimeout = -1 ;
  // for interactive external program, defined by QProcess

static const int // arbitrary values
  DefTO =    5 , // 5 s for run the external program.
  MaxTO = 3600 ; // 1 h

static const QString
  MountCmdDef = "udevil mount"  , UnlckCmdDef = "" ,
  UnmntCmdDef = "udevil umount" , LockCmdDef  = "" ,
  EjectCmdDef = "eject"         , HideDevsDef = "" , AddImCmdDef = "" ;

static const int
  MountTODef = DefTO , UnmntTODef = DefTO , UnlckTODef = 0 ,
  EjectTODef = DefTO , AddImTODef = DefTO , LockTODef  = 0 ;

static const bool
  MntNewDef = false , MntMediaDef = false , MntStartDef = false ;

static const QString // Icons.
  MountPix  = ":/icons/mount.png"  , UnmntPix  = ":/icons/unmount.png" ,
  EjectPix  = ":/icons/eject.png"  , UnlckPix  = ":/icons/unlock.png"  ,
  LockPix   = ":/icons/lock.png"   , AddImgPix = ":/icons/fsimg.png"   ,
  ConfigPix = ":/icons/config.png" , AboutPix  = ":/icons/info.png"    ,
  TMountPix = ":/icons/tmount.png" , ExitPix   = ":/icons/exit.png"    ;

static CPtr // Tooltips.
  MountTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "The device node name (/dev/...) will be<br/>"
    "appended to the end of given command line." ) ,
  UnmntTtp  = QT_TRANSLATE_NOOP ( "LOpts" ,
    "The mountpoint pathname (/media/...) will be<br/>"
    "appended to the end of given command line." ) ,
  LockTtp   = QT_TRANSLATE_NOOP ( "LOpts" ,
    "The mapping name will be appended<br/>"
    "to the end of given command line." ) ,
  HideTtp   = QT_TRANSLATE_NOOP ( "LOpts" ,
    "Type a space-separated list of regexps, b.e.<br/>"
    "'/dev/sd[ab].* /dev/sdc1' (restart required)." ) ,
  AddImgTtp = QT_TRANSLATE_NOOP ( "LOpts" ,
    "The selected file name will be appended<br/>"
    "to the end of given command line." ) ,
  TimeTtp   = QT_TRANSLATE_NOOP ( "LOpts" ,
    "Timeout in secs for given command,<br/> 0 means no timeout." ) ,
  EjectTtp  = MountTtp , UnlckTtp = MountTtp ;

static CPtr // Labels.
  MountLbl    = QT_TRANSLATE_NOOP ( "LOpts" , "&Mount"     ) ,
  UnmntLbl    = QT_TRANSLATE_NOOP ( "LOpts" , "&Unmount"   ) ,
  EjectLbl    = QT_TRANSLATE_NOOP ( "LOpts" , "&Eject"     ) ,
  UnlckLbl    = QT_TRANSLATE_NOOP ( "LOpts" , "Unloc&k"    ) ,
  LockLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "&Lock"      ) ,
  AddImgLbl   = QT_TRANSLATE_NOOP ( "LOpts" , "Add &Image" ) ,
  HideLbl     = QT_TRANSLATE_NOOP ( "LOpts" , "&Hidden devices"       ) ,
  MntNewLbl   = QT_TRANSLATE_NOOP ( "LOpts" , "Mount &new devices"    ) ,
  MntMediaLbl = QT_TRANSLATE_NOOP ( "LOpts" , "Mount inser&ted media" ) ,
  MntStartLbl = QT_TRANSLATE_NOOP ( "LOpts" , "Mount on &start"       ) ;

QFrame * NFrame ( ) {
  QFrame * F = new QFrame ; F -> setFrameShape ( QFrame :: HLine ) ;
  return F ;
}// NFrame

LOpts :: LOpts ( QWidget * parent ) : QDialog ( parent ) {

  setWindowTitle ( TPref + tr ( "Settings" ) ) ;
  setWindowIcon  ( QIcon ( ConfigPix ) ) ;

  if ( Conf . contains ( MountCmdItm ) ) {
    MountCmdVal = Conf . value ( MountCmdItm ) . toString ( ) ;
  } else { MountCmdVal = MountCmdDef ;
  }//fi
  if ( Conf . contains ( MountTOItm  ) ) {
    MountTOVal  = Conf . value ( MountTOItm  ) . toInt ( ) ;
  } else { MountTOVal  = MountTODef  ;
  }//fi
  if ( Conf . contains ( UnmntCmdItm ) ) {
    UnmntCmdVal = Conf . value ( UnmntCmdItm ) . toString ( ) ;
  } else { UnmntCmdVal = UnmntCmdDef ;
  }//fi
  if ( Conf . contains ( UnmntTOItm  ) ) {
    UnmntTOVal  = Conf . value ( UnmntTOItm  ) . toInt ( ) ;
  } else { UnmntTOVal  = UnmntTODef  ;
  }//fi
  if ( Conf . contains ( EjectCmdItm ) ) {
    EjectCmdVal = Conf . value ( EjectCmdItm ) . toString ( ) ;
  } else { EjectCmdVal = EjectCmdDef ;
  }//fi
  if ( Conf . contains ( EjectTOItm  ) ) {
    EjectTOVal  = Conf . value ( EjectTOItm  ) . toInt ( ) ;
  } else { EjectTOVal  = EjectTODef  ;
  }//fi
  if ( Conf . contains ( UnlckCmdItm ) ) {
    UnlckCmdVal = Conf . value ( UnlckCmdItm ) . toString ( ) ;
  } else { UnlckCmdVal = UnlckCmdDef ;
  }//fi
  if ( Conf . contains ( UnlckTOItm  ) ) {
    UnlckTOVal  = Conf . value ( UnlckTOItm  ) . toInt ( ) ;
  } else { UnlckTOVal  = UnlckTODef  ;
  }//fi
  if ( Conf . contains ( LockCmdItm  ) ) {
    LockCmdVal  = Conf . value ( LockCmdItm  ) . toString ( ) ;
  } else { LockCmdVal  = LockCmdDef  ;
  }//fi
  if ( Conf . contains ( LockTOItm   ) ) {
    LockTOVal   = Conf . value ( LockTOItm   ) . toInt ( ) ;
  } else { LockTOVal   = LockTODef   ;
  }//fi
  if ( Conf . contains ( AddImCmdItm ) ) {
    AddImCmdVal = Conf . value ( AddImCmdItm ) . toString ( ) ;
  } else { AddImCmdVal = AddImCmdDef ;
  }//fi
  if ( Conf . contains ( AddImTOItm  ) ) {
    AddImTOVal  = Conf . value ( AddImTOItm  ) . toInt ( ) ;
  } else { AddImTOVal  = AddImTODef  ;
  }//fi
  if ( Conf . contains ( HideDevsItm ) ) {
    HideDevsVal = Conf . value ( HideDevsItm ) . toString ( ) ;
  } else { HideDevsVal = HideDevsDef ;
  }//fi
  if ( Conf . contains ( MntNewItm ) ) {
    MntNewVal   = Conf . value ( MntNewItm   ) . toBool ( ) ;
  } else { MntNewVal   = MntNewDef   ;
  }//fi
  if ( Conf . contains ( MntMediaItm ) ) {
    MntMediaVal = Conf . value ( MntMediaItm ) . toBool ( ) ;
  } else { MntMediaVal = MntMediaDef ;
  }//fi
  if ( Conf . contains ( MntStartItm ) ) {
    MntStartVal = Conf . value ( MntStartItm ) . toBool ( ) ;
  } else { MntStartVal = MntStartDef ;
  }//fi

  MountCmdLine = new QLineEdit ( MountCmdVal  ) ;
  MountCmdLine -> setToolTip ( tr ( MountTtp  ) ) ;
  UnmntCmdLine = new QLineEdit ( UnmntCmdVal  ) ;
  UnmntCmdLine -> setToolTip ( tr ( UnmntTtp  ) ) ;
  EjectCmdLine = new QLineEdit ( EjectCmdVal  ) ;
  EjectCmdLine -> setToolTip ( tr ( EjectTtp  ) ) ;
  UnlckCmdLine = new QLineEdit ( UnlckCmdVal  ) ;
  UnlckCmdLine -> setToolTip ( tr ( UnlckTtp  ) ) ;
  LockCmdLine  = new QLineEdit ( LockCmdVal   ) ;
  LockCmdLine  -> setToolTip ( tr ( LockTtp   ) ) ;
  AddImCmdLine = new QLineEdit ( AddImCmdVal  ) ;
  AddImCmdLine -> setToolTip ( tr ( AddImgTtp ) ) ;
  HideDevsLine = new QLineEdit ( HideDevsVal  ) ;
  HideDevsLine -> setToolTip ( tr ( HideTtp   ) ) ;

  MountTOSpin  = new QSpinBox ;
  MountTOSpin -> setMaximum   ( MaxTO ) ;
  MountTOSpin -> setValue     ( MountTOVal   ) ;
  MountTOSpin -> setToolTip   ( tr ( TimeTtp ) ) ;
  UnmntTOSpin  = new QSpinBox ;
  UnmntTOSpin -> setMaximum   ( MaxTO ) ;
  UnmntTOSpin -> setValue     ( UnmntTOVal   ) ;
  UnmntTOSpin -> setToolTip   ( tr ( TimeTtp ) ) ;
  EjectTOSpin  = new QSpinBox ;
  EjectTOSpin -> setMaximum   ( MaxTO ) ;
  EjectTOSpin -> setValue     ( EjectTOVal   ) ;
  EjectTOSpin -> setToolTip   ( tr ( TimeTtp ) ) ;
  UnlckTOSpin  = new QSpinBox ;
  UnlckTOSpin -> setMaximum   ( MaxTO ) ;
  UnlckTOSpin -> setValue     ( UnlckTOVal   ) ;
  UnlckTOSpin -> setToolTip   ( tr ( TimeTtp ) ) ;
  LockTOSpin   = new QSpinBox ;
  LockTOSpin  -> setMaximum   ( MaxTO ) ;
  LockTOSpin  -> setValue     ( LockTOVal    ) ;
  LockTOSpin  -> setToolTip   ( tr ( TimeTtp ) ) ;
  AddImTOSpin  = new QSpinBox ;
  AddImTOSpin -> setMaximum   ( MaxTO ) ;
  AddImTOSpin -> setValue     ( AddImTOVal   ) ;
  AddImTOSpin -> setToolTip   ( tr ( TimeTtp ) ) ;

  MntNewBox    = new QCheckBox ( tr ( MntNewLbl   ) ) ;
  MntNewBox   -> setChecked    ( MntNewVal    ) ;
  MntMediaBox  = new QCheckBox ( tr ( MntMediaLbl ) ) ;
  MntMediaBox -> setChecked    ( MntMediaVal  ) ;
  MntStartBox  = new QCheckBox ( tr ( MntStartLbl ) ) ;
  MntStartBox -> setChecked    ( MntStartVal  ) ;

  QGridLayout * Lay = new QGridLayout ;
  QLabel * Lbl ; int R = 0 ;

  Lbl = new QLabel ( QString ( ) . fill ( 'M' , 24 ) ) ;
  Lay -> setColumnMinimumWidth ( 2 , Lbl -> sizeHint ( ) . width ( ) ) ;
  delete Lbl ;

  Lay -> addWidget ( new QLabel ( tr ( "Actions:"       ) ) , R    , 1 ) ;
  Lay -> addWidget ( new QLabel ( tr ( "Command lines:" ) ) , R    , 2 ) ;
  Lay -> addWidget ( new QLabel ( tr ( "Timeouts:"      ) ) , R ++ , 3 ) ;

  Lay -> addWidget ( NFrame ( ) , R    , 0 , 1 , 2 ) ;
  Lay -> addWidget ( NFrame ( ) , R    , 2 ) ;
  Lay -> addWidget ( NFrame ( ) , R ++ , 3 ) ;

  QPixmap P ; int H = MountCmdLine -> sizeHint ( ) . height ( ) ;
  QString Ttp ;

  P . load ( MountPix ) ; Ttp = MountCmdLine -> toolTip ( ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lay -> addWidget ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( tr ( MountLbl ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lbl -> setBuddy ( MountCmdLine ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( MountCmdLine , R    , 2 ) ;
  Lay -> addWidget  ( MountTOSpin  , R ++ , 3 ) ;

  P . load ( UnmntPix ) ; Ttp = UnmntCmdLine -> toolTip ( ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( tr ( UnmntLbl ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lbl -> setBuddy ( UnmntCmdLine ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( UnmntCmdLine , R    , 2 ) ;
  Lay -> addWidget  ( UnmntTOSpin  , R ++ , 3 ) ;

  P . load ( EjectPix ) ; Ttp = EjectCmdLine -> toolTip ( ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lay -> addWidget ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( tr ( EjectLbl ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lbl -> setBuddy ( EjectCmdLine ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( EjectCmdLine , R    , 2 ) ;
  Lay -> addWidget  ( EjectTOSpin  , R ++ , 3 ) ;

  P . load ( UnlckPix ) ; Ttp = UnlckCmdLine -> toolTip ( ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lay -> addWidget ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( tr ( UnlckLbl ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lbl -> setBuddy ( UnlckCmdLine ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( UnlckCmdLine , R    , 2 ) ;
  Lay -> addWidget  ( UnlckTOSpin  , R ++ , 3 ) ;

  P . load ( LockPix  ) ; Ttp = LockCmdLine -> toolTip ( ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lay -> addWidget ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( tr ( LockLbl ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lbl -> setBuddy ( LockCmdLine ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( LockCmdLine , R    , 2 ) ;
  Lay -> addWidget  ( LockTOSpin  , R ++ , 3 ) ;

  P . load ( AddImgPix  ) ; Ttp = AddImCmdLine -> toolTip ( ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lay -> addWidget ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( tr ( AddImgLbl ) ) ;
  Lbl -> setToolTip ( Ttp ) ; Lbl -> setBuddy ( AddImCmdLine ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( AddImCmdLine , R    , 2 ) ;
  Lay -> addWidget  ( AddImTOSpin  , R ++ , 3 ) ;

  Lay -> addWidget ( NFrame ( ) , R ++ , 0 , 1 , -1 ) ;

  Lbl = new QLabel  ( tr ( HideLbl ) ) ; Ttp = HideDevsLine -> toolTip ( ) ;
  Lbl -> setToolTip ( Ttp ) ; Lbl -> setBuddy ( HideDevsLine ) ;
  Lay -> addWidget  ( Lbl , R  , 1 ) ;
  Lay -> addWidget  ( HideDevsLine , R ++ , 2 ) ;

  Lay -> addWidget ( NFrame ( ) , R ++ , 0 , 1 , -1 ) ;

  Lay -> addWidget ( MntStartBox , R ++ , 2 ) ;
  Lay -> addWidget ( new  QLabel ( tr ( "Handling:" ) ) , R , 1 ) ;
  Lay -> addWidget ( MntNewBox   , R ++ , 2 ) ;
  Lay -> addWidget ( MntMediaBox , R ++ , 2 ) ;

  Lay -> addWidget ( NFrame ( ) , R ++ , 0 , 1 , -1 ) ;

  QPushButton * pbOk   = new QPushButton ( tr ( "&Ok"     ) ) ;
  QPushButton * pbCanc = new QPushButton ( tr ( "&Cancel" ) ) ;
  connect ( pbOk   , SIGNAL ( clicked ( ) ) , SLOT ( accept ( ) ) ) ;
  connect ( pbCanc , SIGNAL ( clicked ( ) ) , SLOT ( reject ( ) ) ) ;
  Lay -> addWidget ( pbOk   , R , 1 ) ;
  Lay -> addWidget ( pbCanc , R , 2 ) ;

  setLayout ( Lay ) ;

}// LOpts

int LOpts :: exec ( ) {
  int R  =  QDialog :: exec ( ) ;
  if ( R == QDialog :: Accepted ) {
    MountCmdVal = MountCmdLine -> text  ( ) ;
    UnmntCmdVal = UnmntCmdLine -> text  ( ) ;
    EjectCmdVal = EjectCmdLine -> text  ( ) ;
    UnlckCmdVal = UnlckCmdLine -> text  ( ) ;
    LockCmdVal  = LockCmdLine  -> text  ( ) ;
    AddImCmdVal = AddImCmdLine -> text  ( ) ;
    HideDevsVal = HideDevsLine -> text  ( ) ;
    MountTOVal  = MountTOSpin  -> value ( ) ;
    UnmntTOVal  = UnmntTOSpin  -> value ( ) ;
    EjectTOVal  = EjectTOSpin  -> value ( ) ;
    UnlckTOVal  = UnlckTOSpin  -> value ( ) ;
    LockTOVal   = LockTOSpin   -> value ( ) ;
    AddImTOVal  = AddImTOSpin  -> value ( ) ;
    MntNewVal   = MntNewBox    -> isChecked ( ) ;
    MntMediaVal = MntMediaBox  -> isChecked ( ) ;
    MntStartVal = MntStartBox  -> isChecked ( ) ;
  } else {
    MountCmdLine -> setText    ( MountCmdVal ) ;
    UnmntCmdLine -> setText    ( UnmntCmdVal ) ;
    EjectCmdLine -> setText    ( EjectCmdVal ) ;
    UnlckCmdLine -> setText    ( UnlckCmdVal ) ;
    LockCmdLine  -> setText    ( LockCmdVal  ) ;
    AddImCmdLine -> setText    ( AddImCmdVal ) ;
    HideDevsLine -> setText    ( HideDevsVal ) ;
    MountTOSpin  -> setValue   ( MountTOVal  ) ;
    UnmntTOSpin  -> setValue   ( UnmntTOVal  ) ;
    EjectTOSpin  -> setValue   ( EjectTOVal  ) ;
    UnlckTOSpin  -> setValue   ( UnlckTOVal  ) ;
    LockTOSpin   -> setValue   ( LockTOVal   ) ;
    AddImTOSpin  -> setValue   ( AddImTOVal  ) ;
    MntNewBox    -> setChecked ( MntNewVal   ) ;
    MntMediaBox  -> setChecked ( MntMediaVal ) ;
    MntStartBox  -> setChecked ( MntStartVal ) ;
  }
  return R ;
}// LOpts :: Execute

LOpts :: ~LOpts ( ) {
  Conf . setValue ( MountCmdItm , MountCmdVal ) ;
  Conf . setValue ( MountTOItm  , MountTOVal  ) ;
  Conf . setValue ( UnmntCmdItm , UnmntCmdVal ) ;
  Conf . setValue ( UnmntTOItm  , UnmntTOVal  ) ;
  Conf . setValue ( EjectCmdItm , EjectCmdVal ) ;
  Conf . setValue ( EjectTOItm  , EjectTOVal  ) ;
  Conf . setValue ( UnlckCmdItm , UnlckCmdVal ) ;
  Conf . setValue ( UnlckTOItm  , UnlckTOVal  ) ;
  Conf . setValue ( LockCmdItm  , LockCmdVal  ) ;
  Conf . setValue ( LockTOItm   , LockTOVal   ) ;
  Conf . setValue ( AddImCmdItm , AddImCmdVal ) ;
  Conf . setValue ( AddImTOItm  , AddImTOVal  ) ;
  Conf . setValue ( HideDevsItm , HideDevsVal ) ;
  Conf . setValue ( MntNewItm   , MntNewVal   ) ;
  Conf . setValue ( MntMediaItm , MntMediaVal ) ;
  Conf . setValue ( MntStartItm , MntStartVal ) ;
}// ~LOpts

QStringList LOpts :: HideDevs ( ) {
  return HideDevsVal . split ( ' ' , QString :: SkipEmptyParts ) ;
}// LOpts :: HideDevs

QString LOpts :: MountCmd ( ) { return MountCmdVal ; }// LOpts :: MountCmd

QString LOpts :: UnmntCmd ( ) { return UnmntCmdVal ; }// LOpts :: UnmntCmd

QString LOpts :: EjectCmd ( ) { return EjectCmdVal ; }// LOpts :: EjectCmd

QString LOpts :: UnlckCmd ( ) { return UnlckCmdVal ; }// LOpts :: UnlckCmd

QString LOpts :: LockCmd  ( ) { return LockCmdVal  ; }// LOpts :: LockCmd

QString LOpts :: AddImCmd ( ) { return AddImCmdVal ; }// LOpts :: AddImCmd

int  LOpts :: MountTO ( ) { return ms ( MountTOVal ) ; }// LOpts :: MountTO

int  LOpts :: UnmntTO ( ) { return ms ( UnmntTOVal ) ; }// LOpts :: MountTO

int  LOpts :: EjectTO ( ) { return ms ( EjectTOVal ) ; }// LOpts :: MountTO

int  LOpts :: UnlckTO ( ) { return ms ( UnlckTOVal ) ; }// LOpts :: MountTO

int  LOpts :: LockTO  ( ) { return ms ( LockTOVal  ) ; }// LOpts :: MountTO

int  LOpts :: AddImTO ( ) { return ms ( AddImTOVal ) ; }// LOpts :: MountTO

bool LOpts :: MntNew    ( ) { return MntNewVal   ; }// LOpts :: MntNew

bool LOpts :: MntMedia  ( ) { return MntMediaVal ; }// LOpts :: MntMedia

bool LOpts :: MntStart  ( ) { return MntStartVal ; }// LOpts :: MntStart

QString LOpts :: MountIcon ( ) { return MountPix ; }// LOpts :: MountIcon

QString LOpts :: UnmntIcon ( ) { return UnmntPix ; }// LOpts :: UnmntIcon

QString LOpts :: UnlckIcon ( ) { return UnlckPix ; }// LOpts :: UnlckIcon

QString LOpts :: EjectIcon ( ) { return EjectPix ; }// LOpts :: EjectIcon

QString LOpts :: LockIcon  ( ) { return LockPix  ; }// LOpts :: LockIcon

QString LOpts :: AddImIcon ( ) { return AddImgPix ; }// LOpts :: AddImIcon

QString LOpts :: AboutIcon ( ) { return AboutPix    ; }// LOpts :: AboutIcon

QString LOpts :: ExitIcon  ( ) { return ExitPix     ; }// LOpts :: ExitIcon

QString LOpts :: ConfIcon  ( ) { return ConfigPix   ; }// LOpts :: ConfIcon

QString LOpts :: TMntIcon  ( ) { return TMountPix   ; }// LOpts :: TMntIcon

int LOpts :: ms ( int Sec ) { return Sec ? Sec * 1000 : NoTimeout ;
}// LOpts :: ms

//eof LOpts.cpp
