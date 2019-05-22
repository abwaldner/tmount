#!/bin/sh

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

  exa () { # <string>  # Prepare a string to use as XML attribute.
    printf '%s' "${1}" | sed '
      s/&/\&amp;/g ; # must be first
      s/</\&lt;/g ; s/>/\&gt;/g ; s/"/\&quot;/g ; s/'"'"'/\&apos;/g
    '
  } # exa

  if P=$( pidof -s tmount ) ; then
    L0='tmount:'
    L1=$( gt 'Devices'    ) R1="kill -USR1 ${P}"
    L2=$( gt 'Additional' ) R2="kill -USR2 ${P}"
  else
    L0="tmount $( gt 'not running:' )"
    L1=$( gt 'Start'      ) R1='tmount -wait 0'
    L2=$( gt 'Cancel'     ) R2='true'
  fi

  echo '<?xml version="1.0" encoding="utf-8"?>' ; echo '
    <openbox_pipe_menu>
      <separator label="'"$( exa "${L0}" )"'"/>
      <item label="'"$( exa "${L1}" )"'">
        <action name="Execute"><command>'"${R1}"'</command></action>
      </item>
      <item label="'"$( exa "${L2}" )"'">
        <action name="Execute"><command>'"${R2}"'</command></action>
      </item>
    </openbox_pipe_menu>
  '

#eof obm-tmount.sh
