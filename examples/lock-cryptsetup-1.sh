#!/bin/sh

  # ktsuss is graphical version of su.

  exec ktsuss /sbin/cryptsetup close "${1}" 2>/dev/null

#eof
