
Rem
bbdoc: EntityPort
End Rem
Module axe.entityport

ModuleInfo "Version: 0.01"
ModuleInfo "Author: Simon Armstrong"
ModuleInfo "License: Blitz Shared Source Code"
ModuleInfo "Copyright: Blitz Research Ltd"
ModuleInfo "Modserver: BRL"

ModuleInfo "History: 0.01 Initial Release"
ModuleInfo "Please run the accompanying runmefirst.bmx file before compiling this module"

Strict

Import pub.freeprocess

?win32

Global acidprocess:TProcess
Global acidpipe:TStream

Global WritePos
Global StringPos
Global IntBuffer[]
Global FloatBuffer:Float Ptr
Global StringBuffer:TList

Include "entitycommands.bmx"

Rem
bbdoc: Useless?
End Rem
Function OpenEntityPort()
	ResetBufferedProcess("entityconsole",65536)
	Return acidprocess<>Null
End Function

Function CloseEntityPort()
	If acidprocess
		acidprocess.Terminate
		acidprocess=Null
	EndIf
End Function

Function ResetBufferedProcess(cmd$,buffersize=65536)
	IntBuffer=New Int[BUFFERSIZE/4]
	FloatBuffer=Float Ptr(Byte Ptr(IntBuffer))
	StringBuffer:TList=New TList
	WritePos=1
	acidprocess=CreateProcess(cmd$,HIDECONSOLE)
	If Not acidprocess RuntimeError "Failed to run entityconsole process"
	acidpipe=acidprocess.pipe
	Local greet=acidpipe.ReadInt()
	If greet<>12345 RuntimeError "Unknown greeting from entityconsole process"
End Function

Function FlushBuffer()
	Local byteptr:Byte Ptr=IntBuffer
	Local bytes=WritePos*4
	IntBuffer[0]=bytes-4
	While bytes
		Local out=acidpipe.write(byteptr,bytes)
		byteptr:+out
		bytes:-out
	Wend
	Local textbytes,s$
	textbytes=0
	For Local s$=EachIn StringBuffer
		textbytes:+s.length+1
	Next
	acidpipe.writeint textbytes
	For Local s$=EachIn StringBuffer
		acidpipe.write s.toCString(),s.length+1
	Next
	acidpipe.flush()	
	WritePos=1
	StringPos=0
	StringBuffer.Clear
End Function

Function StringResult$()
	FlushBuffer
	Local n=acidpipe.ReadInt()
	If n=0 Return
	Local chars:Byte[n]
	acidpipe.ReadBytes chars,n
	Return String.FromBytes(chars,n)
End Function

Function FloatResult#()
	FlushBuffer
	Return acidpipe.ReadFloat()
End Function

Function IntResult()
	FlushBuffer
	Local i=acidpipe.ReadInt()
	Return i
End Function

?