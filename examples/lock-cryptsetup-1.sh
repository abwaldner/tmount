#!/bin/sh

  Cmd='/sbin/cryptsetup'

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

  Warn () { # <text>
    Dlg --warning --no-markup --text "${1}"
  } # Warn

  MySu () {
    exec 2>/dev/null ktsuss \
      -m "$( gt 'Enter root password' )" -u root -- "${1}"
  } # MySu

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
  else gt 'Config error: unmounting disabled' >&2
  fi

  ! [ "${M}" ] &&
  if ! [ -e "${N}" ] ; then echo "${1} $( gt 'released.' )"
  elif [ "$( id -u )" != 0 ] ; then MySu "${0} $( l "${1}" )"
  elif E=$( eval " ${Cmd} close -- $( l "${1}" ) " 2>&1 ) #"
  then echo "${1} $( gt 'released.' )" ; sleep 1
  elif [ true = "${TMOUNT_Lock_show:-}" ]
  then echo "${E}" ; sleep 1 # sleep is workaround for ktsuss
  else Warn "$( echo "${E}" | uniq )"
  fi

#eof
