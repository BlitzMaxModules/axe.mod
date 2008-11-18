SuperStrict
Framework BRL.Math
Import BRL.StandardIO
Import BRL.System

Import axe.portaudio

'Import "libportaudio.a"

Global stream:Byte Ptr

Global err:Int

Global sine:Double[400]
Global chan_phase:Float[4]
Global chan_add:Float[]=[0#,0#,0#,0#]

Global patt:Float[]=[..
							0.0,	6.0,	8.0,	0.5,..
							2.0,	0.0,	8.0,	0.5,..
							0.0,	0.0,	8.0,	0.0,..
							2.0,	0.0,	8.0,	0.0,..
							0.0,	6.5,	0.0,	1.0,..
							2.0,	0.0,	0.0,	1.0,..
							0.0,	0.0,	0.0,	0.0,..
							2.0,	0.0,	0.0,	0.5,..
							0.0,	6.0,	8.0,	0.5,..
							2.0,	0.0,	8.0,	0.0,..
							0.0,	0.0,	8.0,	1.0,..
							2.0,	0.0,	8.0,	1.0,..
							0.0,	6.5,	0.0,	0.0,..
							2.0,	0.0,	0.0,	0.0,..
							0.0,	0.0,	0.0,	0.5,..
							2.0,	0.0,	0.0,	0.5,..
							0.0,	6.0,	8.0,	0.0,..
							2.0,	0.0,	8.0,	0.0,..
							0.0,	0.0,	8.0,	1.0,..
							2.0,	0.0,	8.0,	1.0,..
							0.0,	6.5,	0.0,	0.0,..
							2.0,	0.0,	0.0,	0.0,..
							0.0,	0.0,	0.0,	0.5,..
							2.0,	0.0,	0.0,	0.5..
							]
Global lines:Int=23

For Local i:Int=0 To 399
	sine[i] = ( Sin(360!*(Double(i)/400.0!)) )
Next

error( Pa_Initialize() ,"init")

playstreamPA(stream,myCallback)



Rem
Delay(500)
chan_add[0]=1.5
Print "0"
Delay(500)
chan_add[0]=0
chan_add[1]=2
Print "1"
Delay(500)
chan_add[1]=0
chan_add[3]=2.5
Print "3"
Delay(500)
chan_add[3]=0
chan_add[2]=3
Print "2"
Delay(500)
EndRem
Global patpos:Int
For Local n:Int=0 To lines
	chan_add[0]=patt[patpos]
	chan_add[1]=patt[patpos+1]
	chan_add[2]=patt[patpos+2]
	chan_add[3]=patt[patpos+3]
	patpos:+4
	Delay(200)
Next

Input 

error ( Pa_CloseStream( stream ),"close stream" )

End

Function myCallback:Int( inputBuffer:Double Ptr, outputBuffer:Float Ptr, framesPerBuffer:Int,outTime:Double, userData:Byte Ptr )

	For Local i:Int=0 Until framesPerBuffer 
		outputbuffer[0]=(sine[chan_phase[0]]+sine[chan_phase[3]])/2.0
		outputbuffer:+1
		outputbuffer[0]=(sine[chan_phase[1]]+sine[chan_phase[2]])/2.0
		outputbuffer:+1
		For Local n:Int=0 To 3
			chan_phase[n]:+chan_add[n]
			If chan_phase[n]>=400 Then chan_phase[n]:-400
		Next

	Next
	Return 0
End Function



Function error(e:Int,s:String)
	If e=0 Then Return
	Pa_Terminate()
	Print "error "+e+" in "+s
	Print "message "+String.FromCString(Pa_GetErrorText(e))
	End
EndFunction	

Function playstreamPA(stream:Byte Ptr Var,callback:Byte Ptr)

	error( ..
	Pa_OpenStream( Varptr stream,paNoDevice,0,paFloat32,Null,Pa_GetDefaultOutputDeviceID(),..
			              	2,paFloat32,Null,44100!,256,0,paClipOff,..
	              			Callback,Varptr(sine[0]) )..
	,"open stream")	
	
	error( Pa_StartStream( stream ),"start stream" )
	Print "playing"



EndFunction
