#!/bin/sh

  # zenity/qarma -------------------------------------------------------------

  Dlg  () {
    zenity 2>/dev/null --title 'tmount' \
      --window-icon '/usr/share/pixmaps/tmount.png' "${@}" ||
    ! echo 'Cancelled.' >&2
  } # Dlg

  Mode () { # preferably for qarma
    case $( Dlg --forms --add-combo 'Select' \
                --text 'LUKS passphrase input method:' \
                --combo-values 'Interactive|Key File'  )
    in I* ) printf '%s' '-i' ;; K* ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  Mode () { # preferably for zenity
    case $( Dlg --list --radiolist --hide-header \
                --text 'LUKS passphrase input method:' \
                --column 1 --column 2 TRUE 'Interactive' FALSE 'Key File' )
    in I* ) printf '%s' '-i' ;; K* ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  Psw  () { Dlg --entry --hide-text --text "${1}" ; } # Psw

  FSel () { Dlg --file-selection --title 'tmount - Select a key file'
  } # FSel

  Warn () { Dlg --warning --no-markup --text "${1}" ; } # Warn

  # Xdialog ------------------------------------------------------------------

  Dlg  () {
    Xdialog 2>/dev/null --title 'tmount' --stdout "${@}" ||
    ! echo 'Cancelled.' >&2
  } # Dlg

  Mode () {
    Dlg --no-tags --radiolist 'LUKS passphrase input method:' 0 0 3 \
          '-i' 'Interactive' on '-k' 'Key File' off
  } # Mode

  Psw  () { Dlg --password --inputbox "${1}" 0 0 ; } # Psw

  FSel () { Dlg --backtitle 'Select a key file' --no-buttons --fselect ./ 0 0
  } # FSel

  Warn () {
    Dlg --left --icon '/usr/share/pixmaps/tmount.png' \
        --backtitle 'Warning!' --msgbox "${1}" 0 0
  } # Warn

  # gtkdialog ----------------------------------------------------------------

  Dlg  () ( # '(...)' used instead of 'local'.
    H='<window title="tmount" image-name="/usr/share/pixmaps/tmount.png">'
    F='condition="command_is_true( [ '
    F="${F}"'$''KEY_SYM = Escape ] && echo yes )"'
    F='<action signal="key-press-event" '"${F}"'>Exit:Cancel</action>'
    F="${F}"' </window>'
    eval "$( D="${H} ${1} ${F}" gtkdialog -c -p D 2>/dev/null )"
    [ OK = "${EXIT}" ] && echo "${VAL}" || ! echo 'Cancelled.' >&2
  ) # Dlg

  Mode () {
    case $( # 'local' hot need in subshell.
      F='condition="command_is_true( [ '
      F="${F}"'$''KEY_SYM = Return ] && echo yes )"'
      F='<action signal="key-press-event" '"${F}"'>Exit:OK</action>'
      Dlg '
        <frame LUKS passphrase input method:>
          <vbox>
            <radiobutton>
              <label>Interactive</label> <variable>VAL</variable>
            </radiobutton>
            <radiobutton> <label>Key File</label> </radiobutton>
            '"${F}"'
          </vbox>
          <hbox> <button ok></button> <button cancel></button> </hbox>
        </frame>
      '
    ) in true ) printf '%s' '-i' ;; false ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  Psw  () {
    Dlg '
      <frame '"${1}"'>
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
      <frame Select a key file:>
        <chooser action="0">
          <default>./</default> <variable>VAL</variable>
          <width>600</width><height>400</height>
          <action when="file-activated">Exit:OK</action>
        </chooser>
        <hbox> <button cancel></button> </hbox>
      </frame>
    '
  } # FSel

  Warn () {
    Dlg '
      <vbox>
        <hbox>
          <pixmap> <input file icon="dialog-warning"></input> </pixmap>
          <text label="'"${1}"'"></text>
        </hbox>
        <hbox> <button ok></button> </hbox>
      </vbox>
    '
  } # Warn

  # yad ----------------------------------------------------------------------

  Dlg  () {
    yad 2>/dev/null --title 'tmount' --center \
      --window-icon '/usr/share/pixmaps/tmount.png' "${@}" ||
    ! echo 'Cancelled.' >&2
  } # Dlg

  Mode () {
    case $( Dlg --entry --text 'LUKS passphrase input method:' \
                --entry-label 'Select' 'Interactive' 'Key File' )
    in I* ) printf '%s' '-i' ;; K* ) printf '%s' '-k' ;; * ) ! : ;;
    esac
  } # Mode

  Psw  () { Dlg --entry --hide-text --text "${1}" ; } # Psw

  FSel () { Dlg --file --title 'tmount - Select a key file' ; } # FSel

  Warn () {
    Dlg --button gtk-ok --image dialog-warning --no-markup --text "${1}"
  } # Warn

#eof -------------------------------------------------------------------------
