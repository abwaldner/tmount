#!/bin/sh

  # Don't use this with regular files!

  Dlg () {
    qarma 2>/dev/null --title tmount \
      --window-icon /usr/share/pixmaps/tmount.png "${@}" ||
    ! echo Cancelled. >&2
  } # Dlg

  Mode () { # preferably for qarma
    case $( Dlg --forms --add-combo 'Select' \
                --text 'LUKS passphrase input method:' \
                --combo-values 'Interactive|Key File'  ) in
      I* ) printf '%s' -i ;; K* ) printf '%s' -k ;; * ) ! : ;;
    esac
  } # Mode

  Psw  () { Dlg --entry --hide-text --text "${1}" ; } # Psw

  FSel () { Dlg --file-selection --title 'tmount - Select a key file'
  } # FSel

  GP () { lsblk -dpno "${2}" "${1}" ; } # GP - get property for device

  case "${1}" in -k|-i|-a ) M="${1}" ;; * ) M='' ;; esac
  case  ${#}  in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] || echo "Usage: ${0##*/} [-k|-i|-a] {device|file}" >&2

  [ "${M}" = '-a' ] && M=$( Mode )

  F='-' L=''

  [ "${M}" ] &&
  if [ '-k' = "${M}" ] ; then F=$( FSel )
  else L=$( Psw "Enter LUKS passphrase for ${1}" )
  fi &&
  printf '%s' "${L}" | pmount -p "${F}" "${1}" &&
  { N=$( lsblk -plno NAME "${1}" | tail -n 1 ) P=${N##*/}
    F=$( GP "${N}" FSTYPE ) L=$( GP "${N}" LABEL ) R=$( realpath "${N}" )
    printf 'Device %s mapped to %s\n%s -> %s\n' \
           "${1}" "${P}" "${N}" "${R}"
    printf '%s (%s, [%s], %s)\nmounted on %s\n' \
           "${R##*/}" "${F:-(no FS)}" "${L:-(no label)}" \
           "$( GP "${N}" SIZE )" "$( GP "${N}" MOUNTPOINT )"
  }

#eof
