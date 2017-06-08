
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QApplication>

#include "Listener.h"

static const char // ".conf" file items.
  * MountCmdItm = "Mount_command"   , * MountTOItm  = "Mount_timeout"   ,
  * UnmntCmdItm = "Unmount_command" , * UnmntTOItm  = "Unmount_timeout" ,
  * EjectCmdItm = "Eject_command"   , * EjectTOItm  = "Eject_timeout"   ,
  * UnlckCmdItm = "Unlock_command"  , * UnlckTOItm  = "Unlock_timeout"  ,
  * LockCmdItm  = "Lock_command"    , * LockTOItm   = "Lock_timeout"    ,
  * AddImCmdItm = "Add_Img_command" , * AddImTOItm  = "Add_Img_timeout" ,
  * HideDevsItm = "Hidden_devices"  , * MntNewItm   = "Mount_new"       ,
  * MntMediaItm = "Mount_media"     , * MntStartItm = "Mount_on_start"  ;

// Defaults.

static const int NoTimeout = -1 ;
  // for interactive external program, defined by QProcess

static const int // arbitrary values
  DefTimeout = 5    , // 5 s for run the external program.
  MaxTimeout = 3600 ; // 1 h

static const char
  * MountCmdDef = "udevil mount"  , * UnlckCmdDef = "" ,
  * UnmntCmdDef = "udevil umount" , * LockCmdDef  = "" ,
  * EjectCmdDef = "eject"         , * HideDevsDef = "" ,
  * AddImCmdDef = "" ;

static const int
  MountTODef = DefTimeout , UnmntTODef = DefTimeout ,
  EjectTODef = DefTimeout , UnlckTODef = NoTimeout  ,
  LockTODef  = NoTimeout  , AddImTODef = DefTimeout ;

static const bool
  MntNewDef = false , MntMediaDef = false , MntStartDef = false ;

static const char
  * MountCmdPix = ":/icons/mount.png"   ,
  * UnmntCmdPix = ":/icons/unmount.png" ,
  * EjectCmdPix = ":/icons/eject.png"   ,
  * UnlckCmdPix = ":/icons/unlock.png"  ,
  * LockCmdPix  = ":/icons/lock.png"    ,
  * AddImCmdPix = ":/icons/fsimg.png"   ,
  * ConfigPix   = ":/icons/config.png"  ,
  * AboutPix    = ":/icons/info.png"    ,
  * TMountPix   = ":/icons/tmount.png"  ,
  * ExitPix     = ":/icons/exit.png"    ;

LOpts :: LOpts ( QWidget * parent ) : QDialog ( parent ) {

  QString // Labels.
    ActTitleLbl = tr ( "Actions:"              ) ,
    CmdTitleLbl = tr ( "Command lines:"        ) ,
    TOTitleLbl  = tr ( "Timeouts:"             ) ,
    MountCmdLbl = tr ( "M&ount"                ) ,
    UnmntCmdLbl = tr ( "&Unmount"              ) ,
    EjectCmdLbl = tr ( "&Eject"                ) ,
    UnlckCmdLbl = tr ( "Unloc&k"               ) ,
    LockCmdLbl  = tr ( "&Lock"                 ) ,
    AddImCmdLbl = tr ( "Add &Image"            ) ,
    HideDevsLbl = tr ( "&Hidden devices"       ) ,
    MntNewLbl   = tr ( "Mount &new devices"    ) ,
    MntMediaLbl = tr ( "Mount inserted &media" ) ,
    MntStartLbl = tr ( "Mount on &start"       ) ;

  QString // Tooltips.
    MountCmdTtp = tr ( "The device node name (/dev/...) will be<br/>"
                       "appended to the end of given command line." ) ,
    UnmntCmdTtp = tr ( "The mountpoint pathname (/media/...) will be<br/>"
                       "appended to the end of given command line." ) ,
    LockCmdTtp  = tr ( "The mapping name will be appended<br/>"
                       "to the end of given command line." ) ,
    HideDevsTtp = tr ( "Type a space-separated list of regexps, b.e.<br/>"
                       "'/dev/sd[ab].* /dev/sdc1' (restart required)." ) ,
    AddImCmdTtp = tr ( "The selected file name will be appended<br/>"
                       "to the end of given command line." ) ,
    TimeoutTtp  = tr ( "Timeout in secs for given command,<br/>"
                       " 0 means no timeout." ) ,
    EjectCmdTtp = MountCmdTtp , UnlckCmdTtp = MountCmdTtp ;

  setWindowTitle ( TPref + tr ( "Settings" ) ) ;
  setWindowIcon  ( QIcon ( ConfigPix ) ) ;

  NeedSave = false ;

  if ( Conf . contains ( MountCmdItm ) ) {
    MountCmdVal = Conf . value ( MountCmdItm ) . toString ( ) ;
  } else { NeedSave = true ; MountCmdVal = MountCmdDef ;
  }//fi
  if ( Conf . contains ( MountTOItm  ) ) {
    MountTOVal  = Conf . value ( MountTOItm  ) . toInt ( ) ;
  } else { NeedSave = true ; MountTOVal  = MountTODef  ;
  }//fi
  if ( Conf . contains ( UnmntCmdItm ) ) {
    UnmntCmdVal = Conf . value ( UnmntCmdItm ) . toString ( ) ;
  } else { NeedSave = true ; UnmntCmdVal = UnmntCmdDef ;
  }//fi
  if ( Conf . contains ( UnmntTOItm  ) ) {
    UnmntTOVal  = Conf . value ( UnmntTOItm  ) . toInt ( ) ;
  } else { NeedSave = true ; UnmntTOVal  = UnmntTODef  ;
  }//fi
  if ( Conf . contains ( EjectCmdItm ) ) {
    EjectCmdVal = Conf . value ( EjectCmdItm ) . toString ( ) ;
  } else { NeedSave = true ; EjectCmdVal = EjectCmdDef ;
  }//fi
  if ( Conf . contains ( EjectTOItm  ) ) {
    EjectTOVal  = Conf . value ( EjectTOItm  ) . toInt ( ) ;
  } else { NeedSave = true ; EjectTOVal  = EjectTODef  ;
  }//fi
  if ( Conf . contains ( UnlckCmdItm ) ) {
    UnlckCmdVal = Conf . value ( UnlckCmdItm ) . toString ( ) ;
  } else { NeedSave = true ; UnlckCmdVal = UnlckCmdDef ;
  }//fi
  if ( Conf . contains ( UnlckTOItm  ) ) {
    UnlckTOVal  = Conf . value ( UnlckTOItm  ) . toInt ( ) ;
  } else { NeedSave = true ; UnlckTOVal  = UnlckTODef  ;
  }//fi
  if ( Conf . contains ( LockCmdItm  ) ) {
    LockCmdVal  = Conf . value ( LockCmdItm  ) . toString ( ) ;
  } else { NeedSave = true ; LockCmdVal  = LockCmdDef  ;
  }//fi
  if ( Conf . contains ( LockTOItm   ) ) {
    LockTOVal   = Conf . value ( LockTOItm   ) . toInt ( ) ;
  } else { NeedSave = true ; LockTOVal   = LockTODef   ;
  }//fi
  if ( Conf . contains ( AddImCmdItm ) ) {
    AddImCmdVal = Conf . value ( AddImCmdItm ) . toString ( ) ;
  } else { NeedSave = true ; AddImCmdVal = AddImCmdDef ;
  }//fi
  if ( Conf . contains ( AddImTOItm  ) ) {
    AddImTOVal  = Conf . value ( AddImTOItm  ) . toInt ( ) ;
  } else { NeedSave = true ; AddImTOVal  = AddImTODef  ;
  }//fi
  if ( Conf . contains ( HideDevsItm ) ) {
    HideDevsVal = Conf . value ( HideDevsItm ) . toString ( ) ;
  } else { NeedSave = true ; HideDevsVal = HideDevsDef ;
  }//fi
  if ( Conf . contains ( MntNewItm ) ) {
    MntNewVal   = Conf . value ( MntNewItm   ) . toBool ( ) ;
  } else { NeedSave = true ; MntNewVal   = MntNewDef   ;
  }//fi
  if ( Conf . contains ( MntMediaItm ) ) {
    MntMediaVal = Conf . value ( MntMediaItm ) . toBool ( ) ;
  } else { NeedSave = true ; MntMediaVal = MntMediaDef ;
  }//fi
  if ( Conf . contains ( MntStartItm ) ) {
    MntStartVal = Conf . value ( MntStartItm ) . toBool ( ) ;
  } else { NeedSave = true ; MntStartVal = MntStartDef ;
  }//fi

  MountCmdLine = new QLineEdit ( MountCmdVal ) ;
  UnmntCmdLine = new QLineEdit ( UnmntCmdVal ) ;
  EjectCmdLine = new QLineEdit ( EjectCmdVal ) ;
  UnlckCmdLine = new QLineEdit ( UnlckCmdVal ) ;
  LockCmdLine  = new QLineEdit ( LockCmdVal  ) ;
  AddImCmdLine = new QLineEdit ( AddImCmdVal ) ;
  HideDevsLine = new QLineEdit ( HideDevsVal ) ;

  MountTOSpin  = new QSpinBox  ( ) ;
  MountTOSpin -> setMaximum    ( MaxTimeout  ) ;
  MountTOSpin -> setValue      ( MountTOVal  ) ;
  UnmntTOSpin  = new QSpinBox  ( ) ;
  UnmntTOSpin -> setMaximum    ( MaxTimeout  ) ;
  UnmntTOSpin -> setValue      ( UnmntTOVal  ) ;
  EjectTOSpin  = new QSpinBox  ( ) ;
  EjectTOSpin -> setMaximum    ( MaxTimeout  ) ;
  EjectTOSpin -> setValue      ( EjectTOVal  ) ;
  UnlckTOSpin  = new QSpinBox  ( ) ;
  UnlckTOSpin -> setMaximum    ( MaxTimeout  ) ;
  UnlckTOSpin -> setValue      ( UnlckTOVal  ) ;
  LockTOSpin   = new QSpinBox  ( ) ;
  LockTOSpin  -> setMaximum    ( MaxTimeout  ) ;
  LockTOSpin  -> setValue      ( LockTOVal   ) ;
  AddImTOSpin  = new QSpinBox  ( ) ;
  AddImTOSpin -> setMaximum    ( MaxTimeout  ) ;
  AddImTOSpin -> setValue      ( AddImTOVal  ) ;

  MntNewBox    = new QCheckBox ( MntNewLbl   ) ;
  MntNewBox   -> setChecked    ( MntNewVal   ) ;
  MntMediaBox  = new QCheckBox ( MntMediaLbl ) ;
  MntMediaBox -> setChecked    ( MntMediaVal ) ;
  MntStartBox  = new QCheckBox ( MntStartLbl ) ;
  MntStartBox -> setChecked    ( MntStartVal ) ;

  QGridLayout * Lay = new QGridLayout ; QLabel * Lbl ; int R = 0 ;

  Lbl = new QLabel  ( ActTitleLbl ) ; Lay -> addWidget ( Lbl , R    , 1 ) ;
  Lbl = new QLabel  ( CmdTitleLbl ) ; Lay -> addWidget ( Lbl , R    , 2 ) ;
  Lbl = new QLabel  ( TOTitleLbl  ) ; Lay -> addWidget ( Lbl , R ++ , 3 ) ;

  QPixmap P ; int H = MountCmdLine -> sizeHint ( ) . height ( ) ;

  P . load  ( MountCmdPix ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( MountCmdTtp ) ; Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( MountCmdLbl ) ;
  Lbl -> setToolTip ( MountCmdTtp ) ; Lbl -> setBuddy ( MountCmdLine  ) ;
  MountCmdLine -> setToolTip ( MountCmdTtp ) ;
  MountTOSpin  -> setToolTip ( TimeoutTtp  ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( MountCmdLine  , R    , 2 ) ;
  Lay -> addWidget  ( MountTOSpin   , R ++ , 3 ) ;

  P . load  ( UnmntCmdPix ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( UnmntCmdTtp ) ; Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( UnmntCmdLbl ) ;
  Lbl -> setToolTip ( UnmntCmdTtp ) ; Lbl -> setBuddy ( UnmntCmdLine  ) ;
  UnmntCmdLine -> setToolTip ( UnmntCmdTtp ) ;
  UnmntTOSpin  -> setToolTip ( TimeoutTtp  ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( UnmntCmdLine  , R    , 2 ) ;
  Lay -> addWidget  ( UnmntTOSpin   , R ++ , 3 ) ;

  P . load  ( EjectCmdPix ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( EjectCmdTtp ) ; Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( EjectCmdLbl ) ;
  Lbl -> setToolTip ( EjectCmdTtp ) ; Lbl -> setBuddy ( EjectCmdLine  ) ;
  EjectCmdLine -> setToolTip ( EjectCmdTtp ) ;
  EjectTOSpin  -> setToolTip ( TimeoutTtp  ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( EjectCmdLine  , R    , 2 ) ;
  Lay -> addWidget  ( EjectTOSpin   , R ++ , 3 ) ;

  P . load  ( UnlckCmdPix ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( UnlckCmdTtp ) ; Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( UnlckCmdLbl ) ;
  Lbl -> setToolTip ( UnlckCmdTtp ) ; Lbl -> setBuddy ( UnlckCmdLine  ) ;
  UnlckCmdLine -> setToolTip ( UnlckCmdTtp ) ;
  UnlckTOSpin  -> setToolTip ( TimeoutTtp  ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( UnlckCmdLine  , R    , 2 ) ;
  Lay -> addWidget  ( UnlckTOSpin   , R ++ , 3 ) ;

  P . load  ( LockCmdPix  ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( LockCmdTtp  ) ; Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( LockCmdLbl  ) ;
  Lbl -> setToolTip ( LockCmdTtp  ) ; Lbl -> setBuddy ( LockCmdLine   ) ;
  LockCmdLine  -> setToolTip ( LockCmdTtp  ) ;
  LockTOSpin   -> setToolTip ( TimeoutTtp  ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( LockCmdLine   , R    , 2 ) ;
  Lay -> addWidget  ( LockTOSpin    , R ++ , 3 ) ;

  P . load  ( AddImCmdPix ) ;
  Lbl = new QLabel  ( ) ; Lbl -> setPixmap ( P . scaled ( H , H ) ) ;
  Lbl -> setToolTip ( AddImCmdTtp ) ; Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lbl = new QLabel  ( AddImCmdLbl ) ;
  Lbl -> setToolTip ( AddImCmdTtp ) ; Lbl -> setBuddy ( AddImCmdLine  ) ;
  AddImCmdLine -> setToolTip ( AddImCmdTtp ) ;
  AddImTOSpin  -> setToolTip ( TimeoutTtp  ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( AddImCmdLine  , R    , 2 ) ;
  Lay -> addWidget  ( AddImTOSpin   , R ++ , 3 ) ;

  Lbl = new QLabel  ( HideDevsLbl ) ;
  Lbl -> setToolTip ( HideDevsTtp ) ; Lbl -> setBuddy ( HideDevsLine  ) ;
  HideDevsLine -> setToolTip ( HideDevsTtp ) ;
  Lay -> addWidget  ( Lbl , R , 1 ) ;
  Lay -> addWidget  ( HideDevsLine  , R ++ , 2 ) ;

  Lay -> addWidget  ( MntStartBox   , R ++ , 2 ) ;
  Lay -> addWidget  ( MntNewBox     , R ++ , 2 ) ;
  Lay -> addWidget  ( MntMediaBox   , R ++ , 2 ) ;

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
    NeedSave = true ;
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
  if ( NeedSave ) {
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
  }//fi
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

QString LOpts :: MountIcon ( ) { return MountCmdPix ; }// LOpts :: MountIcon

QString LOpts :: UnmntIcon ( ) { return UnmntCmdPix ; }// LOpts :: UnmntIcon

QString LOpts :: UnlckIcon ( ) { return UnlckCmdPix ; }// LOpts :: UnlckIcon

QString LOpts :: EjectIcon ( ) { return EjectCmdPix ; }// LOpts :: EjectIcon

QString LOpts :: LockIcon  ( ) { return LockCmdPix  ; }// LOpts :: LockIcon

QString LOpts :: AddImIcon ( ) { return AddImCmdPix ; }// LOpts :: AddImIcon

QString LOpts :: AboutIcon ( ) { return AboutPix    ; }// LOpts :: AboutIcon

QString LOpts :: ExitIcon  ( ) { return ExitPix     ; }// LOpts :: ExitIcon

QString LOpts :: ConfIcon  ( ) { return ConfigPix   ; }// LOpts :: ConfIcon

QString LOpts :: TMntIcon  ( ) { return TMountPix   ; }// LOpts :: TMntIcon

int LOpts :: ms ( int Sec ) { return Sec ? Sec * 1000 : NoTimeout ;
}// LOpts :: ms

//eof LOpts.cpp
