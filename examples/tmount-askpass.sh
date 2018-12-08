#!/bin/sh

  Dlg () {
    qarma 2>/dev/null --title 'tmount' \
      --window-icon '/usr/share/pixmaps/tmount.png' "${@}" ||
    ! echo 'Cancelled.' >&2
  } # Dlg

  Psw () { Dlg --entry --hide-text --text "${1}" ; } # Psw

  Psw 'sudo: Enter your password'

#eof
