
VERSION = "0.0.6"

TEMPLATE = app
SOURCES += tmount.cpp QUnixApp.cpp
SOURCES += Listener.cpp LOpts.cpp TrayIcon.cpp
SOURCES += QUdev.cpp QMounts.cpp
HEADERS += defs.h LOpts.h Listener.h QUdev.h QMounts.h TrayIcon.h QUnixApp.h
LIBS += -ludev
CONFIG += release
RESOURCES += tmount.qrc
TRANSLATIONS = translations/tmount_ru.ts
QT += gui core widgets
DEFINES += VERSION=\\\"$$VERSION\\\"

unix {
  INCLUDEPATH += /usr/include
  PREFIX = $$(PREFIX)
  isEmpty( PREFIX ):PREFIX = /usr
  target.path = $$PREFIX/bin/
  APP_TRANSLATIONS = $$PREFIX/share/$$TARGET/translations/
  DEFINES += APP_TRANSLATIONS=\\\"$$APP_TRANSLATIONS\\\"
  locale.path = $$APP_TRANSLATIONS
  locale.files = translations/*.qm
  icon.files = icons/tmount.png
  icon.path = $$PREFIX/share/pixmaps/
  desktop.path = $$PREFIX/share/applications
  desktop.files = *.desktop
  docs.path = $$PREFIX/share/$$TARGET/docs/
  docs.files = README README.ru *.txt *.txt.ru examples/*
  INSTALLS += target locale icon desktop docs
}
