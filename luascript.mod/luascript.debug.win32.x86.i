ModuleInfo "Version: 1.15"
ModuleInfo "Author: Thomas Mayer"
ModuleInfo "License: Public Domain"
ModuleInfo "Modserver: BRL"
ModuleInfo "History: 1.15 Release"
ModuleInfo "History: ScriptEngine -> TLuaScriptEngine"
ModuleInfo "History: changed lua_open to lua_Lnewstate for LUA 5.1"
ModuleInfo "History: disabled luaopen_io due to unknown problem"
ModuleInfo "History: removed lua_state parameters from numerous methods"
import brl.blitz
import axe.lua
import brl.linkedlist
import brl.basic
SE_TYPE_NUMBER%=0
SE_TYPE_STRING%=1
SE_TYPE_BOOLEAN%=2
SE_TYPE_TABLE%=3
SE_Parameter^brl.blitz.Object{
.m_paramType%&
.m_valueNumber!&
.m_valueString$&
.m_valueBoolean%&
-New%()="_axe_luascript_SE_Parameter_New"
-Delete%()="_axe_luascript_SE_Parameter_Delete"
}="axe_luascript_SE_Parameter"
TLuaScriptEngine^brl.blitz.Object{
.m_started%&
.m_source$&
.m_lastErrorString$&
.m_lastErrorNumber%&
.m_lua_state@*&
.m_paramList:brl.linkedlist.TList&
.m_resultString$&
.m_resultNumber!&
.m_resultBoolean%&
.m_resultType%&
-New%()="_axe_luascript_TLuaScriptEngine_New"
-Delete%()="_axe_luascript_TLuaScriptEngine_Delete"
-Reset%()="_axe_luascript_TLuaScriptEngine_Reset"
-ReturnStringToLua%(value$)="_axe_luascript_TLuaScriptEngine_ReturnStringToLua"
-ReturnNumberToLua%(value!)="_axe_luascript_TLuaScriptEngine_ReturnNumberToLua"
-ReturnBooleanToLua%(value%)="_axe_luascript_TLuaScriptEngine_ReturnBooleanToLua"
-AddFunction%(func%(ls@*),name$)="_axe_luascript_TLuaScriptEngine_AddFunction"
-BeginLUAFunctionCall%()="_axe_luascript_TLuaScriptEngine_BeginLUAFunctionCall"
-CheckString$(index%)="_axe_luascript_TLuaScriptEngine_CheckString"
-CheckNumber!(index%)="_axe_luascript_TLuaScriptEngine_CheckNumber"
-CheckBoolean%(index%)="_axe_luascript_TLuaScriptEngine_CheckBoolean"
-AddStringParameter%(value$)="_axe_luascript_TLuaScriptEngine_AddStringParameter"
-AddNumberParameter%(value!)="_axe_luascript_TLuaScriptEngine_AddNumberParameter"
-AddBooleanParameter%(value%)="_axe_luascript_TLuaScriptEngine_AddBooleanParameter"
-CallFunction%(functionName$,wantResult%)="_axe_luascript_TLuaScriptEngine_CallFunction"
-GetResultType%()="_axe_luascript_TLuaScriptEngine_GetResultType"
-GetResultNumber!()="_axe_luascript_TLuaScriptEngine_GetResultNumber"
-GetResultBoolean%()="_axe_luascript_TLuaScriptEngine_GetResultBoolean"
-GetResultString$()="_axe_luascript_TLuaScriptEngine_GetResultString"
-ProcessError%(errorCode%)="_axe_luascript_TLuaScriptEngine_ProcessError"
-SetScriptText%(scriptText$,scriptName$)="_axe_luascript_TLuaScriptEngine_SetScriptText"
-RunScriptFile%(scriptFile$)="_axe_luascript_TLuaScriptEngine_RunScriptFile"
-RunScript%()="_axe_luascript_TLuaScriptEngine_RunScript"
-GetLastErrorString$()="_axe_luascript_TLuaScriptEngine_GetLastErrorString"
-GetLastErrorNumber%()="_axe_luascript_TLuaScriptEngine_GetLastErrorNumber"
-ShutDown%()="_axe_luascript_TLuaScriptEngine_ShutDown"
-Started%()="_axe_luascript_TLuaScriptEngine_Started"
}="axe_luascript_TLuaScriptEngine"
