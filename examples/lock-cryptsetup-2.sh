#!/bin/sh

# ---------------------------------------------------------------------------

  alias myterm='exec xfce4-terminal \
    --disable-server --geometry=40x10 --hide-menubar --hide-toolbar \
    --show-borders --icon /usr/share/pixmaps/tmount.png --title tmount -x'

# ---------------------------------------------------------------------------

#  alias myterm='exec qterminal --geometry 400x100 --title tmount -e'

# ---------------------------------------------------------------------------

  if tty >/dev/null ; then
    su -c "/sbin/cryptsetup close \"${1}\"" ||
    { echo ; echo 'Press Enter to continue...' ; read ; }
  else myterm "${0}" "${@}" 2>/dev/null
  fi

#eof
