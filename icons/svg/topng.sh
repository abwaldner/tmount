#!/bin/sh

  I="${1}"
  S="${2:-48}"

  #  inkscape -z --vacuum-defs -l "${I}" -f "${I}"

  inkscape -z -w "${S}" -h "${S}" -y 0 -e "${I%.svg}-${S}.png" -f "${I}"
    # for inkscape 0.9x

  #  rsvg-convert -d 96 -w "${S}" -h "${S}" -o "${I%.svg}-${S}.png" "${I}"

#eof
