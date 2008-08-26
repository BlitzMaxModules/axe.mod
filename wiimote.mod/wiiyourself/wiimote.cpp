// _______________________________________________________________________________
//
//	 - WiiYourself! - native C++ Wiimote library  v0.99b.
//	  (c) gl.tter 2007 - http://wiiyourself.gl.tter.org
//
//	  see License.txt for conditions of use.  see History.txt for change log.
// _______________________________________________________________________________
//
//  wiimote.cpp  (tab = 4 spaces)

#define _ASSERT

// disable warning "C++ exception handler used, but unwind semantics are not enabled."
//				   in <xstring> (I don't use it - or just enable C++ excepionts)
#pragma warning(disable: 4530)
#include "wiimote.h"
#pragma warning(default: 4530)
#include <setupapi.h>
extern "C" {
//#include <hidsdi.h>		// from WinDDK
}
#include <sys/types.h>	// for _start
#include <sys/stat.h>	// "
#include <process.h>	// for _beginthreadex()
#include <math.h>		// for orientation
#include <mmreg.h>		// for WAVEFORMATEXTENSIBLE
#include <mmsystem.h>	// for timeGetTime()

#include <ddk/hidsdi.h>

// auto-link with the necessary libs
//#pragma comment(lib, "setupapi.lib")	
//#pragma comment(lib, "hid.lib")		// from WinDDK
#pragma comment(lib, "winmm.lib")	// timeGetTime()
#pragma comment(lib, "user32.lib")	// MessageBox
#ifdef _DEBUG
# ifdef _UNICODE
#  pragma comment(lib, "WiiYourself!_dU.lib")
# else
#  pragma comment(lib, "WiiYourself!_d.lib")
# endif
#else // RELEASE
# ifdef _UNICODE
#  pragma comment(lib, "WiiYourself!_U.lib")
# else
#  pragma comment(lib, "WiiYourself!.lib")
# endif
#endif

// ------------------------------------------------------------------------------------
// helpers
// ------------------------------------------------------------------------------------

int sign(int val)  { return (val<0)? -1 : 1; }
int square(int val) { return val*val; }
int min(int a,int b) { return (a<b)?a:b;}

//template<class T> __forceinline T square(const T& val)  { return val*val; }

#define ARRAY_SIZE(array)	(sizeof(array)/sizeof(array[0]))

// ------------------------------------------------------------------------------------
//  Tracing & Debugging
// ------------------------------------------------------------------------------------
//  TRACE and WARN currently use OutputDebugString() via _TRACE - change to suit
#define PREFIX				 _T("WiiYourself! : ")
//#define TRACE(fmt, ...)		 _TRACE(PREFIX		    fmt		     _T("\n"), __VA_ARGS__)
//#define WARN(fmt, ...)		 _TRACE(PREFIX _T("* ") fmt _T(" *") _T("\n"), __VA_ARGS__)

// uncomment any of these for deeper debugging
//#define DEEP_TRACE(fmt, ...) _TRACE(PREFIX _T("|") fmt _T("\n"), __VA_ARGS__)
//#define BEEP_DEBUG_READS
//#define BEEP_DEBUG_WRITES
//#define BEEP_ON_ORIENTATION_ESTIMATE
//#define BEEP_ON_PERIODIC_STATUSREFRESH

// internals:
//  auto-strip code from these macros if they weren't defined
#ifndef TRACE
# define TRACE(...)
#endif
#ifndef DEEP_TRACE
# define DEEP_TRACE(...)		
#endif
#ifndef WARN
# define WARN(...)
#endif
// ------------------------------------------------------------------------------------
static void _cdecl _TRACE (const TCHAR* fmt, ...)
	{
	static TCHAR buffer[256];
	if (!fmt) return;

	va_list	 argptr;
	va_start (argptr, fmt);
//	_vsntprintf_s(buffer, ARRAY_SIZE(buffer), _TRUNCATE, fmt, argptr);
	va_end (argptr);

	OutputDebugString(buffer);
	}
// ------------------------------------------------------------------------------------
static void _cdecl _Message (const TCHAR* fmt, ...)
	{
	static TCHAR buffer[256];
	if (!fmt) return;

	va_list	 argptr;
	va_start (argptr, fmt);
//	_vsntprintf_s(buffer, ARRAY_SIZE(buffer), _TRUNCATE, fmt, argptr);
	va_end (argptr);

	MessageBox(NULL, buffer, PREFIX, MB_OK|MB_TOPMOST);
	}

// ------------------------------------------------------------------------------------
//  wiimote
// ------------------------------------------------------------------------------------
// class statics
HMODULE		   wiimote::HidDLL				  = NULL;
unsigned	   wiimote::_TotalCreated		  = 0;
unsigned	   wiimote::_TotalConnected		  = 0;
hidwrite_ptr   wiimote::_HidD_SetOutputReport = NULL;
const unsigned wiimote::FreqLookup [10] = // (keep in sync with 'speaker_freq')
								{    0, 4200, 3920, 3640, 3360,
								  3130,	2940, 2760, 2610, 2470 };
const TCHAR*   wiimote::ButtonNameFromBit		 [16] =
								{ _T("Left") , _T("Right"), _T("Down"), _T("Up"),
								  _T("Plus") , _T("??")   , _T("??")  , _T("??") ,
								  _T("Two")  , _T("One")  , _T("B")   , _T("A") ,
								  _T("Minus"), _T("??")   , _T("??")  , _T("Home") };
const TCHAR*   wiimote::ClassicButtonNameFromBit [16] =
								{ _T("??")   , _T("TrigR")  , _T("Plus") , _T("Home"),
								  _T("Minus"), _T("TrigL") , _T("Down") , _T("Right") ,
								  _T("Up")   , _T("Left")   , _T("ZR")   , _T("X") ,
								  _T("A")    , _T("Y")      , _T("B")    , _T("ZL") };
// ------------------------------------------------------------------------------------
wiimote::wiimote ()
	:
	DataRead			 (CreateEvent(NULL, FALSE, FALSE, NULL)),
	Handle				 (INVALID_HANDLE_VALUE),
	bStatusReceived		 (false), // for output method detection
	bConnectionLost		 (false), // set if write fails after connection
	bUseHIDwrite		 (false), // if OS supports it
	ChangedCallback		 (NULL),
	CallbackTriggerFlags (CHANGED_ALL),
	InternalChanged		 (NO_CHANGE),
	CurrentSample		 (NULL),
	HIDwriteThread		 (NULL),
	ReadParseThread		 (NULL),
	SampleThread		 (NULL),
	AsyncRumbleThread	 (NULL),
	AsyncRumbleTimeout	 (0)
	{
	_ASSERT(DataRead != INVALID_HANDLE_VALUE);
				
	// if this is the first wiimote object, detect & enable HID write support
	if(++_TotalCreated == 1)
		{
		_ASSERT(!HidDLL);
		HidDLL = LoadLibrary(_T("hid.dll"));
		_ASSERT(HidDLL);
		if(!HidDLL)
			WARN(_T("Couldn't load hid.dll - shouldn't happen!"));
		else{
			_HidD_SetOutputReport = (hidwrite_ptr)
									GetProcAddress(HidDLL, "HidD_SetOutputReport");
			if(_HidD_SetOutputReport)
				TRACE(_T("OS supports HID writes."));
			else
				TRACE(_T("OS doesn't support HID writes."));
			}
		}

	// clear our public and private state data completely (including deadzones)
	Clear		  (true);
	Internal.Clear(true);

	// and the state history vars (for state recording)
	memset(&Recording, 0, sizeof(Recording));

	// for overlapped IO (Read/WriteFile)
	memset(&Overlapped, 0, sizeof(Overlapped));
	Overlapped.hEvent	  = DataRead;
	Overlapped.Offset	  =
	Overlapped.OffsetHigh = 0;

	// for async HID output method
	InitializeCriticalSection(&HIDwriteQueueLock);
	// for polling
	InitializeCriticalSection(&StateLock);

	// request millisecond timer accuracy
	timeBeginPeriod(1);		
	}
// ------------------------------------------------------------------------------------
wiimote::~wiimote ()
	{
	Disconnect();

	// events & critical sections are kept open for the lifetime of the object,
	//  so tidy them up here:
	if(DataRead != INVALID_HANDLE_VALUE)
		CloseHandle(DataRead);

	DeleteCriticalSection(&HIDwriteQueueLock);
	DeleteCriticalSection(&StateLock);

	// tidy up timer accuracy request
	timeEndPeriod(1);		

	// release HID DLL (for dynamic HID write method)
	if((--_TotalCreated == 0) && HidDLL)
		{
		FreeLibrary(HidDLL);
		HidDLL				  = NULL;
		_HidD_SetOutputReport = NULL;
		}
	}

// ------------------------------------------------------------------------------------
bool wiimote::Connect (unsigned wiimote_index, bool force_hidwrites)
	{
	if(wiimote_index == FIRST_AVAILABLE)
		TRACE(_T("Connecting first available Wiimote:"));
	else
		TRACE(_T("Connecting Wiimote %u:"), wiimote_index);

	// auto-disconnect if user is being naughty
	if(IsConnected())
		Disconnect();

	// get the GUID of the HID class
	GUID guid;
	HidD_GetHidGuid(&guid);

	// get a handle to all devices that are part of the HID class
	// Brian: Fun fact:  DIGCF_PRESENT worked on my machine just fine.  I reinstalled
	//        Vista, and now it no longer finds the Wiimote with that parameter enabled...
	HDEVINFO dev_info = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_DEVICEINTERFACE);// | DIGCF_PRESENT);
	if(!dev_info) {
		WARN(_T("couldn't get device info"));
		return false;
		}

	// enumerate the devices
	SP_DEVICE_INTERFACE_DATA didata;
	didata.cbSize = sizeof(didata);
	
	unsigned index			=  0;
	int		 wiimotes_found = -1;
	while(SetupDiEnumDeviceInterfaces(dev_info, NULL, &guid, index, &didata))
		{
		// get the buffer size for this device detail instance
		DWORD req_size = 0;
		SetupDiGetDeviceInterfaceDetail(dev_info, &didata, NULL, 0, &req_size, NULL);

		// (bizarre way of doing it) create a buffer large enough to hold the
		//  fixed-size detail struct components, and the variable string size
		SP_DEVICE_INTERFACE_DETAIL_DATA *didetail =
								(SP_DEVICE_INTERFACE_DETAIL_DATA*) new BYTE[req_size];
		_ASSERT(didetail);
		didetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		// now actually get the detail struct
		if(!SetupDiGetDeviceInterfaceDetail(dev_info, &didata, didetail,
										    req_size, &req_size, NULL)) {
			WARN(_T("couldn't get devinterface info for %u"), index);
			break;
			}

		// open a handle to the device to query it (this will succeed even if
		//  the wiimote is already Connect()ed or in use elsewhere)
		DEEP_TRACE(_T(".. querying device %s"), didetail->DevicePath);
		Handle = CreateFile(didetail->DevicePath, 0, FILE_SHARE_READ|FILE_SHARE_WRITE,
												  NULL, OPEN_EXISTING, 0, NULL);
		if(Handle == INVALID_HANDLE_VALUE) {
			DEEP_TRACE(_T(".... failed with err %x (probably harmless)."), 
					   GetLastError());
			goto skip;
			}
	
		// get the device attributes
		HIDD_ATTRIBUTES attrib;
		attrib.Size = sizeof(attrib);
		if(HidD_GetAttributes(Handle, &attrib))
			{
			// is this a wiimote?
			if((attrib.VendorID != VID) || (attrib.ProductID != PID))
				goto skip;

			// yes, but is it the one we're interested in?
			wiimotes_found++;
			if((wiimote_index != FIRST_AVAILABLE) &&
			   (wiimote_index != wiimotes_found))
				goto skip;

			// the wiimote is installed, but it may not be currently paired:
			if(wiimote_index == FIRST_AVAILABLE)
				TRACE(_T(".. opening Wiimote %u:"), wiimotes_found);
			else
				TRACE(_T(".. opening:"));

			// re-open the handle, but this time we don't allow write sharing
			//  (that way subsequent calls can still _discover_ wiimotes above, but
			//   will correctly fail here if they're already connected)
			CloseHandle(Handle);
			Handle = CreateFile(didetail->DevicePath, GENERIC_READ|GENERIC_WRITE,
													FILE_SHARE_READ,
													NULL, OPEN_EXISTING,
													FILE_FLAG_OVERLAPPED, NULL);
			if(Handle == INVALID_HANDLE_VALUE) {
				TRACE(_T(".... failed with err %x (already connected?)"),
					  GetLastError());
				goto skip;
				}

			// clear the wiimote state & buffers
			Clear		  (false);		// preserves existing deadzones
			Internal.Clear(false);		// "
			InternalChanged = NO_CHANGE;

			memset(ReadBuff , 0, sizeof(ReadBuff));
			memset(WriteBuff, 0, sizeof(WriteBuff));

			bConnectionLost = false;
			
			// enable async reading
			BeginAsyncRead();

			// autodetect which write method the Bluetooth stack supports,
			//  by requesting the wiimote status report:
			if(force_hidwrites && !_HidD_SetOutputReport) {
				TRACE(_T(".. can't force HID writes (not supported)"));
				force_hidwrites = false;
				}

			if(force_hidwrites)
				TRACE(_T(".. (HID writes forced)"));
			else{
				//  - try WriteFile() first as it's the most efficient (it uses
				//     harware interrupts where possible and is async-capable):
				bUseHIDwrite = false;
				RequestStatusReport();
				//  and wait for the report to arrive:
				DWORD last_time = timeGetTime();
				while(!bStatusReceived && ((timeGetTime()-last_time) < 500))
					Sleep(5);
				TRACE(_T(".. WriteFile() %s."), bStatusReceived? _T("succeeded") :
																 _T("failed"));
				}

			// try HID write method (if supported)
			if(!bStatusReceived && _HidD_SetOutputReport)
				{
				//  (we didn't get it, so) try with the HID output method:
				bUseHIDwrite = true;
				RequestStatusReport();
				// wait for the report to arrive:
				DWORD last_time = timeGetTime();
				while(!bStatusReceived && ((timeGetTime()-last_time) < 500))
					Sleep(5);
				// did we get it?
				TRACE(_T(".. HID write %s."), bStatusReceived? _T("succeeded") :
															   _T("failed"));
				}

			// still failed?
			if(!bStatusReceived) {
				WARN(_T("output failed - wiimote is not connected (or confused)."));
				Disconnect();
				goto skip;
				}

			// connected succesfully:
			_TotalConnected++;
			// reset it
			Reset();
			// read the wiimote calibration info
			ReadCalibration();
			// refresh the public state from the internal one (so that extension
			//  status etc. is available straight away)
			_RefreshState(false);
			// and show when we want to trigger the next periodic status request
			//  (for battery level and connection loss detection)
			NextStatusTime = timeGetTime() + REQUEST_STATUS_EVERY_MS;

			// tidy up
			delete[] (BYTE*)didetail;
			break;
			}
skip:
		// tidy up
		delete[] (BYTE*)didetail;

		if(Handle != INVALID_HANDLE_VALUE) {
			CloseHandle(Handle);
			Handle = INVALID_HANDLE_VALUE;
			}
		// if this was the specified wiimote index, abort
		if((wiimote_index != FIRST_AVAILABLE) &&
		   (wiimote_index == (wiimotes_found-1)))
		   break;

		index++;
		}

	// clean up our list
	SetupDiDestroyDeviceInfoList(dev_info);

	if(IsConnected()) {
		TRACE(_T(".. Connected!"));
		return true;
		}
	TRACE(_T(".. connection failed."));
	return false;
	}
// ------------------------------------------------------------------------------------
void wiimote::Disconnect ()
	{
	if(Handle == INVALID_HANDLE_VALUE)
		return;

	TRACE(_T("Disconnect()."));
	
	if(IsConnected())
		{
		_ASSERT(_TotalConnected > 0); // sanity
		_TotalConnected--;
		
		if(!bConnectionLost)
			Reset();
		}

	CloseHandle(Handle);
	Handle = INVALID_HANDLE_VALUE;

	bStatusReceived = false;

	// close the read thread
	if(ReadParseThread) {
		// unblock it so it can realise we're closing and exit straight away
		SetEvent(DataRead);
		CloseHandle(ReadParseThread);
		ReadParseThread	   = NULL;
		}
	// close the rumble thread
	if(AsyncRumbleThread) {
		CloseHandle(AsyncRumbleThread);
		AsyncRumbleThread  = NULL;
		AsyncRumbleTimeout = 0;
		}
	// and the sample streaming thread
	if(SampleThread) {
		CloseHandle(SampleThread);
		SampleThread	   = NULL;
		}

	// and clear the state
	Clear		  (false); // (preserves deadzones)
	Internal.Clear(false); // "
	InternalChanged = NO_CHANGE;
	}
// ------------------------------------------------------------------------------------
inline void wiimote::Reset ()
	{
	TRACE(_T("Resetting wiimote."));
	// stop updates (by setting report type to non-continous buttons-only)
	SetReportType(IN_BUTTONS, false);
	SetRumble	 (false);
	SetLEDs		 (0x00);
	MuteSpeaker  (true);
	EnableSpeaker(false);
	Sleep(100); // avoids loosing the extension calibration data on Connect()
	}
// ------------------------------------------------------------------------------------
unsigned __stdcall wiimote::ReadParseThreadfunc (void* param)
	{
	// this thread waits for the async ReadFile() to deliver data & parses it.
	//  it also requests periodic status updates, deals with connection loss
	//  and ends state recordings with a specific duration:
	_ASSERT(param);
	wiimote    &remote	   = *(wiimote*)param;
	OVERLAPPED &overlapped = remote.Overlapped;
	unsigned exit_code	   = 0; // (success)

	while(1)
		{
		// wait until the overlapped read completes, or the timeout is reached:
		DWORD wait = WaitForSingleObject(overlapped.hEvent, 500);

		// before we deal with the result, let's do some housekeeping:

		//  if we were recently Disconect()ed, exit now
		if(remote.Handle == INVALID_HANDLE_VALUE) {
			DEEP_TRACE(_T("read thread: wiimote was disconnected"));
			break;
			}
		//  ditto if the connection was lost (eg. through a failed write)
		if(remote.bConnectionLost)
			{
connection_lost:
			TRACE(_T("read thread: connection to wiimote was lost"));
			remote.Disconnect();
			remote.InternalChanged = (state_change_flags)
								(remote.InternalChanged | CONNECTION_LOST);
			// report via the callback (if any)
			if(remote.ChangedCallback &&
			   (remote.CallbackTriggerFlags & CONNECTION_LOST))
				{
				remote._RefreshState(true);
				remote.ChangedCallback(remote, CONNECTION_LOST);
				}
			break;
			}

		DWORD time = timeGetTime();
		//  is a periodic status request due? (but not if we're streaming audio,
		//									   we don't want to cause a glitch)
		if(remote.IsConnected() && !remote.IsPlayingAudio() &&
		   (time > remote.NextStatusTime))
			{
#ifdef BEEP_ON_PERIODIC_STATUSREFRESH
			Beep(2000,50);
#endif
			remote.RequestStatusReport();
			// and schedule the next one
			remote.NextStatusTime = time + REQUEST_STATUS_EVERY_MS;
			}

		//  if we're state recording and have reached the specified duration, stop
		if(remote.Recording.bEnabled && (remote.Recording.EndTimeMS != UNTIL_STOP) &&
		   (time >= remote.Recording.EndTimeMS))
		   remote.Recording.bEnabled = false;

		// now handle the wait result:
		//  did the wait time out?
		if(wait == WAIT_TIMEOUT) {
			DEEP_TRACE(_T("read thread: timed out"));
			continue; // wait again
			}
		//  did an error occurr?
		if(wait != WAIT_OBJECT_0) {
			DEEP_TRACE(_T("read thread: error waiting!"));
			remote.bConnectionLost = true;
			// deal with it straight away to avoid a longer delay
			goto connection_lost;
			}
	
		// data was received:
#ifdef BEEP_DEBUG_READS
		Beep(500,1);
#endif
		DWORD read = 0;
		//  get the data read result
		GetOverlappedResult(remote.Handle, &overlapped, &read, TRUE);
		//  if we read data, parse it
		if(read) {
			DEEP_TRACE(_T("read thread: parsing data"));
			remote.OnReadData(read);
			}
		else
			DEEP_TRACE(_T("read thread: didn't get any data??"));
		}

	TRACE(_T("(ending read thread)"));
#ifdef BEEP_DEBUG_READS
	if(exit_code != 0)
		Beep(200,1000);
#endif
	return exit_code;
	}
// ------------------------------------------------------------------------------------
bool wiimote::BeginAsyncRead ()
	{
	// (this is also called before we're fully connected)
	_ASSERT(Handle != INVALID_HANDLE_VALUE);

	DEEP_TRACE(_T(".. starting async read"));
#ifdef BEEP_DEBUG_READS
	Beep(1000,1);
#endif

	DWORD read;
	if (!ReadFile(Handle, ReadBuff, REPORT_LENGTH, &read, &Overlapped)) {
		DWORD err = GetLastError();
		if(err != ERROR_IO_PENDING) {
			DEEP_TRACE(_T(".... ** ReadFile() failed! **"));
			return false;
			}
		}

	// launch the completion wait/callback thread
	if(!ReadParseThread) {
		ReadParseThread = (HANDLE)_beginthreadex(NULL, 0, ReadParseThreadfunc,
												 this, 0, NULL);
		DEEP_TRACE(_T(".... creating read thread"));
		_ASSERT(ReadParseThread);
		if(!ReadParseThread)
			return false;
		SetThreadPriority(ReadParseThread, WORKER_THREAD_PRIORITY);
		}

	// if ReadFile completed while we called, signal the thread to proceed
	if(read) {
		DEEP_TRACE(_T(".... got data right away"));
		SetEvent(DataRead);
		}
	return true;
	}
// ------------------------------------------------------------------------------------
void wiimote::OnReadData (DWORD bytes_read)
	{
	_ASSERT(bytes_read == REPORT_LENGTH);

	// copy our input buffer
	BYTE buff [REPORT_LENGTH];
	memcpy(buff, ReadBuff, bytes_read);

	// start reading again
	BeginAsyncRead();

	// parse it
	ParseInput(buff);
	}
// ------------------------------------------------------------------------------------
void wiimote::SetReportType (input_report type, bool continuous)
	{
	_ASSERT(IsConnected());
	if(!IsConnected())
		return;

	ReportType = type;

	switch(type)
		{
		case IN_BUTTONS_ACCEL_IR:
			EnableIR(wiimote_state::ir::EXTENDED);
			break;
		case IN_BUTTONS_ACCEL_IR_EXT:
			EnableIR(wiimote_state::ir::BASIC);
			break;
		default:
			DisableIR();
			break;
		}

	ClearReport();
	WriteBuff[0] = OUT_TYPE;
	WriteBuff[1] = (continuous ? 0x04 : 0x00) | GetRumbleBit();
	WriteBuff[2] = type;
	WriteReport();
	}
// ------------------------------------------------------------------------------------
void wiimote::SetLEDs (BYTE led_bits)
	{
	_ASSERT(IsConnected());
	if(!IsConnected())
		return;

	_ASSERT(led_bits <= 0x0f);
	led_bits &= 0xf;
	
	ClearReport();
	WriteBuff[0] = OUT_LEDs;
	WriteBuff[1] = (led_bits<<4) | GetRumbleBit();
	WriteReport();

	Internal.LED.Bits = led_bits;
	}
// ------------------------------------------------------------------------------------
void wiimote::SetRumble (bool on)
	{
	_ASSERT(IsConnected());
	if(!IsConnected())
		return;

	if(Internal.bRumble == on)
		return;

	Internal.bRumble = on;

	// if we're streaming audio, we don't need to send a report (sending it makes
	// the audio glitch, and the rumble bit is sent with every report anyway)
	if(IsPlayingAudio())
		return;

	ClearReport();
	WriteBuff[0] = OUT_STATUS;
	WriteBuff[1] = on? 0x01 : 0x00;
	WriteReport();
	}
// ------------------------------------------------------------------------------------
unsigned __stdcall wiimote::AsyncRumbleThreadfunc (void* param)
	{
	// auto-disables rumble after x milliseconds:
	_ASSERT(param);
	wiimote &remote = *(wiimote*)param;
	
	while(remote.IsConnected())
		{
		if(remote.AsyncRumbleTimeout)
			{
			DWORD current_time = timeGetTime();
			if(current_time >= remote.AsyncRumbleTimeout)
				{
				if(remote.Internal.bRumble)
					remote.SetRumble(false);
				remote.AsyncRumbleTimeout = 0;
				}
			Sleep(1);
			}
		else
			Sleep(4);
		}
	return 0;
	}
// ------------------------------------------------------------------------------------
void wiimote::RumbleForAsync (unsigned milliseconds)
	{
	// rumble for a fixed amount of time
	_ASSERT(IsConnected());
	if(!IsConnected())
		return;

	SetRumble(true);

	// show how long thread should wait to disable rumble again
	// (it it's currently rumbling it will just extend the time)
	AsyncRumbleTimeout = timeGetTime() + milliseconds;

	// create the thread?
	if(AsyncRumbleThread)
		return;

	AsyncRumbleThread = (HANDLE)_beginthreadex(NULL, 0, AsyncRumbleThreadfunc, this,
											   0, NULL);
	_ASSERT(AsyncRumbleThread);
	if(!AsyncRumbleThread) {
		WARN(_T("couldn't create rumble thread!"));
		return;
		}
	SetThreadPriority(AsyncRumbleThread, WORKER_THREAD_PRIORITY);
	}
// ------------------------------------------------------------------------------------
void wiimote::RequestStatusReport ()
	{
	// (this can be called before we're fully connected)
	_ASSERT(Handle != INVALID_HANDLE_VALUE);
	if(Handle == INVALID_HANDLE_VALUE)
		return;

	ClearReport();
	WriteBuff[0] = OUT_STATUS;
	WriteBuff[1] = GetRumbleBit();
	WriteReport();
	}
// ------------------------------------------------------------------------------------
bool wiimote::ReadAddress (int address, short size)
	{
	// asynchronous
	ClearReport();
	WriteBuff[0] = OUT_READMEMORY;
	WriteBuff[1] = ((address & 0xff000000) >> 24) | GetRumbleBit();
	WriteBuff[2] =  (address & 0x00ff0000) >> 16;
	WriteBuff[3] =  (address & 0x0000ff00) >>  8;
	WriteBuff[4] =  (address & 0x000000ff);
	WriteBuff[5] =  (size	 & 0xff00	 ) >>  8;
	WriteBuff[6] =  (size	 & 0xff);
	return WriteReport();
	}
// ------------------------------------------------------------------------------------
void wiimote::WriteData (int address, BYTE size, const BYTE* buff)
	{
	// asynchronous
	ClearReport();
	WriteBuff[0] = OUT_WRITEMEMORY;
	WriteBuff[1] = ((address & 0xff000000) >> 24) | GetRumbleBit();
	WriteBuff[2] =  (address & 0x00ff0000) >> 16;
	WriteBuff[3] =  (address & 0x0000ff00) >>  8;
	WriteBuff[4] =  (address & 0x000000ff);
	WriteBuff[5] = size;
	memcpy(WriteBuff+6, buff, size);
	WriteReport();
	}
// ------------------------------------------------------------------------------------
int wiimote::ParseInput (BYTE* buff)
	{
	int changed = 0;

	// lock our internal state (so RefreshState() is blocked until we're done
	EnterCriticalSection(&StateLock);

	switch(buff[0])
		{
		case IN_BUTTONS:
			DEEP_TRACE(_T(".. parsing buttons."));
			changed |= ParseButtons(buff);
			break;
		case IN_BUTTONS_ACCEL:
			DEEP_TRACE(_T(".. parsing buttons/accel."));
			changed |= ParseButtons(buff);
			changed |= ParseAccel  (buff);
			break;
		case IN_BUTTONS_ACCEL_EXT:
			DEEP_TRACE(_T(".. parsing extenion/accel."));
			changed |= ParseButtons  (buff);
			changed |= ParseAccel    (buff);
			DecryptBuffer (buff, REPORT_LENGTH);
			changed |= ParseExtension(buff, 6);
			break;
		case IN_BUTTONS_ACCEL_IR:
			DEEP_TRACE(_T(".. parsing ir/accel."));
			changed |= ParseButtons(buff);
			changed |= ParseAccel  (buff);
			changed |= ParseIR	   (buff);
			break;
		case IN_BUTTONS_ACCEL_IR_EXT:
			DEEP_TRACE(_T(".. parsing ir/extenion/accel."));
			changed |= ParseButtons(buff);
			changed |= ParseAccel  (buff);
			changed |= ParseIR	   (buff);
			DecryptBuffer (buff, REPORT_LENGTH);
			changed |= ParseExtension(buff, 16);
			break;
		case IN_READADDRESS:
			DEEP_TRACE(_T(".. parsing read address."));
			changed |= ParseButtons	   (buff);
			changed |= ParseReadAddress(buff);
			break;
		case IN_STATUS:
			{
			DEEP_TRACE(_T(".. parsing status."));
			changed |= ParseStatus(buff);
			// show that we received the status report (used for output method
			//  detection during Connect())
			bStatusReceived = true;
			}
			break;
		
		default:
			DEEP_TRACE(_T(".. ** Unknown input ** (happens)."));
			///_ASSERT(0);
			//Debug.WriteLine("Unknown report type: " + type.ToString());
			LeaveCriticalSection(&StateLock);
			return false;
		}

	// if we're recording and some state we care about has changed, insert it into
	//  the state history
	if(Recording.bEnabled && (changed & Recording.TriggerFlags))
		{
		DEEP_TRACE(_T(".. adding state to history"));
		state_event event;
		event.time_ms = timeGetTime();
		event.state	  = *(wiimote_state*)this;
		Recording.StateHistory->push_back(event);
		}

	// for polling: show which state has changed since the last RefreshState()
	InternalChanged = (state_change_flags)(InternalChanged | changed);

	// callbacks: call it (if set & state the app is interested in has changed)
	if(ChangedCallback && (changed & CallbackTriggerFlags)) {
		_RefreshState(true);
		DEEP_TRACE(_T(".. calling state change callback"));
		ChangedCallback(*this, (state_change_flags)changed);
		}
	
	LeaveCriticalSection(&StateLock);

	DEEP_TRACE(_T(".. parse complete."));
	return true;
	}
// ------------------------------------------------------------------------------------
state_change_flags wiimote::_RefreshState (bool for_callbacks)
	{
	// nothing changed since the last call?
	if(InternalChanged == NO_CHANGE)
		return NO_CHANGE;

	// copy the internal state to our public data:
	//  synchronise the interal state with the read/parse thread (we don't want
	//   values changing during the copy)
	if(!for_callbacks)
		EnterCriticalSection(&StateLock);
	
	// remember which state changed since the last call
	state_change_flags changed = InternalChanged;
	
	// preserve the application-set deadzones (if any)
	 joystick::deadzone nunchuk_deadzone	  = Nunchuk.Joystick.DeadZone;
	 joystick::deadzone classic_joyl_deadzone = ClassicController.JoystickL.DeadZone;
	 joystick::deadzone classic_joyr_deadzone = ClassicController.JoystickR.DeadZone;
		
	 // copy the internal state to the public one
	 *(wiimote_state*)this = Internal;
	 if(!for_callbacks)
		InternalChanged = NO_CHANGE;
	 
	 // restore the application-set deadzones
	 Nunchuk.Joystick.DeadZone			  = nunchuk_deadzone;
	 ClassicController.JoystickL.DeadZone = classic_joyl_deadzone;
	 ClassicController.JoystickR.DeadZone = classic_joyr_deadzone;

	 if(!for_callbacks)
		 LeaveCriticalSection(&StateLock);
	
	return changed;
	}
// ------------------------------------------------------------------------------------
void wiimote::InitializeExtension ()
	{
	WriteData  (REGISTER_EXTENSION_INIT, 0x00);
	ReadAddress(REGISTER_EXTENSION_TYPE, 2);
	}
// ------------------------------------------------------------------------------------
void wiimote::DecryptBuffer (BYTE *buff, unsigned size)
	{
	for(unsigned i=0; i<size; i++)
		buff[i] = ((buff[i] ^ 0x17) + 0x17) & 0xff;
	}
// ------------------------------------------------------------------------------------
int wiimote::ParseStatus (BYTE* buff)
	{
	// parse the buttons
	int changed = ParseButtons(buff);
			
	// get the battery level
	BYTE battery_raw = buff[6];
	if(Internal.BatteryRaw != battery_raw)
		changed |= BATTERY_CHANGED;
	Internal.BatteryRaw	 = battery_raw;
	// it is *estimated* that 200 is the maximum possible battery level
	Internal.BatteryPercent = battery_raw / 2;

	// leds
	BYTE leds = buff[3] >> 4;
	if(leds != Internal.LED.Bits)
		changed |= LEDS_CHANGED;
	Internal.LED.Bits = leds;

	bool extension = ((buff[3] & 0x02) != 0);
	if(extension)
		{
		if(!Internal.bExtension)//(ExtensionType == wiimote_state::NONE))// ||
		   //(ExtensionType == wiimote_state::PARTIALLY_INSERTED))
			{
			TRACE(_T("Extension connected:"));
			Internal.bExtension = extension;
			InitializeExtension();
			// reenable reports
//			SetReportType(ReportType);
			}
		}
	else if(Internal.bExtension)
		{
		TRACE(_T("Extension disconnected."));
		Internal.bExtension    = false;
		Internal.ExtensionType = wiimote_state::NONE;
		changed		 |= EXTENSION_DISCONNECTED;
		// renable reports
		SetReportType(ReportType);
		}
	
	return changed;
	}
// ------------------------------------------------------------------------------------
int wiimote::ParseButtons (BYTE* buff)
	{
	int changed = 0;
	
	WORD bits = *(WORD*)(buff+1);
	if(bits != Internal.Button.Bits)
		changed |= BUTTONS_CHANGED;
	Internal.Button.Bits = bits;
	
	return changed;
	}
// ------------------------------------------------------------------------------------
bool wiimote::EstimateOrientationFrom (wiimote_state::acceleration &accel)
	{
	// Orientation estimate from acceleration data (shared between wiimote and nunchuk)
	//  return true if the orientation was updated

	//  assume the controller is stationary if the acceleration vector is near
	//  1g for several updates (this may not always be correct)
	float length_sq = square(accel.X) + square(accel.Y) + square(accel.Z);

	// TODO: as I'm comparing squared length, I really need different
	//		  min/max epsilons...
	#define DOT(x1,y1,z1, x2,y2,z2)	((x1*x2) + (y1*y2) + (z1*z2))

	static const float epsilon = 0.2f;
	if((length_sq >= (1.f-epsilon)) && (length_sq <= (1.f+epsilon)))
		{
		if(++WiimoteNearGUpdates < 2)
			return false;
		
		// wiimote seems to be stationary:  normalize the current acceleration
		//  (ie. the assumed gravity vector)
		float inv_len = 1.f / sqrtf(length_sq);
		float x = accel.X * inv_len;
		float y = accel.Y * inv_len;
		float z = accel.Z * inv_len;

		// copy the values
		accel.Orientation.X = x;
		accel.Orientation.Y = y;
		accel.Orientation.Z = z;

		// and extract pitch & roll from them:
		// (may not be optimal)
		float pitch = -asinf(y) * 57.2957795f;
		float roll  =  asinf(x) * 57.2957795f;
		if(z < 0) {
			pitch = (y < 0)?  180 - pitch : -180 - pitch;
			roll  = (x < 0)? -180 - roll  :  180 - roll;
			}

		accel.Orientation.Pitch = pitch;
		accel.Orientation.Roll  = roll;

		// show that we just updated orientation
		accel.Orientation.UpdateAge = 0;
#ifdef BEEP_ON_ORIENTATION_ESTIMATE
		Beep(2000, 1);
#endif
		return true; // updated
		}

	// not updated this time:
	WiimoteNearGUpdates	= 0;
	// age the last orientation update
	accel.Orientation.UpdateAge++;
	return false;
	}
// ------------------------------------------------------------------------------------
void wiimote::ApplyJoystickDeadZones (wiimote_state::joystick &joy)
	{
	// apply the deadzones to each axis (if set)
	if((joy.DeadZone.X > 0.f) && (joy.DeadZone.X <= 1.f))
		{
		if(fabs(joy.X) <= joy.DeadZone.X)
			joy.X = 0;
		else{
			joy.X -= joy.DeadZone.X * sign(joy.X);
			joy.X /= 1.f - joy.DeadZone.X;
			}
		}
	if((joy.DeadZone.Y > 0.f) && (joy.DeadZone.Y <= 1.f))
		{
		if(fabs(joy.Y) <= joy.DeadZone.Y)
			joy.Y = 0;
		else{
			joy.Y -= joy.DeadZone.Y * sign(joy.Y);
			joy.Y /= 1.f - joy.DeadZone.Y;
			}
		}
	}
// ------------------------------------------------------------------------------------
int wiimote::ParseAccel (BYTE* buff)
	{
	int changed = 0;
	
	BYTE raw_x = buff[3];
	BYTE raw_y = buff[4];
	BYTE raw_z = buff[5];

	if((raw_x != Internal.Acceleration.RawX) ||
	   (raw_y != Internal.Acceleration.RawY) ||
	   (raw_z != Internal.Acceleration.RawZ))
	   changed |= ACCEL_CHANGED;
	
	Internal.Acceleration.RawX = raw_x;
	Internal.Acceleration.RawY = raw_y;
	Internal.Acceleration.RawZ = raw_z;

	Internal.Acceleration.X = ((float)Internal.Acceleration.RawX  - Internal.CalibrationInfo.X0) / 
								   ((float)Internal.CalibrationInfo.XG - Internal.CalibrationInfo.X0);
	Internal.Acceleration.Y = ((float)Internal.Acceleration.RawY  - Internal.CalibrationInfo.Y0) /
								   ((float)Internal.CalibrationInfo.YG - Internal.CalibrationInfo.Y0);
	Internal.Acceleration.Z = ((float)Internal.Acceleration.RawZ  - Internal.CalibrationInfo.Z0) /
								   ((float)Internal.CalibrationInfo.ZG - Internal.CalibrationInfo.Z0);

	// see if we can estimate the orientation from the current values
	if(EstimateOrientationFrom(Internal.Acceleration))
		changed |= ORIENTATION_CHANGED;
	
	return changed;
	}
// ------------------------------------------------------------------------------------
int wiimote::ParseIR (BYTE* buff)
	{
	if(Internal.IR.Mode == wiimote_state::ir::OFF)
		return NO_CHANGE;

	// take a copy of the existing IR state (so we can detect changes)
	wiimote_state::ir prev_ir = Internal.IR;

	// only updates the other values if the dots are visible (so that the last
	//  valid values stay unmodified)
	switch(Internal.IR.Mode)
		{
		case wiimote_state::ir::BASIC:
			// 2 dots are encoded in 5 bytes, so read 2 at a time
			for(unsigned step=0; step<2; step++)
				{
				ir::dot &dot0 = Internal.IR.Dot[step*2  ];
				ir::dot &dot1 = Internal.IR.Dot[step*2+1];
				const unsigned offs = 6 + (step*5); // 5 bytes for 2 dots

				dot0.bFound = !(buff[offs  ] == 0xff && buff[offs+1] == 0xff);
				dot1.bFound = !(buff[offs+3] == 0xff && buff[offs+4] == 0xff);
			
				if(dot0.bFound) {
					dot0.RawX = buff[offs  ] | ((buff[offs+2] >> 4) & 0x03) << 8;;
					dot0.RawY = buff[offs+1] | ((buff[offs+2] >> 6) & 0x03) << 8;;
					dot0.X    = 1.f - (dot0.RawX / (float)wiimote_state::ir::MAX_RAW_X);
					dot0.Y    =	      (dot0.RawY / (float)wiimote_state::ir::MAX_RAW_Y);
					}
				if(dot1.bFound) {
					dot1.RawX = buff[offs+3] | ((buff[offs+2] >> 0) & 0x03) << 8;
					dot1.RawY = buff[offs+4] | ((buff[offs+2] >> 2) & 0x03) << 8;
					dot1.X    = 1.f - (dot1.RawX / (float)wiimote_state::ir::MAX_RAW_X);
					dot1.Y    =	      (dot1.RawY / (float)wiimote_state::ir::MAX_RAW_Y);
					}
				}
			break;
		
		case wiimote_state::ir::EXTENDED:
			// each dot is encoded into 3 bytes
			for(unsigned index=0; index<4; index++)
				{
				ir::dot &dot = Internal.IR.Dot[index];
				const unsigned offs = 6 + (index * 3);
			
				dot.bFound = !(buff[offs  ]==0xff && buff[offs+1]==0xff &&
							   buff[offs+2]==0xff);
		
				if(dot.bFound) {
					dot.RawX = buff[offs  ] | ((buff[offs+2] >> 4) & 0x03) << 8;
					dot.RawY = buff[offs+1] | ((buff[offs+2] >> 6) & 0x03) << 8;
					dot.X    = 1.f - (dot.RawX / (float)wiimote_state::ir::MAX_RAW_X);
					dot.Y    =	     (dot.RawY / (float)wiimote_state::ir::MAX_RAW_Y);
					dot.Size = buff[offs+2] & 0x0f;
					}
				}
			break;

		case wiimote_state::ir::FULL:
			_ASSERT(0); // not supported yet;
			break;
		}

	return (memcmp(&prev_ir, &Internal.IR, sizeof(Internal.IR)) != 0)? IR_CHANGED : 0;
	}
// ------------------------------------------------------------------------------------
int wiimote::ParseExtension (BYTE *buff, unsigned offset)
	{
	int changed = 0;
	
	switch(Internal.ExtensionType)
		{
		case wiimote_state::NUNCHUK:
			{
			// buttons
			bool c = (buff[offset+5] & 0x02) == 0;
			bool z = (buff[offset+5] & 0x01) == 0;
			
			if((c != Internal.Nunchuk.C) || (z != Internal.Nunchuk.Z))
				changed |= NUNCHUK_BUTTONS_CHANGED;
			
			Internal.Nunchuk.C = c;
			Internal.Nunchuk.Z = z;

			// acceleration
			{
			wiimote_state::acceleration &accel = Internal.Nunchuk.Acceleration;
			
			BYTE raw_x = buff[offset+2];
			BYTE raw_y = buff[offset+3];
			BYTE raw_z = buff[offset+4];
			if((raw_x != accel.RawX) || (raw_y != accel.RawY) || (raw_z != accel.RawZ))
				changed |= NUNCHUK_ACCEL_CHANGED;

			accel.RawX = raw_x;
			accel.RawY = raw_y;
			accel.RawZ = raw_z;

			wiimote_state::nunchuk::calibration_info &calib =
													Internal.Nunchuk.CalibrationInfo;
			accel.X = ((float)raw_x - calib.X0) / ((float)calib.XG - calib.X0);
			accel.Y = ((float)raw_y - calib.Y0) / ((float)calib.YG - calib.Y0);
			accel.Z = ((float)raw_z - calib.Z0) / ((float)calib.ZG - calib.Z0);

			// try to extract orientation from the accel:
			if(EstimateOrientationFrom(accel))
				changed |= NUNCHUK_ORIENTATION_CHANGED;
			}
			{
			// joystick:
			wiimote_state::joystick &joy = Internal.Nunchuk.Joystick;

			float raw_x = buff[offset+0];
			float raw_y = buff[offset+1];
			
			if((raw_x != joy.RawX) || (raw_y != joy.RawY))
				changed |= NUNCHUK_JOYSTICK_CHANGED;

			joy.RawX = raw_x;
			joy.RawY = raw_y;

			// apply the calibration data
			wiimote_state::nunchuk::calibration_info &calib =
													Internal.Nunchuk.CalibrationInfo;
			if(Internal.Nunchuk.CalibrationInfo.MaxX != 0x00)
				joy.X = ((float)raw_x - calib.MidX) / ((float)calib.MaxX - calib.MinX);
			if(calib.MaxY != 0x00)
				joy.Y = ((float)raw_y - calib.MidY) / ((float)calib.MaxY - calib.MinY);

			// i prefer the outputs to range -1 - +1 (note this also affects the
			//  deadzone calculations)
			joy.X *= 2;	joy.Y *= 2;

			// apply the public deadzones to the internal state (if set)
			joy.DeadZone = Nunchuk.Joystick.DeadZone;
			ApplyJoystickDeadZones(joy);
			}
			}
			break;
		
		case wiimote_state::CLASSIC:
		case wiimote_state::CLASSIC_GUITAR:
			{
			// buttons:
			WORD bits = *(WORD*)(buff+offset+4);
			bits = ~bits; // need to invert bits since 0 is down, and 1 is up

			if(bits != Internal.ClassicController.Button.Bits)
				changed |= CLASSIC_BUTTONS_CHANGED;

			Internal.ClassicController.Button.Bits = bits;

			// joysticks:
			wiimote_state::joystick &joyL = Internal.ClassicController.JoystickL;
			wiimote_state::joystick &joyR = Internal.ClassicController.JoystickR;

			float l_raw_x = (float) (buff[offset+0] & 0x3f);
			float l_raw_y = (float) (buff[offset+1] & 0x3f);
			float r_raw_x = (float)((buff[offset+2]		    >> 7) |
								   ((buff[offset+1] & 0xc0) >> 5) |
								   ((buff[offset+0] & 0xc0) >> 3));
			float r_raw_y = (float) (buff[offset+2] & 0x1f);

			if((joyL.RawX != l_raw_x) || (joyL.RawY != l_raw_y))
				changed |= CLASSIC_JOYSTICK_L_CHANGED;
			if((joyR.RawX != r_raw_x) || (joyR.RawY != r_raw_y))
				changed |= CLASSIC_JOYSTICK_R_CHANGED;

			joyL.RawX = l_raw_x; joyL.RawY = l_raw_y;
			joyR.RawX = r_raw_x; joyR.RawY = r_raw_y;

			// apply calibration
			wiimote_state::classic_controller::calibration_info &calib =
											Internal.ClassicController.CalibrationInfo;
			if(calib.MaxXL != 0x00)
				joyL.X = (joyL.RawX - calib.MidXL) / ((float)calib.MaxXL - calib.MinXL);
			if(calib.MaxYL != 0x00)
				joyL.Y = (joyL.RawY - calib.MidYL) / ((float)calib.MaxYL - calib.MinYL);
			if(calib.MaxXR != 0x00)
				joyR.X = (joyR.RawX - calib.MidXR) / ((float)calib.MaxXR - calib.MinXR);
			if(calib.MaxYR != 0x00)
				joyR.Y = (joyR.RawY - calib.MidYR) / ((float)calib.MaxYR - calib.MinYR);

			// i prefer the joystick outputs to range -1 - +1 (note this also affects
			//  the deadzone calculations)
			joyL.X *= 2; joyL.Y *= 2; joyR.X *= 2; joyR.Y *= 2;

			// apply the public deadzones to the internal state (if set)
			joyL.DeadZone = ClassicController.JoystickL.DeadZone;
			joyR.DeadZone = ClassicController.JoystickR.DeadZone;
			ApplyJoystickDeadZones(joyL);
			ApplyJoystickDeadZones(joyR);

			// triggers
			BYTE raw_trigger_l = ((buff[offset+2] & 0x60) >> 2) |
								  (buff[offset+3]		  >> 5);
			BYTE raw_trigger_r =   buff[offset+3] & 0x1f;
			
			if((raw_trigger_l != Internal.ClassicController.RawTriggerL) ||
			   (raw_trigger_r != Internal.ClassicController.RawTriggerR))
			   changed |= CLASSIC_TRIGGERS_CHANGED;
			
			Internal.ClassicController.RawTriggerL  = raw_trigger_l;
			Internal.ClassicController.RawTriggerR  = raw_trigger_r;

			if(calib.MaxTriggerL != 0x00)
				Internal.ClassicController.TriggerL =
						 (float)Internal.ClassicController.RawTriggerL / 
						((float)calib.MaxTriggerL -	calib.MinTriggerL);

			if(calib.MaxTriggerR != 0x00)
				Internal.ClassicController.TriggerR =
						 (float)Internal.ClassicController.RawTriggerR / 
						((float)calib.MaxTriggerR - calib.MinTriggerR);
			}
			break;
		}
	return changed;
	}
// ------------------------------------------------------------------------------------
int wiimote::ParseReadAddress (BYTE* buff)
	{
	int changed			  = 0;

	if((buff[3] & 0x08) != 0) {
		WARN(_T("error: read address not valid."));
		_ASSERT(0);
		return NO_CHANGE;
		}
	else if((buff[3] & 0x07) != 0) {
		WARN(_T("error: attempt to read from write-only registers."));
		_ASSERT(0);
		return NO_CHANGE;
		}

	int size = buff[3] >> 4;

	// decode the address that was queried
	int address = buff[4]<<8 | buff[5];

	// *NOTE*: this is a major (but convenient) hack!  The returned data only
	//          contains the lower two bytes of the address that was queried.
	//			as these don't collide between any of the addresses/registers
	//			we currently read, it's OK to match just those two bytes

	// and skip the header:
	buff += 6;

	switch(address)
		{
		case (REGISTER_CALIBRATION & 0xffff):
			{
			_ASSERT(size == 6);
			Internal.CalibrationInfo.X0 = buff[0];
			Internal.CalibrationInfo.Y0 = buff[1];
			Internal.CalibrationInfo.Z0 = buff[2];
			Internal.CalibrationInfo.XG = buff[4];
			Internal.CalibrationInfo.YG = buff[5];
			Internal.CalibrationInfo.ZG = buff[6];
			
			//changed |= CALIBRATION_CHANGED;	
			}
			break;
			
		case (REGISTER_EXTENSION_TYPE & 0xffff):
			{
			_ASSERT(size == 1);
		
			if(*(WORD*)buff == wiimote_state::NUNCHUK)
				{
				// sometimes it comes in more than once?
				if(Internal.ExtensionType == wiimote_state::NUNCHUK)
					break;
				TRACE(_T(".. Nunchuk!"));
				Internal.ExtensionType = wiimote_state::NUNCHUK;
				// and start a query for the calibration data
				ReadAddress(REGISTER_EXTENSION_CALIBRATION, 16);
				}
			else if((*(WORD*)buff == wiimote_state::CLASSIC))
				{
				// sometimes it comes in more than once?
				if(Internal.ExtensionType == wiimote_state::CLASSIC)
					break;
				TRACE(_T(".. Classic Controller!"));
				Internal.ExtensionType = wiimote_state::CLASSIC;
				// and start a query for the calibration data
				ReadAddress(REGISTER_EXTENSION_CALIBRATION, 16);
				}
			else if((*(WORD*)buff == wiimote_state::CLASSIC_GUITAR))
				{
				// sometimes it comes in more than once?
				if(Internal.ExtensionType == wiimote_state::CLASSIC_GUITAR)
					break;
				TRACE(_T(".. Guitar Controller!"));
				Internal.ExtensionType = wiimote_state::CLASSIC_GUITAR;
				// and start a query for the calibration data
				ReadAddress(REGISTER_EXTENSION_CALIBRATION, 16);
				}
			else if(*(WORD*)buff == wiimote_state::PARTIALLY_INSERTED) {
				// sometimes it comes in more than once?
				if(Internal.ExtensionType == wiimote_state::PARTIALLY_INSERTED)
					break;
				TRACE(_T(".. partially inserted!"));
				Internal.ExtensionType = wiimote_state::PARTIALLY_INSERTED;
				changed |= EXTENSION_PARTIALLY_INSERTED;
				// try initializing the extension again next time by requesting
				//  another status report (this usually fixes it)
				Internal.bExtension = false;
				RequestStatusReport();
				}
			else{
				WARN(_T("unknown extension controller found (0x%x)"), buff[0]);
				}
			}
			break;
		
		case (REGISTER_EXTENSION_CALIBRATION & 0xffff):
			{
			_ASSERT(size == 15);
			DecryptBuffer(buff, 16);

			switch(Internal.ExtensionType)
				{
				case wiimote_state::NUNCHUK:
					{
					wiimote_state::nunchuk::calibration_info &calib =
													Internal.Nunchuk.CalibrationInfo;
					calib.X0   = buff[ 0];
					calib.Y0   = buff[ 1];
					calib.Z0   = buff[ 2];
					calib.XG   = buff[ 4];
					calib.YG   = buff[ 5];
					calib.ZG   = buff[ 6];
					calib.MaxX = buff[ 8];
					calib.MinX = buff[ 9];
					calib.MidX = buff[10];
					calib.MaxY = buff[11];
					calib.MinY = buff[12];
					calib.MidY = buff[13];

					changed |= NUNCHUK_CONNECTED;//|NUNCHUK_CALIBRATION_CHANGED;
					// reenable reports
					SetReportType(ReportType);
					}
					break;
				
				case wiimote_state::CLASSIC:
				case wiimote_state::CLASSIC_GUITAR:
					{
					wiimote_state::classic_controller::calibration_info &calib =
											Internal.ClassicController.CalibrationInfo;
					calib.MaxXL = buff[ 0] >> 2;
					calib.MinXL = buff[ 1] >> 2;
					calib.MidXL = buff[ 2] >> 2;
					calib.MaxYL = buff[ 3] >> 2;
					calib.MinYL = buff[ 4] >> 2;
					calib.MidYL = buff[ 5] >> 2;
					calib.MaxXR = buff[ 6] >> 3;
					calib.MinXR = buff[ 7] >> 3;
					calib.MidXR = buff[ 8] >> 3;
					calib.MaxYR = buff[ 9] >> 3;
					calib.MinYR = buff[10] >> 3;
					calib.MidYR = buff[11] >> 3;
					// this doesn't seem right...
					//	calib.MinTriggerL = buff[12] >> 3;
					//	calib.MaxTriggerL = buff[14] >> 3;
					//	calib.MinTriggerR = buff[13] >> 3;
					//	calib.MaxTriggerR = buff[15] >> 3;
					calib.MinTriggerL = 0;
					calib.MaxTriggerL = 31;
					calib.MinTriggerR = 0;
					calib.MaxTriggerR = 31;

					changed |= CLASSIC_CONNECTED;//|CLASSIC_CALIBRATION_CHANGED;
					// reenable reports
					SetReportType(ReportType);
					}
					break;
				}
			}
			break;

		default:
			_ASSERT(0); // shouldn't happen
			break;
		}
	
	return changed;
	}
// ------------------------------------------------------------------------------------
void wiimote::ReadCalibration ()
	{
	// this appears to change the report type to 0x31
	ReadAddress(REGISTER_CALIBRATION, 7);
	}
// ------------------------------------------------------------------------------------
void wiimote::EnableIR (wiimote_state::ir::mode mode)
	{
	Internal.IR.Mode = mode;

	ClearReport();
	WriteBuff[0] = OUT_IR;
	WriteBuff[1] = 0x04 | GetRumbleBit();
	WriteReport();

	ClearReport();
	WriteBuff[0] = OUT_IR2;
	WriteBuff[1] = 0x04 | GetRumbleBit();
	WriteReport();

	static const BYTE ir_sens1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0xc0};
	static const BYTE ir_sens2[] = {0x40, 0x00};

	WriteData(REGISTER_IR, 0x08);
	WriteData(REGISTER_IR_SENSITIVITY_1, sizeof(ir_sens1), ir_sens1);
	WriteData(REGISTER_IR_SENSITIVITY_2, sizeof(ir_sens2), ir_sens2);
	WriteData(REGISTER_IR_MODE, mode);
	}
// ------------------------------------------------------------------------------------
void wiimote::DisableIR ()
	{
	Internal.IR.Mode = wiimote_state::ir::OFF;

	ClearReport();
	WriteBuff[0] = OUT_IR;
	WriteBuff[1] = GetRumbleBit();
	WriteReport();

	ClearReport();
	WriteBuff[0] = OUT_IR2;
	WriteBuff[1] = GetRumbleBit();
	WriteReport();
	}
// ------------------------------------------------------------------------------------
unsigned __stdcall wiimote::HIDwriteThreadfunc (void* param)
	{
	_ASSERT(param);
	TRACE(_T("(starting HID write thread)"));
	wiimote &remote = *(wiimote*)param;

	while(remote.Handle != INVALID_HANDLE_VALUE)
		{
		// try to write the oldest entry in the queue
		if(!remote.HIDwriteQueue.empty())
			{
#ifdef BEEP_DEBUG_WRITES
			Beep(1500,1);
#endif
			EnterCriticalSection(&remote.HIDwriteQueueLock);
			 BYTE *buff = remote.HIDwriteQueue.front();
	 		LeaveCriticalSection(&remote.HIDwriteQueueLock);

			_ASSERT(buff);

			if(!_HidD_SetOutputReport(remote.Handle, buff, REPORT_LENGTH))
				{
				DWORD err = GetLastError();
				if((err != ERROR_BUSY)	&&	 // "the requested resource is in use"
				   (err != ERROR_NOT_READY)) // "the device is not ready"
					{
					if(err == ERROR_NOT_SUPPORTED) {
						WARN(_T("BT Stack doesn't suport HID writes!"));
						goto remove_entry;
						}
					else{
						DEEP_TRACE(_T("HID write to Wiimote failed (err %u)! - "), err);
						// if this worked previously, the connection was probably lost
						if(remote.IsConnected())
							remote.bConnectionLost = true;
						}

					//_T("aborting write thread"), err);
					//return 911;
					}
				}
			else{
remove_entry:
				delete[] buff;
				EnterCriticalSection(&remote.HIDwriteQueueLock);
				 remote.HIDwriteQueue.pop();
		 		LeaveCriticalSection(&remote.HIDwriteQueueLock);
				}
			}
		Sleep(1);
		}

	TRACE(_T("ending HID write thread"));
	return 0;
	}
// ------------------------------------------------------------------------------------
bool wiimote::WriteReport (BYTE *buff)
	{
	// NULL means 'use WriteBuff'
	if(!buff)
		buff = WriteBuff;

#ifdef BEEP_DEBUG_WRITES
	Beep(2000,1);
#endif

	if(bUseHIDwrite)
		{
		// HidD_SetOutputReport: +: works on MS Bluetooth stacks (WriteFile doesn't).
		//						 -: is synchronous, so make it async
		if(!HIDwriteThread)
			{
			HIDwriteThread = (HANDLE)_beginthreadex(NULL, 0, HIDwriteThreadfunc,
													this, 0, NULL);
			_ASSERT(HIDwriteThread);
			if(!HIDwriteThread) {
				WARN(_T("couldn't create HID write thread!"));
				return false;
				}
			SetThreadPriority(HIDwriteThread, WORKER_THREAD_PRIORITY);
			}
		// insert the write request into the thread's queue
		BYTE *buff_copy = new BYTE[REPORT_LENGTH];
		_ASSERT(buff_copy);
		memcpy(buff_copy, buff, REPORT_LENGTH);

		EnterCriticalSection(&HIDwriteQueueLock);
		 HIDwriteQueue.push(buff_copy);
		LeaveCriticalSection(&HIDwriteQueueLock);
		return true;
		}

	// WriteFile:
	DWORD written;
	// have we been asked to cancel any pending IO
	if(!WriteFile(Handle, buff, REPORT_LENGTH, &written, &Overlapped))
		{
		DWORD error = GetLastError();
		if(error != ERROR_IO_PENDING) {
			DEEP_TRACE(_T("WriteFile failed!"));
			// if it worked previously, assume we lost the connection
			if(IsConnected())
				bConnectionLost = true;
			return false;
			}
		}
	return true;
	}
// ------------------------------------------------------------------------------------
// experimental speaker support:
// ------------------------------------------------------------------------------------
bool wiimote::MuteSpeaker (bool on)
	{
	_ASSERT(IsConnected());
	if(!IsConnected())
		return false;

	if(Internal.Speaker.bMuted == on)
		return true;

	if(on) TRACE(_T("muting speaker."  ));
	else   TRACE(_T("unmuting speaker."));

	WriteBuff[0] = OUT_SPEAKER_MUTE;
	WriteBuff[1] = (on? 0x04 : 0x00) | GetRumbleBit();
	if(!WriteReport())
		return false;
	
	Internal.Speaker.bMuted = on;
	return true;
	}
// ------------------------------------------------------------------------------------
bool wiimote::EnableSpeaker (bool on)
	{
	_ASSERT(IsConnected());
	if(!IsConnected())
		return false;

	if(Internal.Speaker.bEnabled == on)
		return true;

	if(on) TRACE(_T("enabling speaker.")); else TRACE(_T("disabling speaker."));

	WriteBuff[0] = OUT_SPEAKER_ENABLE;
	WriteBuff[1] = (on? 0x04 : 0x00) | GetRumbleBit();
	if(!WriteReport())
		return false;

	if(!on) {
		Internal.Speaker.Freq   = FREQ_NONE;
		Internal.Speaker.Volume = 0;
		MuteSpeaker(true);
		}

	Internal.Speaker.bEnabled = on;
	return true;
	}
// ------------------------------------------------------------------------------------
#ifdef TR4
// TEMP:
extern int hzinc;
#endif

unsigned __stdcall wiimote::SampleStreamThreadfunc (void* param)
	{
	TRACE(_T("(starting sample thread)"));
	// sends a simple square wave sample stream
	wiimote &remote = *(wiimote*)param;
	
	static BYTE squarewave_report[REPORT_LENGTH] =
		{ OUT_SPEAKER_DATA, 20<<3, 0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,
								   0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3, };
	static BYTE sample_report [REPORT_LENGTH] = 
		{ OUT_SPEAKER_DATA, 0 };

	bool		   last_playing    = false;
	DWORD		   frame		   = 0;
	DWORD		   frame_start     = 0;
	unsigned	   total_samples   = 0;
	unsigned	   sample_index	   = 0;
	wiimote_sample *current_sample = NULL;
	
	// TODO: duration!!
	while(remote.IsConnected())
		{
		bool playing = remote.IsPlayingAudio();
		
		if(!playing)
			Sleep(1);
		else{
			const unsigned freq_hz  = FreqLookup[remote.Internal.Speaker.Freq];
#ifdef TR4
			const float    frame_ms = 1000 / ((freq_hz+hzinc) / 40.f); // 20bytes = 40 samples per write
#else
			const float    frame_ms = 1000 / (freq_hz		  / 40.f); // 20bytes = 40 samples per write
#endif

			// has the sample just changed?
			bool sample_changed = (current_sample != remote.CurrentSample);
			current_sample		= (wiimote_sample*)remote.CurrentSample;

// (attempts to minimise glitches)
//#define FIRSTFRAME_IS_SILENT	// send all-zero for first frame

#ifdef FIRSTFRAME_IS_SILENT
			bool silent_frame = false;
#endif
			if(!last_playing || sample_changed) {
				frame		  = 0;
				frame_start   = timeGetTime();
				total_samples = current_sample? current_sample->length : 0;
				sample_index  = 0;
#ifdef FIRSTFRAME_IS_SILENT
				silent_frame  = true;
#endif
				}

			// are we streaming a sample?
			if(current_sample)
				{
				if(sample_index < current_sample->length)
					{
					// (remember that samples are 4bit, ie. 2 per byte)
					unsigned samples_left   = (current_sample->length - sample_index);
					unsigned report_samples = min(samples_left, 40);
					// round the entries up to the nearest multiple of 2
					unsigned report_entries = (report_samples+1) >> 1;
					
					sample_report[1] = (report_entries<<3) | remote.GetRumbleBit();

#ifdef FIRSTFRAME_IS_SILENT
					if(silent_frame) {
						// send all-zeroes
						for(unsigned index=0; index<report_entries; index++)
							sample_report[2+index] = 0;
						remote.WriteReport(sample_report);
						}
					else
#endif
					{
						for(unsigned index=0; index<report_entries; index++)
							sample_report[2+index] =
									current_sample->samples[(sample_index>>1)+index];
						remote.WriteReport(sample_report);
						sample_index += report_samples;
						}
					}
				else{
					// we reached the sample end
					remote.CurrentSample		   = NULL;
					current_sample				   = NULL;
					remote.Internal.Speaker.Freq   = FREQ_NONE;
					remote.Internal.Speaker.Volume = 0;
					}
				}
			// no, a squarewave
			else{
				squarewave_report[1] = (20<<3) | remote.GetRumbleBit();
				remote.WriteReport(squarewave_report);
#if 0
				// verify that we're sending at the correct rate (we are)
				DWORD elapsed		   = (timeGetTime()-frame_start);
				unsigned total_samples = frame * 40;
				float elapsed_secs	   = elapsed / 1000.f;
				float sent_persec	   = total_samples / elapsed_secs;
#endif
				}

			frame++;

			// send the first two buffers immediately (seems to lessen startup
			//  startup glitches - presumably we're filling a small sample
			//  (or general input) buffer on the wiimote)
//			if(frame > 2) {
				while((timeGetTime()-frame_start) < (unsigned)(frame*frame_ms))
					Sleep(1);
//				}
			}

		last_playing = playing;
		}
	
	TRACE(_T("(ending sample thread)"));
	return 0;
	}
// ------------------------------------------------------------------------------------
bool wiimote::Load16bitMonoSampleWAV (const TCHAR* filepath, wiimote_sample &out)
	{
	speaker_freq freq;
	bool res;
	// converts unsigned 16bit mono .wav audio data to the 4bit ADPCM variant
	//  used by the Wiimote, and returns the data in a BYTE array (user must
	//  delete[] when no longer needed):
	memset(&out, 0, sizeof(out));

	TRACE(_T("Loading '%s'"), filepath);

	FILE *file;
//	_tfopen_s(&file, filepath, _T("rb"));
	file=fopen(filepath,_T("rb"));
	_ASSERT(file);
	if(!file) {
		WARN(_T("Couldn't open '%s"), filepath);
		return false;
		}

	// parse the .wav file
	struct riff_chunkheader {
		char  ckID [4];
		DWORD ckSize;
		char  formType [4];
		};
	struct chunk_header {
		char  ckID [4];
		DWORD ckSize;
		};
	union {
		WAVEFORMATEX		 x;
		WAVEFORMATEXTENSIBLE xe;
		} wf;

	riff_chunkheader riff_chunkheader;
	chunk_header	 chunk_header;

	#define READ(data)			if(fread(&data, sizeof(data), 1, file) != 1) { \
									TRACE(_T(".wav file corrupt"));			   \
									fclose(file);							   \
									return false;							   \
									}
	#define READ_SIZE(ptr,size)	if(fread(ptr, size, 1, file) != 1) {		   \
									TRACE(_T(".wav file corrupt"));			   \
									fclose(file);							   \
									return false;							   \
									}
	// read the riff chunk header
	READ(riff_chunkheader);

	// valid RIFF file?
	_ASSERT(!strncmp(riff_chunkheader.ckID, "RIFF", 4));
	if(strncmp(riff_chunkheader.ckID, "RIFF", 4))
		goto unsupported; // nope
	// valid WAV variant?
	_ASSERT(!strncmp(riff_chunkheader.formType, "WAVE", 4));
	if(strncmp(riff_chunkheader.formType, "WAVE", 4))
		goto unsupported; // nope

	// find the format & data chunks
	freq = FREQ_NONE;
	while(1)
		{
		READ(chunk_header);
		
		if(!strncmp(chunk_header.ckID, "fmt ", 4))
			{
			// not a valid .wav file?
			if(chunk_header.ckSize < 16 ||
			   chunk_header.ckSize > sizeof(WAVEFORMATEXTENSIBLE))
				goto unsupported;

			READ_SIZE((BYTE*)&wf.x, chunk_header.ckSize);

			// now we know it's true wav file
			bool extensible = (wf.x.wFormatTag == WAVE_FORMAT_EXTENSIBLE);
			int format	    = extensible? wf.xe.SubFormat.Data1 :
										  wf.x .wFormatTag;
			// must be uncompressed PCM (the format comparisons also work on
			//  the 'extensible' header, even though they're named differently)
			if(format != WAVE_FORMAT_PCM) {
				_TRACE(_T(".. not uncompressed PCM"));
				goto unsupported;
				}

			// must be mono, 16bit
			if((wf.x.nChannels != 1) || (wf.x.wBitsPerSample != 16)) {
				_TRACE(_T(".. %d bit, %d channel%s"), wf.x.wBitsPerSample,
													  wf.x.nChannels,
													 (wf.x.nChannels>1? _T("s"):_T("")));
				goto unsupported;
				}

			// must be _near_ a supported speaker frequency range (but allow some
			//  tolerance, especially as the values aren't final yet):
			unsigned	   sample_freq = wf.x.nSamplesPerSec;
			const unsigned epsilon	   = 100; // for now
			
			for(unsigned index=1; index<ARRAY_SIZE(FreqLookup); index++)
				{
				if((sample_freq+epsilon) >= FreqLookup[index] &&
				   (sample_freq-epsilon) <= FreqLookup[index]) {
					freq = (speaker_freq)index;
					TRACE(_T(".. using speaker freq %u"), FreqLookup[index]);
					break;
					}
				}
			if(freq == FREQ_NONE) {
				WARN(_T("Couldn't (loosely) match .wav samplerate %u Hz to speaker"),
					 sample_freq);
				goto unsupported;
				}
			}
		else if(!strncmp(chunk_header.ckID, "data", 4))
			{
			// grab the data
			unsigned total_samples = chunk_header.ckSize / wf.x.nBlockAlign;
			if(total_samples == 0)
				goto corrupt_file;
			
			short *samples = new short[total_samples];
			size_t read = fread(samples, 2, total_samples, file);
			fclose(file);
			if(read != total_samples)
				{
				if(read == 0) {
					delete[] samples;
					goto corrupt_file;
					}
				// got a different number, but use them anyway
				WARN(_T("found %s .wav audio data than expected (%u/%u samples)"),
					((read < total_samples)? _T("less") : _T("more")),
					read, total_samples);

				total_samples = read;
				}

			// and convert them
			res = Convert16bitMonoSamples(samples, true, total_samples,
											  freq, out);
			delete[] samples;
			return res;
			}
		else{
			// unknown chunk, skip its data
			DWORD chunk_bytes = (chunk_header.ckSize + 1) & ~1L;
			if(fseek(file, chunk_bytes, SEEK_CUR))
				goto corrupt_file;
			}
		}

corrupt_file:
	WARN(_T(".wav file is corrupt"));
	fclose(file);
	return false;

unsupported:
	WARN(_T(".wav file format not supported (must be mono 16bit PCM)"));
	fclose(file);
	return false;
	}
// ------------------------------------------------------------------------------------
bool wiimote::Load16BitMonoSampleRAW (const TCHAR*   filepath,
									  bool		     _signed,
									  speaker_freq   freq,
									  wiimote_sample &out)
	{
	bool res;
	// converts (.wav style) unsigned 16bit mono raw data to the 4bit ADPCM variant
	//  used by the Wiimote, and returns the data in a BYTE array (user must
	//  delete[] when no longer needed):
	memset(&out, 0, sizeof(out));

	// get the length of the file
	struct _stat file_info;
	if(_tstat(filepath, &file_info)) {
		WARN(_T("couldn't get filesize for '%s'"), filepath);
		return false;
		}
	
	DWORD len = file_info.st_size;
	_ASSERT(len);
	if(!len) {
		WARN(_T("zero-size sample file '%s'"), filepath);
		return false;
		}

	unsigned total_samples = (len+1) / 2; // round up just in case file is corrupt
	// allocate a buffer to hold the samples to convert
	short *samples = new short[total_samples]; 
	_ASSERT(samples);
	if(!samples) {
		TRACE(_T("Couldn't open '%s"), filepath);
		return false;
		}

	// load them
	FILE *file;
//	_tfopen_s(&file, filepath, _T("rb"));
	file=fopen(filepath, _T("rb"));
	_ASSERT(file);
	if(!file) {
		TRACE(_T("Couldn't open '%s"), filepath);
		goto error;
		}

	res = (fread(samples, 1, len, file) == len);
	fclose(file);

	if(!res) {
		WARN(_T("Couldn't load file '%s'"), filepath);
		goto error;
		}

	// and convert them
	res = Convert16bitMonoSamples(samples, _signed, total_samples, freq, out);
	delete[] samples;
	return res;

error:
	delete[] samples;
	return false;
	}
// ------------------------------------------------------------------------------------
bool wiimote::Convert16bitMonoSamples (const short*   samples,
									   bool		      _signed,
									   DWORD		  length,
									   speaker_freq   freq,
									   wiimote_sample &out)
	{
	// converts 16bit mono sample data to the native 4bit format used by the Wiimote,
	//  and returns the data in a BYTE array (caller must delete[] when no
	//  longer needed):
	memset(&out, 0, sizeof(0));

	_ASSERT(samples && length);
	if(!samples || !length)
		return NULL;

	// allocate the output buffer
	out.samples = new BYTE[length];
	_ASSERT(out.samples);
	if(!out.samples)
		return NULL;

	// clear it
	memset(out.samples, 0, length);
	out.length = length;
	out.freq   = freq;

	// ADPCM code, adapted from
	//  http://www.wiindows.org/index.php/Talk:Wiimote#Input.2FOutput_Reports
	static const int index_table[16] = {  -1,  -1,  -1,  -1,   2,   4,   6,   8,
										  -1,  -1,  -1,  -1,   2,   4,   6,   8 };
	static const int diff_table [16] = {   1,   3,   5,   7,   9,  11,  13,  15,
										  -1,  -3,  -5,  -7,  -9, -11, -13,  15 };
	static const int step_scale [16] = { 230, 230, 230, 230, 307, 409, 512, 614,
										 230, 230, 230, 230, 307, 409, 512, 614 };
	// Encode to ADPCM, on initialization set adpcm_prev_value to 0 and adpcm_step
	//  to 127 (these variables must be preserved across reports)
	int adpcm_prev_value = 0;
	int adpcm_step		 = 127;

	for(size_t i=0; i<length; i++)
		{
		// convert to 16bit signed
		int value = samples[i];// (8bit) << 8);// | samples[i]; // dither it?
		if(!_signed)
			value -= 32768;
		// encode:
		int  diff = value - adpcm_prev_value;
		BYTE encoded_val = 0;
		if(diff < 0) {
			encoded_val |= 8;
			diff = -diff;
			}
		diff = (diff << 2) / adpcm_step;
		if (diff > 7)
			diff = 7;
		encoded_val |= diff;
		adpcm_prev_value += ((adpcm_step * diff_table[encoded_val]) / 8);
		if(adpcm_prev_value  >  0x7fff)
			adpcm_prev_value =  0x7fff;
		if(adpcm_prev_value  < -0x8000)
			adpcm_prev_value = -0x8000;
		adpcm_step = (adpcm_step * step_scale[encoded_val]) >> 8;
		if(adpcm_step < 127)
			adpcm_step = 127;
		if(adpcm_step > 24567)
			adpcm_step = 24567;
		if(i & 1)
			out.samples[i>>1] |= encoded_val;
		else
			out.samples[i>>1] |= encoded_val << 4;
		}

	return true;
	}
// ------------------------------------------------------------------------------------
bool wiimote::PlaySample (const wiimote_sample &sample, BYTE volume, 
						  speaker_freq freq_override)
	{
	_ASSERT(IsConnected());
	if(!IsConnected())
		return false;

	speaker_freq freq = freq_override? freq_override : sample.freq;
	BYTE		 vol  = volume;

	TRACE(_T("playing sample."));
	EnableSpeaker(true);
	MuteSpeaker  (true);

#if 0
	// combine everything into one write - faster, seems to work?
	BYTE bytes[9] = { 0x00, 0x00, 0x00, 10+freq, vol, 0x00, 0x00, 0x01, 0x01 };
	WriteData(0x04a20001, sizeof(bytes), bytes);
#else
	// Write 0x01 to register 0x04a20009 
	WriteData(0x04a20009, 0x01);
	// Write 0x08 to register 0x04a20001 
	WriteData(0x04a20001, 0x08);
	// Write 7-byte configuration to registers 0x04a20001-0x04a20008 
	BYTE bytes[7] = { 0x00, 0x00, 0x00, 10+freq, volume, 0x00, 0x00 };
	WriteData(0x04a20001, sizeof(bytes), bytes);
	// + Write 0x01 to register 0x04a20008 
	WriteData(0x04a20008, 0x01);
#endif

	Internal.Speaker.Freq   = freq;
	Internal.Speaker.Volume = volume;
	CurrentSample			= &sample;

	MuteSpeaker(false);

	return StartSampleThread();
	}
// ------------------------------------------------------------------------------------
bool wiimote::StartSampleThread ()
	{
	if(SampleThread)
		return true;

	SampleThread = (HANDLE)_beginthreadex(NULL, 0, SampleStreamThreadfunc,
										  this, 0, NULL);
	_ASSERT(SampleThread);
	if(!SampleThread) {
		WARN(_T("couldn't create sample thread!"));
		MuteSpeaker  (true);
		EnableSpeaker(false);	
		return false;
		}
	SetThreadPriority(SampleThread, WORKER_THREAD_PRIORITY);
	return true;
	}
// ------------------------------------------------------------------------------------
bool wiimote::PlaySquareWave (speaker_freq freq, BYTE volume)
	{
	_ASSERT(IsConnected());
	if(!IsConnected())
		return false;

	// if we're already playing a sample, stop it first
	if(IsPlayingSample())
		CurrentSample = NULL;
	// if we're already playing a square wave at this freq and volume, return
	else if(IsPlayingAudio() && (Internal.Speaker.Freq   == freq) &&
								(Internal.Speaker.Volume == volume))
		return true;

	TRACE(_T("playing square wave."));
	// stop playing samples
	CurrentSample = 0;

	EnableSpeaker(true);
	MuteSpeaker  (true);

#if 0
	// combined everything into one write - much faster, seems to work?
	BYTE bytes[9] = { 0x00, 0x00, 0x00, freq, volume, 0x00, 0x00, 0x01, 0x1 };
	WriteData(0x04a20001, sizeof(bytes), bytes);
#else
	// write 0x01 to register 0xa20009 
	WriteData(0x04a20009, 0x01);
	// write 0x08 to register 0xa20001 
	WriteData(0x04a20001, 0x08);
	// write default sound mode (4bit ADPCM, we assume) 7-byte configuration
	//  to registers 0xa20001-0xa20008 
	BYTE bytes[7] = { 0x00, 0x00, 0x00, 10+freq, volume, 0x00, 0x00 };
	WriteData(0x04a20001, sizeof(bytes), bytes);
	// write 0x01 to register 0xa20008 
	WriteData(0x04a20008, 0x01);
#endif

	Internal.Speaker.Freq   = freq;
	Internal.Speaker.Volume = volume;

	MuteSpeaker(false);
	return StartSampleThread();
	}
// ------------------------------------------------------------------------------------
void wiimote::RecordState (state_history	  &events_out,
						   unsigned			  max_time_ms,
						   state_change_flags change_trigger)
	{
	// user being naughty?
	if(Recording.bEnabled)
		StopRecording();

	// clear the list
	if(!events_out.empty())
		events_out.clear();

	// start recording 
	Recording.StateHistory = &events_out;
	Recording.StartTimeMS  = timeGetTime();
	Recording.EndTimeMS    = Recording.StartTimeMS + max_time_ms;
	Recording.TriggerFlags = change_trigger;
	// (as this call happens outside the read/parse thread, set the boolean
	//  which will enable reocrding last, so that all params are in place)
	Recording.bEnabled	   = true;
	}
// ------------------------------------------------------------------------------------
void wiimote::StopRecording ()
	{
	if(!Recording.bEnabled)
		return;

	Recording.bEnabled = false;
	// make sure the read/parse thread has time to notice the change (else it might
	//  still write one more state to the list)
	Sleep(10); // too much?
	}
// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
