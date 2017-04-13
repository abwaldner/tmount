
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#ifndef LOPTS_H
#define LOPTS_H

#include <QSettings>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

class LOpts : public QDialog { Q_OBJECT
  public slots :
    int exec ( ) ;
  public :
    explicit LOpts ( QWidget * parent = 0 ) ; virtual ~LOpts ( ) ;
    QStringList HideDevs ( ) ;
    QString MountCmd ( ) ;
    QString UnmntCmd ( ) ;
    QString EjectCmd ( ) ;
    QString LockCmd  ( ) ;
    QString UnlckCmd ( ) ;
    bool MntNew    ( ) ;
    bool MntMedia  ( ) ;
    bool MntStart  ( ) ;
  private :
    QSettings Conf ;
    bool  NeedSave ;
    QString MountCmdVal , UnmntCmdVal , EjectCmdVal ,
            HideDevsVal , UnlckCmdVal , LockCmdVal  ;
    bool    MntNewVal   , MntMediaVal , MntStartVal ;
    QLineEdit * MountCmdLine , * UnmntCmdLine , * EjectCmdLine ,
              * HideDevsLine , * UnlckCmdLine , * LockCmdLine  ;
    QCheckBox * MntNewBox    , * MntMediaBox  , * MntStartBox  ;
} ; // LOpts

#endif // LOPTS_H

//eof LOpts.h
