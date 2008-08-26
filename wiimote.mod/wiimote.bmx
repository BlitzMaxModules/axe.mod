
Strict

Rem
bbdoc: Wiimote Driver for Windows and MacOS 10.4+
about:
Talk to Wiimotes with a bluetooth enabled PC.
End Rem
Module axe.WiiMote

ModuleInfo "Version: 1.01"
ModuleInfo "Author:  wiiyourself  (c) 2007 gl.tter - http://wiiyourself.gl.tter.org"
ModuleInfo "Author:  wiiremoteframework (c) 2006 Hiroaki Kimura"
ModuleInfo "Author:  blitzmax glue - Simon Armstrong"
ModuleInfo "License: Public Domain"
ModuleInfo "Modserver: BRL"

ModuleInfo "History: 1.01 Release"
ModuleInfo "History: Started MacOS wiiremoteframework support"
ModuleInfo "History: 1.00 Release"
ModuleInfo "History: Initial release"

?Win32
Import "-lhid"
Import "-lhidparse"
Import "-lsetupapi"
Import "wiiyourself/wiimote.cpp"
Import "wiiyourself/wiiglue.cpp"
?MacOS
Import "-framework IOBluetooth"
Import "wiiremoteframework/WiiRemote.m"
Import "wiiremoteframework/WiiRemoteDiscovery.m"
Import "wiiremoteframework/WiiRemoteGlue.m"
?

Extern "C" 
Rem
bbdoc: Listen for Wiimotes
about: May in future reset the library. Returns 0 if successful.
End Rem
Function OpenWiimotes()
Rem
bbdoc: Disconnects all Wiimotes
End Rem
Function CloseWiimotes()
Rem
bbdoc: Polls all connected Wiimotes
about: Returns current number of connected Wiimotes, call this
function to capture the state of all Wiimotes before using
the WiimoteButton and WiimoteAxis commands.
End Rem
Function PollWiimotes()
Rem
bbdoc: Return button bits
End Rem
Function WiimoteButton(button_id)
Rem
bbdoc: Return axis values
End Rem
Function WiimoteAxis:Float(axiis_id)
Rem
bbdoc: Set status of Wiimote LED display
End Rem
Function SetWiimoteLEDS(wiimote_id,bits)
Rem
bbdoc: Set status of Wiimote rumble motor
End Rem
Function SetWiimoteRumble(wiimote_id,onoff)
End Extern

Function TestWiimotes()
	DebugLog "OpenWiimotes()="+OpenWiimotes()
	
	Local count,i,j
	
	While True
		count=PollWiimotes()

		DebugLog "count="+count

		For i=0 Until count*8
			DebugLog "~tbutton["+i+"]="+WiimoteButton(i)
		Next
		For i=0 Until count*32
			DebugLog "~taxiis["+i+"]="+WiimoteAxis(i)
		Next
		
		SetWiimoteRumble(0,WiimoteButton(0)&2048)
		
		Delay 100
	Wend
End Function

?
