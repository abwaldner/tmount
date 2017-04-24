# Copyright 1999-2017 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v3
# Alexander B. Waldner, 2017

EAPI=6
inherit qmake-utils

DESCRIPTION="block devices mounter/unmounter for system tray"
HOMEPAGE="https://github.com/abwaldner/tmount"
LICENSE="GPL-3"

SRC_URI="https://github.com/abwaldner/tmount/archive/${PV}.tar.gz -> ${P}.tar.gz"
SLOT="0"
KEYWORDS="x86"
IUSE="qt5"

RDEPEND="
  qt5? (
    dev-qt/qtcore:5
    dev-qt/qtgui:5
    dev-qt/qtwidgets:5
  )
  !qt5? (
    dev-qt/qtcore:4
    dev-qt/qtgui:4
  )
  virtual/libudev
"

DEPEND="${RDEPEND}
  qt5? ( dev-qt/linguist-tools )
"

src_configure () {

  local bd

  if use qt5 ; then bd=$(qt5_get_bindir) ; else bd=$(qt4_get_bindir) ; fi
  "${bd}/lrelease" "${PN}.pro" || die "lrelease failed"

  if use qt5 ; then eqmake5 ; else eqmake4 ; fi || die "qmake failed"

}

src_install () {

  local f l p="translations/${PN}_"

  for f in "${p}"*.qm ; do
    l=${f#${p}} ; l=${l%.qm} ; has "${l}" ${LINGUAS} || rm -f "${f}"
  done

  emake INSTALL_ROOT="${D}" install

}

#eof
