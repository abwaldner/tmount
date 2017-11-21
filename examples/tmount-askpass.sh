#!/bin/sh

  Dlg () {
    qarma --window-icon /usr/share/pixmaps/tmount.png --title tmount "${@}" ||
    ! echo "Cancelled." >&2
  } # Dlg

  Dlg --entry --hide-text --text 'sudo: Enter your password'

#eof
