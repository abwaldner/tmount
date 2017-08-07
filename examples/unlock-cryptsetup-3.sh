#!/bin/sh

  #   The "qarma" (http://github.com/luebking/qarma) is full compatible
  # Qt replacement for zenity.  If you don't have "qarma", use
  # 'zenity 2>/dev/null'.
  alias mydlg='qarma \
    --window-icon /usr/share/pixmaps/tmount.png --title tmount'

  export SUDO_ASKPASS="$( dirname ${0} )"/tmount-askpass.sh

  E='Usage: cmd [-k|-i|-a] [device|file]'
  case "${1}" in -k|-i|-a ) M="${1}" ;; * ) M='' ;; esac
  case  ${#}  in 1 ) M='-a' ;; 2 ) shift ;; * ) M='' ;; esac

  [ "${M}" = '-a' ] && {
    M=$( mydlg --forms --text 'LUKS passphrase input method:' \
           --add-combo 'Select' --combo-values 'Interactive|Key File' ) ||
    E="Cancelled (${?})."
    case "${M}" in 'Interactive' ) M='-i' ;; 'Key File' ) M='-k' ;; esac
  }

  if [ -z "${M}" ] ; then ! echo "${E}" >&2
  else

    P="luks-${1##*/}"
    E=$(
      if [ '-k' = "${M}" ] ; then
        F=$( mydlg --file-selection --title 'tmount - Select a key file' ) &&
        sudo -A /sbin/cryptsetup open "${1}" "${P}" -d "${F}"
      else
        mydlg --entry --hide-text --text "Enter LUKS passphrase for ${1}" |
        sudo -A /sbin/cryptsetup open "${1}" "${P}"
      fi 2>&1
    )
    if [ ${?} = 0 ] ; then echo "${1} mapped to ${P}."
    else ! echo "${E:-Cancelled (${?}).}" >&2
    fi

  fi

#eof
