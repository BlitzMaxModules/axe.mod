SuperStrict

Framework BRL.GLGraphics
Import BRL.FreeAudioAudio
Import BRL.WAVLoader
Import maxgui.drivers
Import BRL.Timer
Import brl.eventqueue

Import axe.portaudio

'Import "libportaudio.a"

SetGraphicsDriver GLGraphicsDriver()

Const paNoDevice:Int=-1

Const paFloat32	:Int=1
Const paInt8	:Int=32
Const paUInt8	:Int=64


Const paClipOff:Int=1

Global samples:TAudioSample[4]
samples[0]=LoadAudioSample("bassdrum.wav")
samples[1]=LoadAudioSample("clap.wav")
samples[2]=LoadAudioSample("whistle.wav")
samples[3]=LoadAudioSample("snare.wav")
Rem
Local tbp:Byte Ptr
tbp=samples[0].samples
For Local n:Int=0 Until samples[0].length
Print tbp[0]
tbp:+1	
Next
EndRem
Global stream:Byte Ptr

Global err:Int


Global chan_pos:Float[4]
Global chan_add:Float[]=[0#,0#,0#,0#]
Global chan_samp:Int[]=[0,3,1,2]
Global bp:Int[4]

Global note:Float[]=[..
							0.5,	0.0,	0.0,	0.40,..
							0.5,	0.0,	0.0,	0.40,..
							0.0,	1.0,	0.0,	0.40,..
							0.0,	0.0,	0.0,	0.40,..
							0.5,	0.0,	0.0,	0.0,..
							0.5,	0.0,	0.0,	0.0,..
							0.0,	1.0,	0.50,	0.0,..
							0.0,	0.0,	0.50,	0.0,..
							0.5,	0.0,	0.0,	0.0,..
							0.5,	0.0,	0.0,	0.0,..
							0.0,	1.0,	0.0,	0.0,..
							0.0,	0.0,	0.0,	0.0,..
							0.5,	0.0,	0.0,	0.40,..
							0.5,	0.0,	0.0,	0.40,..
							0.0,	1.0,	0.50,	0.40,..
							0.0,	0.0,	0.50,	0.40,..
							0.5,	0.0,	0.0,	0.0,..
							0.5,	0.0,	0.0,	0.0,..
							0.0,	1.0,	0.0,	0.0,..
							0.0,	0.0,	0.0,	0.0,..
							0.5,	0.0,	0.0,	0.0,..
							0.5,	0.0,	0.0,	0.0,..
							0.0,	1.0,	0.5,	0.0,..
							0.0,	0.0,	0.5,	0.0..
							]
Global lines:Int=23



error( Pa_Initialize() ,"init")


playstreamPA(stream,myCallback)


Global patpos:Int





Global Window:TGadget
Global Canvas:TGadget

Global Tick:Int
Global tim:TTimer
	

window = CreateWindow("",0,0,640,480)
canvas = CreateCanvas(0,0,ClientWidth(window),ClientHeight(window),window,PANEL_BORDER)',GRAPHICS_BACKBUFFER)
Canvas.SetLayout(1,1,1,1)
ActivateGadget canvas
SetGraphics CanvasGraphics(Canvas)
AddHook EmitEventHook,eventhook
tim=CreateTimer(60)
	
	

	

	



 

While True
	WaitEvent()
Wend





End



Function eventhook:Object(id:Int,data:Object,context:Object)

	If TEvent(data).id=EVENT_WINDOWCLOSE Then
		error ( Pa_CloseStream( stream ),"close stream" )
		Pa_Terminate()
		End			
	EndIf
	
	
	If TEvent(data).id=EVENT_TIMERTICK
		glclear GL_COLOR_BUFFER_BIT
		GLDrawText ("hello!"+(patpos/4),20,20)	
		Flip 0		
	
		tick:+1
		If tick>6 Then
			tick=0
			For Local n:Int=0 To 3
				chan_add[n]=note[patpos+n]
				If chan_add[n]=0 Then chan_pos[n]=0
			Next
			patpos:+4
			If patpos>lines*4 Then patpos=0

				
		EndIf
	EndIf	
	Return data	
EndFunction

Function myCallback:Int( inputBuffer:Byte Ptr, outputBuffer:Byte Ptr,..
                           framesPerBuffer:Int,outTime:Double, userData:Byte Ptr )
	
	For Local i:Int=0 Until framesPerBuffer 
		For Local n:Int=0 To 3
			chan_pos[n]:+chan_add[n]
			If chan_pos[n]>=samples[chan_samp[n]].length Then 
				'chan_pos[n]:-samples[chan_samp[n]].length
				chan_add[n]=0
				chan_pos[n]=0
			EndIf
			bp[n]=Int samples[chan_samp[n]].samples
			bp[n]:+chan_pos[n]
		Next	
			
		
	
		outputbuffer[0]=(Byte Ptr(bp[0])[0]+Byte Ptr(bp[3])[0])/2'(Byte bp[0]+Byte bp[3])/2.0
		outputbuffer:+1
		outputbuffer[0]=(Byte Ptr(bp[1])[0]+Byte Ptr(bp[2])[0])/2'(Byte bp[1]+Byte bp[2])/2.0
		outputbuffer:+1


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
					              	2,paUInt8,Null,44100!,256,0,paClipOff,..
			              			Callback,Null )..
	,"open stream")	
	
	error( Pa_StartStream( stream ),"start stream" )
	Print "playing"



EndFunction
