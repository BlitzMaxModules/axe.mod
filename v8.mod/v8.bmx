
Strict

Rem
bbdoc: v8 javascript engine
about:
thankyou googlecode v8 team!
End Rem
Module axe.v8

ModuleInfo "Version: trunk"
ModuleInfo "Authors: Copyright 2009 the V8 project authors"
ModuleInfo "License: MIT BSD seee LICENCE file for more info"
ModuleInfo "History: 1.01 Release"
ModuleInfo "History: Initial release"

ModuleInfo "CC_OPTS: -D V8_TARGET_ARCH_IA32"
	
Import "src/ai32/*.h"
Import "src/*.h"

Import "libraries.cpp"	' baked with scons

?macos
Import "src/platform-macos.cpp"
?linux
Import "src/platform-linux.cpp"
Import "src/platform-posix.cpp"
?win32
Import "src/platform-win32.cpp"
?

Import "src/dtoa-config.c"

Import "src/ia32/cfg-ia32.cpp"

Import "src/ia32/assembler-ia32.cpp"
Import "src/ia32/builtins-ia32.cpp"
Import "src/ia32/codegen-ia32.cpp"
Import "src/ia32/cpu-ia32.cpp"
Import "src/ia32/debug-ia32.cpp"
Import "src/ia32/disasm-ia32.cpp"
Import "src/ia32/frames-ia32.cpp"
Import "src/ia32/ic-ia32.cpp"
Import "src/ia32/jump-target-ia32.cpp"
Import "src/ia32/macro-assembler-ia32.cpp"
Import "src/ia32/regexp-macro-assembler-ia32.cpp"
Import "src/ia32/register-allocator-ia32.cpp"
Import "src/ia32/simulator-ia32.cpp"
Import "src/ia32/stub-cache-ia32.cpp"
Import "src/ia32/virtual-frame-ia32.cpp"


Import "src/cfg.cpp"
Import "src/log-utils.cpp"
Import "src/frame-element.cpp"

Import "src/accessors.cpp"
Import "src/allocation.cpp"

Import "src/api.cpp"

Import "src/assembler.cpp"
Import "src/ast.cpp"
Import "src/bootstrapper.cpp"
Import "src/builtins.cpp"
Import "src/checks.cpp"
Import "src/code-stubs.cpp"
Import "src/codegen.cpp"
Import "src/compilation-cache.cpp"
Import "src/compiler.cpp"
Import "src/contexts.cpp"
Import "src/conversions.cpp"
Import "src/counters.cpp"

Import "src/dateparser.cpp"
Import "src/debug-agent.cpp"
Import "src/debug.cpp"
Import "src/disassembler.cpp"
Import "src/execution.cpp"
Import "src/factory.cpp"
Import "src/flags.cpp"
Import "src/frames.cpp"
Import "src/func-name-inferrer.cpp"
Import "src/global-handles.cpp"
Import "src/handles.cpp"
Import "src/hashmap.cpp"
Import "src/heap.cpp"
Import "src/ic.cpp"
Import "src/interpreter-irregexp.cpp"
Import "src/jsregexp.cpp"
Import "src/jump-target.cpp"
Import "src/log.cpp"
Import "src/mark-compact.cpp"
Import "src/messages.cpp"
Import "src/objects-debug.cpp"
Import "src/objects.cpp"
Import "src/oprofile-agent.cpp"
Import "src/parser.cpp"

Import "src/prettyprinter.cpp"
Import "src/property.cpp"
Import "src/regexp-macro-assembler-irregexp.cpp"
Import "src/regexp-macro-assembler-tracer.cpp"
Import "src/regexp-macro-assembler.cpp"
Import "src/regexp-stack.cpp"
Import "src/register-allocator.cpp"
Import "src/rewriter.cpp"
Import "src/runtime.cpp"
Import "src/scanner.cpp"
Import "src/scopeinfo.cpp"
Import "src/scopes.cpp"
Import "src/serialize.cpp"
Import "src/snapshot-common.cpp"
Import "src/snapshot-empty.cpp"
Import "src/spaces.cpp"
Import "src/string-stream.cpp"
Import "src/stub-cache.cpp"
Import "src/token.cpp"
Import "src/top.cpp"
Import "src/unicode.cpp"
Import "src/usage-analyzer.cpp"
Import "src/utils.cpp"
Import "src/v8-counters.cpp"
Import "src/v8.cpp"
Import "src/v8threads.cpp"
Import "src/variables.cpp"
Import "src/version.cpp"
Import "src/virtual-frame.cpp"
Import "src/zone.cpp"

Import "b8.cpp"

Extern 
Function v8main%(script$z)
End Extern

Function v8(script$)
	DebugLog "v8:"+script
	v8main(script)
End Function



Rem
Import "src/platform-freebsd.cpp"
Import "src/platform-nullos.cpp"
Import "src/mksnapshot.cpp"
Import "src/d8-posix.cpp" 
Import "src/d8-windows.cpp"
Import "src/d8.cpp"
Import "src/d8-debug.cpp" 
EndRem
