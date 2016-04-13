
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
    QString HideDevs   ( ) ;
    QString MountCmd   ( ) ;
    QString UnmountCmd ( ) ;
    QString EjectCmd   ( ) ;
    bool    MountNew   ( ) ;
    bool    MountMedia ( ) ;
    bool    MountStart ( ) ;
  private :
    QSettings Conf ;
    bool  NeedSave ;
    QString MCData , UCData , ECData , HDData ;
    bool    MNData , MMData , MSData ;
    QLineEdit * MCLine ,  * UCLine , * ECLine , * HDLine ;
    QCheckBox * MNBox  ,  * MMBox  , * MSBox  ;
} ; // LOpts

#endif // LOPTS_H

//eof LOpts.h
