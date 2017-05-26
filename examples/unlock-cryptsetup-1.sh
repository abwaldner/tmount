#!/bin/sh

  # qarma is full compatible Qt replacement for zenity.
  # ktsuss is graphical version of su.

  if [ $( id -u ) = 0 ] ; then
    qarma --entry --hide-text --title tmount \
          --text "Enter LUKS passphrase for ${1}" |
    /sbin/cryptsetup open "${1}" "luks-${1##*/}"
  else exec ktsuss "${0}" "${1}" 2>/dev/null
  fi

#eof
