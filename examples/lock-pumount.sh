#!/bin/sh

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

  N=/dev/mapper/${1}

  while [ -e "${N}" ] && pumount "${N}" ; do : ; done

  ! [ -e "${N}" ] && echo "${1} $( gt 'released.' )"

#eof
