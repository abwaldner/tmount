#!/bin/sh

  #   qarma (http://github.com/luebking/qarma) is full
  # compatible Qt replacement for zenity.

  alias qarma='qarma \
    --window-icon /usr/share/pixmaps/tmount.png --title tmount'

  MSel () { echo 'Interactive' ; } # MSel ## Force interactive input.

#  MSel () {
#    qarma --forms --text 'LUKS passphrase input method:' \
#          --add-combo 'Select' --combo-values 'Interactive|File'
#  } # MSel

  M=$( MSel ) &&
  E=$(
    if [ 'File' = "${M}" ] ; then
      F=$( qarma --file-selection --title 'tmount - Select a key file' ) &&
      pmount -p "${F}" "${1}" 2>&1
    else
      qarma --entry --hide-text --text "Enter LUKS passphrase for ${1}" |
      pmount "${1}" 2>&1
    fi
  ) ||
  ! echo "${E:-"Cancelled."}" >&2

#eof
