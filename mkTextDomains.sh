#!/bin/sh

  for F in "${1}"/*.mo ; do
    D=${F%.mo}/LC_MESSAGES/ ; mkdir -p "${D}" ; mv -f "${F}" "${D}/${2}.mo"
  done

#eof
