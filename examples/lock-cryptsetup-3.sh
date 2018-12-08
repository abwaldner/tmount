#!/bin/sh

  Cmd='/sbin/cryptsetup'
  Ask='tmount-askpass.sh' # should be in the same directory
  SUDO_ASKPASS="$( dirname "${0}" )/${Ask}" ; export SUDO_ASKPASS

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  GP () { lsblk -dpno "${2}" "${1}" ; } # GP - get property for device

  N="/dev/mapper/${1}" M=$( GP "${N}" MOUNTPOINT )
  C=${TMOUNT_Unmount_command:-}

  if ! [ "${M}" ] || [ "${C}" ] ; then
    while [ "${M}" ] && eval " ${C} $( l "${M}" ) " ; do
      M=$( GP "${N}" MOUNTPOINT )
    done
  else echo 'Config error: unmounting disabled' >&2
  fi

  ! [ "${M}" ] &&
  { ! [ -e "${N}" ] || sudo -A "${Cmd}" close -- "${1}" ; } &&
  echo "${1} released."

#eof
