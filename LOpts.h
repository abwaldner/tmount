
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#ifndef LOPTS_H
#define LOPTS_H

#include <QSettings>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>

class LOpts : public QDialog { Q_OBJECT
  public slots : int exec ( ) ;
  public :
    explicit LOpts ( QWidget * parent = 0 ) ; virtual ~LOpts ( ) ;
    QString MountCmd ( ) , UnmntCmd ( ) , EjectCmd ( ) ,
            UnlckCmd ( ) , LockCmd  ( ) , AddImCmd ( ) ;
    int MountTO ( ) , UnmntTO ( ) , EjectTO ( ) ,
        UnlckTO ( ) , LockTO  ( ) , AddImTO ( ) ;
    QStringList HideDevs ( ) ;
    bool MntNew ( ) , MntMedia ( ) , MntStart ( ) ;
    QString MountIcon ( ) , UnmntIcon ( ) , LockIcon ( ) ,
            AddImIcon ( ) , UnlckIcon ( ) , ExitIcon ( ) ,
            EjectIcon ( ) , AboutIcon ( ) , ConfIcon ( ) , TMntIcon ( ) ;
  private :
    QSettings Conf ; bool  NeedSave ;
    QString MountCmdVal , UnmntCmdVal , EjectCmdVal ,
            UnlckCmdVal , LockCmdVal  , AddImCmdVal , HideDevsVal ;
    int     MountTOVal  , UnmntTOVal  , EjectTOVal  ,
            UnlckTOVal  , LockTOVal   , AddImTOVal  ;
    bool    MntNewVal   , MntMediaVal , MntStartVal ;
    QLineEdit * MountCmdLine , * UnmntCmdLine , * EjectCmdLine ,
              * UnlckCmdLine , * LockCmdLine  , * AddImCmdLine ,
              * HideDevsLine ;
    QSpinBox  * MountTOSpin  , * UnmntTOSpin  , * EjectTOSpin  ,
              * UnlckTOSpin  , * LockTOSpin   , * AddImTOSpin  ;
    QCheckBox * MntNewBox    , * MntMediaBox  , * MntStartBox  ;
    static int  ms ( int Sec ) ; // convert secs to ms or NoTimeout
} ; // LOpts

#endif // LOPTS_H

//eof LOpts.h
