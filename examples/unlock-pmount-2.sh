#!/bin/sh

# ---------------------------------------------------------------------------

  alias myterm='exec xfce4-terminal \
    --disable-server --geometry=40x10 --hide-menubar --hide-toolbar \
    --show-borders --icon /usr/share/pixmaps/tmount.png --title tmount -x'

# ---------------------------------------------------------------------------

#  alias myterm='exec qterminal --geometry 400x100 --title tmount -e'

# ---------------------------------------------------------------------------

  FSel () { : ; } # FSel ## Force interactive input.

#  FSel () {
#    local N
#    echo 'Enter key filename or empty string for interactive input.' >&2
#    read N && echo "${N}"
#  } # FSel

  if tty >/dev/null ; then
    F=$( FSel ) &&
    if [ "${F}" ] ; then pmount -p "${F}" "${1}" ; else pmount "${1}" ; fi ||
    { echo ; echo 'Press Enter to continue...' ; read ; }
  else myterm "${0}" "${@}" 2>/dev/null
  fi

#eof
