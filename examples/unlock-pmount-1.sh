#!/bin/sh

  # Don't use this with regular files!

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

  Dlg () { # <form options>...
    qarma 2>/dev/null --title 'tmount' \
      --window-icon '/usr/share/pixmaps/tmount.png' "${@}" ||
    ! gt 'Cancelled.' >&2
  } # Dlg

  Mode () {
    case $( Dlg --forms --add-combo "$( gt 'Select' )" \
                --text "$( gt 'LUKS passphrase input method:' )" \
                --combo-values "I$( gt 'nteractive' )|K$( gt 'ey File' )" ) #"
    in I* ) printf '%s' '-i' ;; K* ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  Psw () { # <prompt line>
    Dlg --entry --hide-text --text "${1}"
  } # Psw

  FSel () {
    Dlg --file-selection --title "tmount - $( gt 'Select a key file' )"
  } # FSel

  GP () { # <device> <property name>
    lsblk -dpno "${2}" "${1}" # Get property ($2) for device ($1).
  } # GP

  case ${1} in -k|-i|-a ) M=${1} ;; * ) M='' ;; esac
  case ${#} in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] ||
  echo "$( gt 'Usage' ): ${0##*/} [-k|-i|-a] {$( gt 'device|file' )}" >&2

  [ "${M}" = '-a' ] && M=$( Mode )

  F='-' L=''

  [ "${M}" ] &&
  if [ '-k' = "${M}" ] ; then F=$( FSel )
  else L=$( Psw "$( gt 'Enter LUKS passphrase for' ) ${1}" ) #"
  fi &&
  printf '%s' "${L}" | pmount -p "${F}" "${1}" &&
  { N=$( lsblk -plno NAME "${1}" | tail -n 1 ) ; P=${N##*/}
    F=$( GP "${N}" FSTYPE ) L=$( GP "${N}" LABEL ) R=$( realpath "${N}" )
    S="$( GP "${N}" SIZE )" Z=$( GP "${N}" FSSIZE )
    printf '%s %s %s %s\n%s -> %s\n%s %s (%s, [%s]) %s\n%s %s\n' \
      "$( gt 'Device' )" "${1}" "$( gt 'mapped to' )" "${P}" "${N}" "${R}" \
      "${R##*/}" "${S}" "${F}" "${L:-$( gt '(no label)' )}" "${Z}" \
      "$( gt 'mounted on' )" "$( GP "${N}" MOUNTPOINT )"
  }

#eof
