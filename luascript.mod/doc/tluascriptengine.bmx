Strict

Import axe.luascript

Function LUA_MyFunction( ls:Byte Ptr )
     lua.ReturnStringToLua("Hello, World!")
     Return 1  'returning one variable
End Function

Global lua:TLuaScriptEngine=New TLuaScriptEngine

lua.reset()

lua.addFunction(LUA_MyFunction,"MyFunction")

lua.SetScriptText "print(MyFunction())","myscript"

lua.RunScript

lua.Shutdown()
