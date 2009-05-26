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


#ifndef V8_CODEGEN_INL_H_
#define V8_CODEGEN_INL_H_

#include "codegen.h"
#include "register-allocator-inl.h"

namespace v8 {
namespace internal {


void DeferredCode::SetEntryFrame(Result* arg) {
  ASSERT(generator()->has_valid_frame());
  generator()->frame()->Push(arg);
  enter()->set_entry_frame(new VirtualFrame(generator()->frame()));
  *arg = generator()->frame()->Pop();
}


void DeferredCode::SetEntryFrame(Result* arg0, Result* arg1) {
  ASSERT(generator()->has_valid_frame());
  generator()->frame()->Push(arg0);
  generator()->frame()->Push(arg1);
  enter()->set_entry_frame(new VirtualFrame(generator()->frame()));
  *arg1 = generator()->frame()->Pop();
  *arg0 = generator()->frame()->Pop();
}


// -----------------------------------------------------------------------------
// Support for "structured" code comments.
//
// By selecting matching brackets in disassembler output,
// code segments can be identified more easily.

#ifdef DEBUG

class Comment BASE_EMBEDDED {
 public:
  Comment(MacroAssembler* masm, const char* msg)
    : masm_(masm),
      msg_(msg) {
    masm_->RecordComment(msg);
  }

  ~Comment() {
    if (msg_[0] == '[')
      masm_->RecordComment("]");
  }

 private:
  MacroAssembler* masm_;
  const char* msg_;
};

#else

class Comment BASE_EMBEDDED {
 public:
  Comment(MacroAssembler*, const char*)  {}
};

#endif  // DEBUG


} }  // namespace v8::internal

#endif  // V8_CODEGEN_INL_H_