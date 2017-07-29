
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#include <QDir>

#include "QUdev.h"
#include <libudev.h>

Udev :: Udev  ( ) { mUdev = udev_new ( ) ; }// Udev

Udev :: ~Udev ( ) { udev_unref ( mUdev ) ; }// ~Udev

Udev :: Udev ( const Udev & Ud ) { mUdev = udev_ref ( Ud . mUdev ) ;
}// Udev ( const Udev & ) ;

Udev & Udev :: operator= ( const Udev & Ud ) {
  if ( this != & Ud ) {
    udev_unref ( mUdev ) ; mUdev = udev_ref ( Ud . mUdev ) ;
  }//fi
  return * this ;
}// Udev :: operator=

UdevMon :: UdevMon ( const Udev & Context ) {
  mMon = udev_monitor_new_from_netlink ( Context . mUdev , "udev" ) ;
}// UdevMon ( const Udev & )

UdevMon :: UdevMon ( const UdevMon & Mon ) {
  mMon = udev_monitor_ref ( Mon . mMon ) ;
}// UdevMon ( const UdevMon & )

UdevMon & UdevMon :: operator= ( const UdevMon & Mon ) {
  if ( this != & Mon ) {
    udev_monitor_unref ( mMon ) ; mMon = udev_monitor_ref ( Mon . mMon ) ;
  }//fi
  return * this ;
}// UdevMon :: operator=

UdevMon :: ~UdevMon ( ) { udev_monitor_unref ( mMon ) ; }// ~UdevMon

int UdevMon :: AddMatch ( CPtr Subs , CPtr DTp ) {
  return
    udev_monitor_filter_add_match_subsystem_devtype ( mMon , Subs , DTp ) ;
}// UdevMon :: addMatch

int UdevMon :: EnableReceiving ( ) {
  return mMon ? udev_monitor_enable_receiving ( mMon ) : -22 ;
    // this is workaround for udev bug (segmentation fault)
}// UdevMon :: EnableReceiving

int UdevMon :: GetFD ( ) { return udev_monitor_get_fd ( mMon ) ;
}// UdevMon :: GetFD

UdevDev :: UdevDev ( const Udev & Context , QString SysPath ) {
  mDev = udev_device_new_from_syspath (
           Context . mUdev , SysPath . toLatin1 ( ) ) ;
}// UdevDev ( const Udev & , QString )

UdevDev :: UdevDev ( const UdevMon & Monitor ) {
  mDev = udev_monitor_receive_device ( Monitor . mMon ) ;
}// UdevDev ( const UdevMon & )

UdevDev :: UdevDev ( const UdevDev & Dev ) {
  mDev = udev_device_ref ( Dev . mDev ) ;
}// UdevDev ( const UdevDev & )

UdevDev & UdevDev :: operator= ( const UdevDev & Dev ) {
  if ( this != & Dev ) {
    udev_device_unref ( mDev ) ; mDev = udev_device_ref ( Dev . mDev ) ;
  }//fi
  return * this ;
}// UdevDev :: operator=

UdevDev :: ~UdevDev ( ) { udev_device_unref ( mDev ) ; }// ~UdevDev

QString UdevDev :: SysPath ( ) {
  return QString ( udev_device_get_syspath ( mDev ) ) ;
}// UdevDev :: SysPath

QString UdevDev :: Action  ( ) {
  return QString ( udev_device_get_action  ( mDev ) ) ;
}// UdevDev :: Action

QString UdevDev :: DevNode ( ) {
  return QString ( udev_device_get_devnode ( mDev ) ) ;
}// UdevDev :: DevNode

QString UdevDev :: DevNum  ( ) {
  dev_t DN = udev_device_get_devnum ( mDev ) ;
  return QString ( "%1:%2" ) . arg ( major ( DN ) ) . arg ( minor ( DN ) ) ;
}// UdevDev :: DevNum

QString UdevDev :: DevType ( ) {
  return QString ( udev_device_get_devtype ( mDev ) ) ;
}// UdevDev :: DevType

QString UdevDev :: SubSys ( ) {
  return QString ( udev_device_get_subsystem ( mDev ) ) ;
}// UdevDev :: SubSys

QString UdevDev :: Property ( CPtr Key ) {
  return QString ( udev_device_get_property_value ( mDev , Key ) ) ;
}// UdevDev :: Property

QString UdevDev :: SysAttr  ( CPtr Key ) {
  return QString ( udev_device_get_sysattr_value  ( mDev , Key ) ) ;
}// UdevDev :: SysAttr

QStringList UdevDev :: Holders ( ) {
  return QDir ( SysPath ( ) + "/holders" ) .
           entryList ( QDir :: Dirs | QDir :: NoDotAndDotDot ) ;
}// UdevDev :: Holders

UdevDev & UdevDev :: FindParent ( CPtr Subs , CPtr DType ) {
  UdevDev * P = new UdevDev ( * this ) ;
  udev_device_unref ( P -> mDev ) ;
  P -> mDev = udev_device_ref (
    udev_device_get_parent_with_subsystem_devtype ( mDev , Subs , DType )
  ) ;
  return * P ;
}// UdevDev :: FindParent

UdevEnum :: UdevEnum ( const Udev & Context ) {
  mEnum = udev_enumerate_new ( Context . mUdev ) ;
}// UdevEnum ( const Udev & )

UdevEnum :: UdevEnum ( const UdevEnum & En ) {
  mEnum = udev_enumerate_ref ( En . mEnum ) ;
}// UdevEnum ( const UdevEnum & )

UdevEnum & UdevEnum :: operator= ( const UdevEnum & En ) {
  if ( this != & En ) {
    udev_enumerate_unref ( mEnum ) ;
    mEnum = udev_enumerate_ref ( En . mEnum ) ;
  }//fi
  return * this ;
}// UdevEnum :: operator=

UdevEnum :: ~UdevEnum ( ) { udev_enumerate_unref ( mEnum ) ; }// ~UdevEnum

int UdevEnum :: MatchSubsys ( CPtr Subsys ) {
  return udev_enumerate_add_match_subsystem ( mEnum , Subsys ) ;
}// UdevEnum :: MatchSubsys

int UdevEnum :: MatchProperty ( CPtr Prop , CPtr Val ) {
  return udev_enumerate_add_match_property ( mEnum , Prop , Val ) ;
}// UdevEnum :: MatchProperty

int UdevEnum :: MatchParent ( const UdevDev & Root ) {
  return udev_enumerate_add_match_parent ( mEnum , Root . mDev ) ;
}// UdevEnum :: MatchParent

int UdevEnum :: ScanDevs ( ) { return udev_enumerate_scan_devices ( mEnum ) ;
}// UdevEnum :: ScanDevs

UdevList UdevEnum :: GetList ( ) {
  UdevList L ;
  struct udev_list_entry * N = udev_enumerate_get_list_entry ( mEnum ) ;
  while ( N ) {
    L += UdevPair ( udev_list_entry_get_name  ( N ) ,
                    udev_list_entry_get_value ( N ) ) ;
    N = udev_list_entry_get_next ( N ) ;
  }//done
  return L ;
}// UdevEnum :: GetList

//eof QUdev.cpp
