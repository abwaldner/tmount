#!/bin/sh

  # qarma is full compatible Qt replacement for zenity

  qarma --entry --hide-text --title tmount \
        --text "Enter LUKS passphrase for ${1}" |
  pmount "${1}"

#eof
