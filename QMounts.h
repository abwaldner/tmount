
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2022.

#ifndef QMOUNTS_H
#define QMOUNTS_H

#include <QStringList>
#include <QFile>

struct StatVFS {
  qulonglong TotalSize , FreeSize , AvailSize ; // in bytes.
  ulong BlockSize  , NameMax   ;
  ulong TotalFiles , FreeFiles , AvailFiles ;
  int  ErrNo ;
  bool MandLock , NoATime , NoDev  , NoDirATm , NoExec ,
       NoSUId   , RdOnly  , RelATm , Sync ;
} ; // StatVFS

class Mounts : public QObject { Q_OBJECT
  public :
    explicit Mounts ( QObject * parent = nullptr ) ; virtual ~Mounts ( ) ;
    QStringList MPoints ( const QString & DevNum ) const ; // "maj:min"
      // isEmpty if device not mounted.
    void RefreshMountInfo ( ) ;
    static QString DecodeIFS ( const QString & S ) ; // man getmntent(3)
    static QString EncodeIFS ( const QString & S ) ;
    static StatVFS GetStatFS ( const QString & F ) ;
  private :
    Q_DISABLE_COPY ( Mounts )
    QStringList MInfoTab ; QFile MInfoFile ;
  signals : void  Changed ( ) ;
  private slots : void MntAct ( int sock ) ;
} ; // Mounts

#endif // QMOUNTS_H

//eof QMounts.h
