#!/bin/sh

  D='' P=/sys$( udevadm info -q path "${1}" ) &&
  if [ 1 = "$( cat "${P}/removable" )" ] ; then sudo -n eject -M "${1}"
  else
    while [ "${P}" ] && [ ! "${D}" ] ; do
      N=${P}/remove ; [ -f "${N}" ] && D=${N} ; P=${P%/*}
    done
    if [ "${D}" ] ; then echo 1 | sudo -n tee "${D}" >/dev/null
    else ! echo "Device ${1} is not removable." >&2
    fi
  fi && echo "You can remove ${1}."

#eof
