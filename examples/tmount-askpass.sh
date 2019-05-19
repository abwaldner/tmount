#!/bin/sh

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

  Dlg () { # <form options>...
    qarma 2>/dev/null --title 'tmount' \
      --window-icon '/usr/share/pixmaps/tmount.png' "${@}" ||
    ! gt 'Cancelled.' >&2
  } # Dlg

  Psw () { # <prompt line>
    Dlg --entry --hide-text --text "${1}"
  } # Psw

  Psw "sudo: $( gt 'Enter your password' )"

#eof
