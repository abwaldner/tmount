#!/bin/sh

# ---------------------------------------------------------------------------

  alias myterm='exec xfce4-terminal \
    --disable-server --geometry=40x10 --hide-menubar --hide-toolbar \
    --show-borders --icon /usr/share/pixmaps/tmount.png --title tmount -e'

# ---------------------------------------------------------------------------

#  alias myterm='exec qterminal --geometry 400x100 --title tmount -e'

# ---------------------------------------------------------------------------

  if tty >/dev/null
  then exec su -c "/sbin/cryptsetup open ${1} luks-${1##*/}"
  else myterm "${0} ${1}" 2>/dev/null
  fi

#eof
