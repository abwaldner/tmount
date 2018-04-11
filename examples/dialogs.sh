#!/bin/sh

  # zenity/qarma --------------------------------------------------------------

  Dlg () {
    zenity 2>/dev/null --title tmount \
      --window-icon /usr/share/pixmaps/tmount.png "${@}" ||
    ! echo Cancelled. >&2
  } # Dlg

  Mode () { # preferably for qarma
    case $( Dlg --forms --add-combo 'Select' \
                --text 'LUKS passphrase input method:' \
                --combo-values 'Interactive|Key File'  ) in
      I* ) echo -i ;; K* ) echo -k ;; * ) ! :
    esac
  } # Mode

  Mode () { # preferably for zenity
    case $( Dlg --list --radiolist --hide-header \
                --text 'LUKS passphrase input method:' \
                --column 1 --column 2 TRUE Interactive FALSE 'Key File' ) in
      I* ) echo -i ;; K* ) echo -k ;; * ) ! :
    esac
  } # Mode

  Psw  () { Dlg --entry --hide-text --text "${1}" ; } # Psw

  FSel () { Dlg --file-selection --title 'tmount - Select a key file'
  } # FSel

  Warn () { Dlg --warning --no-markup --text "${1}" ; } # Warn

  # Xdialog -------------------------------------------------------------------

  Dlg () {
    Xdialog 2>/dev/null --title tmount --stdout "${@}" ||
    ! echo Cancelled. >&2
  } # Dlg

  Mode () {
    Dlg --no-tags --radiolist 'LUKS passphrase input method:' 0 0 3 \
          -i Interactive on -k 'Key File' off
  } # Mode

  Psw  () { Dlg --password --inputbox "${1}" 0 0 ; } # Psw

  FSel () { Dlg --backtitle 'Select a key file' --no-buttons --fselect ./ 0 0
  } # FSel

  Warn () {
    Dlg --left --icon /usr/share/pixmaps/tmount.png \
        --backtitle 'Warning!' --msgbox "${1}" 0 0
  } # Warn

  # gtkdialog -----------------------------------------------------------------

  Dlg () {
    local EXIT VAL Hdr Ftr
    Hdr='<window title="tmount" image-name="/usr/share/pixmaps/tmount.png">'
    Ftr='condition="command_is_true( [ $KEY_SYM = Escape ] && echo yes )"'
    Ftr='<action signal="key-press-event" '"${Ftr}"'>Exit:Cancel</action>'
    Ftr="${Ftr}"' </window>'
    eval "$( D="${Hdr} ${1} ${Ftr}" gtkdialog -c -p D 2>/dev/null )"
    [ OK = "${EXIT}" ] && echo "${VAL}" || ! echo Cancelled. >&2
  } # Dlg

  Mode () {
    local Frm
    Frm='condition="command_is_true( [ $KEY_SYM = Return ] && echo yes )"'
    Frm='<action signal="key-press-event" '"${Frm}"'>Exit:OK</action>'
    Frm='
      <frame LUKS passphrase input method:>
        <vbox>
          <radiobutton>
            <label>Interactive</label> <variable>VAL</variable>
          </radiobutton>
          <radiobutton> <label>Key File</label> </radiobutton> '"${Frm}"'
        </vbox>
        <hbox> <button ok></button> <button cancel></button> </hbox>
      </frame>
    '
    Frm=$( Dlg "${Frm}" )
    case "${Frm}" in true ) echo -i ;; false ) echo -k ;; * ) ! : ;; esac
  } # Mode

  Psw () {
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
        </chooser> <hbox> <button cancel></button> </hbox>
      </frame>
    '
  } # FSel

  Warn () {
    Dlg '
      <vbox>
        <text label="Warning!"></text> <text label="'"${1}"'"></text>
        <hbox> <button ok></button> </hbox>
      </vbox>
    '
  } # Warn

#eof --------------------------------------------------------------------------
