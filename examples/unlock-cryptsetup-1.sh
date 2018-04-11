#!/bin/sh

  Dlg () {
    qarma 2>/dev/null --title tmount \
      --window-icon /usr/share/pixmaps/tmount.png "${@}" ||
    ! echo Cancelled. >&2
  } # Dlg

  Mode () { # preferably for qarma
    case $( Dlg --forms --add-combo 'Select' \
                --text 'LUKS passphrase input method:' \
                --combo-values 'Interactive|Key File'  ) in
      I* ) echo -i ;; K* ) echo -k ;; * ) ! :
    esac
  } # Mode

  Psw  () { Dlg --entry --hide-text --text "${1}" ; } # Psw

  FSel () { Dlg --file-selection --title 'tmount - Select a key file'
  } # FSel

  Warn () { Dlg --warning --no-markup --text "${1}" ; } # Warn

  MySu () { exec 2>/dev/null ktsuss -u root -- "${@}" ; } # MySu

  case "${1}" in -k|-i|-a ) M="${1}" ;; * ) M='' ;; esac
  case  ${#}  in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] || echo "Usage: ${0##*/} [-k|-i|-a] {device|file}" >&2

  [ "${M}" = '-a' ] && M=$( Mode )

  [ "${M}" ] &&
  if [ "$( id -u )" != 0 ] ; then MySu "${0}" "${M}" "${@}"
  else
    F='-' L='' P="luks-${1##*/}" N="/dev/mapper/${P}"
    E=$( {
      if [ '-k' = "${M}" ] ; then F=$( FSel )
      else L=$( Psw "Enter LUKS passphrase for ${1}" )
      fi &&
      printf '%s' "${L}" | /sbin/cryptsetup open "${1}" "${P}" -d "${F}"
    } 2>&1 ) &&
    lsblk -plno FSTYPE,SIZE,LABEL "${N}" | {
      read F S L ; R=$( realpath "${N}" )
      X='Device %s mapped to %s.\n%s -> %s\n%s (%s, [%s], %s)\n'
      printf "${X}" "${1}" "${P}" "${N}" "${R}" \
                    "${R##*/}" "${F}" "${L:-(no label)}" "${S}"
    } ||
    if [ false = "${TMOUNT_Unlock_show}" ] ; then Warn "${E}"
    else echo "${E}" ; sleep 1
    fi
  fi

#eof
