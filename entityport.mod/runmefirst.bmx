' runmefirst.bmx

' creates invoke.bb and entitycommands.bmx files for building both
' entityconsole.exe with blitz3d and the axe.entityport module
' respectively 

Strict

Global CommandList:TList=New TList

Local decls$=BlitzCCCommands()
If Not decls RuntimeError("Failed to obtain commandlist from Blitz3D compiler")

While decls$
	Local cmd:TCommand=TCommand.Create(RemoveNextLine(decls$))
	CommandList.AddLast cmd
Wend

Local bmxstream:TStream=WriteFile("entitycommands.bmx")
WriteLine bmxstream,bmxfromcommandlist()
CloseFile bmxstream

Local bbstream:TStream=WriteFile("invoke.bb")
WriteLine bbstream,invokefromcommandlist()
CloseFile bbstream

Print "invoke.bb and bbcommands.bmx files created"
Print "now build exe from entityconsole.bb"
Print "and then run acidpipe.bmx"

End

Type TCommand
	Const NULLTYPE=0
	Const INTTYPE=1
	Const FLOATTYPE=2
	Const STRINGTYPE=3

	Global NAMES$[]=["<null>","Int","Float","String"]
	Global MAXNAMES$[]=["<>","","#","$"]
	
	Field	name$,help$
	Field	args
	Field	argname$[32]
	Field	argtype[32]	'arg[0]=return type
	
	Method ToString$()
		Local s$=name'help+"  "+name
		If argtype[0] s:+MAXNAMES[argtype[0]]+"()"
		If args s:+args
		Return s
	End Method

	Method ToBBString$()
		Local s$		
		For Local i=1 To args
			Select argtype[i]
				Case INTTYPE
					s:+"~t~t~t"+argname[i]+"=PeekInt(bank,pc+"+(i-1)*4+")~n"
				Case FLOATTYPE
					s:+"~t~t~t"+argname[i]+"=PeekFloat(bank,pc+"+(i-1)*4+")~n"
				Case STRINGTYPE
					s:+"~t~t~t"+argname[i]+"=PeekString(bank,pc+"+(i-1)*4+")~n"
			End Select		
		Next
		Select argtype[0]
			Case INTTYPE
				s:+"~t~t~tPokeInt bank,0,"+help+"~n"
				s:+"~t~t~twin32WriteFile stdout,bank,4,resultbuffer,0~n"
			Case FLOATTYPE
				s:+"~t~t~tPokeFloat bank,0,"+help+"~n"
				s:+"~t~t~twin32WriteFile stdout,bank,4,resultbuffer,0~n"
			Case STRINGTYPE
				s:+"~t~t~tn=PokeString(bank,0,"+help+")~n"
				s:+"~t~t~twin32WriteFile stdout,bank,n,resultbuffer,0~n"
			Default
				s:+"~t~t~t"+help+"~n"
		End Select
		s:+"~t~t~tReturn pc+"+args*4
		Return s		
	End Method
			
	Method ToBMXString$()
		Local s$				
'		s$="Rem ~nbbdoc: "+name+"~nEnd Rem~n"		
		s$:+"Function "+name
		If argtype[0] s:+MAXNAMES[argtype[0]]	'return type
		s:+"("
		For Local i=1 To args
			If i>1 s:+","
			s:+"_"+argname[i]
		Next
		s:+")~n"		
		s:+"~tIntBuffer[WritePos]="+Upper(name)+"_COMMAND~n"
		For Local i=1 To args
			Select argtype[i]
			Case INTTYPE
				s:+"~tIntBuffer[WritePos+"+(i)+"]=_"+argname[i]+"~n"
			Case FLOATTYPE
				s:+"~tFloatBuffer[WritePos+"+(i)+"]=_"+argname[i]+"~n"
			Case STRINGTYPE
				s:+"~tIntBuffer[WritePos+"+(i)+"]=StringPos~n"
				s:+"~tStringPos:+_"+argname[i]+".length+1~n"
				s:+"~tStringBuffer.AddLast _"+argname[i]+"~n"
			End Select
		Next
		s:+"~tWritePos:+"+(args+1)+"~n"		
		If argtype[0]
			s:+"~tReturn "+NAMES[argtype[0]]+"Result()~n"
		EndIf
		s:+"End Function~n"
		Return s
	End Method
	
	Function Create:TCommand(d$)
		Local c:TCommand=New TCommand
		Local q,p,r$,t

		d$=Replace$(d$,"[","")
		d$=Replace$(d$,"]","")
		d$=Replace$(d$,"global","world")
		d$=Replace$(d$,"type","mode")
		d$=Replace$(d$,"first","first_")
		
		c.help=d$	
		p=Instr(d$,"(")
		If p>2
			q=p-1
			r$=Mid$(d$,p-2,1)
			t=INTTYPE
			If r$="#" t=FLOATTYPE;q=p-2
			If r$="$" t=STRINGTYPE;q=p-2
			c.argtype[0]=t
			c.name=Mid$(d$,1,q-1)
			p=p+1
		Else
			p=Instr(d$," ")
			If p=0 c.name=d$;Return c
			c.name=Mid$(d$,1,p-1)
		EndIf
	
		p=Instr(d$," ")
		If (p) d$=Mid$(d$,p)
		d$=Replace$(d$,"(","")
		d$=Replace$(d$,")","")
'		d$=Replace$(d$,"[","")
'		d$=Replace$(d$,"]","")
		d$=Replace$(d$," ","")
		p=1
		If d$="" Return c
	
		While True
			q=Instr(d$,",",p)
			If q=0 q=Len(d$)+1
			If q=0 Return c
			r$=Mid$(d$,q-1,1)
			t=INTTYPE
			If r$="#" t=FLOATTYPE
			If r$="$" t=STRINGTYPE
			c.args=c.args+1
			c.argtype[c.args]=t
			c.argname[c.args]=d$[p-1..q-1]	'"_"+ simon was here
			p=q+1
			If p>Len(d$) Or c.args=10 Return c
		Wend
	End Function
End Type

Function RemoveNextLine$(code:String Var)
	Local res$=code
	Local n=Instr(code,"~r")
	If n
		code=code[n+1..]
		res=res[..n-1]
	Else
		code=""
	EndIf
'	Print "res="+res
	Return res		
End Function

Function BlitzCCCommands$()
	Local blitz3dpath$="c:\Program Files\Blitz3d"
	blitz3dpath=RequestDir("Please Select Your Blitz3D Folder",blitz3dpath)
	putenv_ "blitzpath="+blitz3dpath
	Print getenv_("blitzpath")
	Local cmd$="~q"+blitz3dpath+"\bin\blitzcc.exe~q +k"	
	Local	process:TProcess
	Local	bytes:Byte[]
	Local	k$
	process=CreateProcess(cmd$,HIDECONSOLE)
	If process
		While True
			bytes=process.pipe.ReadPipe()
			If bytes
				k:+String.FromBytes(bytes,Len bytes)
			EndIf
			If Not process.status() Exit				
			Delay 10
		Wend
	EndIf	

	Local i=Instr(k$,"~nRuntimeStats")
	If i 
		i=Instr(k$,"~n",i+1)
		k$=k$[i..]
	EndIf
	i=Instr(k$,"~nCallDLL")
	If i 
		i=Instr(k$,"~n",i+1)
		k$=k$[..i]
	EndIf
	Return k$

End Function
	
Function bmxfromcommandlist$()
	Local code$
	Local index=1	
	For Local cmd:TCommand=EachIn CommandList
		code$:+"Const "+Upper(cmd.name)+"_COMMAND="+index+"~n"
		index:+1
	Next
	For Local cmd:TCommand=EachIn CommandList
		code$:+cmd.ToBMXString()+"~n"
	Next
	Return code$
End Function


Function invokefromcommandlist$()
	Local code$
	Local index=1	
	
	code$="Function Invoke(cmd,bank,textbank,pc)~n~tSelect cmd~n"

	For Local cmd:TCommand=EachIn CommandList
'		Print "Const "+Upper(cmd.name)+"_COMMAND="+index
		code$:+"~t~tCase "+index+" ;"+cmd.name+"~n"		
		code$:+cmd.ToBBString()+"~n"
		index:+1
	Next
	code$:+"~tEnd Select~nEnd Function~n~n"
	Return code$
End Function
