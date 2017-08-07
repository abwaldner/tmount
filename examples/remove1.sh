#!/bin/sh

  P="/sys$( udevadm info -q path "${1}" )" &&
  R=$( cat "${P}/removable" ) &&
  if [ "${R}" = 1 ]
  then eject -M "${1}" && echo "You can remove ${1}"
  else ! echo "Device ${1} is not removable." >&2
  fi

#eof
