#!/bin/sh

  SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.sh" ; export SUDO_ASKPASS
#  SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.tk" ; export SUDO_ASKPASS

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  N="/dev/mapper/${1}"

  if M=$( lsblk -no MOUNTPOINT "${N}" ) ; [ "${M}" ] ; then
    Cmd=${TMOUNT_Unmount_command:-}
    if [ "${Cmd}" ] ; then eval " ${Cmd} $( l "${M}" )"
    else ! echo "${1} mounted on ${M} and unmount disabled by config." >&2
    fi
  fi &&
  sudo -A /sbin/cryptsetup close "${1}" && echo "${1} released."

#eof
