
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#include <QTextStream>

#include "QMounts.h"

static const char * const MInfoFileName = "/proc/self/mountinfo" ;

static QString sect ( const QString & S , int K ) {
  return S . section ( ' ' , K , K ) ;
}// sect

Mounts :: Mounts ( ) {
  MInfoFile . setFileName ( MInfoFileName ) ; RefreshMountInfo ( ) ;
}// Mounts

Mounts :: ~Mounts ( ) { }// ~Mounts

QStringList Mounts :: MPoints ( const QString & DevNum ) {
  QStringList M ;
  foreach ( const QString L , MInfoTab ) {
    if ( sect ( L , 2 ) == DevNum ) { M << sect ( L , 4 ) ; }//fi
  }//done
  return M ;
}// Mounts :: MPoints

QString Mounts :: DecodeIFS ( const QString & S ) {
  return QString ( S ) . // backslash should be unescaped last.
           replace ( "\\011" , "\t" ) . replace ( "\\040" , " "  ) .
           replace ( "\\012" , "\n" ) . replace ( "\\134" , "\\" ) ;
}// Mounts :: DecodeIFS

QString Mounts :: EncodeIFS ( const QString & S ) {
  return QString ( S ) . // backslash should be escaped first.
           replace ( "\\" , "\\134" ) . replace ( " "  , "\\040" ) .
           replace ( "\t" , "\\011" ) . replace ( "\n" , "\\012" ) ;
}// Mounts :: EncodeIFS

void Mounts :: RefreshMountInfo ( ) {
  MInfoFile . open  ( QFile :: ReadOnly ) ;
  MInfoTab = QTextStream  ( & MInfoFile ) . readAll ( ) . split ( '\n' ) ;
  MInfoFile . close ( ) ;
}// Mounts :: RefreshMountInfo

MntMonitor :: MntMonitor ( ) {
  MInfoFile . setFileName  ( MInfoFileName ) ;
  MInfoFile . open ( QFile :: ReadOnly ) ;
}// MntMonitor

MntMonitor :: ~MntMonitor ( ) { MInfoFile . close ( ) ; }// ~MntMonitor

int MntMonitor :: GetFD ( ) { return MInfoFile . handle ( ) ; }// GetFD

//eof QMounts.cpp
