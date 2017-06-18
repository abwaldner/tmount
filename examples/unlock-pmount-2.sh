#!/bin/sh

# ---------------------------------------------------------------------------
  alias myterm='exec 2>/dev/null xfce4-terminal \
    --disable-server --geometry=40x10 --hide-menubar --hide-toolbar \
    --show-borders --icon /usr/share/pixmaps/tmount.png --title tmount -x'
# ---------------------------------------------------------------------------
#  alias myterm='exec 2>/dev/null qterminal \
#          --geometry 400x100 --title tmount -e'
# ---------------------------------------------------------------------------

  case "${1}" in -k|-i|-a ) M="${1}" ;; * ) M='' ;; esac
  case  ${#}  in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac

  if [ -z "${M}" ] ; then ! echo 'Usage: cmd [-k|-i|-a] [device|file]' >&2
  else

    if ! tty >/dev/null ; then myterm "${0}" "${M}" "${@}"
    else

      [ "${M}" = '-a' ] && {
        echo 'Enter'
        echo '  "k" for key file selection,'
        echo '  "i" for password interactive input,'
        echo 'or any to cancel...'
        read M
        case "${M}" in K|k ) M='-k' ;; I|i ) M='-i' ;; * ) M='' ;; esac
      }

      case "${M}" in
        -k )
          echo 'Enter key file name'
          echo '  or empty string to cancel...'
          IFS='' read -r M && [ "${M}" ] && pmount -p "${M}" "${1}"
        ;;
        -i ) pmount "${1}" ;;
      esac ||
      { echo ; echo 'Press Enter to continue...' ; read ; }

    fi

  fi

#eof
