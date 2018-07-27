#!/bin/sh

  Cmd='/sbin/cryptsetup open'

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

  Warn () { Dlg --warning --no-markup --text "${1}" ; } # Warn

  MySu () { exec 2>/dev/null ktsuss -u root -- "${@}" ; } # MySu

  HasFS () { udevadm info "${1}" | grep -Fxq 'E: ID_FS_USAGE=filesystem'
  } # HasFS

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  case "${1}" in -k|-i|-a ) M="${1}" ;; * ) M='' ;; esac
  case  ${#}  in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] || echo "Usage: ${0##*/} [-k|-i|-a] {device|file}" >&2

  [ "${M}" = '-a' ] && M=$( Mode )
  F='-' L='' P="luks-${1##*/}" N="/dev/mapper/${P}"

  [ "${M}" ] &&
  if [ "$( id -u )" != 0 ] ; then MySu "${0}" "${M}" "${@}"
  elif E=$( {
         if [ '-k' = "${M}" ] ; then F=$( FSel )
         else L=$( Psw "Enter LUKS passphrase for ${1}" )
         fi &&
         printf '%s' "${L}" |
           eval "${Cmd} $( l "${1}" ) $( l "${P}" ) -d $( l "${F}" )"
       } 2>&1 )
  then
    lsblk -no FSTYPE,SIZE,LABEL "${N}" | {
      read -r F S L ; R=$( realpath "${N}" ) L="${L:-(no label)}"
      printf 'Device %s mapped to %s.\n%s -> %s\n%s (%s, [%s], %s)\n' \
        "${1}" "${P}" "${N}" "${R}" "${R##*/}" "${F}" "${L}" "${S}"
      Cmd=${TMOUNT_Mount_command:-}
      if [ "${Cmd}" ] && HasFS "${R}" ; then eval " ${Cmd} $( l "${R}" )" ; fi
    }
  elif [ false = "${TMOUNT_Unlock_show:-}" ] ; then Warn "${E}"
  else echo "${E}" ; sleep 1 # sleep is workaround for ktsuss
  fi

#eof
