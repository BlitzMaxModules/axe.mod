
Rem
bbdoc: Communication port driver
End Rem

Module axe.FreePort

ModuleInfo "Version: 1.04"
ModuleInfo "Author: Simon Armstrong"
ModuleInfo "License: Public Domain"
ModuleInfo "Copyright: Armstrong Communications Ltd"
ModuleInfo "Modserver: BRL"

ModuleInfo "History: 1.05 Release"
ModuleInfo "History: added Linux support"
ModuleInfo "History: 1.04 Release"
ModuleInfo "History: added StandardIOPort global port"
ModuleInfo "History: 1.03 Release"
ModuleInfo "History: hardcoded comtest code working on win32 and osx"
ModuleInfo "History: 1.02 Release"
ModuleInfo "History: added appleos enumeration"
ModuleInfo "History: 1.01 Release"
ModuleInfo "History: win32 native com port support"

Strict

Import pub.freeprocess

' native interface exposes
' ComOpen(port,baud)
' ComCount()
' ComName$(index)
' ComInfo$(index)



?MacOS

Import "freeport.macos.c"
Import "-framework IOKit"

Extern "C"
Function enumports(charbuffer:Byte Ptr,buffersize)	'null separated device list
Function openserial(portname$z,baud)
Function closeserial(handle)
End Extern

Global applecount
Global applebuffer:Byte[4096]
Global appleports$[]

Function ComCount()
	applecount=enumports(applebuffer,4096)
	Local a$=String.FromCString(applebuffer)
'	DebugLog "a$="+a$
	appleports=New String[applecount]
	Local p,q
	For Local i=0 Until applecount
		q=a.find("|",p)
		If q=-1 q=Len a
		appleports[i]=a[p..q]
		p=q+1
	Next
	Return applecount
End Function

Function ComName$(index)
	Local n$,p
	n=appleports[index]
	p=n.find("-") 
	If p=-1 p=Len n
	Return n[..p]
End Function

Function ComInfo$(index)
	Local n$,p
	n=appleports[index]
	p=n.find("-") 
	If p<>-1 Return n[p+1..]
End Function

?Win32
Import pub.win32
Import "freeport.win32.bmx"
?Linux

Import "freeport.linux.c"

Extern "C"
Function openserial(portname$z,baud)
Function closeserial(handle)
End Extern

Global LinuxComList:TList

Function ComCount()
	Local devdir$[]
	Local dev$
	
	LinuxComList=New TList
	
	devdir=LoadDir("/dev")
	For dev=EachIn devdir
		If dev[..3]="tty"
			LinuxComList.addlast "/dev/"+dev
			DebugLog dev
		EndIf
	Next  
	Return LinuxComList.Count()
End Function

Function ComName$(index)
End Function

Function ComInfo$(index)
End Function
?

Type TComPort Extends TPipeStream
	Field		handle

	Method Open(port$,baud)
		handle=openserial(port,baud)
		If handle 
			readhandle=handle	'setup pipestream handles
			writehandle=handle		
			Return True
		EndIf
	End Method
	
	Method Close()	'overrides pipestream double close
		closeserial(handle)
	End Method
End Type

Type TStandardIOPort Extends TPipeStream
	Method New()
		readhandle=0'stdin_
		writehandle=1'stdout_		
?win32
		readhandle=GetStdHandle(-10)'STD_INPUT_HANDLE
		writehandle=GetStdHandle(-11)'STD_OUTPUT_HANDLE
?	
	End Method
	
	Method Close()	'overrides pipestream double close
	End Method
End Type

Global StandardIOPort:TStandardIOPort=New TStandardIOPort

Function PortCount()
	Return ComCount()
End Function

Function PortName$(index)
	Return ComName(index)
End Function

Function PortInfo$(index)
	Return ComInfo(index)
End Function
