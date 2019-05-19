#   This "ebuild" template file is a part of "tmount" project.
# See COPYING file for terms of usage.
# Alexander B. Waldner, 2017-2019.

EAPI=6
inherit qmake-utils

DESCRIPTION="Block devices mounter/unmounter for system tray (and not only)"
HOMEPAGE="https://github.com/abwaldner/tmount"
LICENSE="GPL-3"
SLOT="0"
IUSE="+qt5"

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

DEPEND="${RDEPEND}"

src_configure () {

  if use qt5 ; then eqmake5 ; else eqmake4 ; fi || die "qmake failed"

}

src_install () {

  local f l t='translations/'
  local p="${t}/${PN}_"

  for f in "${p}"*.qm ; do
    l=${f#${p}} ; l=${l%.qm} # get language code
    has "${l}" ${LINGUAS} || {
      rm -f "${f}"          # qt translation
      rm -f "${p}/${l}.msg" # tcl translation
      rm -f "${p}/${l}.mo"  # gettext translation
      rm -f *."${l}"        # docs
    }
  done

  emake INSTALL_ROOT="${D}" install

}

#eof
