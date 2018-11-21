
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2018.

#include <QTextStream>
#include <QSocketNotifier>

#include "QMounts.h"

static const char * const MInfoFileName = "/proc/self/mountinfo" ;

inline QString sect ( const QString & S , int K , QChar Sep = ' ' ) {
  return S . section ( Sep , K , K ) ;
}// sect

Mounts :: Mounts ( QObject * parent ) :
                   QObject ( parent ) , MInfoFile ( MInfoFileName ) {
  MInfoFile . open ( QFile :: ReadOnly ) ; RefreshMountInfo ( ) ;
  QSocketNotifier * Ntfr = new QSocketNotifier (
      MInfoFile . handle ( ) , QSocketNotifier :: Exception , this ) ;
  connect ( Ntfr , SIGNAL ( activated ( int ) ) ,
            this , SLOT   ( MntAct    ( int ) ) ) ;
}// Mounts

Mounts :: ~Mounts ( ) { MInfoFile . close ( ) ; }// ~Mounts

void Mounts :: MntAct ( int sock ) { ( void ) sock ; emit Changed ( ) ;
}// Mounts :: MntAct

QStringList Mounts :: MPoints ( const QString & DevNum ) const {
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
