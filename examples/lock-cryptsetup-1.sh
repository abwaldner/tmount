#!/bin/sh

  Dlg () {
    qarma --window-icon /usr/share/pixmaps/tmount.png --title tmount "${@}" ||
    ! echo "Cancelled." >&2
  } # Dlg

  MySu () { exec 2>/dev/null ktsuss -u root -- "${@}" ; } # MySu

  if [ $( id -u ) != 0 ] ; then MySu "${0}" "${@}"
  elif ! E=$( /sbin/cryptsetup close "${1}" 2>&1 ) ; then
    Dlg --warning --text "$( echo "${E}" | uniq | sed ' s/$/<br\/>/ ' )"
  fi

#eof
