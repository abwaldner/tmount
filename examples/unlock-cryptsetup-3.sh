#!/bin/sh

  Dlg () {
    qarma 2>/dev/null --title tmount \
      --window-icon /usr/share/pixmaps/tmount.png "${@}" ||
    ! echo Cancelled. >&2
  } # Dlg

  Mode () {
    case $( Dlg --forms --add-combo 'Select' \
                --text 'LUKS passphrase input method:' \
                --combo-values 'Interactive|Key File'  ) in
      I* ) printf '%s' -i ;; K* ) printf '%s' -k ;; * ) ! : ;;
    esac
  } # Mode

  Psw  () { Dlg --entry --hide-text --text "${1}" ; } # Psw

  FSel () { Dlg --file-selection --title 'tmount - Select a key file'
  } # FSel

  SUDO_ASKPASS="$( dirname "${0}" )/tmount-askpass.sh" ; export SUDO_ASKPASS

  case "${1}" in -k|-i|-a ) M="${1}" ;; * ) M='' ;; esac
  case  ${#}  in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] || echo "Usage: ${0##*/} [-k|-i|-a] {device|file}" >&2

  [ "${M}" = '-a' ] && M=$( Mode )

  F='-' L='' P="luks-${1##*/}" N="/dev/mapper/${P}"
  [ "${M}" ] &&
  if [ '-k' = "${M}" ] ; then F=$( FSel )
  else  L=$( Psw "Enter LUKS passphrase for ${1}" )
  fi &&
  printf '%s' "${L}" |
    sudo -A /sbin/cryptsetup open "${1}" "${P}" -d "${F}" &&
  lsblk -no FSTYPE,SIZE,LABEL "${N}" | {
    read -r F S L ; R=$( realpath "${N}" ) L="${L:-(no label)}"
    printf 'Device %s mapped to %s.\n%s -> %s\n%s (%s, [%s], %s)\n' \
      "${1}" "${P}" "${N}" "${R}" "${R##*/}" "${F}" "${L}" "${S}"
  }

#eof
