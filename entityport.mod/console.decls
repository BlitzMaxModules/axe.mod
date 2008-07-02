; nitro blitz interface

.lib "Kernel32.dll"

apiCreateFile%(name$,access,mode,securrity,dist,attr,template):"CreateFileA"
apiGetCommState%(hFile, lpDCB*) : "GetCommState" 
apiSetCommState%(hFile, lpCDB*) : "SetCommState" 

win32AttachConsole%( processid ) : "AttachConsole"
win32GetStdHandle%( stdhandle ) : "GetStdHandle"
win32WriteFile%( filehandle,databuffer*,numbytes,resultbuffer*,overlap ) : "WriteFile"
win32ReadFile%( filehandle,databuffer*,numbytes,resultbuffer*,overlap ) : "ReadFile"
