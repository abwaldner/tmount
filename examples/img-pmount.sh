#!/bin/sh

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

  if file -bL "${1}" | grep '^LUKS ' ; then
#    exec "$( dirname "${0}" )"/unlock-cryptsetup-3.sh "${1}"
    ! gt 'Not implemented.' >&2
  else exec pmount "${1}"
  fi

#eof
