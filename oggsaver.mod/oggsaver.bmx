
Strict

Rem
bbdoc: OGG saver
about:
The axe.oggsaver module provides the ability to save OGG format #brl.audiosample.AudioSamples.
End Rem
Module axe.oggsaver

ModuleInfo "Version: 1.01"
ModuleInfo "Author: Simon Armstrong"
ModuleInfo "License: Public Domain"
ModuleInfo "Copyright: Blitz Research Ltd"
ModuleInfo "Modserver: BRL"

ModuleInfo "History: 1.01 Release"
ModuleInfo "History: axe.saveogg module created due to growth restrictions on brl.oggloader"

Import Pub.OggVorbis
Import BRL.AudioSample

Import "../../pub.mod/oggvorbis.mod/libogg-1.1.3/include/*.h"
Import "../../pub.mod/oggvorbis.mod/libvorbis-1.1.2/include/*.h"
Import "../../pub.mod/oggvorbis.mod/libvorbis-1.1.2/lib/*.h"
Import "../../pub.mod/oggvorbis.mod/libvorbis-1.1.2/lib/vorbisenc.c"

Import "oggencoder.c"

Extern
Function Encode_Ogg(outputstream:Object,callback:Byte Ptr,freq,channels,samples:Float Ptr,length,compression:Float)
End Extern

Private

Function writefunc(bytes:Byte Ptr,count,stream:TStream) NODEBUG
	stream.WriteBytes bytes,count
End Function	

Public

Rem
bbdoc: Save an Audio Sample in OGG format
about:
#SaveOGG saves @sample to @url in OGG format. If successful, #SaveOGG returns
True, otherwise False.<br>
<br>
The optional @compression parameter should be in the range -0.1 to 1.0,
where -0.1 indicates maximum compression and 1.0 indicates best quality.
End Rem
Function SaveOGG(sample:TAudioSample,URL:Object,compression#=0.1)
	Local output:TStream
	Local samples:Float[],channels,rate,count,i,res,w
	Select sample.format
		Case SF_MONO8
			channels=1
		Case SF_MONO16LE
			channels=1			
		Case SF_MONO16BE
			channels=1
		Case SF_STEREO8
			channels=2
		Case SF_STEREO16LE
			channels=2
		Case SF_STEREO16BE
			channels=2
	End Select
	rate=sample.hertz
	count=channels*sample.length
	samples=New Float[count]
	Local s:Byte Ptr=sample.samples
	Select sample.format
		Case SF_MONO8,SF_STEREO8
			For i=0 Until count
				samples[i]=(s[i]-127.5)/127.5
			Next
		Case SF_MONO16LE,SF_STEREO16LE
			For i=0 Until count
				w=(s[1] Shl 8)|(s[0])
				If (w&$8000) w=w|$ffff0000	'sign extend
				samples[i]=w/32768.0				
				s=s+2
			Next	
		Case SF_MONO16BE,SF_STEREO16BE
			For i=0 Until count
				w=(s[0] Shl 8)|(s[1])
				If (w&$8000) w=w|$ffff0000	'sign extend
				samples[i]=w/32768.0				
				s=s+2
			Next	
	End Select		
	output=WriteStream(url)
	If Not output Return -1
	res=Encode_Ogg(output,writefunc,rate,channels,samples,sample.length,compression)
	CloseStream(output)
	Return res
End Function



Rem
bbdoc: Save an Audio Sample in WAV format
about:
#SaveWAV saves @sample to @url in WAV format. If successful, #SaveWAV returns
True, otherwise False.<br>
End Rem
Function SaveWAV(sample:TAudioSample,URL:Object)
	Local output:TStream
	Local channels
	
	Select sample.format
		Case SF_MONO8
			channels=1
		Case SF_MONO16LE
			channels=1			
		Case SF_MONO16BE
			channels=1
		Case SF_STEREO8
			channels=2
		Case SF_STEREO16LE
			channels=2
		Case SF_STEREO16BE
			channels=2
	End Select

	Local bitrate=8
	
	Local block = channels * bitrate / 8

	Local count=block*sample.length


	output=WriteStream(url)
	If Not output Return -1
	
	'write wav header info
	output.WriteString("RIFF")						'"RIFF" file description header (4 bytes)
	output.WriteInt(count + 40)						'file size - 8 (4 bytes)
	output.WriteString("WAVE")						'"WAVE" description header (4 bytes)
	output.WriteString("fmt ")						'"fmt " description header (4 bytes)
	output.WriteInt(16)								'size of WAVE section chunk (4 bytes)
	output.WriteShort(1)							'WAVE type format (2 bytes)
	output.WriteShort(channels)						'mono/stereo (2 bytes)
	output.WriteInt(sample.hertz)					'sample rate (4 bytes)
	output.WriteInt(sample.hertz * block)			'avg bytes/sec (4 bytes)
	output.WriteShort(block)						'Block alignment (2 bytes)
	output.WriteShort(bitrate)						'Bits/sample (2 bytes)
	output.WriteString("data")						'"data" description header (4 bytes)
	output.WriteInt(count)							'size of data chunk (4 bytes)
	
	For Local i=0 Until count
		output.WriteByte sample.samples[i]
	Next

	CloseStream(output)
	
	Return True

End Function
