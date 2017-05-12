
//   This file is a part of code of "tmount" program.
// See COPYING file for terms of usage.
// Alexander B. Waldner, 2016.

#ifndef DEFS_H
#define DEFS_H

  #ifndef VERSION

    #define VERSION "0.0.0"

  #endif // VERSION

  #define APPLICATION  "tmount"

  #define ORGANIZATION APPLICATION

  #define LICENSE "GPLv3"

  #define COPYRYGHT "(C) 2016,2017 Alexander B. Waldner."

  #ifndef APP_TRANSLATIONS

    #define APP_TRANSLATIONS ( qApp -> applicationDirPath ( ) + \
                                              "/translations" )

  #endif // APP_TRANSLATIONS

#endif // DEFS_H

//eof defs.h
