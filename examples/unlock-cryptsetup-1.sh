#!/bin/sh

  # qarma is full compatible Qt replacement for zenity.
  # ktsuss is graphical version of su.

  qarma --entry --hide-text --title tmount \
        --text "Enter LUKS passphrase for ${1}" |
  ktsuss /sbin/cryptsetup open ${1} luks-${1##*/} 2>/dev/null

#eof
