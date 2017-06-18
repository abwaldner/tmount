#!/bin/sh

  #   The "qarma" (http://github.com/luebking/qarma) is full compatible
  # Qt replacement for zenity.  If you don't have "qarma", use
  # 'zenity 2>/dev/null'.
  alias mydlg='exec qarma \
    --window-icon /usr/share/pixmaps/tmount.png --title tmount'

  mydlg --entry --hide-text --text 'Enter your password'

#eof
