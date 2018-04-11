#!/bin/sh

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
      read M
      case "${M}" in K|k ) M='-k' ;; I|i ) M='-i' ;; * ) M='' ;; esac
    }

    case "${M}" in
      -k )
        echo 'Enter key file name' ; echo '  or empty string to cancel...'
        IFS='' read -r F && [ "${F}" ] && pmount -p "${F}" "${1}" ;;
      -i ) pmount "${1}" ;;
    esac &&
    lsblk -plno NAME,FSTYPE,SIZE,MOUNTPOINT,LABEL "${1}" | {
      read N ; read N F S M L ; R=$( realpath "${N}" )
      X='Device %s mapped to %s.\n%s -> %s\n%s (%s, [%s], %s)\n'
      X="${X}"'mounted on %s\n'
      printf "${X}" "${1}" "${N##*/}" "${N}" "${R}" \
                    "${R##*/}" "${F}" "${L:-(no label)}" "${S}" "${M}"
      sleep 2
    } || { echo ; echo 'Press Enter to continue...' ; read G ; }

  fi

#eof
