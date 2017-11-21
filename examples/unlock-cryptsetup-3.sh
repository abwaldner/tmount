#!/bin/sh

  Dlg () {
    qarma --window-icon /usr/share/pixmaps/tmount.png --title tmount "${@}" ||
    ! echo "Cancelled." >&2
  } # Dlg

  export SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.sh"

  case "${1}" in -k|-i|-a ) M="${1}" ;; * ) M='' ;; esac
  case  ${#}  in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] || echo "Usage: ${0##*/} [-k|-i|-a] {device|file}" >&2

  [ "${M}" = '-a' ] && {
    M=$( Dlg --forms --text 'LUKS passphrase input method:' \
             --add-combo 'Select' --combo-values 'Interactive|Key File' )
    case "${M}" in 'Interactive' ) M='-i' ;; 'Key File' ) M='-k' ;; esac
  }

  F='-' L= P="luks-${1##*/}"
  [ "${M}" ] &&
  if [ '-i' = "${M}" ] ; then
    L=$( Dlg --entry --hide-text --text "Enter LUKS passphrase for ${1}" )
  else F=$( Dlg --file-selection --title 'tmount - Select a key file' )
  fi &&
  printf '%s' "${L}" |
    sudo -A /sbin/cryptsetup open "${1}" "${P}" -d "${F}" &&
  echo "${1} mapped to ${P}."

#eof
