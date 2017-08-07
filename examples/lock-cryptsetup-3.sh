#!/bin/sh

  export SUDO_ASKPASS="$( dirname ${0} )"/tmount-askpass.sh

  if E=$( sudo -A /sbin/cryptsetup close "${1}" 2>&1 )
  then   echo "${1} released."
  else ! echo "${E:-Cancelled (${?}).}" >&2
  fi

#eof
