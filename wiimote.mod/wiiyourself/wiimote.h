// _______________________________________________________________________________
//
//	 - WiiYourself! - native C++ Wiimote library  v0.99b.
//	  (c) gl.tter 2007 - http://wiiyourself.gl.tter.org
//
//	  see License.txt for conditions of use.  see History.txt for change log.
// _______________________________________________________________________________
//
//  wiimote.h
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>		// auto Unicode/Ansi support
//#include <crtdbg.h>
#include <queue>		// for HID write method
#include <list>			// for state recording
 using namespace std;

#include <assert.h>
#include "wiimote_state.h"

// configs:
 //  we request periodic status report updates to refresh the battery level
//   and to detect connection loss (through failed writes)
#define REQUEST_STATUS_EVERY_MS		1000
//  all threads (read/parse, audio streaming, async rumble...) use this priority
#define WORKER_THREAD_PRIORITY		THREAD_PRIORITY_HIGHEST

 // internals
#define	WIIYOURSELF_VERSION_MAJOR	0
#define	WIIYOURSELF_VERSION_MINOR1	9
#define	WIIYOURSELF_VERSION_MINOR2	6
#define WIIYOURSELF_VERSION_STR		_T("0.99b")

// clarity
typedef HANDLE EVENT;


// state data changes can be signalled to the app via a callback.  Set the wiimote
//  object's 'ChangedCallback' any time to enable them.  'changed' is a combination
//  of flags indicating which state has changed since the last callback.
typedef void (*state_changed_callback)	(class wiimote	    &owner,
										 state_change_flags changed);

// internals
typedef BOOLEAN (__stdcall *hidwrite_ptr)(HANDLE HidDeviceObject,
										  PVOID  ReportBuffer,
										  ULONG  ReportBufferLength);

// wiimote class - connects and manages a wiimote and its optional extensions
//                 (Nunchuk/Classic Controller), and exposes their state
class wiimote : public wiimote_state
	{
	public:
		wiimote ();
		~wiimote ();

	public:
		// wiimote data input mode (use with SetReportType())
		//  (only enable what you need to save battery power)
		enum input_report
			{
			// combinations if buttons/acceleration/IR/Extension data
			IN_BUTTONS				= 0x30,
			IN_BUTTONS_ACCEL		= 0x31,
			IN_BUTTONS_ACCEL_IR		= 0x33,	// reports IR EXTENDED data (dot sizes)
			IN_BUTTONS_ACCEL_EXT	= 0x35,
			IN_BUTTONS_ACCEL_IR_EXT	= 0x37,	// reports IR BASIC data (no dot sizes)
			};


	public: // convenience accessors:
		inline bool	IsConnected		 () const { return bStatusReceived; }
		// if IsConnected() unexpectedly returns false, connection was probably lost
		inline bool	ConnectionLost	 () const { return bConnectionLost; }
		inline bool	NunchukConnected () const { return (Internal.bExtension && (Internal.ExtensionType==wiimote_state::NUNCHUK)); }
		inline bool	ClassicConnected ()	const { return (Internal.bExtension && (Internal.ExtensionType==wiimote_state::CLASSIC)); }
		inline bool	IsPlayingAudio	 () const { return (Internal.Speaker.Freq && Internal.Speaker.Volume); }
		inline bool	IsPlayingSample	 () const { return IsPlayingAudio() && (CurrentSample != NULL); }
		inline bool	IsUsingHIDwrites () const { return bUseHIDwrite; }
		inline bool	IsRecordingState () const { return Recording.bEnabled; }

		static inline unsigned TotalConnected() { _TotalConnected; }


	public: // data
		// optional callbacks - set these to your own fuctions (if required)
		state_changed_callback   ChangedCallback;
		//  you can avoid unnecessary callback overhead by specifying a mask
		//   of which state changes should trigger them (default is any)
		state_change_flags		 CallbackTriggerFlags;

		// get the button name from its bit index (some bits are unused)
		static const TCHAR*		 ButtonNameFromBit		  [16];
		// same for the Classic Controller
		static const TCHAR*		 ClassicButtonNameFromBit [16];
		// get the frequency from speaker_freq enum
		static const unsigned	 FreqLookup				  [10];

	public: // methods
		// call Connect() first - returns true if wiimote was found & enabled
		//  - 'wiimote_index' specifies which *installed* (not necessarily
		//     *connected*) wiimote should be tried (0 = first).
		//    if you just want the first *available* wiimote that isn't already
		//     in use, pass in FIRST_AVAILABLE (default).
		//  - 'force_hidwrites' forces HID output method - less efficient so
		//	   only use for testing.
		static const unsigned FIRST_AVAILABLE = -1;
		bool Connect				 (unsigned wiimote_index = FIRST_AVAILABLE,
									  bool force_hidwrites   = false);
		// disconnect from the controller and stop reading data from it
		void Disconnect				 ();
		// set wiimote reporting mode (call after Connnect())
		// continous = true forces the wiimote to send constant updates, even when
		//			    nothing has changed.
		//			 = false only sends data when something has changed (note that
		//			    acceleration data will cause frequent updates anyway as it
		//			    jitters even when the wiimote is stationary)
		void SetReportType			 (input_report type, bool continuous = false);
		
		// to read the state via polling (reading the public state data direct from
		//  the wiimote object) you must call RefreshState() at the top of every pass.
		//  returns a combination of flags to indicate which state (if any) has
		//  changed since the last call.
		inline state_change_flags RefreshState ()	{ return _RefreshState(false); }

		// reset the wiimote (changes report type to non-continuous buttons,
		//					  clears LEDs & rumble, mutes & disables speaker)
		void Reset					 ();
		// set/clear the wiimote LEDs
		void SetLEDs				 (BYTE led_bits); // bits 0-3
		// set/clear rumble
		void SetRumble				 (bool on);
		// alternative - rumble for a fixed amount of time
		void RumbleForAsync			 (unsigned milliseconds);

		// experimental speaker support:
		bool MuteSpeaker			 (bool on);
		bool EnableSpeaker			 (bool on);
		bool PlaySquareWave			 (speaker_freq freq, BYTE volume = 0x40);
		// note: PlaySample currently streams from the passed-in wiimote_sample -
		//		  don't delete it until playback has stopped.
		bool PlaySample				 (const		   wiimote_sample &sample,
									  BYTE		   volume		 = 0x40,
									  speaker_freq freq_override = FREQ_NONE);

		// 16bit mono sample loading/conversion to native format:
		//  .wav sample
		static bool Load16bitMonoSampleWAV	 (const TCHAR* filepath,
											  wiimote_sample &out);
		//  raw 16bit mono audio data (can be signed or unsigned)
		static bool Load16BitMonoSampleRAW	 (const TCHAR* filepath,
											  bool		   _signed,
											  speaker_freq freq,
											  wiimote_sample &out);
		//  converts a 16bit mono sample array to a wiimote_sample
		static bool Convert16bitMonoSamples  (const short* samples,
											  bool		   _signed,
											  DWORD		   length,
											  speaker_freq freq,
											  wiimote_sample &out);
		
		// state recording - records state snapshots to a 'state_history' supplied
		//					  by the caller.  states are timestamped and only added
		//					  to the list when the specified state changes.
		struct state_event {
			DWORD		  time_ms;	// system timestamp in milliseconds
			wiimote_state state;
			};
		typedef list<state_event> state_history;
		static const unsigned UNTIL_STOP = -1;
		// - pass in a 'state_history' list, and don't destroy/change it until
		//	  recording is stopped.  note the list will be cleared first.
		// - you can request a specific duration (and use IsRecordingState() to detect
		//    the end), or UNTIL_STOP.  StopRecording() can be called either way.
		// - you can specify specific state changes that will trigger an insert
		//    into the history (others are ignored).
		void RecordState  (state_history	  &events_out,
						   unsigned			  max_time_ms		  = UNTIL_STOP,
						   state_change_flags change_trigger	  = CHANGED_ALL);
		void StopRecording ();


	private: // methods
		// start reading asynchronously from the controller
		bool BeginAsyncRead			();
		// requests status update (battery level, extension status etc)
		void RequestStatusReport	();
		// reads address or register from Wiimote asynchronously (the result is
		//  parsed internally whenever it arrives)
		bool ReadAddress			(int address, short size);
		// write a single BYTE to a wiimote address or register
		inline void WriteData		(int address, BYTE data) { WriteData(address, 1, &data); }
		// write a BYTE array to a wiimote address or register
		void WriteData				(int address, BYTE size, const BYTE* buff);
		// callback when data is ready to be processed
		void OnReadData				(DWORD bytes_read);
		// parse individual reports by type
		int	 ParseInput				(BYTE* buff);
		// initializes an extension when plugged.
		void InitializeExtension	();
		// decrypts data sent from the extension
		void DecryptBuffer			(BYTE* buff, unsigned size);
		// parses a status report
		int	 ParseStatus		    (BYTE* buff);
		// parses the buttons
		int	 ParseButtons			(BYTE* buff);
		// parses accelerometer data
		int	 ParseAccel				(BYTE* buff);
		bool EstimateOrientationFrom(wiimote_state::acceleration &accel);
		void ApplyJoystickDeadZones (wiimote_state::joystick &joy);
		// parse IR data from report
		int  ParseIR				(BYTE* buff);
		// parse data from an extension.
		int  ParseExtension			(BYTE* buff, unsigned offset);
		// parse data returned from a read report
		int  ParseReadAddress		(BYTE* buff);
		// read calibration information stored on Wiimote
		void ReadCalibration		();
		// turn on the IR sensor (the mode must match the reporting mode caps)
		void EnableIR				(wiimote_state::ir::mode mode);
		// disable the IR sensor
		void DisableIR				();
		// write a report to the Wiimote (NULL = use 'WriteBuff')
		bool WriteReport			(BYTE* buff = NULL);
		bool StartSampleThread		();
		// initialize the report data buffer
		inline void ClearReport		()	{ memset(WriteBuff, 0, REPORT_LENGTH); }
		// returns the rumble BYTE that needs to be sent with reports.
		inline BYTE GetRumbleBit	()	const { return (Internal.bRumble ? 0x01 : 0x00); }
		// copy the accumulated parsed state into the public state
		state_change_flags _RefreshState (bool for_callbacks);

		// static thread funcs:
		static unsigned __stdcall ReadParseThreadfunc   (void* param);
		static unsigned __stdcall AsyncRumbleThreadfunc (void* param);
		static unsigned __stdcall SampleStreamThreadfunc(void* param);
		static unsigned __stdcall HIDwriteThreadfunc	(void* param);

	private: // data
		// wiimote output comands
		enum output_report
			{
			OUT_NONE			= 0x00,
			OUT_LEDs			= 0x11,
			OUT_TYPE			= 0x12,
			OUT_IR				= 0x13,
			OUT_SPEAKER_ENABLE	= 0x14,
			OUT_STATUS			= 0x15,
			OUT_WRITEMEMORY		= 0x16,
			OUT_READMEMORY		= 0x17,
			OUT_SPEAKER_DATA	= 0x18,
			OUT_SPEAKER_MUTE	= 0x19,
			OUT_IR2				= 0x1a,
			};
		// input reports used only internally:
		static const int IN_STATUS						= 0x20;
		static const int IN_READADDRESS					= 0x21;
		// wiimote device IDs:
		static const int VID							= 0x057e; // 'Nintendo'
		static const int PID							= 0x0306; // 'Wiimote'
		// sure, we could find this out the hard way using HID, but it's 22
		static const int REPORT_LENGTH					= 22;
		// wiimote registers
		static const int REGISTER_CALIBRATION			= 0x0016;
		static const int REGISTER_IR					= 0x04b00030;
		static const int REGISTER_IR_SENSITIVITY_1		= 0x04b00000;
		static const int REGISTER_IR_SENSITIVITY_2		= 0x04b0001a;
		static const int REGISTER_IR_MODE				= 0x04b00033;
		static const int REGISTER_EXTENSION_INIT		= 0x04a40040;
		static const int REGISTER_EXTENSION_TYPE		= 0x04a400fe;
		static const int REGISTER_EXTENSION_CALIBRATION	= 0x04a40020;

		HANDLE			 Handle;		  // read/write device handle
		OVERLAPPED		 Overlapped;	  // for async Read/WriteFile() IO
		HANDLE			 ReadParseThread; // waits for overlapped reads & parses result
		EVENT			 DataRead;		  // signals overlapped read complete
		bool			 bUseHIDwrite;	  // alternative write method (less efficient
										  //  but required for some BT stacks (eg. MS')
		// HidD_SetOutputReport is only supported from XP onwards, so detect &
		//  load it dynamically:
		static HMODULE	 HidDLL;	
		static hidwrite_ptr _HidD_SetOutputReport;
		
		volatile bool	 bStatusReceived; // for output method detection
		volatile bool	 bConnectionLost; // auto-Disconnect()s if set
		static unsigned	 _TotalCreated;
		static unsigned	 _TotalConnected;
		input_report	 ReportType;	  // type of data the wiimote delivers	
		// IO buffers
		BYTE			 WriteBuff [REPORT_LENGTH];
		BYTE			 ReadBuff  [REPORT_LENGTH];
		// for polling, state is updated on a thread internally, and made public
		//  via _RefreshState()
		CRITICAL_SECTION StateLock;	
		wiimote_state	 Internal;
		state_change_flags InternalChanged; // state changes since last RefreshState()
		// periodic status report requests (for battery level and connection loss
		//  detection)
		DWORD			 NextStatusTime;
		// async Hidd_WriteReport() thread
		HANDLE			 HIDwriteThread;
		queue<BYTE*>	 HIDwriteQueue;
		CRITICAL_SECTION HIDwriteQueueLock;	// queue must be locked before being modified
		// async rumble
		HANDLE			 AsyncRumbleThread;	// automatically disables rumble if requested
		volatile DWORD	 AsyncRumbleTimeout;
		// orientation estimation
		unsigned		 WiimoteNearGUpdates;
		unsigned		 NunchukNearGUpdates;
		// audio
		HANDLE			 SampleThread;
		const wiimote_sample* volatile CurrentSample;	// otherwise playing square wave
		// state recording
		struct recording
			{
			volatile bool	bEnabled;
			state_history	*StateHistory;
			volatile DWORD	StartTimeMS;
			volatile DWORD	EndTimeMS;		// can be UNTIL_STOP
			unsigned		TriggerFlags;	// wiimote changes trigger a state event
			unsigned		ExtTriggerFlags;// extension changes "
			} Recording;
	};