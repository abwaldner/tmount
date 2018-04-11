#!/bin/sh

  I="${1}"
  S="${2:-48}"

  inkscape --without-gui --export-width="${S}" --export-height="${S}" \
    --export-background-opacity=0 --export-png="${I%.svg}-${S}.png" "${I}"

  # rsvg-convert -w "${S}" -h "${S}" -o "${I%.svg}-${S}.png" "${I}"

#eof
