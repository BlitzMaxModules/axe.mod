
Strict

Rem
bbdoc: v8 javascript engine
about:
thankyou googlecode v8 team!
End Rem
Module axe.v8

ModuleInfo "Version: trunk"
ModuleInfo "Authors: Copyright 2009 the V8 project authors"
ModuleInfo "License: MIT BSD see LICENCE file for more info"
ModuleInfo "History: 1.01 Release"
ModuleInfo "History: Initial release"
ModuleInfo "History: 1.02 Release"
ModuleInfo "History: Updated to V8 Version 1.3.11"

?Intel
Import "intelv8.bmx"

Import "b8.cpp"

Extern 
Function v8main%(script$z)
End Extern

Function v8(script$)
	DebugLog "v8:"+script
	v8main(script)
End Function

?


Rem
Import "src/platform-freebsd.cpp"
Import "src/platform-nullos.cpp"
Import "src/mksnapshot.cpp"
Import "src/d8-posix.cpp" 
Import "src/d8-windows.cpp"
Import "src/d8.cpp"
Import "src/d8-debug.cpp" 
EndRem

?
