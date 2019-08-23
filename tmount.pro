
VERSION = "0.0.10"

SOURCES = $$files( *.cpp )
HEADERS = $$files( *.h )
RESOURCES += $${TARGET}.qrc
TRANSLATIONS = $$files( translations/*.ts )
CONFIG += release
QT += gui core widgets
LIBS += -ludev
DEFINES += VERSION=\\\"$${VERSION}\\\"

unix : !mac {
  isEmpty( PREFIX ) : PREFIX = $$(PREFIX)
  isEmpty( PREFIX ) : PREFIX = /usr/
  APP_TRANSLATIONS = $${PREFIX}/share/$${TARGET}/translations/
  DEFINES += APP_TRANSLATIONS=\\\"$${APP_TRANSLATIONS}\\\"
  target.path   = $${PREFIX}/bin/
  icon.path     = $${PREFIX}/share/pixmaps/
  desktop.path  = $${PREFIX}/share/applications/
  docs.path     = $${PREFIX}/share/$${TARGET}/docs/
  exmpl.path    = $${PREFIX}/share/$${TARGET}/examples/
  locale.path   = $${APP_TRANSLATIONS}
  postinst.path = $${PREFIX}/
  icon.files    = icons/$${TARGET}.png
  desktop.files = $${TARGET}.desktop
  docs.files    = README *.txt COPYING LICENSE ChangeLog
  exmpl.files   = examples/*
  locale.files  = translations/*.qm translations/*.msg translations/*.mo
  docs.files   += *.ru
  postinst.extra = ./mkTextDomains.sh $${locale.path} $${TARGET}
  INSTALLS += target locale icon desktop docs exmpl postinst
}

# eof
