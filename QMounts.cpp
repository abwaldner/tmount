
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2019.

#include <QTextStream>
#include <QSocketNotifier>
#include <sys/statvfs.h>

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

StatVFS Mounts :: GetStatFS ( const QString & F ) {
  struct statvfs B ;
  StatVFS S = {
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    false , false , false , false , false , false , false , false , false
  } ;
  if ( ! :: statvfs ( QFile :: encodeName ( F ) . constData ( ) , & B ) ) {
    const qulonglong FrSz = qulonglong ( B . f_frsize ) ;
    S . BlockSize = B . f_bsize  ; S . NameMax = B . f_namemax ;
    S . TotalSize = B . f_blocks * FrSz ; S . TotalFiles = B . f_files  ;
    S . FreeSize  = B . f_bfree  * FrSz ; S . FreeFiles  = B . f_ffree  ;
    S . AvailSize = B . f_bavail * FrSz ; S . AvailFiles = B . f_favail ;
    S . MandLock  = ( B . f_flag & ST_MANDLOCK    ) != 0 ;
    S . NoATime   = ( B . f_flag & ST_NOATIME     ) != 0 ;
    S . NoDev     = ( B . f_flag & ST_NODEV       ) != 0 ;
    S . NoDirATm  = ( B . f_flag & ST_NODIRATIME  ) != 0 ;
    S . NoExec    = ( B . f_flag & ST_NOEXEC      ) != 0 ;
    S . NoSUId    = ( B . f_flag & ST_NOSUID      ) != 0 ;
    S . RdOnly    = ( B . f_flag & ST_RDONLY      ) != 0 ;
    S . RelATm    = ( B . f_flag & ST_RELATIME    ) != 0 ;
    S . Sync      = ( B . f_flag & ST_SYNCHRONOUS ) != 0 ;
  } else { S . ErrNo = errno ;
  }//fi
  return S ;
}// Mounts :: GetStatFS

//eof QMounts.cpp
