
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#include <QTextCodec>

#include "QMounts.h"

static const char * MInfoFileName = "/proc/self/mountinfo" ;

Mounts :: Mounts ( ) {
  MInfoFile . setFileName ( MInfoFileName ) ; RefreshMountInfo ( ) ;
}// Mounts

Mounts :: ~Mounts ( ) { }// ~Mounts

QStringList Mounts :: MPoints ( const QString & DevNode ) {
  QString E = EncodeIFS ( DevNode ) ; // Overcaution.
  QStringList M ;
  foreach ( QString L , MInfoTab ) {
    if ( L . section ( ' ' , -2 , -2 ) == E ) {
      M += L . section ( ' ' , 4 , 4 ) ;
    }//fi
  }//done
  return M ;
}// Mounts :: MPoints

QString Mounts :: EncodeIFS ( const QString & S ) {
  return QString ( S ) . // backslash should be escaped first.
           replace ( "\\" , "\\134" ) . replace ( " "  , "\\040" ) .
           replace ( "\t" , "\\011" ) . replace ( "\n" , "\\012" ) ;
}// Mounts :: EncodeIFS

QString Mounts :: DecodeIFS ( const QString & S ) {
  return QString ( S ) . // backslash should be unescaped last.
           replace ( "\\011" , "\t" ) . replace ( "\\040" , " "  ) .
           replace ( "\\012" , "\n" ) . replace ( "\\134" , "\\" ) ;
}// Mounts :: DecodeIFS

void Mounts :: RefreshMountInfo ( ) {
  MInfoFile . open  ( QFile :: ReadOnly ) ;
  // QIODevice could not read the files from the procfs line-by-line.
  MInfoTab = QTextCodec :: codecForLocale () ->
               toUnicode (  MInfoFile . readAll ( ) ) .
                              split ( '\n' , QString :: SkipEmptyParts ) ;
  MInfoFile . close ( ) ;
}// Mounts :: RefreshMITab

MntMonitor :: MntMonitor ( ) {
  MInfoFile . setFileName  ( MInfoFileName ) ;
  MInfoFile . open ( QFile :: ReadOnly ) ;
}// MntMonitor

MntMonitor :: ~MntMonitor ( ) { MInfoFile . close ( ) ; }// ~MntMonitor

int MntMonitor :: GetFD ( ) { return MInfoFile . handle ( ) ; }// GetFD

//eof QMounts.cpp
