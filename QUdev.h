
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2019.

//   The minimal wrapper for "objects" of "libudev" library.
// Implemented only really used in the "tmount" program functions.

#ifndef QUDEV_H
#define QUDEV_H

#include <QObject>
#include <QString>
#include <QPair>
#include <QList>

struct udev_device  ; struct udev_enumerate  ;
struct udev_monitor ; struct udev_list_entry ; struct udev ;
class Udev ;  class UdevMon  ; class UdevDev ; // for internal use

typedef struct QPair < QString , QString > UdevPair ;
typedef class  QList < UdevPair > UdevList ;
  // Replacement for "udev_list_entry".

typedef const char * const CPtr ;

class UdevEnum {
  public :
    explicit UdevEnum ( const Udev & Context ) ; virtual ~UdevEnum ( ) ;
    int MatchSubsys   ( CPtr Subsys ) ;
    int MatchProperty ( CPtr Prop , CPtr Val ) ;
    int MatchSysAttr  ( CPtr Prop , CPtr Val ) ;
    int MatchParent   ( const UdevDev & Root ) ;
    int ScanDevs  ( ) ;
      //   Functions above returns 0 on success,
      // otherwise a negative error value.
    UdevList GetList ( ) const ;
  private :
    udev_enumerate * mEnum ;
    explicit UdevEnum    ( const UdevEnum & En ) ;
    UdevEnum & operator= ( const UdevEnum & En ) ;
} ; // UdevEnum

class UdevDev {
  public :
    explicit UdevDev ( const Udev    & Context ,
                       const QString & SysPath ) ;
    explicit UdevDev ( const UdevMon & Monitor ) ;
    explicit UdevDev ( const UdevDev & Dev ) ;
    virtual ~UdevDev ( ) ;
    QString DevNode  ( ) const ; // isNull if no device node exists.
    QString SysPath  ( ) const ; // isNull if internal udev error only.
    QString Action   ( ) const ; // isNull if no action available.
    QString DevNum   ( ) const ; // MAJOR:MINOR
    QString DevType  ( ) const ; // disk, partition, etc.
    QString SubSys   ( ) const ;
    QString Property ( CPtr Key ) const ; // isNull if key not present.
    QString SysAttr  ( CPtr Key ) const ; // isNull if key not present.
    UdevDev & FindParent ( CPtr Subs , CPtr DType ) const ;
  friend int UdevEnum :: MatchParent ( const UdevDev & Root ) ;
  private :
    udev_device * mDev ;
    UdevDev & operator= ( const UdevDev & Dev ) ;
} ; // UdevDev

class UdevMon : public QObject { Q_OBJECT
  // Monitors "udev" events only, no "kernel".
  public :
    explicit UdevMon ( const Udev & Context , QObject * parent = nullptr ) ;
    virtual ~UdevMon ( ) ;
    int AddMatch ( CPtr Subs , CPtr DevType ) ;
    int EnableReceiving (  ) ;
      //   Functions abowe returns 0 on success,
      // otherwise a negative error value.
  friend UdevDev :: UdevDev ( const UdevMon & Monitor ) ;
  private :
    Q_DISABLE_COPY ( UdevMon )
    udev_monitor * mMon ;
  signals : void  Changed ( ) ;
  private slots : void DevAct ( int sock ) ;
} ; // UdevMon

class Udev {
  public : explicit Udev ( )  ; virtual ~Udev ( ) ;
  friend UdevMon  :: UdevMon  ( const Udev    & Context ,
                                      QObject * parent  ) ;
  friend UdevDev  :: UdevDev  ( const Udev    & Context ,
                                const QString & SysPath ) ;
  friend UdevEnum :: UdevEnum ( const Udev    & Context ) ;
  private :
    udev * mUdev ;
    explicit Udev    ( const Udev & Ud ) ;
    Udev & operator= ( const Udev & Ud ) ;
} ; // Udev

#endif // QUDEV_H

//eof QUdev.h
