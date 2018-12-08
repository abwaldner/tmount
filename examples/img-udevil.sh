#!/bin/sh

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  if file -bL "${1}" | grep '^LUKS '
  then C=${TMOUNT_Unlock_command:-} ; else C=' udevil mount '
  fi

  if [ "${C}" ] ; then eval exec " ${C} $( l "${1}" )"
  else ! echo 'LUKS disabled by config.' >&2
  fi

#eof
