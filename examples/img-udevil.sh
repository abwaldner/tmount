#!/bin/sh

  if file -bL "${1}" | grep '^LUKS ' ; then
    P="${TMOUNT_Unlock_command}"
    if [ "${P}" ] ; then exec ${P} "${1}"
    else ! echo "Disabled by configuration." >&2
    fi
  else udevil mount "${1}"
  fi

#eof
