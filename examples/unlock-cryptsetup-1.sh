#!/bin/sh

  #   qarma (http://github.com/luebking/qarma) is full
  # compatible Qt replacement for zenity.
  #   ktsuss (http://github.com/nomius/ktsuss) is graphical version of su.

  alias qarma='qarma \
    --window-icon /usr/share/pixmaps/tmount.png --title tmount'

  MSel () { echo 'Interactive' ; } # MSel ## Force interactive input.

#  MSel () {
#    qarma --forms --text 'LUKS passphrase input method:' \
#          --add-combo 'Select' --combo-values 'Interactive|File'
#  } # MSel

  if [ $( id -u ) = 0 ] ; then
    M=$( MSel ) &&
    E=$(
      if [ 'File' = "${M}" ] ; then
        F=$( qarma --file-selection --title 'tmount - Select a key file' ) &&
        /sbin/cryptsetup open "${1}" "luks-${1##*/}" -d "${F}" 2>&1
      else
        qarma --entry --hide-text --text "Enter LUKS passphrase for ${1}" |
        /sbin/cryptsetup open "${1}" "luks-${1##*/}" 2>&1
      fi
    ) ||
    qarma --warning --text "${E:-"Cancelled."}"
  else exec ktsuss "${0}" "${1}" 2>/dev/null
  fi

#eof
