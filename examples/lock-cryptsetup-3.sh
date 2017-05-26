#!/bin/sh

  export SUDO_ASKPASS="$( dirname ${0} )"/tmount-askpass.sh

  exec sudo -A -- /sbin/cryptsetup close "${1}"

#eof
