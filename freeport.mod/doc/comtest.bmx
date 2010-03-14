' comtest.bmx

Import axe.freeport
Import pub.freeprocess

Strict

Function CAvail(cstream)
	Local _pos=ftell_( cstream )
	fseek_ cstream,0,SEEK_END_
	Local _size=ftell_( cstream )
	fseek_ cstream,_pos,SEEK_SET_
	Return _size-_pos
End Function



DebugLog "ComCount()="+ComCount()

'port=New TComPort

'If port.Open("COM33",115200)	
'"COM33",115200)		
'Local com$="\\.\USBSER000"

?MacOS
Local com$="/dev/cu.usbmodem1B141"
?Linux
Local com$="/dev/ttyUSB0"	'richard's usb 2 serial adapter 
?




Local port:TComPort

port=New TComPort

If port.Open(com,115200)	'Device\
	DebugLog "port open!"
	While True
		Local buffer:Byte[512]
		Local bytes
		bytes=port.Read(buffer,512)
		If bytes>0
			Local l$=String.frombytes(buffer,bytes).Replace(Chr(0),"~~z")
			DebugLog "bytes="+bytes+" l="+l
			For Local i=0 Until bytes
'				DebugLog "."+Chr(buffer[i])	
			Next
		EndIf		
		Local chars=CAvail(stdin_)
		If chars>0
			DebugLog "> chars="+chars
			If chars>512 chars=512
'			Local l$=StandardIOStream.ReadLine()+"~n~r"
			StandardIOStream.Readbytes(buffer,chars)	'+"~n~r"
			Local l$=String.frombytes(buffer,chars)
			DebugLog "l="+l
'			port.Write(buffer,chars)	'
			port.write(l.toCString(),l.length)
		EndIf
'		DebugLog CAvail(stdin_)	' StandardIOStream.Size()
		PollSystem
		Delay 20
	Wend
	port.Close()
Else
	DebugLog "failed to open port"
EndIf

'If port.Open("com1",9600)


