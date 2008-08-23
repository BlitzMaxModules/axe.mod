' saveogg.bmx

Import axe.oggasver

Local sample:TAudioSample=CreateAudioSample( 11025,11025,SF_MONO16LE )

For Local k=0 Until 11025
        sample.samples[k]=Sin(k*360/32)*32767
Next

SaveOGG(sample,"sinwave.ogg")

