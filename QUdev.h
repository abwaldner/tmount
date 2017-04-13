
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

//   The minimal wrapper for "objects" of "libudev" library.
// Implemented only really used in the "tmount" program functions.

#ifndef QUDEV_H
#define QUDEV_H

#include <QString>
#include <QPair>
#include <QList>

struct udev_device  ; struct udev_enumerate  ;
struct udev_monitor ; struct udev_list_entry ;
struct udev ; class Udev ; class UdevMon ; // for internal use

typedef struct QPair < QString , QString > UdevPair ;
typedef class  QList < UdevPair > UdevList ;
  // Replacement for "udev_list_entry".

class UdevDev {
  public  :
    explicit UdevDev ( const Udev    * Context , QString SysPath ) ;
    explicit UdevDev ( const UdevMon * Monitor ) ;
    virtual ~UdevDev ( ) ;
    QString DevNode  ( ) ; // isNull if no device node exists.
    QString SysPath  ( ) ; // isNull if internal udev error only.
    QString Action   ( ) ; // isNull if no action available.
    QString DevNum   ( ) ; // MAJOR:MINOR
    QString Property ( const char * Key ) ; // isNull if key not present.
    QString SysAttr  ( const char * Key ) ; // isNull if key not present.
    QStringList Holders ( ) ;
  private : struct udev_device * mDev ;
} ; // UdevDev

class UdevMon { // "udev" events only, no "kernel".
  public  :
    explicit UdevMon ( const Udev * Context ) ; virtual ~UdevMon ( ) ;
    int  AddMatch ( const char * subs , const char * devt ) ;
    int  EnableReceiving ( ) ;
      //   Functions abowe returns 0 on success,
      // otherwise a negative error value.
    int GetFD ( ) ;
  friend UdevDev :: UdevDev ( const UdevMon * Monitor ) ;
  private : struct udev_monitor * mMon ;
} ; // UdevMon

class UdevEnum {
  public :
    explicit UdevEnum ( const Udev * Context ) ; virtual ~UdevEnum ( ) ;
    int MatchSubsys   ( const char * Subsys  ) , ScanDevs  ( ) ;
      //   Functions above returns 0 on success,
      // otherwise a negative error value.
    UdevList GetList ( ) ;
  private : struct udev_enumerate * mEnum ;
} ; // UdevEnum

class Udev {
  public  : explicit Udev ( ) ; virtual ~Udev ( ) ;
  friend UdevMon  :: UdevMon  ( const Udev * Context ) ;
  friend UdevDev  :: UdevDev  ( const Udev * Context , QString SysPath ) ;
  friend UdevEnum :: UdevEnum ( const Udev * Context ) ;
  private : struct udev * mUdev ;
} ; // Udev

#endif // QUDEV_H

//eof QUdev.h
