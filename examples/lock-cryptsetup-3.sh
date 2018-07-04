#!/bin/sh

  SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.sh" ; export SUDO_ASKPASS
#  SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.tk" ; export SUDO_ASKPASS

  if M=$( lsblk -no MOUNTPOINT "/dev/mapper/${1}" ) ; [ "${M}" ] ; then
    C=${TMOUNT_Unmount_command:-}
    if [ "${C}" ] ; then eval "${C}" "${M}"
    else ! echo "${1} mounted on ${M} and unmount disabled by config." >&2
    fi
  fi &&
  sudo -A /sbin/cryptsetup close "${1}" && echo "${1} released."

#eof
