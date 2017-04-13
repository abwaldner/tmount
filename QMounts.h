
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#ifndef QMOUNTS_H
#define QMOUNTS_H

#include <QString>
#include <QStringList>
#include <QFile>

class Mounts {
  public :
    explicit Mounts ( ) ; virtual ~Mounts ( ) ;
    QStringList MPoints ( const QString & DevNum ) ;
      // isEmpty if device not mounted.
    static QString DecodeIFS ( const QString & S ) ; // man getmntent(3)
    static QString EncodeIFS ( const QString & S ) ;
    void RefreshMountInfo  ( ) ;
  private :
    QStringList MInfoTab ;
    QFile MInfoFile ;
} ; // Mounts

class MntMonitor {
  public :
    explicit MntMonitor ( ) ; virtual ~MntMonitor ( ) ;
    int GetFD ( ) ;
  private :
    QFile MInfoFile ;
} ; // MntMonitor

#endif // QMOUNTS_H

//eof QMounts.h
