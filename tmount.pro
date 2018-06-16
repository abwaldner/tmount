
VERSION = "0.0.8"

TEMPLATE = app
SOURCES += tmount.cpp QUnixApp.cpp
SOURCES += Listener.cpp LOpts.cpp TrayIcon.cpp
SOURCES += QUdev.cpp QMounts.cpp
HEADERS += defs.h QUnixApp.h
HEADERS += Listener.h LOpts.h TrayIcon.h
HEADERS += QUdev.h QMounts.h PTName.h
LIBS   += -ludev
CONFIG += release
RESOURCES += tmount.qrc
TRANSLATIONS = $$files( translations/*.ts )
QT += gui core widgets
DEFINES += VERSION=\\\"$$VERSION\\\"

unix : !mac {
#  INCLUDEPATH *= /usr/include
#    Qt 5.9
  PREFIX = $$(PREFIX)
  isEmpty( PREFIX ) : PREFIX = /usr
  target.path = $$PREFIX/bin/
  APP_TRANSLATIONS = $$PREFIX/share/$$TARGET/translations/
  DEFINES += APP_TRANSLATIONS=\\\"$$APP_TRANSLATIONS\\\"
  LRELEASE = $$[QT_INSTALL_BINS]/lrelease
  locale.path   = $$APP_TRANSLATIONS
  locale.files  = $$replace( TRANSLATIONS , ".ts" , ".qm" )
  locale.CONFIG = no_check_exist
  exists( $$LRELEASE ) : locale.commands = $$LRELEASE $$_PRO_FILE_
  icon.files = icons/tmount.png
  icon.path  = $$PREFIX/share/pixmaps/
  desktop.path  = $$PREFIX/share/applications
  desktop.files = *.desktop
  docs.path   = $$PREFIX/share/$$TARGET/docs/
  docs.files  = README *.txt COPYING LICENSE ChangeLog examples/*
  docs.files += *.ru
  INSTALLS += target locale icon desktop docs
  QMAKE_DISTCLEAN += $$locale.files
}
