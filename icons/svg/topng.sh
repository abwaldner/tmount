#!/bin/sh

I="${1}"
S="${2:-48}"

inkscape --without-gui --export-dpi=72 --export-background-opacity=0 \
         --export-width="${S}" --export-height="${S}" \
         --export-png="${I%.svg}.${S}.png" "${I}"

#eof
