#!/bin/sh

  N=/dev/mapper/${1}

  while [ -e "${N}" ] && pumount "${N}" ; do : ; done

  ! [ -e "${N}" ] && echo "${1} released."

#eof
