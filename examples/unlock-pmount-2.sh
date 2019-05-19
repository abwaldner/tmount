#!/bin/sh

  # Don't use this with regular files!

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

  GP () { # <device> <property name>
    lsblk -dpno "${2}" "${1}" # Get property ($2) for device ($1).
  } # GP

  case ${1} in -k|-i|-a ) M=${1} ;; * ) M='' ;; esac
  case ${#} in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] ||
  echo "$( gt 'Usage' ): ${0##*/} [-k|-i|-a] {$( gt 'device|file' )}" >&2

  [ "${M}" ] &&
  if ! tty >/dev/null ; then MyTerm "${0}" "${M}" "${@}"
  else

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
      { IFS='' read -r F && [ "${F}" ] ; } || M=''
    }

    case ${M} in
      -k ) pmount -p "${F}" "${1}" ;; -i ) pmount "${1}" ;;
       * ) ! gt 'Cancelled.' >&2 ;;
    esac &&
    { N=$( lsblk -plno NAME "${1}" | tail -n 1 ) ; P=${N##*/}
      F=$( GP "${N}" FSTYPE ) L=$( GP "${N}" LABEL ) R=$( realpath "${N}" )
      printf '%s %s %s %s\n%s -> %s\n%s (%s, [%s], %s)\n%s %s\n' \
        "$( gt 'Device' )" "${1}" "$( gt 'mapped to' )" "${P}" "${N}" "${R}" \
        "${R##*/}" "${F}" "${L:-$( gt '(no label)' )}" "$( GP "${N}" SIZE )" \
        "$( gt 'mounted on' )" "$( GP "${N}" MOUNTPOINT )"
    }

    echo ; gt 'Press Enter to continue...' ; read -r M

  fi

#eof
