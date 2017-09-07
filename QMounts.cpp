
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#include <QTextStream>

#include "QMounts.h"

static const char * const MInfoFileName = "/proc/self/mountinfo" ;

static QString sect ( const QString & S , int K ) {
  return S . section ( ' ' , K , K ) ;
}// sect

Mounts :: Mounts ( ) : MInfoFile ( MInfoFileName ) {
  MInfoFile . open ( QFile :: ReadOnly ) ; RefreshMountInfo ( ) ;
}// Mounts

Mounts :: ~Mounts ( ) { MInfoFile . close ( ) ; }// ~Mounts

QStringList Mounts :: MPoints ( const QString & DevNum ) {
  QStringList M ;
  foreach ( const QString L , MInfoTab ) {
    if ( sect ( L , 2 ) == DevNum ) { M << sect ( L , 4 ) ; }//fi
  }//done
  return M ;
}// Mounts :: MPoints

void Mounts :: RefreshMountInfo ( ) {
  MInfoFile . reset ( ) ;
  MInfoTab  = QTextStream ( & MInfoFile ) . readAll ( ) . split ( '\n' ) ;
}// Mounts :: RefreshMountInfo

int Mounts :: GetFD ( ) { return MInfoFile . handle ( ) ; }// Mounts :: GetFD

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

//eof QMounts.cpp
