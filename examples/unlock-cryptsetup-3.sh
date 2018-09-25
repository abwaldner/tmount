#!/bin/sh

  Cmd='/sbin/cryptsetup'
  Ask='tmount-askpass.sh' # should be in the same directory
  SUDO_ASKPASS="$( dirname "${0}" )/${Ask}" ; export SUDO_ASKPASS

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

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  GP () { lsblk -plno "${2}" "${1}" ; } # GP - get property for device

  HasFS () { udevadm info "${1}" | grep -Fxq 'E: ID_FS_USAGE=filesystem'
  } # HasFS

  GenMN () { # generate cryptsetups map name
    mktemp -u -p /dev/mapper \
      "$( printf '%s' "${1##*/}" | tr -c '[:alnum:]#+-.:=@' '_')-XXX"
  } # GenMN

  case "${1}" in -k|-i|-a ) M="${1}" ;; * ) M='' ;; esac
  case  ${#}  in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] || echo "Usage: ${0##*/} [-k|-i|-a] {device|file}" >&2

  [ "${M}" = '-a' ] && M=$( Mode )

  F='-' L='' N=$( GenMN "${1}" ) P=${N##*/}
  Cmd="${Cmd} open $( l "${1}" ) ${P}"

  [ "${M}" ] &&
  if [ '-k' = "${M}" ] ; then F=$( FSel )
  else L=$( Psw "Enter LUKS passphrase for ${1}" )
  fi &&
  printf '%s' "${L}" | eval sudo -A "${Cmd} -d $( l "${F}" )" &&
  { F=$( GP "${N}" FSTYPE ) L=$( GP "${N}" LABEL ) S=$( GP "${N}" SIZE )
    R=$( realpath "${N}" ) F=${F:-(no FS)} L=${L:-(no label)}
    printf 'Device %s mapped to %s.\n%s -> %s\n%s (%s, [%s], %s)\n' \
      "${1}" "${P}" "${N}" "${R}" "${R##*/}" "${F}" "${L}" "${S}"
    if HasFS "${R}" ; then
      C=${TMOUNT_Mount_command:- ! echo Mounting disabled for >&2 }
      eval " ${C} $( l "${R}" )"
    fi
  }

#eof
