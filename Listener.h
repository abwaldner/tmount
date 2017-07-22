
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#ifndef LISTENER_H
#define LISTENER_H

#include <QMenu>
#include <QContextMenuEvent>
#include <QProcessEnvironment>

#include "LOpts.h"
#include "QUdev.h"
#include "QMounts.h"

#define TPref ( qApp -> applicationName ( ) + " - " )

//  Arbitrary value.
static const int StartTimeout = 2000 ; // 2 s for start of external program.

typedef const char * const CPtr ;

typedef QAction * ActPtr ;
typedef class QList < ActPtr  > ActList ;

class Listener : public QMenu { Q_OBJECT

  public :

    explicit Listener ( QWidget * parent = 0 ) ; virtual ~Listener ( ) ;
    void exec ( const QPoint & Loc ) ;
    LOpts Opt ;

  protected : virtual void contextMenuEvent ( QContextMenuEvent * event ) ;

  private slots :

    void DeviceAction ( int socket ) ;
    void MountAction  ( int socket ) ;
    void AddImage ( ) ;

  private :

    void RemoveDevice     ( UdevDev & Dev ) ;
    bool AddDevice        ( UdevDev & Dev , bool TryMount ) ;
    void SetActions       ( UdevDev & Dev ) ; // Redraw menu items for device.
    ActList FindActs      ( const QString & Name ) ;
      // Find items for device in the devices menu.
    int  ExecCmd          ( const QString & Cmd ,
                            const QString & Arg , int Timeout ) ;
    QStringList MPoints   ( UdevDev & Dev ) ;
    QStringList MapDevs   ( UdevDev & Dev ) ;
    static bool Ejectable ( UdevDev & Dev ) ;
    static QString ToHum  ( qulonglong KB ) ;

    enum ActReq { reqNoAct , reqEject , reqRemove } ;

    QIcon MIcon , UIcon , EIcon , DIcon , LIcon ;
    Udev  UdevContext ;
    UdevMon  * UMonitor ;
    MntMonitor MMonitor ;
    QStringList DevList ;
    QString     CurrDev ;
    Mounts MInfo ;
    QProcessEnvironment Env ;
    ActReq Suppl ;

} ; // Listener

#endif // LISTENER_H

//eof Listener.h
