#!/bin/sh

  Cmd='/sbin/cryptsetup'

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

# ---------------------------------------------------------------------------
  MyTerm () {
    exec 2>/dev/null xfce4-terminal --disable-server --hide-menubar \
      --hide-toolbar --show-borders --icon '/usr/share/pixmaps/tmount.png' \
      --geometry=40x10 --title 'tmount' -x "${@}"
  } # MyTerm
# ---------------------------------------------------------------------------
#  MyTerm () {
#    exec 2>/dev/null qterminal --geometry 400x100 --title 'tmount' -e "${@}"
#  } # MyTerm
# ---------------------------------------------------------------------------

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

  [ "${M}" ] &&
  if ! tty >/dev/null ; then MyTerm "${0}" "${M}" "${@}"
  else

    N=$( GenMN "${1}" ) ; P=${N##*/}
    Cmd="${Cmd} open $( l "${1}" ) ${P}"

    [ "${M}" = '-a' ] && {
      printf '%s\n  "k" %s\n  "i" %s\n%s\n' "$( gt 'Enter' )" \
        "$( gt 'for key file selection,' )" \
        "$( gt 'for password interactive input,' )" \
        "$( gt 'or any to cancel...' )"
      read -r M
      case ${M} in K|k ) M='-k' ;; I|i ) M='-i' ;; * ) M='' ;; esac
    }

    [ "${M}" = '-k' ] && {
      printf '%s\n  %s\n' "$( gt 'Enter key file name' )" \
                          "$( gt 'or empty string to cancel...' )"
      if IFS='' read -r F && [ "${F}" ]
      then Cmd="${Cmd} -d $( l "${F}" )" ; else M=''
      fi
    }

    if [ "${M}" ] ; then echo 'su - ' ; su -c "${Cmd}"
    else ! gt 'Cancelled.' >&2
    fi &&
    if HasFS "${N}" ; then
      C=${TMOUNT_Mount_command:-}
      if [ "$C" ] ; then eval " ${C} $( l "${N}" ) " >/dev/null
      else ! gt 'Config error: mounting disabled' >&2
      fi
    fi &&
    { F=$( GP "${N}" FSTYPE ) L=$( GP "${N}" LABEL ) R=$( realpath "${N}" )
      M=$( GP "${N}" MOUNTPOINT ) S=$( GP "${N}" SIZE )
      Z=$( GP "${N}" FSSIZE ) F=${F:-$( PTT "${N}" )} #"
      printf '%s %s %s %s\n%s -> %s\n%s %s (%s, [%s]) %s\n' \
        "$( gt 'Device' )" "${1}" "$( gt 'mapped to' )" "${P}" \
        "${N}" "${R}" "${R##*/}" "${S}" "${F:-$( gt '(no FS)' )}" \
        "${L:-$( gt '(no label)' )}" "${Z}"
      if [ "${M}" ] ; then echo "$( gt 'mounted on' ) ${M}" ; fi
    }

    echo ; gt 'Press Enter to continue...' ; read -r M

  fi

#eof
