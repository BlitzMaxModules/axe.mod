Import axe.Wiimote

Print "OpenWiimotes()="+OpenWiimotes()

Local count,i,j

While True
	count=PollWiimotes()
	Print "count="+count
	For i=0 Until count*8
		Print "~tbutton["+i+"]="+WiimoteButton(i)
	Next
	For i=0 Until count*32
		Print "~taxiis["+i+"]="+WiimoteAxis(i)
	Next
	Delay 100
Wend
