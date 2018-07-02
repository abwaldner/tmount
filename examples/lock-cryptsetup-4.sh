#!/bin/sh

# ---------------------------------------------------------------------------
  MyTerm () {
    exec 2>/dev/null xfce4-terminal --disable-server --hide-menubar \
      --hide-toolbar --show-borders --icon /usr/share/pixmaps/tmount.png \
      --geometry=40x10 --title tmount -x "${@}"
  } # MyTerm
# ---------------------------------------------------------------------------
#  MyTerm () {
#    exec 2>/dev/null qterminal --geometry 400x100 --title tmount -e "${@}"
#  } # MyTerm
# ---------------------------------------------------------------------------

  if tty >/dev/null ; then
    echo 'sudo - enter your password'
    if sudo /sbin/cryptsetup close "${1}"
    then echo "${1} released." ; sleep 1
    else echo ; echo 'Press Enter to continue...' ; read -r G ; G=${G}
    fi
  else MyTerm "${0}" "${@}"
  fi

#eof
