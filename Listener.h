
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#ifndef LISTENER_H
#define LISTENER_H

#include <QMenu>

#include "LOpts.h"
#include "QUdev.h"
#include "QMounts.h"

//  Arbitrary values.
static const int StartTimeout = 2000 ; // 2 s for start of external program.
static const int ExecTimeout  = 5000 ; // 5 s for run the external program.
static const int NoTimeout    =   -1 ; // for interactive external program.

typedef QAction * ActPtr ;
typedef class QList < ActPtr  > ActList ;

class Listener : public QMenu { Q_OBJECT

  public :
    explicit Listener ( QWidget * parent = 0 ) ; virtual ~Listener ( ) ;
    ActPtr exec ( const QPoint & Loc , ActPtr At = 0 ) ;
    LOpts  Opt  ;

  private slots :
    void DeviceAction ( int socket  ) ;
    void MountAction  ( int socket  ) ;
    void About    ( ) ;

  private :

    void RemoveDevice   ( UdevDev & Dev ) ;
    bool AddDevice      ( UdevDev & Dev , bool TryMount ) ;
    void SetActions     ( UdevDev & Dev ) ; // Redraw menu items for device.
    ActList FindActs    ( const QString & Name ) ;
      // Find items for device in the devices menu.
    int  ExecCmd        ( const QString & Cmd ,
                          const QString & Arg ,
                            int Timeout = ExecTimeout  ) ;
    QStringList MPoints ( UdevDev & Dev ) ;

    QIcon UIcon , MIcon ;
    Udev  UdevContext ;
    UdevMon  * UMonitor ;
    MntMonitor MMonitor ;
    QStringList DevList ;
    QString     CurrDev ;
    Mounts MInfo ;

} ; // Listener

#endif // LISTENER_H

//eof Listener.h