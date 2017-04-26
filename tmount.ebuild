#   This "ebuild" template file is a part of "tmount" project.
# See COPYING file for terms of usage.
# Alexander B. Waldner, 2017.

EAPI=6
inherit qmake-utils

DESCRIPTION="block devices mounter/unmounter for system tray"
HOMEPAGE="https://github.com/abwaldner/tmount"
LICENSE="GPL-3"
SLOT="0"
IUSE="qt5"

if [ "${PV}" = "9999" ] ; then
  EGIT_REPO_URI="
    https://github.com/abwaldner/${PN}
    git://github.com/abwaldner/${PN}.git"
  inherit git-r3
  KEYWORDS=""
else
  SRC_URI="
    https://github.com/abwaldner/tmount/archive/${PV}.tar.gz -> ${P}.tar.gz
  "
  KEYWORDS="x86"
fi

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