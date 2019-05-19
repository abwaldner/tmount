#!/bin/sh

  Cmd='/sbin/cryptsetup'
  Ask='tmount-askpass.sh' # should be in the same directory
  SUDO_ASKPASS=$( dirname "${0}" )/${Ask} ; export SUDO_ASKPASS

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

  l () { # <string>  # Substitutes a literal in 'eval' or 'su -c' args.
    printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l

  GP () { # <device> <property name>
    lsblk -dpno "${2}" "${1}" # Get property ($2) for device ($1).
  } # GP

  HasFS () { # <device>
    udevadm info "${1}" | grep -Fxq 'E: ID_FS_USAGE=filesystem'
  } # HasFS

  PTT () { # <device>  # Get Partition Table Type.
    udevadm info "${1}" | sed -n 's/^E: ID_PART_TABLE_TYPE=\(.*\)$/\1 pt/p'
  } # PTT

  GenMN () { # <device>  # Generate cryptsetups map name.
    mktemp -u -p /dev/mapper \
      "_$( printf '%s' "${1##*/}" | tr -c '[:alnum:]#+-.:=@' '_')-XXX"
  } # GenMN

  case ${1} in -k|-i|-a ) M=${1} ;; * ) M='' ;; esac
  case ${#} in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] ||
  echo "$( gt 'Usage' ): ${0##*/} [-k|-i|-a] {$( gt 'device|file' )}" >&2

  [ "${M}" = '-a' ] && M=$( Mode )

  F='-' L='' N=$( GenMN "${1}" ) ; P=${N##*/}
  Cmd="${Cmd} open $( l "${1}" ) ${P}"

  [ "${M}" ] &&
  if [ '-k' = "${M}" ] ; then F=$( FSel )
  else L=$( Psw "$( gt 'Enter LUKS passphrase for' ) ${1}" ) #"
  fi &&
  printf '%s' "${L}" | eval sudo -A " ${Cmd} -d $( l "${F}" ) " &&
  if HasFS "${N}" ; then
    C=${TMOUNT_Mount_command:-}
    if [ "$C" ] ; then eval " ${C} $( l "${N}" ) " >/dev/null
    else ! gt 'Config error: mounting disabled' >&2
    fi
  fi &&
  { F=$( GP "${N}" FSTYPE ) L=$( GP "${N}" LABEL ) R=$( realpath "${N}" )
    M=$( GP "${N}" MOUNTPOINT ) S=$( GP "${N}" SIZE )
    F=${F:-$( PTT "${N}" )} #"
    printf '%s %s %s %s\n%s -> %s\n%s (%s, [%s], %s)\n' \
      "$( gt 'Device' )" "${1}" "$( gt 'mapped to' )" "${P}" \
      "${N}" "${R}" "${R##*/}" "${F:-$( gt '(no FS)' )}" \
      "${L:-$( gt '(no label)' )}" "${S}"
    if [ "${M}" ] ; then echo "$( gt 'mounted on' ) ${M}" ; fi
  }

#eof
