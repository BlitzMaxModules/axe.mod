' freeport.win32.bmx

Import pub.freeprocess
Import brl.linkedlist

Strict

Extern "win32"
Function CreateFileA(lpFileName$z,dwDesiredAccess,dwShareMode,lpSecurityAttributes:Byte Ptr,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile)
Function CreateFileW(lpFileName$w,dwDesiredAccess,dwShareMode,lpSecurityAttributes:Byte Ptr,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile)
Function ClearCommError(hFile,err Ptr,stat:Byte Ptr)
Function GetDefaultCommConfigA(lpFileName$z,lpCC:Byte Ptr,lpdwSize Ptr)
Function SetCommConfig(hCommDev,lpCC:Byte Ptr,dwSize)
Function GetCommConfig(hCommDev,lpCC:Byte Ptr,lpdwSize Ptr)
Function SetupComm(hFile,dwIn,dwOut)
Function GetCommProperties(hFile,lpCommProp:Byte Ptr)
Function SetCommTimeouts(hFile,lpCommTimeouts:Byte Ptr)
Function GetCommState(hFile,lpDCB:Byte Ptr)
Function SetCommState(hFile,lpDCB:Byte Ptr)
Function QueryDosDeviceA(name:Byte Ptr,buffer:Byte Ptr,buffersize)
Function GetLastError()
Function CommConfigDialogA(name$z,hWnd,lpCC:Byte Ptr)
End Extern

' mapiwin.h

Const GENERIC_READ=$80000000
Const GENERIC_WRITE=$40000000

Const FILE_SHARE_READ=1
Const FILE_SHARE_WRITE=2
Const FILE_FLAG_SEQUENTIAL_SCAN=$08000000

Const CREATE_NEW=1
Const CREATE_ALWAYS=2
Const OPEN_EXISTING=3
Const OPEN_ALWAYS=4
Const TRUNCATE_EXISTING=5

Const INVALID_HANDLE_VALUE=-1
'Const Delete=$00010000L
Const FILE_BEGIN=0
Const FILE_CURRENT=1
Const FILE_END=2

Const FILE_ATTRIBUTE_READONLY=$00000001
Const FILE_ATTRIBUTE_HIDDEN=$00000002
Const FILE_ATTRIBUTE_SYSTEM =$00000004
Const FILE_ATTRIBUTE_DIRECTORY=$00000010
Const FILE_ATTRIBUTE_ARCHIVE=$00000020
Const FILE_ATTRIBUTE_NORMAL=$00000080
Const FILE_ATTRIBUTE_TEMPORARY=$00000100

Const FILE_FLAG_WRITE_THROUGH =$80000000
Const FILE_FLAG_RANDOM_ACCESS =$10000000

'winbase.h

Const SP_SERIALCOMM=$00000001

' Provider SubTypes

Const PST_UNSPECIFIED=$00000000
Const PST_RS232=$00000001
Const PST_PARALLELPORT =$00000002
Const PST_RS422=$00000003
Const PST_RS423=$00000004
Const PST_RS449=$00000005
Const PST_MODEM=$00000006
Const PST_FAX=$00000021
Const PST_SCANNER=$00000022
Const PST_NETWORK_BRIDGE =$00000100
Const PST_LAT=$00000101
Const PST_TCPIP_TELNET =$00000102
Const PST_X25=$00000103

' Provider capabilities flags.

Const PCF_DTRDSR=$0001
Const PCF_RTSCTS=$0002
Const PCF_RLSD=$0004
Const PCF_PARITY_CHECK=$0008
Const PCF_XONXOFF =$0010
Const PCF_SETXCHAR=$0020
Const PCF_TOTALTIMEOUTS =$0040
Const PCF_INTTIMEOUTS =$0080
Const PCF_SPECIALCHARS=$0100
Const PCF_16BITMODE =$0200

' Comm provider settable parameters.

Const SP_PARITY =$0001
Const SP_BAUD =$0002
Const SP_DATABITS =$0004
Const SP_STOPBITS =$0008
Const SP_HANDSHAKING=$0010
Const SP_PARITY_CHECK =$0020
Const SP_RLSD =$0040

' Settable baud rates in the provider.

Const BAUD_075=$00000001
Const BAUD_110=$00000002
Const BAUD_134_5=$00000004
Const BAUD_150=$00000008
Const BAUD_300=$00000010
Const BAUD_600=$00000020
Const BAUD_1200 =$00000040
Const BAUD_1800 =$00000080
Const BAUD_2400 =$00000100
Const BAUD_4800 =$00000200
Const BAUD_7200 =$00000400
Const BAUD_9600 =$00000800
Const BAUD_14400=$00001000
Const BAUD_19200=$00002000
Const BAUD_38400=$00004000
Const BAUD_56K=$00008000
Const BAUD_128K =$00010000
Const BAUD_115200 =$00020000
Const BAUD_57600=$00040000
Const BAUD_USER =$10000000

' Settable Data Bits

Const DATABITS_5=1
Const DATABITS_6=2
Const DATABITS_7=4
Const DATABITS_8=8
Const DATABITS_16=16
Const DATABITS_16X=32

' Settable Stop and Parity bits.

Const STOPBITS_10=1
Const STOPBITS_15=2
Const STOPBITS_20=4
Const PARITY_NONE=$100
Const PARITY_ODD=$200
Const PARITY_EVEN=$400
Const PARITY_MARK=$800
Const PARITY_SPACE=$1000

Type COMMPROP 
	Field wPacketLength:Short
	Field wPacketVersion:Short
	Field dwServiceMask
	Field dwReserved1
	Field dwMaxTxQueue
	Field dwMaxRxQueue
	Field dwMaxBaud
	Field dwProvSubType
	Field dwProvCapabilities
	Field dwSettableParams
	Field dwSettableBaud
	Field wSettableData:Short
	Field wSettableStopParity:Short
	Field dwCurrentTxQueue
	Field dwCurrentRxQueue
	Field dwProvSpec1
	Field dwProvSpec2
	Field wcProvChar0:Short
End Type

' Set dwProvSpec1 To COMMPROP_INITIALIZED To indicate that wPacketLength is valid before a call To GetCommProperties(.

Const COMMPROP_INITIALIZED =$E73CF52E

Type COMMSTAT
	Const fCtsHold=1
	Const fDsrHold=2
	Const fRlsdHold=4
	Const fXoffHold=8
	Const fXoffSent=16
	Const fEof=32
	Const fTxim=64
	Field fFlags
	Field cbInQue
	Field cbOutQue
End Type

' DTR Control Flow Values.

Const DTR_CONTROL_DISABLE=0
Const DTR_CONTROL_ENABLE=1
Const DTR_CONTROL_HANDSHAKE=2

' RTS Control Flow Values

Const RTS_CONTROL_DISABLE=0
Const RTS_CONTROL_ENABLE=1
Const RTS_CONTROL_HANDSHAKE=2
Const RTS_CONTROL_TOGGLE=3

Const NOPARITY=0
Const ODDPARITY=1
Const EVENPARITY=2
Const MARKPARITY=3
Const SPACEPARITY=4

Const ONESTOPBIT=0
Const ONE5STOPBITS=1
Const TWOSTOPBITS=2

Type DCB
	Const fBinary=1		' Binary Mode (skip Eof check
	Const fParity=2		' Enable parity checking
	Const fOutxCtsFlow=4	' CTS handshaking on output 
	Const fOutxDsrFlow=8	' DSR handshaking on output 
	Const fDtrControl=48	' DTR Flow control
	Const fDsrSensitivity=64	' DSR Sensitivity
	Const fTXContinueOnXoff=128	' Continue TX when Xoff sent 
	Const fOutX=256		' Enable output X-ON/X-OFF
	Const fInX=512		' Enable Input X-ON/X-OFF 
	Const fErrorChar=1024	' Enable Err Replacement
	Const fNul=2048		' Enable Null stripping 
	Const fRtsControl=4096+8192	' Rts Flow control
	Const fAbortOnError=16384 ' Abort all reads And writes on Error 

	Field DCBlength		' SizeOf(DCB 
	Field BaudRate 		' Baudrate at which running 
	Field flags
	Field wReserved:Short 	' Not currently used
	Field XonLim:Short		' Transmit X-ON threshold 
	Field XoffLim:Short 		' Transmit X-OFF threshold
	Field ByteSize:Byte 		' Number of bits/Byte, 4-8
	Field Parity:Byte		' 0-4=None,Odd,Even,Mark,Space
	Field StopBits:Byte		' 0,1,2 = 1, 1.5, 2 
	Field XonChar:Byte		' Tx And Rx X-ON character
	Field XoffChar:Byte		' Tx And Rx X-OFF character 
	Field	 ErrorChar:Byte	' Error replacement char
	Field EofChar:Byte		' End of Input character
	Field EvtChar:Byte		' Received Event character
	Field wReserved1:Short	' Fill For now. 
End Type

Type COMMTIMEOUTS
	Field ReadIntervalTimeout' Maximum time between read chars. 
	Field ReadTotalTimeoutMultiplier ' Multiplier of characters.
	Field ReadTotalTimeoutConstant ' Constant in milliseconds.
	Field WriteTotalTimeoutMultiplier' Multiplier of characters.
	Field WriteTotalTimeoutConstant' Constant in milliseconds.
End Type
	
Type COMMCONFIG
	Field dwSize 				' Size of the entire struct 
	Field wVersion:Short 		' version of the structure 
	Field wReserved:Short 		' alignment 
' dcb device control block 
	Field DCBlength		' SizeOf(DCB 
	Field BaudRate 		' Baudrate at which running 
	Field flags
	Field wReserveda:Short 	' Not currently used
	Field XonLim:Short		' Transmit X-ON threshold 
	Field XoffLim:Short 		' Transmit X-OFF threshold
	Field ByteSize:Byte 		' Number of bits/Byte, 4-8
	Field Parity:Byte		' 0-4=None,Odd,Even,Mark,Space
	Field StopBits:Byte		' 0,1,2 = 1, 1.5, 2 
	Field XonChar:Byte		' Tx And Rx X-ON character
	Field XoffChar:Byte		' Tx And Rx X-OFF character 
	Field	 ErrorChar:Byte	' Error replacement char
	Field EofChar:Byte		' End of Input character
	Field EvtChar:Byte		' Received Event character
	Field wReserved1:Short	' Fill For now. 

	Field dwProviderSubType	' ordinal value For identifying provider-defined data structure format
	Field dwProviderOffset		' Specifies the offset of provider specific data Field in bytes from the start 
	Field dwProviderSize		' size of the provider-specific data Field 
	Field wcProviderData:Short	' provider-specific data 
	Field wReserved2:Short
End Type

Global _comlist:TList
Global _cominfo:TList

Function ComCount()
	enumcoms
	If _comlist Return _comlist.Count()
End Function

Function ComName$(index)
	If _comlist Return String(_comlist.valueatindex(index))
End Function

Function ComInfo$(index)
	If _cominfo Return String(_cominfo.valueatindex(index))
End Function

Function enumcoms()
	Local buffer:Byte[8192]
	Local buffer2:Byte[8192]
	Local res=QueryDosDeviceA(Null,buffer,8192)	'"COM1".toCString()
	Local list$=String.frombytes(buffer,res)
	Local p,q
	_comlist=New TList
	_cominfo=New TList
	While p<res
		q=list.find(Chr(0),p)
		If q=-1 q=list.length
		Local l$=list[p..q]
		If l[..3]="COM"
			_comlist.AddLast l '+":"
			Local res2=QueryDosDeviceA(l.toCString(),buffer2,8192)
			Local info$=String.frombytes(buffer2,res2)
			info=info.Replace(Chr(0),"~~z")
			_cominfo.AddLast(info)
		EndIf
		p=q+1
	Wend
End Function

Function openserial(comport$,baud)
	Local hcom
	Local cto:COMMTIMEOUTS
	Local state:DCB
	Local res

	hcom=CreateFileA(comport,GENERIC_READ|GENERIC_WRITE,0,Null,OPEN_EXISTING,0,0)
'	If hcom=INVALID_HANDLE_VALUE hcom=CreateFileA(comport,GENERIC_READ|GENERIC_WRITE,0,Null,OPEN_ALWAYS,0,Null)
	If hcom=INVALID_HANDLE_VALUE Return

	cto=New COMMTIMEOUTS
	cto.ReadTotalTimeoutConstant=1
	res=SetCommTimeouts(hcom,cto)
	If res=0 DebugLog "SetCommTimeouts failed err="+GetLastError()

	state=New DCB
	state.DCBlength=SizeOf(state)
	res=GetCommState(hcom,state)
	If res=0 DebugLog "GetCommState failed err="+GetLastError()

	state.BaudRate=BAUD_115200	'baud '// = 115200//38400
	state.flags=DTR_CONTROL_ENABLE|RTS_CONTROL_ENABLE
	state.ByteSize = 8
	state.Parity = NOPARITY
	state.StopBits = ONESTOPBIT
	res=SetCommState(hcom,state)
	If res=0 DebugLog "SetCommState failed err="+GetLastError()

	Return hcom
End Function

Function closeserial(handle)
	fdClose(handle)	
End Function
