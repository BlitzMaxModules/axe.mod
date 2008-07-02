Strict

Rem
bbdoc: LUA Core
End Rem
Module axe.lua

ModuleInfo "Version: 1.22"
ModuleInfo "Author: Tecgraf,PUC-Rio"
ModuleInfo "License: MIT License"
ModuleInfo "Modserver: BRL"
ModuleInfo "Credit: Adapted for BlitzMax by Thomas Mayer, Noel Cower, Andreas Rozek and Simon Armstrong"

ModuleInfo "History: 1.22"
ModuleInfo "History: added lots of definitions to support most of the official Lua 5.1.1 API"
ModuleInfo "History: 1.21"
ModuleInfo "History: removed luac.c from build list"
ModuleInfo "History: 1.20"
ModuleInfo "History: fixed missing paramters in lua_createtable definition"
ModuleInfo "History: 1.19"
ModuleInfo "History: updated with lua5.1.1 source"
ModuleInfo "History: 1.18"
ModuleInfo "History: added extra imports and luaL_openlibs decl"
ModuleInfo "History: 1.17"
ModuleInfo "History: added luaL_loadstring fixed missing lua_dostring"
ModuleInfo "History: 1.16"
ModuleInfo "History: Added lua_newtable as a BMax function"
ModuleInfo "History: Changed extern'd lua_newtable to lua_createtable"
ModuleInfo "History: Added lua_load, lua_dostring and lua_dobuffer."
ModuleInfo "History: 1.15 Release"
ModuleInfo "History: New LUA 5.1 based build"
ModuleInfo "History: Modified constants and added new wrappers for 5.1 compatability"
ModuleInfo "History: 1.14 Release"
ModuleInfo "History: Added luaopen_debug and ldblib.c"
ModuleInfo "History: Replaced byte ptr with $z (CString) where a C string is expected"
ModuleInfo "History: 1.12 Release"
ModuleInfo "History: Removed lua.h import"

Import "lua-5.1/src/lstate.c"
Import "lua-5.1/src/llex.c"
Import "lua-5.1/src/ltm.c"
Import "lua-5.1/src/lstring.c"
Import "lua-5.1/src/ltable.c"
Import "lua-5.1/src/lfunc.c"
Import "lua-5.1/src/ldo.c"
Import "lua-5.1/src/lgc.c"
Import "lua-5.1/src/lzio.c"
Import "lua-5.1/src/lobject.c"
Import "lua-5.1/src/lparser.c"
Import "lua-5.1/src/lvm.c"
Import "lua-5.1/src/lundump.c"
Import "lua-5.1/src/lmem.c"
Import "lua-5.1/src/lcode.c"
Import "lua-5.1/src/ldebug.c"
Import "lua-5.1/src/lopcodes.c"
Import "lua-5.1/src/lapi.c"
Import "lua-5.1/src/ldump.c"
Import "lua-5.1/src/lbaselib.c"
Import "lua-5.1/src/lauxlib.c"
Import "lua-5.1/src/liolib.c"
Import "lua-5.1/src/lmathlib.c"
Import "lua-5.1/src/lstrlib.c"
Import "lua-5.1/src/ltablib.c"
Import "lua-5.1/src/ldblib.c"

Import "lua-5.1/src/linit.c"
Import "lua-5.1/src/loadlib.c"
Import "lua-5.1/src/loslib.c"
'Import "lua-5.1/src/lua.c"
Import "lua-5.1/src/luac.c"
Import "lua-5.1/src/print.c"

' ******************************************************************************
' *                                                                            *
' *                            Constant Definitions                            *
' *                                                                            *
' ******************************************************************************

' **** (lua.h) some basic definitions - just to be complete ****

  Const LUA_VERSION:String   = "Lua 5.1"
  Const LUA_RELEASE:String   = "Lua 5.1.1"
  Const LUA_VERSION_NUM:Int  = 501
  Const LUA_COPYRIGHT:String = "Copyright (C) 1994-2006 Lua.org, PUC-Rio"
  Const LUA_AUTHORS:String   = "R. Ierusalimschy, L. H. de Figueiredo & W. Celes"

' **** (lua.h) option for multiple returns in `lua_pcall' and `lua_call' ****

  Const LUA_MULTRET = -1

' **** (lua.h) pseudo-indices ****

  Const LUA_REGISTRYINDEX = -10000
  Const LUA_ENVIRONINDEX  = -10001
  Const LUA_GLOBALSINDEX  = -10002

' **** (lua.h) thread status (0 is OK) ****

  Const LUA_YIELD_    = 1   ' added _ after LUA_YIELD because of lua_yield func.
  Const LUA_ERRRUN    = 2
  Const LUA_ERRSYNTAX = 3
  Const LUA_ERRMEM    = 4
  Const LUA_ERRERR    = 5

' **** (lua.h) basic types ****

  Const LUA_TNONE          = -1
  Const LUA_TNIL           =  0
  Const LUA_TBOOLEAN       =  1
  Const LUA_TLIGHTUSERDATA =  2
  Const LUA_TNUMBER        =  3
  Const LUA_TSTRING        =  4
  Const LUA_TTABLE         =  5
  Const LUA_TFUNCTION      =  6
  Const LUA_TUSERDATA      =  7
  Const LUA_TTHREAD        =  8

' **** (lua.h) garbage-collection options ****

  Const LUA_GCSTOP       = 0
  Const LUA_GCRESTART    = 1
  Const LUA_GCCOLLECT    = 2
  Const LUA_GCCOUNT      = 3
  Const LUA_GCCOUNTB     = 4
  Const LUA_GCSTEP       = 5
  Const LUA_GCSETPAUSE   = 6
  Const LUA_GCSETSTEPMUL = 7

' ******************************************************************************
' *                                                                            *
' *                          The Lua API (Functions)                           *
' *                                                                            *
' ******************************************************************************

Extern
  Function lua_Alloc:Byte Ptr Ptr   (ud:Byte Ptr, pointer:Byte Ptr, osize:Int, nsize:Int)
  Function lua_atpanic:Byte Ptr     (lua_state:Byte Ptr, panicf:Int(ls:Byte Ptr))
  Function lua_call                 (lua_state:Byte Ptr, nargs:Int, nresults:Int)
  Function lua_checkstack:Int       (lua_state:Byte Ptr, extra:Int)
  Function lua_close                (lua_state:Byte Ptr)
  Function lua_concat               (lua_state:Byte Ptr, n:Int)
  Function lua_cpcall:Int           (lua_state:Byte Ptr, func:Int(ls:Byte Ptr), ud:Byte Ptr)
  Function lua_createtable          (lua_state:Byte Ptr, narr:Int, nrec:Int)
  Function lua_dump:Int             (lua_state:Byte Ptr, writer%(ls@ Ptr,d@ Ptr,sz%,ud@ Ptr), data:Byte Ptr)
  Function lua_equal:Int            (lua_state:Byte Ptr, index1:Int, index2:Int)
  Function lua_error:Int            (lua_state:Byte Ptr)
  Function lua_gc:Int               (lua_state:Byte Ptr, what:Int, data:Int)
  Function lua_getallocf:Byte Ptr   (lua_state:Byte Ptr, ud:Byte Ptr Ptr)
  Function lua_getfenv              (lua_state:Byte Ptr, index:Int)
  Function lua_getfield             (lua_state:Byte Ptr, index:Int, k$z)
  Function lua_getmetatable:Int     (lua_state:Byte Ptr, index:Int)
  Function lua_gettable             (lua_state:Byte Ptr, index:Int)
  Function lua_gettop:Int           (lua_state:Byte Ptr)
  Function lua_insert               (lua_state:Byte Ptr, index:Int)
  Function lua_iscfunction:Int      (lua_state:Byte Ptr, index:Int)
  Function lua_isnumber:Int         (lua_state:Byte Ptr, index:Int)
  Function lua_isstring:Int         (lua_state:Byte Ptr, index:Int)
  Function lua_isuserdata:Int       (lua_state:Byte Ptr, index:Int)
  Function lua_lessthan:Int         (lua_state:Byte Ptr, index1:Int, index2:Int)
  Function lua_load:Int             (lua_state:Byte Ptr, reader$z(ls@ Ptr,d@ Ptr,sz% Ptr), data@ Ptr, chunkname$z)
  Function lua_newstate:Byte Ptr    (f:Byte Ptr, ud:Byte Ptr)
  Function lua_newthread:Byte Ptr   (lua_state:Byte Ptr)
  Function lua_newuserdata:Byte Ptr (lua_state:Byte Ptr, size:Int)
  Function lua_next:Int             (lua_state:Byte Ptr, index:Int)
  Function lua_objlen:Int           (lua_state:Byte Ptr, index:Int)
  Function lua_pcall:Int            (lua_state:Byte Ptr, nargs:Int, nresults:Int, errfunc:Int)
  Function lua_pushboolean          (lua_state:Byte Ptr, b:Int)
  Function lua_pushcclosure         (lua_state:Byte Ptr, fn:Int(ls:Byte Ptr), n:Int)
' function lua_pushfstring$z        (lua_state:byte ptr, fmt$z, ...)
  Function lua_pushinteger          (lua_state:Byte Ptr, n:Long)
  Function lua_pushlightuserdata    (lua_state:Byte Ptr, p:Byte Ptr)
  Function lua_pushlstring          (lua_state:Byte Ptr, s$z, size:Int)
  Function lua_pushnil              (lua_state:Byte Ptr)
  Function lua_pushnumber           (lua_state:Byte Ptr, n:Double)
  Function lua_pushstring           (lua_state:Byte Ptr, s$z)
  Function lua_pushthread:Int       (lua_state:Byte Ptr)
  Function lua_pushvalue            (lua_state:Byte Ptr, index:Int)
' function lua_pushvfstring$z       (lua_state:byte ptr, fmt$z, argp:???)
  Function lua_rawequal:Int         (lua_state:Byte Ptr, index1:Int, index2:Int)
  Function lua_rawget               (lua_state:Byte Ptr, index:Int)
  Function lua_rawgeti              (lua_state:Byte Ptr, index:Int, n:Int)
  Function lua_rawset               (lua_state:Byte Ptr, index:Int)
  Function lua_rawseti              (lua_state:Byte Ptr, index:Int, n:Int)
  Function lua_remove               (lua_state:Byte Ptr, index:Int)
  Function lua_replace              (lua_state:Byte Ptr, index:Int)
  Function lua_resume:Int           (lua_state:Byte Ptr, narg:Int)
  Function lua_setallocf            (lua_state:Byte Ptr, f:Byte Ptr, ud:Byte Ptr)
  Function lua_setfenv:Int          (lua_state:Byte Ptr, index:Int)
  Function lua_setfield             (lua_state:Byte Ptr, index:Int, k$z)
  Function lua_setmetatable:Int     (lua_state:Byte Ptr, index:Int)
  Function lua_settable             (lua_state:Byte Ptr, index:Int)
  Function lua_settop               (lua_state:Byte Ptr, index:Int)
  Function lua_status:Int           (lua_state:Byte Ptr)
  Function lua_toboolean:Int        (lua_state:Byte Ptr, index:Int)
  Function lua_tocfunction:Byte Ptr (lua_state:Byte Ptr, index:Int)
  Function lua_tointeger:Long       (lua_state:Byte Ptr, index:Int)
  Function lua_tolstring$z          (lua_state:Byte Ptr, index:Int, size:Int Ptr)
  Function lua_tonumber:Double      (lua_state:Byte Ptr, index:Int)
  Function lua_topointer:Byte Ptr   (lua_state:Byte Ptr, index:Int)
  Function lua_tothread:Byte Ptr    (lua_state:Byte Ptr, index:Int)
  Function lua_touserdata:Byte Ptr  (lua_state:Byte Ptr, index:Int)
  Function lua_type:Int             (lua_state:Byte Ptr, index:Int)
  Function lua_typename$z           (lua_state:Byte Ptr, tp:Int)
  Function lua_xmove                (fromState:Byte Ptr, toState:Byte Ptr, n:Int)
  Function lua_yield:Int            (lua_state:Byte Ptr, nresults:Int)
End Extern

' ******************************************************************************
' *                                                                            *
' *                            The Lua API (Macros)                            *
' *                                                                            *
' ******************************************************************************

  Function lua_getglobal (lua_state:Byte Ptr, name:String)
    lua_getfield(lua_state, LUA_GLOBALSINDEX, name)
  End Function

  Function lua_isboolean:Int (lua_state:Byte Ptr, index:Int)
    Return (lua_type(lua_state,index) = LUA_TBOOLEAN)
  End Function

  Function lua_isfunction:Int (lua_state:Byte Ptr, index:Int)
    Return (lua_type(lua_state,index) = LUA_TFUNCTION)
  End Function

  Function lua_islightuserdata:Int (lua_state:Byte Ptr, index:Int)
    Return (lua_type(lua_state,index) = LUA_TLIGHTUSERDATA)
  End Function

  Function lua_isnil:Int (lua_state:Byte Ptr, index:Int)
    Return (lua_type(lua_state,index) = LUA_TNIL)
  End Function

  Function lua_istable:Int (lua_state:Byte Ptr, index:Int)
    Return (lua_type(lua_state,index) = LUA_TTABLE)
  End Function

  Function lua_isthread:Int (lua_state:Byte Ptr, index:Int)
    Return (lua_type(lua_state,index) = LUA_TTHREAD)
  End Function

  Function lua_newtable (lua_state:Byte Ptr)
    lua_createtable(lua_state,0,0)
  End Function

  Function lua_pop (lua_state:Byte Ptr, n:Int)
    lua_settop(lua_state,-(n)-1)
  End Function

  Function lua_pushcfunction (lua_state:Byte Ptr, fn:Int(ls:Byte Ptr))
    lua_pushcclosure(lua_state, fn, 0)
  End Function

  Function lua_register (lua_state:Byte Ptr, name$, f:Int(ls:Byte Ptr))
    lua_pushcfunction(lua_state,f)
    lua_setglobal    (lua_state,name)
  End Function

  Function lua_setglobal (lua_state:Byte Ptr, name:String)
    lua_setfield(lua_state, LUA_GLOBALSINDEX, name)
  End Function

  Function lua_tostring:String (lua_state:Byte Ptr, index:Int)
    Return lua_tolstring(lua_state, index, Null)
  End Function

' ******************************************************************************
' *                                                                            *
' *                     The Auxiliary Library (Functions)                      *
' *                                                                            *
' ******************************************************************************

Extern
  Function luaL_addlstring          (B:Byte Ptr, s$z, l:Int)
  Function luaL_addstring           (B:Byte Ptr, s$z)
  Function luaL_addvalue            (B:Byte Ptr)
  Function luaL_argerror:Int        (lua_state:Byte Ptr, numarg:Int, extramsg$z)
  Function luaL_buffinit            (lua_state:Byte Ptr, B:Byte Ptr)
  Function luaL_callmeta:Int        (lua_state:Byte Ptr, obj:Int, e$z)
  Function luaL_checkany            (lua_state:Byte Ptr, narg:Int)
  Function luaL_checkinteger:Long   (lua_state:Byte Ptr, narg:Int)
  Function luaL_checklstring$z      (lua_state:Byte Ptr, narg:Int, size:Int)
  Function luaL_checknumber:Double  (lua_state:Byte Ptr, narg:Int)
' function luaL_checkoption:int     (lua_state:byte ptr, narg:int, def$z, lst$z[])
  Function luaL_checkstack          (lua_state:Byte Ptr, sz:Int, msg$z)
  Function luaL_checktype           (lua_state:Byte Ptr, narg:Int, t:Int)
  Function luaL_checkudata:Byte Ptr (lua_state:Byte Ptr, narg:Int, tname$z)
' function luaL_error:int           (lua_state:byte ptr, fmt$z, ...)
  Function luaL_getmetafield:Int    (lua_state:Byte Ptr, obj:Int, e$z)
  Function luaL_gsub$z              (lua_state:Byte Ptr, s$z, p$z, r$z)
  Function luaL_loadbuffer:Int      (lua_state:Byte Ptr, buff$z, sz:Int, name$z)
  Function luaL_loadfile:Int        (lua_state:Byte Ptr, filename$z)
  Function luaL_loadstring:Int      (lua_state:Byte Ptr, s$z)
  Function luaL_newmetatable:Int    (lua_state:Byte Ptr, tname$z)
  Function luaL_newstate:Byte Ptr   ()
  Function luaL_openlibs            (lua_state:Byte Ptr)
  Function luaL_optinteger:Long     (lua_state:Byte Ptr, narg:Int, d:Long)
  Function luaL_optlstring$z        (lua_state:Byte Ptr, narg:Int, d$z, size:Int)
  Function luaL_optnumber:Double    (lua_state:Byte Ptr, narg:Int, d:Double)
  Function luaL_prepbuffer$z        (B:Byte Ptr)
  Function luaL_pushresult          (B:Byte Ptr)
  Function luaL_ref:Int             (lua_state:Byte Ptr, t:Int)
  Function luaL_register            (lua_state:Byte Ptr, libname$z, l:Byte Ptr)
  Function luaL_typerror:Int        (lua_state:Byte Ptr, narg:Int, tname$z)
  Function luaL_unref               (lua_state:Byte Ptr, t:Int, ref:Int)
  Function luaL_where               (lua_state:Byte Ptr, lvl:Int)
End Extern

' ******************************************************************************
' *                                                                            *
' *                       The Auxiliary Library (Macros)                       *
' *                                                                            *
' ******************************************************************************

  Function luaL_addchar (B:Byte Ptr, c:String)
    luaL_addlstring(B,c,1)
  End Function

  Function luaL_argcheck (lua_state:Byte Ptr, cond:Int, numarg:Int, extramsg:String)
    If (Not cond) Then luaL_argerror(lua_state, numarg, extramsg)
  End Function

  Function luaL_checkint:Int (lua_state:Byte Ptr, narg:Int)
    Return Int(luaL_checkinteger(lua_state, narg))
  End Function

  Function luaL_checklong:Long (lua_state:Byte Ptr, narg:Int)
    Return luaL_checkinteger(lua_state, narg)
  End Function

  Function luaL_checkstring:String (lua_state:Byte Ptr, narg:Int)
    Return luaL_checklstring(lua_state, narg, 0)
  End Function

  Function luaL_dofile:Int (lua_state:Byte Ptr, filename:String)
    If (luaL_loadfile(lua_state,filename) <> 0) Then
      Return 1
    Else
      Return lua_pcall(lua_state, 0, LUA_MULTRET, 0)
    End If
  End Function

  Function luaL_dostring:Int (lua_state:Byte Ptr, str:String)
    If (luaL_loadstring(lua_state,str) <> 0) Then
      Return 1
    Else
      Return lua_pcall(lua_state, 0, LUA_MULTRET, 0)
    End If
  End Function

  Function luaL_getmetatable (lua_state:Byte Ptr, tname:String)
    lua_getfield(lua_state, LUA_REGISTRYINDEX, tname)
  End Function

  Function luaL_optint:Int (lua_state:Byte Ptr, narg:Int, d:Int)
    Return Int(luaL_optinteger(lua_state, narg, Long(d)))
  End Function

  Function luaL_optlong:Long (lua_state:Byte Ptr, narg:Int, d:Long)
    Return luaL_optinteger(lua_state, narg, d)
  End Function

  Function luaL_optstring:String (lua_state:Byte Ptr, narg:Int, d:String)
    Return luaL_optlstring(lua_state, narg, d, 0)
  End Function

  Function luaL_typename:String (lua_state:Byte Ptr, idx:Int)
    Return lua_typename(lua_state, lua_type(lua_state,idx))
  End Function

' ******************************************************************************
' *                                                                            *
' *     compatibility extensions (not to break existing axe.lua programs)      *
' *                                                                            *
' ******************************************************************************

Extern
  Function luaopen_base:Int    (lua_state:Byte Ptr)
  Function luaopen_debug:Int   (lua_state:Byte Ptr)
  Function luaopen_io:Int      (lua_state:Byte Ptr)
  Function luaopen_math:Int    (lua_state:Byte Ptr)
  Function luaopen_os:Int      (lua_state:Byte Ptr)
  Function luaopen_package:Int (lua_state:Byte Ptr)
  Function luaopen_string:Int  (lua_state:Byte Ptr)
  Function luaopen_table:Int   (lua_state:Byte Ptr)
End Extern

  Function lua_dobuffer:Int (lua_state:Byte Ptr, buff:String, sz:Int, name:String)
    If (luaL_loadbuffer(lua_state,buff,sz,name) <> 0) Then
      Return 1
    Else
      Return lua_pcall(lua_state, 0, LUA_MULTRET, 0)
    End If
  End Function

  Function lua_dofile:Int (lua_state:Byte Ptr, filename:String)
    Return luaL_dofile(lua_state,filename)
  End Function

  Function lua_dostring:Int (lua_state:Byte Ptr, str:String)
    Return luaL_dostring(lua_state,str)
  End Function

  Function lua_strlen:Int (lua_state:Byte Ptr, index:Int)
    Return lua_objlen(lua_state,index)
  End Function
