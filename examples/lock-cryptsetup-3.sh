#!/bin/sh

  export SUDO_ASKPASS="$( dirname ${0} )"/tmount-askpass.sh

  E=$( sudo -A /sbin/cryptsetup close "${1}" 2>&1 ) ||
  ! echo "${E:-"Cancelled."}" >&2

#eof
