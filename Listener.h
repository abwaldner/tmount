
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2022.

#ifndef LISTENER_H
#define LISTENER_H

#include <QMenu>
#include <QContextMenuEvent>
#include <QProcessEnvironment>

#include "LOpts.h"
#include "QUdev.h"
#include "QMounts.h"

typedef const char * const CPtr ;

typedef QAction * ActPtr ;
typedef class QList < ActPtr > ActList ;

class Listener : public QMenu { Q_OBJECT

  public :

    explicit Listener ( QWidget * parent = nullptr ) ; virtual ~Listener ( ) ;
    LOpts Opt ;
    void exec ( const QPoint & Loc ) ;

  protected : virtual void contextMenuEvent ( QContextMenuEvent * event ) ;

  private slots :

    void DeviceAction ( ) ; // Handle udev events.
    void MountAction  ( ) ; // Handle mount/unmount events.
    void AddImage     ( ) ;
    void OptChanged   ( ) ;

  private :

    void RemoveDevice      ( const UdevDev & Dev ) ;
    int  UnmntAll          ( const UdevDev & Dev , bool Show ) ;
      // Attempt to unmount all mountpoints for device.
    void SetupDevice       ( const UdevDev & Dev ,
                                   bool TryMount , bool Show ) ;
    void SetActions        ( const UdevDev & Dev ) ; // Reset menu items for.
    bool isTarget          ( const UdevDev & Dev ) const ;
      // Should be visible.
    ActList FindActs       ( const QString & Key ) const ;
      // Find items for device in the devices menu.
    int  ExecCmd           ( const QString & Cmd ,
                             const QString & Arg ,
                             int   Timeout , bool Show ) ;
    void RefreshMInfo      ( ) ;
    QStringList MPoints    ( const UdevDev & Dev ) const ; // Mountpoints for.
    QStringList Parts      ( const UdevDev & Dev ) const ; // Partitions of.
    QStringList Holders    ( const UdevDev & Dev ) const ; // from sysfs.
    QStringList DM_Maps    ( const UdevDev & Dev ) const ; // Maps for cont.
    QStringList Slaves     ( const UdevDev & Dev ) const ; // from sysfs.
    static bool Ejectable  ( const UdevDev & Dev ) ;
    static bool hasFS      ( const UdevDev & Dev ) ; // Filesystem recognized.
    static bool isLUKS     ( const UdevDev & Dev ) ; // It's container.
    static bool isPart     ( const UdevDev & Dev ) ; // It's partition.
    static UdevDev & WDisk ( const UdevDev & Dev ) ; // Whole disk for dev.
    static QString ToHum   ( qulonglong KB ) ; // From KiB to human readable.

    enum ActReq { reqNoAct , reqEject , reqRemove , reqUnmtAll } ;

    Udev UdevContext ; UdevMon UMonitor ;
      // Don't reorder, the monitor constructs from context.
    QRegExp HDevs , FDevs ; bool Verb ;
    Mounts  MInfo ;
    QString CurrDev ; ActReq Suppl ;
    QProcessEnvironment Env ;
    QIcon
      MIcon , UIcon , EIcon , RIcon , DIcon , LIcon , FIcon , SIcon , TIcon ;
    QString TPref ; // Window title prefix from Opt.

} ; // Listener

#endif // LISTENER_H

//eof Listener.h
