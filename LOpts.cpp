
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QApplication>

#include "Listener.h"

static const char // ".conf" file items.
  * MountCmdItm = "Mount_command"  , * UnmntCmdItm = "Unmount_command" ,
  * EjectCmdItm = "Eject_command"  , * HideDevsItm = "Hidden_devices"  ,
  * LockCmdItm  = "Lock_command"   , * UnlckCmdItm = "Unlock_command"  ,
  * MntNewItm   = "Mount_new"      , * MntMediaItm = "Mount_media"     ,
  * MntStartItm = "Mount_on_start" ;

LOpts :: LOpts ( QWidget * parent ) : QDialog ( parent ) {

  // Default values.
  QString MountCmdDef = "udevil mount"  , UnlckCmdDef = "" ,
          UnmntCmdDef = "udevil umount" , LockCmdDef  = "" ,
          EjectCmdDef = "eject"         , HideDevsDef = "" ;
  bool MntNewDef = false , MntMediaDef = false , MntStartDef = false ;

  QString // Labels.
    MountCmdLbl = tr ( "Mount &command"   ) ,
    UnmntCmdLbl = tr ( "&Unmount command" ) ,
    EjectCmdLbl = tr ( "&Eject command"   ) ,
    HideDevsLbl = tr ( "&Hidden devices"  ) ,
    UnlckCmdLbl = tr ( "Unloc&k command"  ) ,
    LockCmdLbl  = tr ( "&Lock  command"   ) ,
    MntNewLbl   = tr ( "Mount &new"       ) ,
    MntMediaLbl = tr ( "Mount &media"     ) ,
    MntStartLbl = tr ( "Mount on &start"  ) ;

  QString // Tooltips.
    MountCmdTtp = tr ( "The device node name (/dev/...) will be<br/>"
                       "appended to the end of given command line."    ) ,
    UnmntCmdTtp = tr ( "The mountpoint pathname (/media/...) will be<br/>"
                       "appended to the end of given command line."    ) ,
    HideDevsTtp = tr ( "Type a space-separated list of regexps, b.e.<br/>"
                       "'/dev/sd[ab].* /dev/sdc1' (restart required)." ) ,
    LockCmdTtp  = tr ( "The mapping name will be<br/>"
                       "appended to the end of given command line."    ) ,
    UnlckCmdTtp = MountCmdTtp , EjectCmdTtp = MountCmdTtp ;

  setWindowTitle ( qApp -> applicationName ( ) + tr ( " - Settings" ) ) ;
  setWindowIcon  ( QIcon ( ":/icons/config.png" ) ) ;

  NeedSave = false ;

  if ( Conf . contains ( MountCmdItm ) ) {
    MountCmdVal = Conf . value ( MountCmdItm ) . toString ( ) ;
  } else { NeedSave = true ; MountCmdVal = MountCmdDef ;
  }//fi
  if ( Conf . contains ( UnmntCmdItm ) ) {
    UnmntCmdVal = Conf . value ( UnmntCmdItm ) . toString ( ) ;
  } else { NeedSave = true ; UnmntCmdVal = UnmntCmdDef ;
  }//fi
  if ( Conf . contains ( EjectCmdItm ) ) {
    EjectCmdVal = Conf . value ( EjectCmdItm ) . toString ( ) ;
  } else { NeedSave = true ; EjectCmdVal = EjectCmdDef ;
  }//fi
  if ( Conf . contains ( UnlckCmdItm ) ) {
    UnlckCmdVal = Conf . value ( UnlckCmdItm ) . toString ( ) ;
  } else { NeedSave = true ; UnlckCmdVal = UnlckCmdDef ;
  }//fi
  if ( Conf . contains ( LockCmdItm  ) ) {
    LockCmdVal  = Conf . value ( LockCmdItm  ) . toString ( ) ;
  } else { NeedSave = true ; LockCmdVal  = LockCmdDef  ;
  }//fi
  if ( Conf . contains ( HideDevsItm ) ) {
    HideDevsVal = Conf . value ( HideDevsItm ) . toString ( ) ;
  } else { NeedSave = true ; HideDevsVal = HideDevsDef ;
  }//fi
  if ( Conf . contains ( MntNewItm ) ) {
    MntNewVal   = Conf . value ( MntNewItm   ) . toBool ( ) ;
  } else { NeedSave = true ; MntNewVal   = MntNewDef ;
  }//fi
  if ( Conf . contains ( MntMediaItm ) ) {
    MntMediaVal = Conf . value ( MntMediaItm ) . toBool ( ) ;
  } else { NeedSave = true ; MntMediaVal = MntMediaDef ;
  }//fi
  if ( Conf . contains ( MntStartItm ) ) {
    MntStartVal = Conf . value ( MntStartItm ) . toBool ( ) ;
  } else { NeedSave = true ; MntStartVal = MntStartDef ;
  }//fi

  QGridLayout * Lay = new QGridLayout ; QLabel * Lbl ; int R = 0 ;

  MountCmdLine = new QLineEdit ( MountCmdVal ) ;
  EjectCmdLine = new QLineEdit ( EjectCmdVal ) ;
  UnmntCmdLine = new QLineEdit ( UnmntCmdVal ) ;
  HideDevsLine = new QLineEdit ( HideDevsVal ) ;
  LockCmdLine  = new QLineEdit ( LockCmdVal  ) ;
  UnlckCmdLine = new QLineEdit ( UnlckCmdVal ) ;
  MntNewBox    = new QCheckBox ( MntNewLbl   ) ;
  MntNewBox   -> setChecked    ( MntNewVal   ) ;
  MntMediaBox  = new QCheckBox ( MntMediaLbl ) ;
  MntMediaBox -> setChecked    ( MntMediaVal ) ;
  MntStartBox  = new QCheckBox ( MntStartLbl ) ;
  MntStartBox -> setChecked    ( MntStartVal ) ;

  Lbl = new QLabel  ( MountCmdLbl ) ; Lbl -> setBuddy ( MountCmdLine ) ;
  Lbl -> setToolTip ( MountCmdTtp ) ;
  MountCmdLine -> setToolTip ( MountCmdTtp ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lay -> addWidget  ( MountCmdLine  , R ++ , 1 ) ;

  Lbl = new QLabel  ( UnmntCmdLbl ) ; Lbl -> setBuddy ( UnmntCmdLine ) ;
  Lbl -> setToolTip ( UnmntCmdTtp ) ;
  UnmntCmdLine -> setToolTip ( UnmntCmdTtp ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lay -> addWidget  ( UnmntCmdLine  , R ++ , 1 ) ;

  Lbl = new QLabel  ( EjectCmdLbl ) ; Lbl -> setBuddy ( EjectCmdLine ) ;
  Lbl -> setToolTip ( EjectCmdTtp ) ;
  EjectCmdLine -> setToolTip ( EjectCmdTtp ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lay -> addWidget  ( EjectCmdLine  , R ++ , 1 ) ;

  Lbl = new QLabel  ( HideDevsLbl ) ; Lbl -> setBuddy ( HideDevsLine ) ;
  Lbl -> setToolTip ( HideDevsTtp ) ;
  HideDevsLine -> setToolTip ( HideDevsTtp ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lay -> addWidget  ( HideDevsLine  , R ++ , 1 ) ;

  Lbl = new QLabel  ( UnlckCmdLbl ) ; Lbl -> setBuddy ( UnlckCmdLine ) ;
  Lbl -> setToolTip ( UnlckCmdTtp ) ;
  UnlckCmdLine -> setToolTip ( UnlckCmdTtp ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lay -> addWidget  ( UnlckCmdLine  , R ++ , 1 ) ;

  Lbl = new QLabel  ( LockCmdLbl  ) ; Lbl -> setBuddy ( LockCmdLine  ) ;
  Lbl -> setToolTip ( LockCmdTtp  ) ;
  LockCmdLine  -> setToolTip ( LockCmdTtp  ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ;
  Lay -> addWidget  ( LockCmdLine   , R ++ , 1 ) ;

  Lay -> addWidget  ( MntNewBox     , R    , 0 ) ;
  Lay -> addWidget  ( MntMediaBox   , R ++ , 1 ) ;
  Lay -> addWidget  ( MntStartBox   , R ++ , 0 ) ;

  QPushButton * pbOk   = new QPushButton ( tr ( "&Ok"     ) ) ;
  QPushButton * pbCanc = new QPushButton ( tr ( "&Cancel" ) ) ;
  connect ( pbOk   , SIGNAL ( clicked ( ) ) , SLOT ( accept ( ) ) ) ;
  connect ( pbCanc , SIGNAL ( clicked ( ) ) , SLOT ( reject ( ) ) ) ;
  Lay -> addWidget ( pbOk   , R , 0 ) ;
  Lay -> addWidget ( pbCanc , R , 1 ) ;

  setLayout ( Lay ) ;

}// LOpts

int LOpts :: exec ( ) {
  int R  =  QDialog :: exec ( ) ;
  if ( R == QDialog :: Accepted ) {
    NeedSave = true ;
    MountCmdVal = MountCmdLine -> text  ( ) ;
    UnmntCmdVal = UnmntCmdLine -> text  ( ) ;
    EjectCmdVal = EjectCmdLine -> text  ( ) ;
    HideDevsVal = HideDevsLine -> text  ( ) ;
    UnlckCmdVal = UnlckCmdLine -> text  ( ) ;
    LockCmdVal  = LockCmdLine  -> text  ( ) ;
    MntNewVal   = MntNewBox    -> isChecked ( ) ;
    MntMediaVal = MntMediaBox  -> isChecked ( ) ;
    MntStartVal = MntStartBox  -> isChecked ( ) ;
  } else {
    MountCmdLine -> setText    ( MountCmdVal ) ;
    UnmntCmdLine -> setText    ( UnmntCmdVal ) ;
    EjectCmdLine -> setText    ( EjectCmdVal ) ;
    HideDevsLine -> setText    ( HideDevsVal ) ;
    UnlckCmdLine -> setText    ( UnlckCmdVal ) ;
    LockCmdLine  -> setText    ( LockCmdVal  ) ;
    MntNewBox    -> setChecked ( MntNewVal   ) ;
    MntMediaBox  -> setChecked ( MntMediaVal ) ;
    MntStartBox  -> setChecked ( MntStartVal ) ;
  }
  return R ;
}// LOpts :: Execute

LOpts :: ~LOpts ( ) {
  if ( NeedSave ) {
    Conf . setValue ( MountCmdItm , MountCmdVal ) ;
    Conf . setValue ( UnmntCmdItm , UnmntCmdVal ) ;
    Conf . setValue ( EjectCmdItm , EjectCmdVal ) ;
    Conf . setValue ( HideDevsItm , HideDevsVal ) ;
    Conf . setValue ( UnlckCmdItm , UnlckCmdVal ) ;
    Conf . setValue ( LockCmdItm  , LockCmdVal  ) ;
    Conf . setValue ( MntNewItm   , MntNewVal   ) ;
    Conf . setValue ( MntMediaItm , MntMediaVal ) ;
    Conf . setValue ( MntStartItm , MntStartVal ) ;
  }//fi
}// ~LOpts

QStringList LOpts :: HideDevs ( ) {
  return HideDevsVal . split ( ' ' , QString :: SkipEmptyParts ) ;
}// LOpts :: HideDevs

QString LOpts :: MountCmd ( ) { return MountCmdVal ; }// LOpts :: MountCmd

QString LOpts :: EjectCmd ( ) { return EjectCmdVal ; }// LOpts :: EjectCmd

QString LOpts :: UnmntCmd ( ) { return UnmntCmdVal ; }// LOpts :: UnmntCmd

QString LOpts :: LockCmd  ( ) { return LockCmdVal  ; }// LOpts :: LockCmd

QString LOpts :: UnlckCmd ( ) { return UnlckCmdVal ; }// LOpts :: UnlckCmd

bool    LOpts :: MntNew   ( ) { return MntNewVal   ; }// LOpts :: MntNew

bool    LOpts :: MntMedia ( ) { return MntMediaVal ; }// LOpts :: MntMedia

bool    LOpts :: MntStart ( ) { return MntStartVal ; }// LOpts :: MntStart

//eof LOpts.cpp
