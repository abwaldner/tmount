#!/bin/sh

  Cmd='/sbin/cryptsetup'

  Dlg () { # <form options>...
    qarma 2>/dev/null --title 'tmount' \
      --window-icon '/usr/share/pixmaps/tmount.png' "${@}" ||
    ! echo 'Cancelled.' >&2
  } # Dlg

  Mode () {
    case $( Dlg --forms --add-combo 'Select' \
                --text 'LUKS passphrase input method:' \
                --combo-values 'Interactive|Key File'  )
    in I* ) printf '%s' '-i' ;; K* ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  Psw () { # <prompt line>
    Dlg --entry --hide-text --text "${1}"
  } # Psw

  FSel () { Dlg --file-selection --title 'tmount - Select a key file'
  } # FSel

  Warn () { # <text>
    Dlg --warning --no-markup --text "${1}"
  } # Warn

  MySu () { exec 2>/dev/null ktsuss -u root -- "${1}" ; } # MySu

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
  [ "${M}" ] || echo "Usage: ${0##*/} [-k|-i|-a] {device|file}" >&2

  [ "${M}" = '-a' ] && M=$( Mode )

  [ "${M}" ] &&
  if [ "$( id -u )" != 0 ] ; then MySu "${0} ${M} $( l "${1}" )"
  else
    F='-' L='' N=$( GenMN "${1}" ) ; P=${N##*/}
    Cmd="${Cmd} open $( l "${1}" ) ${P}"
    E=$( {
      if [ '-k' = "${M}" ] ; then F=$( FSel )
      else L=$( Psw "Enter LUKS passphrase for ${1}" )
      fi &&
      printf '%s' "${L}" | eval "${Cmd} -d $( l "${F}" )" &&
      if HasFS "${N}" ; then
        C=${TMOUNT_Mount_command:-}
        if [ "$C" ] ; then eval " ${C} $( l "${N}" ) " >/dev/null
        else ! echo 'Config error: mounting disabled' >&2
        fi
      fi &&
      { F=$( GP "${N}" FSTYPE ) L=$( GP "${N}" LABEL ) R=$( realpath "${N}" )
        M=$( GP "${N}" MOUNTPOINT ) F=${F:-$( PTT "${N}" )} #"
        printf 'Device %s mapped to %s\n%s -> %s\n%s (%s, [%s], %s)\n' \
               "${1}" "${P}" "${N}" "${R}" "${R##*/}" "${F:-(no FS)}"  \
               "${L:-(no label)}" "$( GP "${N}" SIZE )"
        if [ "${M}" ] ; then echo "mounted on ${M}" ; fi
      }
    } 2>&1 ) ||
    { [ false = "${TMOUNT_Unlock_show:-}" ] && Warn "${E}" ; }
    echo "${E}" ; sleep 1 # sleep is workaround for ktsuss
  fi

#eof
