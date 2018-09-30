#!/bin/sh

  Cmd='/sbin/cryptsetup'

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

  [ "${M}" ] &&
  if ! tty >/dev/null ; then MyTerm "${0}" "${M}" "${@}"
  else

    N=$( GenMN "${1}" ) P=${N##*/}
    Cmd="${Cmd} open $( l "${1}" ) ${P}"

    [ "${M}" = '-a' ] && {
      echo 'Enter'
      echo '  "k" for key file selection,'
      echo '  "i" for password interactive input,'
      echo 'or any to cancel...'
      read -r M
      case "${M}" in K|k ) M='-k' ;; I|i ) M='-i' ;; * ) M='' ;; esac
    }

    [ "${M}" = '-k' ] && {
      echo 'Enter key file name' ; echo '  or empty string to cancel...'
      if IFS='' read -r F && [ "${F}" ]
      then Cmd="${Cmd} -d $( l "${F}" )" ; else M=''
      fi
    }

    if [ "${M}" ] ; then echo 'su - ' ; su -c "${Cmd}"
    else ! echo Cancelled. >&2
    fi &&
    ( F=$( GP "${N}" FSTYPE ) L=$( GP "${N}" LABEL ) S=$( GP "${N}" SIZE )
      R=$( realpath "${N}" ) F=${F:-(no FS)} L=${L:-(no label)}
      printf 'Device %s mapped to %s.\n%s -> %s\n%s (%s, [%s], %s)\n' \
        "${1}" "${P}" "${N}" "${R}" "${R##*/}" "${F}" "${L}" "${S}"
      if HasFS "${R}" ; then
        C=${TMOUNT_Mount_command:- ! echo Mounting disabled for >&2 }
        eval " ${C} $( l "${R}" )"
      fi
    ) # (...) is overcaution for 'eval'

    echo ; echo 'Press Enter to continue...' ; read -r M

  fi

#eof
