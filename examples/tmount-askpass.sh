#!/bin/sh

  #   qarma (http://github.com/luebking/qarma) is full
  # compatible Qt replacement for zenity.

  alias qarma='qarma \
    --window-icon /usr/share/pixmaps/tmount.png --title tmount'

  qarma --entry --hide-text --text 'Enter your password'

#eof
