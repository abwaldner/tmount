#!/bin/sh

  for F in "${INSTALL_ROOT}/${1}/"*.mo ; do
    if [ -f "${F}" ] ; then
      D=${F%.mo}/LC_MESSAGES/ ; mkdir -p "${D}" ; mv -f "${F}" "${D}/${2}.mo"
    fi
  done

#eof
