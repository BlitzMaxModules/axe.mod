Rem
bbdoc: PortAudio Portable Real-Time Audio Library
End Rem
Module axe.portaudio

ModuleInfo "Version: 1.01"
ModuleInfo "Author: Ross Bencina and Phil Burk"
ModuleInfo "Copyright: Copyright 2000 Phil Burk and Ross Bencina"
ModuleInfo "License: Free"
ModuleInfo "History: 1.01 Release"
ModuleInfo "History: First release using portaudio sdk v18.1"

Strict

Import "portaudio_v18_1/pa_common/*.h"
Import "portaudio_v18_1/pa_common/pa_convert.c"
Import "portaudio_v18_1/pa_common/pa_lib.c"
Import "portaudio_v18_1/pa_common/pa_trace.c"
?linux
Import "portaudio_v18_1/pa_unix_oss/pa_unix.c"
Import "portaudio_v18_1/pa_unix_oss/pa_unix_oss.c"
?win32
'Import "portaudio_v18_1/pa_win_ds/dsound_wrapper.c"
'Import "portaudio_v18_1/pa_win_ds/dsound.c"
Import "portaudio_v18_1/pa_win_wmme/pa_win_wmme.c"
?MacOS
Import "-framework CoreAudio"
Import "-framework AudioToolbox"
Import "portaudio_v18_1/pablio/*.h"
Import "portaudio_v18_1/pablio/ringbuffer.c"
Import "portaudio_v18_1/pa_mac_core/pa_mac_core.c"
?

Const PANODEVICE=-1
Const PAFLOAT32=1
Const PACLIPOFF=1

Extern

Function Pa_Initialize()
Function Pa_GetErrorText:Byte Ptr(err)
Function Pa_Terminate()
Function Pa_GetDefaultOutputDeviceID()		
Function Pa_CloseStream(stream:Byte Ptr)
Function Pa_StartStream(stream:Byte Ptr)
Function Pa_OpenStream(..
	stream:Byte Ptr,..
	inputDev,..
	numInputChannels,..
	iformat,..
	inputDriverInfo:Byte Ptr,..
	outputdev,..
	numOutputChans,..
	oformat,..
	outputDriverInfo:Byte Ptr,..
	sampleRate:Double,..
	framesPerBuffer,..
	numberOfBuffers,..
	streamFlags,..
	callback:Byte Ptr,..
	userData:Byte Ptr)

EndExtern

