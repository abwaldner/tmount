
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#ifndef QMOUNTS_H
#define QMOUNTS_H

#include <QStringList>
#include <QFile>

class Mounts : public QObject { Q_OBJECT
  public :
    explicit Mounts ( QObject * parent = 0 ) ; virtual ~Mounts ( ) ;
    QStringList MPoints ( const QString & DevNum ) const ; // "maj:min"
      // isEmpty if device not mounted.
    void RefreshMountInfo ( ) ;
    static QString DecodeIFS ( const QString & S ) ; // man getmntent(3)
    static QString EncodeIFS ( const QString & S ) ;
  private :
    Q_DISABLE_COPY ( Mounts )
    QStringList MInfoTab ; QFile MInfoFile ;
  signals : void  Changed ( ) ;
  private slots : void MntAct ( int sock ) ;
} ; // Mounts

#endif // QMOUNTS_H

//eof QMounts.h
