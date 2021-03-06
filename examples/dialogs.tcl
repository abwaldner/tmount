#!/usr/bin/env expect

  set MsD /usr/share/tmount/translations/ ;# i18n directory
  package require msgcat ; namespace import msgcat::mc
  catch { msgcat::mcload $MsD }

  # zenity/qarma ------------------------------------------------------------

  proc Dlg { Res args } {
    upvar $Res R
    set L { --title tmount --window-icon /usr/share/pixmaps/tmount.png }
    set R [
      if [ set C [ catch { exec zenity 2>/dev/null {*}$L {*}$args } S ] ] {
        puts stderr [ regsub {^child .*abnormally$} $S [ mc Cancelled. ] ]
      } else { set S
      };#fi
    ]
    return $C
  };# Dlg

  proc Mode {} { ;# preferably for qarma
    Dlg M --forms --text [ mc {LUKS passphrase input method:} ] \
          --add-combo [ mc Select ] \
          --combo-values "I[ mc nteractive ]|K[ mc {ey File} ]"
    return [ string tolower [ string range $M 0 0 ] ]
  };# Mode

  proc Mode {} { ;# preferably for zenity
    Dlg M --list --radiolist --hide-header --column 1 --column 2 \
          --text [ mc {LUKS passphrase input method:} ] \
          TRUE "I[ mc nteractive ]" FALSE "K[ mc {ey File} ]"
    return [ string tolower [ string range $M 0 0 ] ]
  };# Mode

  proc Psw { Prompt } {
    if [ Dlg P --entry --hide-text --text $Prompt ] { set P "\004" };#fi
    return "${P}\n"
  };# Psw

  proc FSel {} {
    Dlg F --file-selection --title "tmount - [ mc {Select a key file} ]"
    return $F
  };# FSel

  # Xdialog -----------------------------------------------------------------

  proc Dlg { Res args } {
    upvar $Res R
    set L { --title tmount --stdout }
    set R [
      if [ set C [ catch { exec Xdialog 2>/dev/null {*}$L {*}$args } S ] ] {
        puts stderr [ regsub {^child .*abnormally$} $S [ mc Cancelled. ] ]
      } else { set S
      };#fi
    ]
    return $C
  };# Dlg

  proc Mode {} {
    Dlg M --no-tags --radiolist [ mc {LUKS passphrase input method:} ] \
          0 0 3 i [ mc Interactive ] on k [ mc {Key File} ] off
    return $M
  };# Mode

  proc Psw { Prompt } {
    if [ Dlg P --password --inputbox $Prompt 0 0 ] { set P "\004" };#fi
    return "${P}\n"
  };# Psw

  proc FSel {} {
    Dlg F --backtitle [ mc {Select a key file} ] \
          --no-buttons --fselect ./ 0 0
    return $F
  };# FSel

  # gtkdialog ---------------------------------------------------------------

  proc Dlg { Res args } {
    upvar $Res R ; global env
    append D {
      <window title="tmount" image-name="/usr/share/pixmaps/tmount.png"> } \
        {*}$args {
        <action signal="key-press-event" condition="command_is_true(
            [ $KEY_SYM = Escape ] && echo yes )">Exit:Cancel</action>
      </window>
    }
    set R [
      set env(D) $D ; set S [ exec gtkdialog 2>/dev/null -c -p D ]
      if [ set C [ regexp {EXIT="OK"} $S ] ] {
        regsub {.*VAL="([^"]*)".*} $S {\1} ;#"
      } else { puts stderr [ mc Cancelled. ]
      };#fi
    ]
    return [ expr { ! $C } ]
  };# Dlg

  proc Mode {} {
    Dlg M {
      <frame } [ regsub -all > [ mc {LUKS passphrase input method:} ] {} ] {>
        <vbox>
          <radiobutton>
            <label>} [ regsub -all < [ mc Interactive ] {} ] {</label>
            <variable>VAL</variable>
          </radiobutton>
          <radiobutton>
            <label>} [ regsub -all < [ mc {Key File}  ] {} ] {</label>
          </radiobutton>
          <action signal="key-press-event" condition="command_is_true(
              [ $KEY_SYM = Return ] && echo yes )">Exit:OK</action>
        </vbox>
        <hbox> <button ok></button> <button cancel></button> </hbox>
      </frame>
    }
    return [ regsub false [ regsub true $M i ] k ]
  };# Mode

  proc Psw { Prompt } {
    append D {
      <frame } [ regsub -all > $Prompt {} ] {>
        <entry visibility="false">
          <variable>VAL</variable>
          <action signal="activate">Exit:OK</action>
        </entry>
        <hbox> <button ok></button> <button cancel></button> </hbox>
      </frame>
    }
    if [ Dlg P $D ] { set P "\004" };#fi
    return "${P}\n"
  };# Psw

  proc FSel {} {
    Dlg F {
      <frame } [ regsub -all > [ mc {Select a key file} ] {} ] {>
        <chooser action="0">
           <default>./</default> <variable>VAL</variable>
           <width>600</width> <height>400</height>
           <action when="file-activated">Exit:OK</action>
        </chooser>
        <hbox> <button cancel></button> </hbox>
      </frame>
    }
    return $F
  };# FSel

  # yad ---------------------------------------------------------------------

  proc Dlg { Res args } {
    upvar $Res R
    set L { --title tmount --window-icon /usr/share/pixmaps/tmount.png }
    append L { --center }
    set R [
      if [ set C [ catch { exec yad 2>/dev/null {*}$L {*}$args } S ] ] {
        puts stderr [ regsub {^child .*abnormally$} $S [ mc Cancelled. ] ]
      } else { set S
      };#fi
    ]
    return $C
  };# Dlg

  proc Mode {} {
    Dlg M --entry --text [ mc {LUKS passphrase input method:} ] \
          --entry-label [ mc Select ] \
          "I[ mc nteractive ]" "K[ mc {ey File} ]"
    return [ string tolower [ string range $M 0 0 ] ]
  };# Mode

  proc Psw { Prompt } {
    if [ Dlg P --entry --hide-text --text $Prompt ] { set P "\004" };#fi
    return "${P}\n"
  };# Psw

  proc FSel {} {
    Dlg F --file --title "tmount - [ mc {Select a key file} ]"
    return $F
  };# FSel

#eof ------------------------------------------------------------------------
