
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016-2017.

#ifndef LOPTS_H
#define LOPTS_H

#include <QSettings>
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>

enum loKey {
  kNoKey    ,
  kMountCmd , kUnmntCmd , kEjectCmd , kUnlckCmd , kLockCmd , kAddImCmd ,
  kMountTO  , kUnmntTO  , kEjectTO  , kUnlckTO  , kLockTO  , kAddImTO  ,
  kHideDevs , kMntNew   , kMntMedia , kMntStart ,
  kMountPix , kUnmntPix , kEjectPix , kUnlckPix , kLockPix , kAddImPix ,
  kExitPix  , kConfPix  , kAboutPix , kTMntPix
} ;

class LOpts : public QDialog { Q_OBJECT
  public slots : int exec ( ) ;
  public :
    explicit LOpts ( QWidget * parent = 0 ) ; virtual ~LOpts ( ) ;
    int  toInt  ( loKey K ) ;
    bool toBool ( loKey K ) ; QString toStr ( loKey K ) ;
  private :
    struct Item ; static Item ITbl [ ] ; int ITblSize ; QSettings Conf ;
} ; // LOpts

#endif // LOPTS_H

//eof LOpts.h
