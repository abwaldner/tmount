#!/bin/sh

  Cmd='/sbin/cryptsetup'

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

  GP () { lsblk -dpno "${2}" "${1}" ; } # GP - get property for device

  N="/dev/mapper/${1}"
  C=${TMOUNT_Unmount_command:- ! echo Unmount disabled by config. >&2 }

  while
    M=$( GP "${N}" MOUNTPOINT ) ; [ "${M}" ] && eval " ${C} $( l "${M}" )"
  do : ; done

  ! [ "$( GP "${N}" MOUNTPOINT )" ] &&
  if tty >/dev/null ; then
    ! [ -e "${N}" ] ||
    { echo 'su -' ; su -c "${Cmd} close -- $( l "${1}" )" ; } &&
    echo "${1} released."
    echo ; echo 'Press Enter to continue...' ; read -r M
  else MyTerm "${0}" "${@}"
  fi

#eof
