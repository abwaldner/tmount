
VERSION = "0.0.4"

TARGET = tmount
TEMPLATE = app
SOURCES += tmount.cpp LOpts.cpp Listener.cpp QUdev.cpp QMounts.cpp TrayIcon.cpp
HEADERS += defs.h LOpts.h Listener.h QUdev.h QMounts.h TrayIcon.h
LIBS += -ludev
# CONFIG += debug warn_on
CONFIG += release
RESOURCES += tmount.qrc
TRANSLATIONS = translations/tmount_ru.ts
QT += gui core widgets
DEFINES += VERSION=\\\"$$VERSION\\\"

unix {
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
  docs.files = README README.ru luks.txt luks.txt.ru examples/*.sh
  INSTALLS += target locale icon desktop docs
}
