#!/bin/sh

  #   qarma (http://github.com/luebking/qarma) is full
  # compatible Qt replacement for zenity.
  #   ktsuss (http://github.com/nomius/ktsuss) is graphical version of su.

  alias qarma='qarma \
    --window-icon /usr/share/pixmaps/tmount.png --title tmount'

  if [ $( id -u ) = 0 ] ; then
    E=$( /sbin/cryptsetup close "${1}" 2>&1 ) ||
    qarma --warning --text "$( echo "${E}" | uniq | sed ' s/$/<br\/>/ ' )"
  else exec ktsuss "${0}" "${1}" 2>/dev/null
  fi

#eof
