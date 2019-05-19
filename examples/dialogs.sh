#!/bin/sh

  gt () { # <string>
    TEXTDOMAINDIR='/usr/share/tmount/translations/' \
      gettext -d 'tmount' -s "${1}" 2>/dev/null ||
    printf '%s\n' "${1}" # including 'gettext' absence
  } # gt

  # zenity/qarma -------------------------------------------------------------

  Dlg () { # <form options>...
    zenity 2>/dev/null --title 'tmount' \
      --window-icon '/usr/share/pixmaps/tmount.png' "${@}" ||
    ! gt 'Cancelled.' >&2
  } # Dlg

  # preferably for qarma
  Mode () {
    case $( Dlg --forms --add-combo "$( gt 'Select' )" \
                --text "$( gt 'LUKS passphrase input method:' )" \
                --combo-values "I$( gt 'nteractive' )|K$( gt 'ey File' )" ) #"
    in I* ) printf '%s' '-i' ;; K* ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  # preferably for zenity
  Mode () {
    case $( Dlg --list --radiolist --hide-header --column 1 --column 2 \
                --text "$( gt 'LUKS passphrase input method:' )" \
                TRUE "I$( gt 'nteractive' )" FALSE "K$( gt 'ey File' )" )
    in I* ) printf '%s' '-i' ;; K* ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  Psw () { # <prompt line>
    Dlg --entry --hide-text --text "${1}"
  } # Psw

  FSel () {
    Dlg --file-selection --title "tmount - $( gt 'Select a key file' )"
  } # FSel

  Warn () { # <text>
    Dlg --warning --no-markup --text "${1}"
  } # Warn

  # Xdialog ------------------------------------------------------------------

  Dlg () { # <form options>...
    Xdialog 2>/dev/null --title 'tmount' --stdout "${@}" ||
    ! gt 'Cancelled.' >&2
  } # Dlg

  Mode () {
    Dlg --no-tags --radiolist "$( gt 'LUKS passphrase input method:' )" \
        0 0 3 '-i' "$( gt 'Interactive' )" on '-k' "$( gt 'Key File' )" off
  } # Mode

  Psw () { # <prompt line>
    Dlg --password --inputbox "${1}" 0 0
  } # Psw

  FSel () {
    Dlg --backtitle "$( gt 'Select a key file' )" \
        --no-buttons --fselect ./ 0 0
  } # FSel

  Warn () { # <text>
    Dlg --left --icon '/usr/share/pixmaps/tmount.png' \
        --backtitle "$( gt 'Warning!' )" --msgbox "${1}" 0 0
  } # Warn

  # gtkdialog ----------------------------------------------------------------

  Dlg () ( # <forms>...
    D='
      <window title="tmount" image-name="/usr/share/pixmaps/tmount.png">
        '"${*}"'
        <action signal="key-press-event" condition="command_is_true(
            [ '\$'KEY_SYM = Escape ] && echo yes )">Exit:Cancel</action>
      </window>
    '
    eval "$( D=${D} gtkdialog -c -p D 2>/dev/null )"
    [ OK = "${EXIT}" ] && echo "${VAL}" || ! echo 'Cancelled.' >&2
  ) # Dlg  # '(...)' used instead of 'local' for EXIT and VAL, etc.

  Mode () {
    case $(
      Dlg '
        <frame '"$( gt 'LUKS passphrase input method:' | tr -d '>' )"'>
          <vbox>
            <radiobutton>
              <label>'"$( gt 'Interactive' | tr -d '<' )"'</label>
              <variable>VAL</variable>
            </radiobutton>
            <radiobutton><label>'"$( gt 'Key File' | tr -d '<' )"'</label>
            </radiobutton>
            <action signal="key-press-event" condition="command_is_true(
                [ '\$'KEY_SYM = Return ] && echo yes )">Exit:OK</action>
          </vbox>
          <hbox> <button ok></button> <button cancel></button> </hbox>
        </frame>
      '
    ) in true ) printf '%s' '-i' ;; false ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  Psw () { # <prompt line>
    Dlg '
      <frame '"$( printf '%s' "${1}" | tr -d '>' )"'>
        <entry visibility="false">
          <variable>VAL</variable>
          <action signal="activate">Exit:OK</action>
        </entry>
        <hbox> <button ok></button> <button cancel></button> </hbox>
      </frame>
    '
  } # Psw

  FSel () {
    Dlg '
      <frame '"$( gt 'Select a key file' | tr -d '>' )"'>
        <chooser action="0">
          <default>./</default> <variable>VAL</variable>
          <width>600</width> <height>400</height>
          <action when="file-activated">Exit:OK</action>
        </chooser>
        <hbox> <button cancel></button> </hbox>
      </frame>
    '
  } # FSel

  Warn () { # <text>
    Dlg '
      <frame '"$( gt 'Warning!' | tr -d '>' )"'>
        <vbox>
          <hbox>
            <pixmap> <input file icon="dialog-warning"></input> </pixmap>
            <text label="'"$( printf '%s' "${1}" | tr -d '"' )"'"></text>
          </hbox>
          <hbox> <button ok></button> </hbox>
        </vbox>
      </frame>
    '
  } # Warn

  # yad ----------------------------------------------------------------------

  Dlg () { # <form options>...
    yad 2>/dev/null --title 'tmount' --center \
      --window-icon '/usr/share/pixmaps/tmount.png' "${@}" ||
    ! gt 'Cancelled.' >&2
  } # Dlg

  Mode () {
    case $( Dlg --entry --text "$( gt 'LUKS passphrase input method:' )" \
                --entry-label "$( gt 'Select' )" \
                "I$( gt 'nteractive' )" "K$( gt 'ey File' )" ) #"
    in I* ) printf '%s' '-i' ;; K* ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  Psw () { # <prompt line>
    Dlg --entry --hide-text --text "${1}"
  } # Psw

  FSel () { Dlg --file --title "tmount - $( gt 'Select a key file' )"
  } # FSel

  Warn () { # <text>
    Dlg --button gtk-ok --image dialog-warning --no-markup --text "${1}"
  } # Warn

#eof -------------------------------------------------------------------------
