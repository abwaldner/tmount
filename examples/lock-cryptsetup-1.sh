#!/bin/sh

  Cmd='/sbin/cryptsetup'

  Dlg () { # <form options>...
    qarma 2>/dev/null --title 'tmount' \
      --window-icon '/usr/share/pixmaps/tmount.png' "${@}" ||
    ! echo 'Cancelled.' >&2
  } # Dlg

  Warn () { # <text>
    Dlg --warning --no-markup --text "${1}"
  } # Warn

  MySu () { exec 2>/dev/null ktsuss -u root -- "${1}" ; } # MySu

  l () { # <string>  # Substitutes a literal in 'eval' or 'su -c' args.
    printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l

  GP () { # <device> <property name>
    lsblk -dpno "${2}" "${1}" # Get property ($2) for device ($1).
  } # GP

  N=/dev/mapper/${1} ; M=$( GP "${N}" MOUNTPOINT )
  C=${TMOUNT_Unmount_command:-}

  if ! [ "${M}" ] || [ "${C}" ] ; then
    while [ "${M}" ] && eval " ${C} $( l "${M}" ) " ; do
      M=$( GP "${N}" MOUNTPOINT )
    done
  else echo 'Config error: unmounting disabled' >&2
  fi

  ! [ "${M}" ] &&
  if ! [ -e "${N}" ] ; then echo "${1} released."
  elif [ "$( id -u )" != 0 ] ; then MySu "${0} $( l "${1}" )"
  elif E=$( eval " ${Cmd} close -- $( l "${1}" ) " 2>&1 ) #"
  then echo "${1} released." ; sleep 1
  elif [ true = "${TMOUNT_Lock_show:-}" ]
  then echo "${E}" ; sleep 1 # sleep is workaround for ktsuss
  else Warn "$( echo "${E}" | uniq )"
  fi

#eof
