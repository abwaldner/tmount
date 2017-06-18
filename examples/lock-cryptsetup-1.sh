#!/bin/sh

  #   The "qarma" (http://github.com/luebking/qarma) is full compatible
  # Qt replacement for zenity.  If you don't have "qarma", use
  # 'zenity 2>/dev/null'.
  alias mydlg='qarma \
    --window-icon /usr/share/pixmaps/tmount.png --title tmount'

  # ktsuss (http://github.com/nomius/ktsuss) is graphical version of su.
  alias mysu='exec 2>/dev/null ktsuss -u root --'

  if [ $( id -u ) = 0 ] ; then
    E=$( /sbin/cryptsetup close "${1}" 2>&1 ) ||
    mydlg --warning --text "$( echo "${E}" | uniq | sed ' s/$/<br\/>/ ' )"
  else mysu "${0}" "${@}"
  fi

#eof
