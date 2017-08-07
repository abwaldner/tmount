
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

//   The minimal wrapper for "objects" of "libudev" library.
// Implemented only really used in the "tmount" program functions.

#ifndef QUDEV_H
#define QUDEV_H

#include <QString>
#include <QPair>
#include <QList>

struct udev_device  ; struct udev_enumerate  ;
struct udev_monitor ; struct udev_list_entry ; struct udev ;
class Udev ;  class UdevMon ; class UdevDev ; // for internal use

typedef struct QPair < QString , QString > UdevPair ;
typedef class  QList < UdevPair > UdevList ;
  // Replacement for "udev_list_entry".

typedef const char * const CPtr ;

class UdevEnum {
  public :
    explicit UdevEnum ( const Udev & Context ) ; virtual ~UdevEnum ( ) ;
    int MatchSubsys   ( CPtr Subsys ) ;
    int MatchProperty ( CPtr Prop , CPtr Val ) ;
    int MatchParent   ( const UdevDev & Root ) ;
    int ScanDevs  ( ) ;
      //   Functions above returns 0 on success,
      // otherwise a negative error value.
    UdevList GetList ( ) ;
  private :
    udev_enumerate * mEnum ;
    explicit UdevEnum    ( const UdevEnum & En ) ;
    UdevEnum & operator= ( const UdevEnum & En ) ;
} ; // UdevEnum

class UdevDev {
  public :
    explicit UdevDev ( const Udev    & Context , QString SysPath ) ;
    explicit UdevDev ( const UdevMon & Monitor ) ;
    explicit UdevDev ( const UdevDev & Dev ) ;
    virtual ~UdevDev ( ) ;
    QString DevNode  ( ) ; // isNull if no device node exists.
    QString SysPath  ( ) ; // isNull if internal udev error only.
    QString Action   ( ) ; // isNull if no action available.
    QString DevNum   ( ) ; // MAJOR:MINOR
    QString DevType  ( ) ;
    QString SubSys   ( ) ;
    QString Property ( CPtr Key ) ; // isNull if key not present.
    QString SysAttr  ( CPtr Key ) ; // isNull if key not present.
    UdevDev & FindParent ( CPtr Subs , CPtr DType ) ;
  friend int UdevEnum :: MatchParent ( const UdevDev & Root ) ;
  private :
    udev_device * mDev ;
    UdevDev & operator= ( const UdevDev & Dev ) ;
} ; // UdevDev

class UdevMon { // "udev" events only, no "kernel".
  public :
    explicit UdevMon ( const Udev & Context ) ; virtual ~UdevMon ( ) ;
    int AddMatch ( CPtr Subs , CPtr DTp ) ;
    int EnableReceiving (  ) ;
      //   Functions abowe returns 0 on success,
      // otherwise a negative error value.
    int GetFD ( ) ;
  friend UdevDev :: UdevDev ( const UdevMon & Monitor ) ;
  private :
    udev_monitor * mMon ;
    explicit UdevMon    ( const UdevMon & Mon ) ;
    UdevMon & operator= ( const UdevMon & Mon ) ;
} ; // UdevMon

class Udev {
  public :
    explicit Udev ( ) ; virtual ~Udev ( ) ;
  friend UdevMon  :: UdevMon  ( const Udev & Context ) ;
  friend UdevDev  :: UdevDev  ( const Udev & Context , QString SysPath ) ;
  friend UdevEnum :: UdevEnum ( const Udev & Context ) ;
  private :
    udev * mUdev ;
    explicit Udev    ( const Udev & Ud ) ;
    Udev & operator= ( const Udev & Ud ) ;
} ; // Udev

#endif // QUDEV_H

//eof QUdev.h
