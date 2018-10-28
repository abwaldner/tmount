#!/bin/sh

  Cmd='/sbin/cryptsetup'
  Ask='tmount-askpass.sh' # should be in the same directory
  SUDO_ASKPASS="$( dirname "${0}" )/${Ask}" ; export SUDO_ASKPASS

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  GP () { lsblk -dpno "${2}" "${1}" ; } # GP - get property for device

  N="/dev/mapper/${1}"
  C=${TMOUNT_Unmount_command:- ! echo Unmount disabled by config. >&2 }

  while
    M=$( GP "${N}" MOUNTPOINT ) ; [ "${M}" ] && eval " ${C} $( l "${M}" )"
  do : ; done

  ! [ "$( GP "${N}" MOUNTPOINT )" ] &&
  { ! [ -e "${N}" ] || sudo -A "${Cmd}" close -- "${1}" ; } &&
  echo "${1} released."

#eof
