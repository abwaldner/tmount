#!/bin/sh

  if file -bL "${1}" | grep '^LUKS ' ; then
#    "$( dirname "${0}" )"/unlock-cryptsetup-3.sh "${1}"
    ! echo 'Not implemented.' >&2
  else pmount "${1}"
  fi

#eof
