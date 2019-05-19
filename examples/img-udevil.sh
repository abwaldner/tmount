#!/bin/sh

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

  l () { # <string>  # Substitutes a literal in 'eval' or 'su -c' args.
    printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l

  if file -bL "${1}" | grep '^LUKS '
  then C=${TMOUNT_Unlock_command:-} ; else C=' udevil mount '
  fi

  if [ "${C}" ] ; then eval exec " ${C} $( l "${1}" )"
  else ! gt 'LUKS disabled by config.' >&2
  fi

#eof
