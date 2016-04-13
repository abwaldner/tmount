
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QApplication>

#include "Listener.h"

static const char // ".conf" file items.
  * MC = "Mount_command"  , * UC = "Unmount_command" , * EC = "Eject_command" ,
  * HD = "Hidden_devices" , * MN = "Mount_new"       , * MM = "Mount_media"   ,
  * MS = "Mount_on_start" ;

LOpts :: LOpts ( QWidget * parent ) : QDialog ( parent ) {

  // Default values.
  QString MCdef = "udevil mount"  , ECdef = "eject" ,
          UCdef = "udevil umount" , HDdef = "" ;
  bool MNdef = false , MMdef = false , MSdef = false ;

  QString // Labels.
    MCl = tr ( "Mount &command" ) , UCl = tr ( "&Unmount command" ) ,
    ECl = tr ( "&Eject command" ) , HDl = tr ( "&Hidden devices"  ) ,
    MNl = tr ( "Mount &new"     ) , MMl = tr ( "Mount &media"     ) ,
    MSl = tr ( "Mount on &start"  ) ;

  QString // Tooltips.
    MCt = tr ( "The device node name (/dev/...) will be<br/>"
               "appended to the end of given command line." ) ,
    UCt = tr ( "The mountpoint pathname (/media/...) will be<br/>"
               "appended to the end of given command line." ) ,
    ECt = MCt ,
    HDt = tr ( "Type a space-separated list of devices, b.e.<br/>"
               "'/dev/sda3 /dev/sdb1' (restart required)."  ) ;

  setWindowTitle ( qApp -> applicationName ( ) + tr ( " - Settings" ) ) ;
  setWindowIcon  ( QIcon ( ":/icons/config.png" ) ) ;

  bool NS = false ;

  if ( ! Conf . contains ( MC ) ) { NS = true ; MCData = MCdef ;
  } else { MCData = Conf . value ( MC ) . toString ( ) ;
  }//fi
  if ( ! Conf . contains ( UC ) ) { NS = true ; UCData = UCdef ;
  } else { UCData = Conf . value ( UC ) . toString ( ) ;
  }//fi
  if ( ! Conf . contains ( EC ) ) { NS = true ; ECData = ECdef ;
  } else { ECData = Conf . value ( EC ) . toString ( ) ;
  }//fi
  if ( ! Conf . contains ( HD ) ) { NS = true ; HDData = HDdef ;
  } else { HDData = Conf . value ( HD ) . toString ( ) ;
  }//fi
  if ( ! Conf . contains ( MN ) ) { NS = true ; MNData = MNdef ;
  } else { MNData = Conf . value ( MN ) . toBool ( ) ;
  }//fi
  if ( ! Conf . contains ( MM ) ) { NS = true ; MMData = MMdef ;
  } else { MMData = Conf . value ( MM ) . toBool ( ) ;
  }//fi
  if ( ! Conf . contains ( MS ) ) { NS = true ; MSData = MSdef ;
  } else { MSData = Conf . value ( MS ) . toBool ( ) ;
  }//fi

  NeedSave = NS ;

  QGridLayout * Lay = new QGridLayout ; QLabel * Lbl ; int R = 0 ;

  MCLine = new QLineEdit ( MCData ) ; ECLine = new QLineEdit ( ECData ) ;
  UCLine = new QLineEdit ( UCData ) ; HDLine = new QLineEdit ( HDData ) ;
  MNBox  = new QCheckBox ( MNl  ) ; MNBox -> setChecked ( MNData ) ;
  MMBox  = new QCheckBox ( MMl  ) ; MMBox -> setChecked ( MMData ) ;
  MSBox  = new QCheckBox ( MSl  ) ; MSBox -> setChecked ( MSData ) ;
  Lbl = new QLabel  ( MCl ) ; Lbl -> setBuddy ( MCLine ) ;
  Lbl -> setToolTip ( MCt ) ; MCLine -> setToolTip ( MCt ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ; Lay -> addWidget ( MCLine  , R ++ , 1 ) ;
  Lbl = new QLabel  ( UCl ) ; Lbl -> setBuddy ( UCLine ) ;
  Lbl -> setToolTip ( UCt ) ; UCLine -> setToolTip ( UCt ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ; Lay -> addWidget ( UCLine  , R ++ , 1 ) ;
  Lbl = new QLabel  ( ECl ) ; Lbl -> setBuddy ( ECLine ) ;
  Lbl -> setToolTip ( ECt ) ; ECLine -> setToolTip ( ECt ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ; Lay -> addWidget ( ECLine  , R ++ , 1 ) ;
  Lbl = new QLabel  ( HDl ) ; Lbl -> setBuddy ( HDLine ) ;
  Lbl -> setToolTip ( HDt ) ; HDLine -> setToolTip ( HDt ) ;
  Lay -> addWidget  ( Lbl , R , 0 ) ; Lay -> addWidget ( HDLine  , R ++ , 1 ) ;
  Lay -> addWidget  ( MNBox , R    , 0 ) ;
  Lay -> addWidget  ( MMBox , R ++ , 1 ) ;
  Lay -> addWidget  ( MSBox , R ++ , 0 ) ;

  QPushButton * pbOk   = new QPushButton ( tr ( "&Ok"     ) ) ;
  QPushButton * pbCanc = new QPushButton ( tr ( "&Cancel" ) ) ;
  connect ( pbOk   , SIGNAL ( clicked ( ) ) , SLOT ( accept ( ) ) ) ;
  connect ( pbCanc , SIGNAL ( clicked ( ) ) , SLOT ( reject ( ) ) ) ;
  Lay -> addWidget ( pbOk , R , 0 ) ; Lay -> addWidget ( pbCanc , R , 1 ) ;

  setLayout ( Lay ) ;

}// LOpts

int LOpts :: exec ( ) {
  int R  =  QDialog :: exec ( ) ;
  if ( R == QDialog :: Accepted ) {
    NeedSave = true ;
    MCData = MCLine -> text  ( ) ; UCData = UCLine -> text ( ) ;
    ECData = ECLine -> text  ( ) ; HDData = HDLine -> text ( ) ;
    MNData = MNBox  -> isChecked ( ) ; MMData = MMBox -> isChecked ( ) ;
    MSData = MSBox  -> isChecked ( ) ;
  } else {
    MCLine -> setText   ( MCData ) ; UCLine -> setText  ( UCData ) ;
    ECLine -> setText   ( ECData ) ; HDLine -> setText  ( HDData ) ;
    MNBox -> setChecked ( MNData ) ; MMBox  -> setChecked ( MMData ) ;
    MSBox -> setChecked ( MSData ) ;
  }
  return R ;
}// LOpts :: Execute

LOpts :: ~LOpts ( ) {
  if ( NeedSave ) {
    Conf . setValue ( MC , MCData ) ; Conf . setValue ( UC , UCData ) ;
    Conf . setValue ( EC , ECData ) ; Conf . setValue ( HD , HDData ) ;
    Conf . setValue ( MN , MNData ) ; Conf . setValue ( MM , MMData ) ;
    Conf . setValue ( MS , MSData ) ;
  }//fi
}// ~LOpts

QString LOpts :: HideDevs   ( ) { return HDData ; }// LOpts :: HideDevs

QString LOpts :: MountCmd   ( ) { return MCData ; }// LOpts :: MountCmd

QString LOpts :: EjectCmd   ( ) { return ECData ; }// LOpts :: EjectCmd

QString LOpts :: UnmountCmd ( ) { return UCData ; }// LOpts :: UnmountCmd

bool    LOpts :: MountNew   ( ) { return MNData ; }// LOpts :: MountNew

bool    LOpts :: MountMedia ( ) { return MMData ; }// LOpts :: MountMedia

bool    LOpts :: MountStart ( ) { return MSData ; }// LOpts :: MountStart

//eof LOpts.cpp
