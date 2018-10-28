#!/bin/sh

  Cmd='/sbin/cryptsetup'

  Dlg () {
    qarma 2>/dev/null --title tmount \
      --window-icon /usr/share/pixmaps/tmount.png "${@}" ||
    ! echo Cancelled. >&2
  } # Dlg

  Warn () { Dlg --warning --no-markup --text "${1}" ; } # Warn

  MySu () { exec 2>/dev/null ktsuss -u root -- "${1}" ; } # MySu

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  GP () { lsblk -dpno "${2}" "${1}" ; } # GP - get property for device

  N="/dev/mapper/${1}"
  C=${TMOUNT_Unmount_command:- ! echo Unmount disabled by config. >&2 }

  while
    M=$( GP "${N}" MOUNTPOINT ) ; [ "${M}" ] && eval " ${C} $( l "${M}" )"
  do : ; done

  ! [ "$( GP "${N}" MOUNTPOINT )" ] &&
  if ! [ -e "${N}" ] ; then echo "${1} released."
  elif [ "$( id -u )" != 0 ] ; then MySu "${0} $( l "${1}" )"
  elif E=$( eval "${Cmd} close -- $( l "${1}" )" 2>&1 ) #"
  then echo "${1} released." ; sleep 1
  elif [ true = "${TMOUNT_Lock_show:-}" ]
  then echo "${E}" ; sleep 1 # sleep is workaround for ktsuss
  else Warn "$( echo "${E}" | uniq )"
  fi

#eof
