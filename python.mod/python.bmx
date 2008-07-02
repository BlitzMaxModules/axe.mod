Rem
bbdoc: Python Programming Language
about:
Python is an interpreted object-oriented programming language.
End Rem
Module axe.python

ModuleInfo "Version: 2.4"
ModuleInfo "Authors: Guido van Rossum"
ModuleInfo "License: Python Software Foundation License Version 2"
ModuleInfo "Modserver: BRL"

Import "libs/libpython24.a"

Extern "C"
Function Py_Initialize()
Function PyRun_SimpleString(script$z)
Function Py_InitModule4:Byte Ptr(name$z,methods:Byte Ptr,doc:Byte Ptr,PyObject:Byte Ptr,apiver)
Function Py_Finalize()
Function PyArg_ParseTuple(PyObject:Byte Ptr,name$z)
Function Py_BuildValue:Byte Ptr(numtype$z,value)
End Extern

Const PYTHON_API_VERSION=1012

Const METH_OLDARGS=0
Const METH_VARARGS=1
Const METH_KEYWORDS=2
Const METH_NOARGS=4
Const METH_O=8

OnEnd Py_Finalize
Py_Initialize

Function Py_InitModule:Byte Ptr(name$,methods:Byte Ptr)
	Return Py_InitModule4(name,methods,Null,Null,PYTHON_API_VERSION)
End Function
