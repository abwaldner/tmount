#!/bin/sh

  export SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.sh"
#  export SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.tk"

  sudo -A /sbin/cryptsetup close "${1}" && echo "${1} released."

#eof
