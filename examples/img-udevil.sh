#!/bin/sh

  if file -bL "${1}" | grep '^LUKS ' ; then
    P="${TMOUNT_Unlock_command:-}"
    if [ "${P}" ] ; then eval exec "${P}" "${1}"
    else ! echo "Disabled by configuration." >&2
    fi
  else exec udevil mount "${1}"
  fi

#eof
