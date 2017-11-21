#!/bin/sh

  pumount "/dev/mapper/${1}" && echo "${1} released."

#eof
