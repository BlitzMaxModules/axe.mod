' comtest.bmx

Import axe.freeport
Import pub.freeprocess

Strict

Rem
Local port:TComPort
DebugLog "portcount="+PortCount()
For Local i=0 Until portcount()
	DebugLog "port#"+i+"="+portname(i)'+"~n"
	DebugLog portinfo(i)		'.Replace(Chr(0),"|")+"~n"
Next
EndRem

Type TPort
	Field	inputs:TList=New TList	':TPipeStreams
	Field outputs:TList=New TList	':TPipeStreams
	Field errors:TMap=New TMap

	Method AddInput(in:TPipeStream)
		inputs.AddLast in
	End Method
	
	Method AddOutput(out:TPipeStream)
		outputs.AddLast out
	End Method
	
	Method Flush()
	End Method
End Type

Type TTextPort Extends TPort
	Field linebuffer$[256]
		
	Method Process(text:String[],sender:TPipeStream)
	End Method
	
	Method Flush()
		Local in:TPipeStream
		Local line$,linecount
		For in=EachIn inputs
			While True
				line$=in.ReadLine()
				If line=Object(String(Null)) Exit	'empty line cludge
				linebuffer[linecount]=line
				linecount:+1
				If linecount=linebuffer.length Exit						
			Wend
			If linecount
				Process linebuffer[..linecount],in
			EndIf
		Next
	End Method
End Type

Type TBinaryPort Extends TPort
	Field buffer:Byte[4096]

	Method Process(buffer:Byte Ptr,count,sender:TPipeStream)
	End Method

	Method Flush()
		Local in:TPipeStream
		Local bytes

		For in=EachIn inputs
			bytes=in.Read(buffer,buffer.length)
			If bytes<0 
				errors.Insert in,"READERROR"
				DebugLog "TPort.READERROR!"
				Continue
			EndIf
			If bytes
				Process buffer,bytes,in
			EndIf
		Next
	End Method

End Type

Type TMary Extends TTextPort

	Field name$
	
	Method Connect:TMary(n$,in:TPipeStream,out:TPipeStream)
		name=n
		AddInput in'StandardIOPort		
		AddOutput out'
		Return Self
	End Method

	Method Process(text:String[],sender:TPipeStream)
		Local out:TPipeStream
		Local l$
		For out=EachIn outputs
			For l=EachIn text
				out.WriteLine name+" has "+l	'DebugLog
			Next
		Next
	End Method
End Type

Local test:TMary=New TMary.Connect("Mary",StandardIOPort,StandardIOPort)

Local test2:TMary=New TMary.Connect("Bob",test,StandardIOPort)

While True
	test.Flush
	test2.Flush
	Delay 10
Wend

End



Local stdio:TStandardIOPort
stdio=StandardIOPort

Local hello:Byte Ptr="hello~n".ToCString()

While True
	Local a:Byte=65
	stdio.Write(hello,6)
	Local buffer:Byte[512]
	Local bytes
	bytes=stdio.Read(buffer,512)
	If bytes>0
		Local l$
		l=String.frombytes(buffer,bytes)
		l=l.Replace(Chr(0),"~~z")
		l=l.Replace(Chr(13),"~~r")
		l=l.Replace(Chr(10),"~~n")
		DebugLog "bytes="+bytes+" l="+l
		For Local i=0 Until bytes
			DebugLog "."+Chr(buffer[i])	
		Next
	EndIf		

	Delay 50
Wend


Rem
Function CAvail(cstream)
	Local _pos=ftell_( cstream )
	fseek_ cstream,0,SEEK_END_
	Local _size=ftell_( cstream )
	fseek_ cstream,_pos,SEEK_SET_
	Return _size-_pos
End Function

port=New TComPort

'If port.Open("COM33",115200)	
'"COM33",115200)		
'Local com$="\\.\USBSER000"
Local com$="/dev/cu.usbmodem1B141"

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
EndRem

