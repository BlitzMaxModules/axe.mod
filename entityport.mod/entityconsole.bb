; entityconsole.bb
; by simonarmstrong@blitzbasic.com

; add following win32 decls to userlibs to build
;
;.lib "Kernel32.dll"
;win32AttachConsole%( processid ) : "AttachConsole"
;win32GetStdHandle%( stdhandle ) : "GetStdHandle"
;win32WriteFile%( filehandle,databuffer*,numbytes,resultbuffer*,overlap ) : "WriteFile"
;win32ReadFile%( filehandle,databuffer*,numbytes,resultbuffer*,overlap ) : "ReadFile"

win32AttachConsole -1

Global stdin=win32GetStdHandle(-10)
Global stdout=win32GetStdHandle(-11)

Global databuffer=CreateBank(256)
Global resultbuffer=CreateBank(256)

Const BUFFERSIZE=16*65536

bank=CreateBank(BUFFERSIZE)
textbank=CreateBank(BUFFERSIZE)

Include "invoke.bb"

Function PeekString$(bank,offset)
	While True
		c=PeekByte(bank,offset)
		If c=0 Return s$
		s=s+Chr$(c)
		offset=offset+1
	Wend
End Function

Function PokeString(bank,offset,s$)
	Local n=Len(s)
	PokeInt bank,offset,n
	For i=1 To n
		PokeByte bank,offset+3+i,Asc(Mid$(s$,i,1))
	Next
	Return n+4
End Function

PokeInt bank,0,12345
win32WriteFile stdout,bank,4,resultbuffer,0
res=PeekInt(resultbuffer,0)

While True
	win32ReadFile stdin,bank,4,resultbuffer,0
	size=PeekInt(bank,0)
	res=PeekInt(resultbuffer,0)

	If size=0 End
	win32ReadFile stdin,bank,size,resultbuffer,0
	res=PeekInt(resultbuffer,0)
	win32ReadFile stdin,textbank,4,resultbuffer,0
	
	textsize=PeekInt(textbank,0)
	res=PeekInt(resultbuffer,0)
	If textsize
		win32ReadFile stdin,textbank,textsize,resultbuffer,0
		res=PeekInt(resultbuffer,0)
	EndIf
		
	pc=0
	While pc<size
		cmd=PeekInt(bank,pc)
		pc=Invoke(cmd,bank,textbank,pc+4)
		If pc=0 End
	Wend
Wend

;test invoke	
;Function Invoke(cmd,bank,textbank,pc)
;	Select cmd
;		Case 413		;GFXDRIVERNAME_COMMAND=413
;			r$=GfxDriverName(PeekInt(bank,pc))
;			n=PokeString(bank,0,r$)
;			win32WriteFile stdout,bank,n,resultbuffer,0
;			Return pc+4
;		Case 414		;Const COUNTGFXDRIVERS_COMMAND=414
;			PokeInt bank,0,CountGfxDrivers()
;			win32WriteFile stdout,bank,4,resultbuffer,0
;			Return pc
;	End Select		
;End Function
