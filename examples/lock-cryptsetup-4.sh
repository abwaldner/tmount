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

  l () { printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l - substitutes a literal in 'eval' or 'su -c' arguments

  if M=$( lsblk -no MOUNTPOINT "/dev/mapper/${1}" ) ; [ "${M}" ] ; then
    Cmd=${TMOUNT_Unmount_command:-}
    if [ "${Cmd}" ] ; then eval " ${Cmd} $( l "${M}" )"
    else ! echo "${1} mounted on ${M} and unmount disabled by config." >&2
    fi
  fi &&
  if tty >/dev/null ; then
    echo 'sudo - enter your password'
    sudo /sbin/cryptsetup close "${1}" && echo "${1} released."
    echo ; echo 'Press Enter to continue...' ; read -r M
  else MyTerm "${0}" "${@}"
  fi

#eof
