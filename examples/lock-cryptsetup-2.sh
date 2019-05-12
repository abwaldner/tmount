#!/bin/sh

  Cmd='/sbin/cryptsetup'

# ---------------------------------------------------------------------------
  MyTerm () {
    exec 2>/dev/null xfce4-terminal --disable-server --hide-menubar \
      --hide-toolbar --show-borders --icon '/usr/share/pixmaps/tmount.png' \
      --geometry=40x10 --title 'tmount' -x "${@}"
  } # MyTerm
# ---------------------------------------------------------------------------
#  MyTerm () {
#    exec 2>/dev/null qterminal --geometry 400x100 --title 'tmount' -e "${@}"
#  } # MyTerm
# ---------------------------------------------------------------------------

  l () { # <string>  # Substitutes a literal in 'eval' or 'su -c' args.
    printf 'b%se' "${1}" | sed "s/''*/'\"&\"'/g ; 1 s/^b/'/ ; $ s/e$/'/"
  } # l

  GP () { # <device> <property name>
    lsblk -dpno "${2}" "${1}" # Get property ($2) for device ($1).
  } # GP

  N=/dev/mapper/${1} ; M=$( GP "${N}" MOUNTPOINT )
  C=${TMOUNT_Unmount_command:-}

  if ! [ "${M}" ] || [ "${C}" ] ; then
    while [ "${M}" ] && eval " ${C} $( l "${M}" ) " ; do
      M=$( GP "${N}" MOUNTPOINT )
    done
  else echo 'Config error: unmounting disabled' >&2
  fi

  ! [ "${M}" ] &&
  if tty >/dev/null ; then
    ! [ -e "${N}" ] ||
    { echo 'su -' ; su -c "${Cmd} close -- $( l "${1}" )" ; } &&
    echo "${1} released."
    echo ; echo 'Press Enter to continue...' ; read -r M
  else MyTerm "${0}" "${@}"
  fi

#eof
