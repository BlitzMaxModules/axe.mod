// Copyright 2006-2008 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Platform specific code for NULLOS goes here

// Minimal include to get access to abort, fprintf and friends for bootstrapping
// messages.
#include <stdio.h>
#include <stdlib.h>

#include "v8.h"

#include "platform.h"





// Test for a NaN (not a number) value - usually defined in math.h
int isnan(double x) {
  return _isnan(x);
}


// Test for infinity - usually defined in math.h
int isinf(double x) {
  return (_fpclass(x) & (_FPCLASS_PINF | _FPCLASS_NINF)) != 0;
}

// Test if x is less than y and both nominal - usually defined in math.h
int isless(double x, double y) {
  return isnan(x) || isnan(y) ? 0 : x < y;
}


// Test if x is greater than y and both nominal - usually defined in math.h
int isgreater(double x, double y) {
  return isnan(x) || isnan(y) ? 0 : x > y;
}


// Classify floating point number - usually defined in math.h
int fpclassify(double x) {
  // Use the MS-specific _fpclass() for classification.
  int flags = _fpclass(x);

  // Determine class. We cannot use a switch statement because
  // the _FPCLASS_ constants are defined as flags.
  if (flags & (_FPCLASS_PN | _FPCLASS_NN)) return FP_NORMAL;
  if (flags & (_FPCLASS_PZ | _FPCLASS_NZ)) return FP_ZERO;
  if (flags & (_FPCLASS_PD | _FPCLASS_ND)) return FP_SUBNORMAL;
  if (flags & (_FPCLASS_PINF | _FPCLASS_NINF)) return FP_INFINITE;

  // All cases should be covered by the code above.
  ASSERT(flags & (_FPCLASS_SNAN | _FPCLASS_QNAN));
  return FP_NAN;
}


// Test sign - usually defined in math.h
int signbit(double x) {
  // We need to take care of the special case of both positive
  // and negative versions of zero.
  if (x == 0)
    return _fpclass(x) & _FPCLASS_NZ;
  else
    return x < 0;
}


// Case-insensitive bounded string comparisons. Use stricmp() on Win32. Usually
// defined in strings.h.
int strncasecmp(const char* s1, const char* s2, int n) {
  return _strnicmp(s1, s2, n);
}

int random() {
  return rand();
}


namespace v8 {
namespace internal {

// Test for finite value - usually defined in math.h
int isfinite(double x) {
  return _finite(x);
}

// Give V8 the opportunity to override the default ceil behaviour.
double ceiling(double x) {
  UNIMPLEMENTED();
  return 0;
}


void OS::StrNCpy(Vector<char> dest, const char* src, size_t n) {
	if(n>dest.length()){
		n=dest.length();
	}
  int result = n;
	memcpy(dest.start(), src, n) ;
	USE(result);
  ASSERT(result == 0);
}


// Initialize OS class early in the V8 startup.
void OS::Setup() {
  // Seed the random number generator.
  UNIMPLEMENTED();
}


// Returns the accumulated user time for thread.
int OS::GetUserTime(uint32_t* secs,  uint32_t* usecs) {
  UNIMPLEMENTED();
  *secs = 0;
  *usecs = 0;
  return 0;
}


// Returns current time as the number of milliseconds since
// 00:00:00 UTC, January 1, 1970.
double OS::TimeCurrentMillis() {
  UNIMPLEMENTED();
  return 0;
}


// Returns ticks in microsecond resolution.
int64_t OS::Ticks() {
  UNIMPLEMENTED();
  return 0;
}


// Returns a string identifying the current timezone taking into
// account daylight saving.
const char* OS::LocalTimezone(double time) {
  UNIMPLEMENTED();
  return "<none>";
}


// Returns the daylight savings offset in milliseconds for the given time.
double OS::DaylightSavingsOffset(double time) {
  UNIMPLEMENTED();
  return 0;
}


// Returns the local time offset in milliseconds east of UTC without
// taking daylight savings time into account.
double OS::LocalTimeOffset() {
  UNIMPLEMENTED();
  return 0;
}

int OS::ActivationFrameAlignment() {
  return 8;  // guess for mips
}

FILE* OS::FOpen(const char* path, const char* mode) {
  return fopen( path, mode);
}


// Print (debug) message to console.
void OS::Print(const char* format, ...) {
  UNIMPLEMENTED();
}


// Print (debug) message to console.
void OS::VPrint(const char* format, va_list args) {
  // Minimalistic implementation for bootstrapping.
  vfprintf(stdout, format, args);
}


// Print error message to console.
void OS::PrintError(const char* format, ...) {
  // Minimalistic implementation for bootstrapping.
  va_list args;
  va_start(args, format);
  VPrintError(format, args);
  va_end(args);
}


// Print error message to console.
void OS::VPrintError(const char* format, va_list args) {
  // Minimalistic implementation for bootstrapping.
  vfprintf(stderr, format, args);
}

int OS::SNPrintF(Vector<char> str, const char* format, ...){
//int OS::SNPrintF(char* str, size_t size, const char* format, ...) {
  UNIMPLEMENTED();
  return 0;
}

int OS::VSNPrintF(Vector<char> str,
                       const char* format,
											 va_list args){
//int OS::VSNPrintF(char* str, size_t size, const char* format, va_list args) {
  UNIMPLEMENTED();
  return 0;
}


double OS::nan_value() {
  UNIMPLEMENTED();
  return 0;
}

bool OS::IsOutsideAllocatedSpace(void* address) {
  UNIMPLEMENTED();
  return false;
}


size_t OS::AllocateAlignment() {
  UNIMPLEMENTED();
  return 0;
}


void* OS::Allocate(const size_t requested,
                   size_t* allocated,
                   bool executable) {
  UNIMPLEMENTED();
  return NULL;
}


void OS::Free(void* buf, const size_t length) {
  // TODO(1240712): potential system call return value which is ignored here.
  UNIMPLEMENTED();
}


#ifdef ENABLE_HEAP_PROTECTION

void OS::Protect(void* address, size_t size) {
  UNIMPLEMENTED();
}


void OS::Unprotect(void* address, size_t size, bool is_executable) {
  UNIMPLEMENTED();
}

#endif


void OS::Sleep(int milliseconds) {
  UNIMPLEMENTED();
}


void OS::Abort() {
	throw (L"v8 abortion");
  // Minimalistic implementation for bootstrapping.
  UNIMPLEMENTED();
}


void OS::DebugBreak() {
  UNIMPLEMENTED();
}


OS::MemoryMappedFile* OS::MemoryMappedFile::create(const char* name, int size,
    void* initial) {
  UNIMPLEMENTED();
  return NULL;
}


void OS::LogSharedLibraryAddresses() {
  UNIMPLEMENTED();
}


int OS::StackWalk(Vector<OS::StackFrame> frames) {
  UNIMPLEMENTED();
  return 0;
}


VirtualMemory::VirtualMemory(size_t size) {
  UNIMPLEMENTED();
}


VirtualMemory::~VirtualMemory() {
  UNIMPLEMENTED();
}


bool VirtualMemory::IsReserved() {
  UNIMPLEMENTED();
  return false;
}


bool VirtualMemory::Commit(void* address, size_t size, bool executable) {
  UNIMPLEMENTED();
  return false;
}


bool VirtualMemory::Uncommit(void* address, size_t size) {
  UNIMPLEMENTED();
  return false;
}


class ThreadHandle::PlatformData : public Malloced {
 public:
  explicit PlatformData(ThreadHandle::Kind kind) {
    UNIMPLEMENTED();
  }

  void* pd_data_;
};


ThreadHandle::ThreadHandle(Kind kind) {
  UNIMPLEMENTED();
  // Shared setup follows.
  data_ = new PlatformData(kind);
}


void ThreadHandle::Initialize(ThreadHandle::Kind kind) {
  UNIMPLEMENTED();
}


ThreadHandle::~ThreadHandle() {
  UNIMPLEMENTED();
  // Shared tear down follows.
  delete data_;
}


bool ThreadHandle::IsSelf() const {
  UNIMPLEMENTED();
  return false;
}


bool ThreadHandle::IsValid() const {
  UNIMPLEMENTED();
  return false;
}


Thread::Thread() : ThreadHandle(ThreadHandle::INVALID) {
  UNIMPLEMENTED();
}


Thread::~Thread() {
  UNIMPLEMENTED();
}


void Thread::Start() {
  UNIMPLEMENTED();
}


void Thread::Join() {
  UNIMPLEMENTED();
}


Thread::LocalStorageKey Thread::CreateThreadLocalKey() {
  UNIMPLEMENTED();
  return static_cast<LocalStorageKey>(0);
}


void Thread::DeleteThreadLocalKey(LocalStorageKey key) {
  UNIMPLEMENTED();
}


void* Thread::GetThreadLocal(LocalStorageKey key) {
  UNIMPLEMENTED();
  return NULL;
}


void Thread::SetThreadLocal(LocalStorageKey key, void* value) {
  UNIMPLEMENTED();
}


void Thread::YieldCPU() {
  UNIMPLEMENTED();
}


class NullMutex : public Mutex {
 public:
  NullMutex() : data_(NULL) {
    UNIMPLEMENTED();
  }

  virtual ~NullMutex() {
    UNIMPLEMENTED();
  }

  virtual int Lock() {
    UNIMPLEMENTED();
    return 0;
  }

  virtual int Unlock() {
    UNIMPLEMENTED();
    return 0;
  }

 private:
  void* data_;
};


Mutex* OS::CreateMutex() {
  UNIMPLEMENTED();
  return new NullMutex();
}


class NullSemaphore : public Semaphore {
 public:
  explicit NullSemaphore(int count) : data_(NULL) {
    UNIMPLEMENTED();
  }

  virtual ~NullSemaphore() {
    UNIMPLEMENTED();
  }

  virtual void Wait() {
    UNIMPLEMENTED();
  }

  virtual void Signal() {
    UNIMPLEMENTED();
  }
 private:
  void* data_;
};


Semaphore* OS::CreateSemaphore(int count) {
  UNIMPLEMENTED();
  return 0;//new NullSemaphore(count);
}

#ifdef ENABLE_LOGGING_AND_PROFILING

class ProfileSampler::PlatformData  : public Malloced {
 public:
  PlatformData() {
    UNIMPLEMENTED();
  }
};


ProfileSampler::ProfileSampler(int interval) {
  UNIMPLEMENTED();
  // Shared setup follows.
  data_ = new PlatformData();
  interval_ = interval;
  active_ = false;
}


ProfileSampler::~ProfileSampler() {
  UNIMPLEMENTED();
  // Shared tear down follows.
  delete data_;
}


void ProfileSampler::Start() {
  UNIMPLEMENTED();
}


void ProfileSampler::Stop() {
  UNIMPLEMENTED();
}

#endif  // ENABLE_LOGGING_AND_PROFILING

} }  // namespace v8::internal
