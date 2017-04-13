#!/bin/sh

# ---------------------------------------------------------------------------

  alias myterm='exec xfce4-terminal \
    --disable-server --geometry=40x10 --hide-menubar --hide-toolbar \
    --show-borders --icon /usr/share/pixmaps/tmount.png --title tmount -e'

# ---------------------------------------------------------------------------

#  alias myterm='exec qterminal --geometry 400x100 --title tmount -e'

# ---------------------------------------------------------------------------

  myterm "pmount ${1}" 2>/dev/null

#eof
