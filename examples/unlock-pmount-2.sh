#!/bin/sh

  # Don't use this with regular files!

# ---------------------------------------------------------------------------
  MyTerm () {
    exec 2>/dev/null xfce4-terminal --disable-server --hide-menubar \
      --hide-toolbar --show-borders --icon /usr/share/pixmaps/tmount.png \
      --geometry=40x10 --title tmount -x "${@}"
  } # MyTerm
# ---------------------------------------------------------------------------
#  MyTerm () {
#    exec 2>/dev/null qterminal --geometry 400x100 --title tmount -e "${@}"
#  } # MyTerm
# ---------------------------------------------------------------------------

  GP () { lsblk -plno "${2}" "${1}" ; } # GP - get property for device

  case "${1}" in -k|-i|-a ) M="${1}" ;; * ) M='' ;; esac
  case  ${#}  in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac
  [ "${M}" ] || echo "Usage: ${0##*/} [-k|-i|-a] {device|file}" >&2

  [ "${M}" ] &&
  if ! tty >/dev/null ; then MyTerm "${0}" "${M}" "${@}"
  else

    [ "${M}" = '-a' ] && {
      echo 'Enter' ; echo '  "k" for key file selection,'
      echo '  "i" for password interactive input,'
      echo 'or any to cancel...'
      read -r M
      case "${M}" in K|k ) M='-k' ;; I|i ) M='-i' ;; * ) M='' ;; esac
    }

    [ "${M}" = '-k' ] && {
      echo 'Enter key file name' ; echo '  or empty string to cancel...'
      { IFS='' read -r F && [ "${F}" ] ; } || M=''
    }

    case "${M}" in
      -k ) pmount -p "${F}" "${1}" ;; -i ) pmount "${1}" ;;
       * ) ! echo Cancelled. >&2   ;;
    esac &&
    { N=$( GP "${1}" NAME | tail -n 1 )
      F=$( GP "${N}" FSTYPE ) L=$( GP "${N}" LABEL ) S=$( GP "${N}" SIZE )
      R=$( realpath "${N}" ) F=${F:-(no FS)} L=${L:-(no label)}
      printf 'Device %s mapped to %s.\n%s -> %s\n%s (%s, [%s], %s)\n' \
        "${1}" "${N##*/}" "${N}" "${R}" "${R##*/}" "${F}" "${L}" "${S}"
      printf 'mounted on %s\n' "$( GP "${N}" MOUNTPOINT )"
    }

    echo ; echo 'Press Enter to continue...' ; read -r M

  fi

#eof
