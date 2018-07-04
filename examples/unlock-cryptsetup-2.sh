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

    P="luks-${1##*/}" N="/dev/mapper/${P}"

    [ "${M}" = '-a' ] && {
      echo 'Enter' ; echo '  "k" for key file selection,'
      echo '  "i" for password interactive input,'
      echo 'or any to cancel...'
      read -r M
      case "${M}" in K|k ) M='-k' ;; I|i ) M='-i' ;; * ) M='' ;; esac
    }

    case "${M}" in
      -k )
        echo 'Enter key file name' ; echo '  or empty string to cancel...'
        if IFS='' read -r F && [ "${F}" ]
        then su -c "/sbin/cryptsetup open \"${1}\" \"${P}\" -d \"${F}\""
        else ! echo Cancelled. >&2
        fi ;;
      -i )
        echo 'su - enter root password'
        su -c "/sbin/cryptsetup open \"${1}\" \"${P}\"" ;;
       * ) ! echo Cancelled. >&2 ;;
    esac &&
    lsblk -no FSTYPE,SIZE,LABEL "${N}" | {
      read -r F S L ; R=$( realpath "${N}" ) L="${L:-(no label)}"
      printf 'Device %s mapped to %s.\n%s -> %s\n%s (%s, [%s], %s)\n' \
        "${1}" "${P}" "${N}" "${R}" "${R##*/}" "${F}" "${L}" "${S}"
    }

    echo ; echo 'Press Enter to continue...' ; read -r M

  fi

#eof
