#!/bin/sh

  SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.sh" ; export SUDO_ASKPASS
#  SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.tk" ; export SUDO_ASKPASS

  sudo -A /sbin/cryptsetup close "${1}" && echo "${1} released."

#eof
