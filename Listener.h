
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
static const int StartTO = 2000 ; // 2 s for start of external program.

typedef const char * const CPtr ;

typedef QAction * ActPtr ;
typedef class QList < ActPtr  > ActList ;

class Listener : public QMenu { Q_OBJECT

  public :

    explicit Listener ( QWidget * parent = 0 ) ; virtual ~Listener ( ) ;
    LOpts Opt ;
    QStringList DevList ( ) ;
    void exec ( const QPoint & Loc ) ;

  protected : virtual void contextMenuEvent ( QContextMenuEvent * event ) ;

  private slots :

    void DeviceAction ( int socket ) ; // Handle udev events.
    void MountAction  ( int socket ) ; // Handle mount/unmount events.
    void AddImage ( ) ;

  private :

    void RemoveDevice      ( UdevDev & Dev ) ;
    bool AddDevice         ( UdevDev & Dev , bool TryMount , bool Show ) ;
    void SetActions        ( UdevDev & Dev ) ; // Reset menu items for device.
    ActList FindActs       ( const QString & Name ) ;
      // Find items for device in the devices menu.
    int  ExecCmd           ( const QString & Cmd ,
                             const QString & Arg ,
                             int   Timeout , bool Show ) ;
    QStringList MPoints    ( UdevDev & Dev ) ; // Mountpoints for dev.
    QStringList Parts      ( UdevDev & Dev ) ; // Partitions of dev.
    QStringList Holders    ( UdevDev & Dev ) ;
    QStringList DM_Maps    ( UdevDev & Dev ) ; // Maps for container.
    static bool Ejectable  ( UdevDev & Dev ) ;
    static bool isLUKS     ( UdevDev & Dev ) ; // It's container.
    static bool isPart     ( UdevDev & Dev ) ; // It's partition.
    static UdevDev & WDisk ( UdevDev & Dev ) ; // Whole disk for dev.
    static QString ToHum   ( qulonglong KB ) ; // From KiB to human readable.

    enum ActReq { reqNoAct , reqEject , reqRemove } ;

    QIcon MIcon , UIcon , EIcon , RIcon , DIcon , LIcon , TIcon ;
    Udev  UdevContext ;
    UdevMon  * UMonitor ;
    MntMonitor MMonitor ;
    QString CurrDev ;
    Mounts  MInfo ;
    QProcessEnvironment Env ;
    ActReq  Suppl ;

} ; // Listener

#endif // LISTENER_H

//eof Listener.h
