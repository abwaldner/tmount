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

  if M=$( lsblk -no MOUNTPOINT "/dev/mapper/${1}" ) ; [ "${M}" ] ; then
    C=${TMOUNT_Unmount_command:-}
    if [ "${C}" ] ; then eval "${C}" "${M}"
    else ! echo "${1} mounted on ${M} and unmount disabled by config." >&2
    fi
  fi &&
  if tty >/dev/null ; then
    echo 'sudo - enter your password'
    if sudo /sbin/cryptsetup close "${1}"
    then echo "${1} released." ; sleep 1
    else echo ; echo 'Press Enter to continue...' ; read -r M
    fi
  else MyTerm "${0}" "${@}"
  fi

#eof
