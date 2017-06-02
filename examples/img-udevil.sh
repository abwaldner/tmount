#!/bin/sh

  if file -bL "${1}" | grep '^LUKS ' ; then
    "$( dirname "${0}" )"/unlock-cryptsetup-3.sh "${1}"
  else udevil mount "${1}"
  fi

#eof
