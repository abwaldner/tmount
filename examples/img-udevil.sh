#!/bin/sh

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  if file -bL "${1}" | grep '^LUKS ' ; then
    P="${TMOUNT_Unlock_command:-}"
    if [ "${P}" ] ; then eval exec "${P} $( l "${1}" )"
    else ! echo "Disabled by configuration." >&2
    fi
  else exec udevil mount "${1}"
  fi

#eof
