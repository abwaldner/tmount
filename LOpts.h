
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2022.

#ifndef LOPTS_H
#define LOPTS_H

#include <QSettings>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>

enum loKey {
  kNoKey      ,
  kMountCmd   , kUnmntCmd  , kEjectCmd   , kRemoveCmd  ,
  kUnlockCmd  , kLockCmd   , kAddImgCmd  ,
  kMountTO    , kUnmntTO   , kEjectTO    , kRemoveTO   ,
  kUnlockTO   , kLockTO    , kAddImgTO   , kStartTO    ,
  kHideDevs   , kForceDevs , kVerbose    ,
  kMntNew     , kMntMedia  , kAutoEject  , kMntStart   ,
  kMountPix   , kUnmntPix  , kEjectPix   , kRemovePix  ,
  kUnlockPix  , kLockPix   , kUnrecFPix  , kUnrecSPix  ,
  kAddImgPix  , kExitPix   , kConfPix    , kAboutPix   ,
  kTMntPix    ,
  kMountShow  , kUnmntShow , kEjectShow  , kRemoveShow ,
  kUnlockShow , kLockShow  , kAddImgShow ,
  kNewShow    , kMediaShow , kAutoEjShow , kStartShow
} ;

typedef struct QPair < QString , QString > OptPair ;
typedef class  QList < OptPair > OptList ;

class LOpts : public QDialog { Q_OBJECT
  public slots : int exec ( ) ;
  public :
    explicit LOpts   ( QWidget * parent = nullptr ) ; virtual ~LOpts ( ) ;
    int      toInt   ( loKey K ) const ;
    bool     toBool  ( loKey K ) const ;
    QString  toStr   ( loKey K ) const ;
    OptList  GetAll  ( ) const ;
    QString  AppName ( ) const ; // applicationName from Conf.
  private :
    struct Item ; static Item ITbl [ ] ; static const int ITblSize ;
    QSettings Conf ;
  signals : void Changed ( ) ;
} ; // LOpts

#endif // LOPTS_H

//eof LOpts.h
