#!/bin/sh

  # qarma is full compatible Qt replacement for zenity.

  export SUDO_ASKPASS="$( dirname ${0} )"/tmount-askpass.sh

  qarma --entry --hide-text --title tmount \
        --text "Enter LUKS passphrase for ${1}" |
  sudo -A -- /sbin/cryptsetup open "${1}" "luks-${1##*/}"

#eof
