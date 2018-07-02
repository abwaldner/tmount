#!/bin/sh

  Dlg () {
    qarma 2>/dev/null --title tmount \
      --window-icon /usr/share/pixmaps/tmount.png "${@}" ||
    ! echo Cancelled. >&2
  } # Dlg

  Warn () { Dlg --warning --no-markup --text "${1}" ; } # Warn

  MySu () { exec 2>/dev/null ktsuss -u root -- "${@}" ; } # MySu

  if [ "$( id -u )" != 0 ] ; then MySu "${0}" "${@}"
  elif E=$( /sbin/cryptsetup close "${1}" 2>&1 )
  then echo "${1} released." ; sleep 1
  elif [ true = "${TMOUNT_Lock_show:-}" ]
  then echo "${E}" ; sleep 1 # sleep is workaround for ktsuss
  else Warn "$( echo "${E}" | uniq )"
  fi

#eof
