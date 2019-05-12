#!/bin/sh

  P=/sys$( udevadm info -q path "${1}" ) &&
  if [ 1 = "$( cat "${P}/removable" )" ] ; then eject -M "${1}"
  else ! echo "Device ${1} is not removable." >&2
  fi && echo "You can remove ${1}."

#eof
