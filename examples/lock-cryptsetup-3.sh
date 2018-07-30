#!/bin/sh

  Cmd='/sbin/cryptsetup'
  Ask='tmount-askpass.sh' # should be in the same directory
#  Ask='tmount-askpass.tk'
  SUDO_ASKPASS="$( dirname "${0}" )/${Ask}" ; export SUDO_ASKPASS

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  N="/dev/mapper/${1}"

  if M=$( lsblk -no MOUNTPOINT "${N}" ) ; [ "${M}" ] ; then
    C=${TMOUNT_Unmount_command:-}
    if [ "${C}" ] ; then eval " ${C} $( l "${M}" )"
    else ! echo "${1} mounted on ${M} and unmount disabled by config." >&2
    fi
  fi &&
  sudo -A "${Cmd}" close "${1}" && echo "${1} released."

#eof
