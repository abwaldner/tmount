#!/bin/sh

  Cmd='/sbin/cryptsetup'
  Ask='tmount-askpass.sh' # should be in the same directory
  SUDO_ASKPASS="$( dirname "${0}" )/${Ask}" ; export SUDO_ASKPASS

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

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
  { ! [ -e "${N}" ] || sudo -A "${Cmd}" close -- "${1}" ; } &&
  echo "${1} $( gt 'released.' )"

#eof
