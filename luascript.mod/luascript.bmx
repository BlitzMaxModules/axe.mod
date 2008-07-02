Strict

Rem
bbdoc: LUA Script Engine
End Rem
Module axe.luascript

ModuleInfo "Version: 1.15"
ModuleInfo "Author: Thomas Mayer"
ModuleInfo "License: Public Domain"
ModuleInfo "Modserver: BRL"

ModuleInfo "History: 1.15 Release"
ModuleInfo "History: ScriptEngine -> TLuaScriptEngine"
ModuleInfo "History: changed lua_open to lua_Lnewstate for LUA 5.1"
ModuleInfo "History: disabled luaopen_io due to unknown problem"
ModuleInfo "History: removed lua_state parameters from numerous methods"

Import axe.lua
Import brl.linkedlist
Import brl.basic

Const SE_TYPE_NUMBER 	= 0
Const SE_TYPE_STRING 	= 1
Const SE_TYPE_BOOLEAN 	= 2
Const SE_TYPE_TABLE		= 3

Type SE_Parameter

	Field m_paramType
	Field m_valueNumber:Double
	Field m_valueString:String
	Field m_valueBoolean:Int
	
'	Function CreateString:SE_Parameter(value:String)
'		Local returnParam:SE_Parameter = New SE_Parameter
'		returnParam.m_paramType = SE_TYPE_STRING
'		returnParam.m_valueString = value
'		Return returnParam
'	End Function

'	Function CreateBoolean:SE_Parameter(value:Int)
'		Local returnParam:SE_Parameter = New SE_Parameter
'		returnParam.m_paramType = SE_TYPE_BOOLEAN
'		returnParam.m_valueBoolean = value
'		Return returnParam
'	End Function
	
'	Function CreateNumber:SE_Parameter(value:Double)
'		Local returnParam:SE_Parameter = New SE_Parameter
'		returnParam.m_paramType = SE_TYPE_NUMBER
'		returnParam.m_valueNumber = value
'		Return returnParam
'	End Function
	
End Type

Rem
bbdoc: Lua script engine type
End Rem
Type TLuaScriptEngine
	
	Field m_started:Int
	Field m_source:String
	Field m_lastErrorString:String
	Field m_lastErrorNumber:Int
	Field m_lua_state:Byte Ptr
	Field m_paramList:TList = CreateList()
	Field m_resultString:String = ""
	Field m_resultNumber:Double = 0
	Field m_resultBoolean:Int = 0
	Field m_resultType:Int = -1
			
	' Default constructor
	Method New()
		Reset()
	End Method
	
	Rem
		bbdoc: Resets the engine back to a start state
	End Rem
	Method Reset()
		If (m_lua_state) Then lua_close(m_lua_state)
	
		m_lua_state = luaL_newstate()	'lua_open()
		
		If (m_lua_state) Then
			luaopen_base(m_lua_state)
			luaopen_table(m_lua_state)
			luaopen_string(m_lua_state)
			luaopen_math(m_lua_state)
'			luaopen_io(m_lua_state)
			m_started = True
		Else
			m_started = False
		End If
	End Method
	
	Rem
		bbdoc: Returns a string back to LUA from a callback BMX function
	End Rem
	Method ReturnStringToLua(value:String)
		If (m_started) Then
			lua_pushstring(m_lua_state, value)
		End If
	End Method

	Rem
		bbdoc: Returns a number back to LUA from a callback BMX function
	End Rem
	Method ReturnNumberToLua(value:Double)
		If (m_started) Then
			lua_pushnumber(m_lua_state, value)
		End If
	End Method
	
	Rem
		bbdoc: Returns a boolean back to LUA from a callback BMX function
	End Rem
	Method ReturnBooleanToLua(value:Int)
		If (m_started) Then
			lua_pushboolean(m_lua_state, value)
		End If
	End Method

	Rem
		bbdoc: Adds a Function into the LUA environment
	End Rem
	Method AddFunction(func:Int(ls:Byte Ptr), name:String)
		If (m_started)
			lua_register(m_lua_state, name, func)
		End If
	End Method

	Rem
		bbdoc: Performs initialization required for call a function in LUA
	End Rem
	Method BeginLUAFunctionCall()
		ClearList(m_paramList)
		m_ResultType = -1
		m_ResultString = ""
		m_ResultNumber = 0
		m_ResultBoolean = 0
	End Method
	
	Rem
		bbdoc: Checks a string parameter and return its value
	End Rem
	Method CheckString:String(index:Int)
		Return String.FromCString(luaL_checkstring(m_lua_state, index))
	End Method

	Rem
		bbdoc: Checks a number parameter and return its value
	End Rem
	Method CheckNumber:Double(index:Int)
		Return luaL_checknumber(m_lua_state, index)
	End Method

'	Rem
'		bbdoc: Checks a boolean parameter And Return its value
'	End Rem
	Method CheckBoolean:Int(index:Int)
		Return luaL_checkint(m_lua_state, index)
	End Method

	Rem
		bbdoc: Adds a string parameter to a LUA function call
	End Rem
	Method AddStringParameter(value:String)
		Local param:SE_Parameter = New SE_Parameter
		param.m_paramType = SE_TYPE_STRING
		param.m_valueString = value
		ListAddLast(m_paramList, param)
	End Method
	
	Rem
		bbdoc: Adds a number parameter to a LUA function call
	End Rem
	Method AddNumberParameter(value:Double)
		Local param:SE_Parameter = New SE_Parameter
		param.m_paramType = SE_TYPE_NUMBER
		param.m_valueNumber = value
		ListAddLast(m_paramList, param)
	End Method
	
	Rem
		bbdoc: Adds a boolean parameter to a LUA function call
	End Rem
	Method AddBooleanParameter(value:Int)
		Local param:SE_Parameter = New SE_Parameter
		param.m_paramType = SE_TYPE_BOOLEAN
		param.m_valueBoolean = value
		ListAddLast(m_paramList, param)
	End Method
	
	Rem
		bbdoc: Calls a function in LUA
	End Rem
	Method CallFunction:Int(functionName:String, wantResult:Int)
	    Local param:SE_Parameter
		Local error:Int

		If (m_started) Then
			lua_getglobal(m_lua_state , functionName)

			For param = EachIn m_paramList 
				Select param.m_paramType
					Case SE_TYPE_NUMBER
						lua_pushnumber(m_lua_state, param.m_valueNumber)
					Case SE_TYPE_STRING
						lua_pushstring(m_lua_state, param.m_valueString)
					Case SE_TYPE_BOOLEAN
						lua_pushboolean(m_lua_state, param.m_valueBoolean)
				End Select
			Next

			error = lua_pcall(m_lua_state, CountList(m_paramList), wantResult, 0)
 		
			If ((Not error) And wantResult) Then
				If (lua_isnumber(m_lua_state, -1)) Then
					m_resultType = SE_TYPE_NUMBER
					m_resultNumber = lua_tonumber(m_lua_state, -1)
					lua_pop(m_lua_state, 1)
				Else If (lua_isboolean(m_lua_state, -1)) Then
					m_resultType = SE_TYPE_BOOLEAN
					m_resultBoolean = lua_toboolean(m_lua_state, -1)
					lua_pop(m_lua_state, 1)
				Else If (lua_isstring(m_lua_state, -1)) Then
					m_resultType = SE_TYPE_STRING
					m_resultString = String.FromCString(lua_tostring(m_lua_state, -1))
					lua_pop(m_lua_state, 1)
				End If
			End If
		Else
			m_lastErrorString = "LuaScriptEngine: Engine is not started"
			m_lastErrorNumber = 0
			Return False
		End If
		
		If (Not error) Then Return True
		
		ProcessError(error)
		Return False

 	End Method

	Rem
		bbdoc: Tells the result type of a LUA function call
	End Rem
	Method GetResultType:Int()
		Return m_resultType
	End Method
	
	Rem
		bbdoc: Get the result of a LUA function call as a number
	End Rem
	Method GetResultNumber:Double()
		Return m_resultNumber
	End Method

	Rem
		bbdoc: Get the result of a LUA function call as a boolean
	End Rem
	Method GetResultBoolean:Int()
		Return m_resultBoolean
	End Method
	
	Rem
		bbdoc: Get the result of a LUA function call as a string
	End Rem
	Method GetResultString:String()
		Return m_resultString
	End Method
		
	Rem
		bbdoc: Internal
	End Rem
	Method ProcessError (errorCode:Int)
		m_LastErrorString = String.FromCString(lua_tostring(m_lua_state, -1))
		m_LastErrorNumber = errorCode 
		lua_pop(m_lua_state, -1)
	End Method
	
	Rem
		bbdoc: Set engine script text (performs syntax checks)
	End Rem
	Method SetScriptText:Int(scriptText:String, scriptName:String)
		Local error:Int
		
		m_Source = ""
		
		If (m_started) Then
'			Local t:Byte Ptr=scriptText.ToCString()
 			error = luaL_loadbuffer(m_lua_state, scriptText, scriptText.length, scriptText)
'			MemFree t
		Else
			m_lastErrorString = "LuaScriptEngine: Engine is not started"
			m_lastErrorNumber = 0
			Return False
		End If

		If (Not error) Then
			m_Source = scriptText
			Return True
		End If

		ProcessError(error)
		Return False
	End Method

	Rem
		bbdoc: Runs a script file
	End Rem
	Method RunScriptFile:Int(scriptFile:String)
		Local error:Int

		If (m_started) Then
 			error = lua_dofile(m_lua_state, scriptFile)
		Else
			m_lastErrorString = "LuaScriptEngine: Engine is not started"
			m_lastErrorNumber = 0
			Return False
		End If
		
		If (Not error) Then Return True
		
		ProcessError(error)
		Return False
	End Method
	
	Rem
		bbdoc: Runs a script loaded with SetScriptText
	End Rem
	Method RunScript:Int()
		Local error:Int
		
		If (m_started) Then
			
			If (m_Source.length > 0) Then
 				error = lua_pcall(m_lua_state, 0, 0, 0)
			Else
				m_lastErrorString = "LuaScriptEngine: No script text to execute"
				m_lastErrorNumber = 0
				Return False
			End If
		Else
			m_lastErrorString = "LuaScriptEngine: Engine is not started"
			m_lastErrorNumber = 0
			Return False
		End If
		
		If (Not error) Then Return True
		
		ProcessError(error)
		Return False
	End Method
	
	Rem
		bbdoc: Returns the last error string
	End Rem
	Method GetLastErrorString:String()
		Return m_LastErrorString
	End Method

	Rem
		bbdoc: Returns the last error number
	End Rem
	Method GetLastErrorNumber:Int()
		Return m_LastErrorNumber
	End Method
	
	Rem
		bbdoc: Destroys script engine state
	End Rem
	Method ShutDown()
		If (m_lua_state) Then lua_close(m_lua_state)
		m_started = False
	End Method
		
	Rem
		bbdoc: Returns whether or not the engine has started correctly
	End Rem
	Method Started:Int()
		Return m_started
	End Method
	
End Type
