#!/bin/sh

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

  P=/sys$( udevadm info -q path "${1}" ) &&
  if [ 1 = "$( cat "${P}/removable" )" ] ; then eject -M "${1}"
  else ! echo "$( gt 'Device' ) ${1} $( gt 'is not removable.' )" >&2
  fi && echo "$( gt 'You can remove' ) ${1}"

#eof
