
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#include <QDir>

#include "QUdev.h"
#include <libudev.h>

Udev :: Udev  ( ) { mUdev = udev_new ( ) ; }// Udev

Udev :: ~Udev ( ) { udev_unref ( mUdev ) ; }// ~Udev

UdevMon :: UdevMon ( const Udev * Context ) {
  mMon = udev_monitor_new_from_netlink ( Context -> mUdev , "udev" ) ;
}// UdevMon ( const Udev * )

UdevMon :: ~UdevMon ( ) { udev_monitor_unref ( mMon ) ; }// ~UdevMon

int UdevMon :: AddMatch ( const char * subs , const char * devt ) {
  return
    udev_monitor_filter_add_match_subsystem_devtype ( mMon , subs , devt ) ;
}// UdevMon :: addMatch

int UdevMon :: EnableReceiving ( ) {
  return mMon ? udev_monitor_enable_receiving ( mMon ) : -22 ;
    // this is workaround for udev bug (segmentation fault)
}// UdevMon :: EnableReceiving

int UdevMon :: GetFD ( ) { return udev_monitor_get_fd ( mMon ) ; }// GetFD

UdevDev :: UdevDev ( const Udev * Context , QString SysPath ) {
  mDev = udev_device_new_from_syspath (
           Context -> mUdev , SysPath . toLatin1 ( ) ) ;
}// UdevDev ( const Udev * , QString )

UdevDev :: UdevDev ( const UdevMon * Monitor ) {
  mDev = udev_monitor_receive_device ( Monitor -> mMon ) ;
}// UdevDev ( UdevMon * )

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

QString UdevDev :: DevNum ( ) {
  dev_t DN = udev_device_get_devnum ( mDev ) ;
  return QString ( "%1:%2" ) . arg ( major ( DN ) ) . arg ( minor ( DN ) ) ;
}// UdevDev :: DevNum

QString UdevDev :: Property ( const char * Key  ) {
  return QString ( udev_device_get_property_value ( mDev , Key ) ) ;
}// UdevDev :: Property

QString UdevDev :: SysAttr  ( const char * Key  ) {
  return QString ( udev_device_get_sysattr_value  ( mDev , Key ) ) ;
}// UdevDev :: SysAttr

QStringList UdevDev :: Holders ( ) {
  return QDir ( SysPath ( ) + "/holders" ) .
           entryList ( QDir :: Dirs | QDir :: NoDotAndDotDot ) ;
}// UdevDev :: Holders

UdevEnum :: UdevEnum ( const Udev * Context ) {
  mEnum = udev_enumerate_new ( Context -> mUdev ) ;
}// UdevEnum ( const Udev * )

UdevEnum :: ~UdevEnum ( ) { udev_enumerate_unref ( mEnum ) ; }// ~UdevEnum

int UdevEnum :: MatchSubsys ( const char * Subsys ) {
  return udev_enumerate_add_match_subsystem ( mEnum , Subsys ) ;
}// UdevEnum :: MatchSubsys

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
