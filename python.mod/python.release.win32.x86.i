ModuleInfo "Version: 2.4"
ModuleInfo "Authors: Guido van Rossum"
ModuleInfo "License: Python Software Foundation License Version 2"
ModuleInfo "Modserver: BRL"
import brl.blitz
import "libs/libpython24.a"
Py_Initialize%()="Py_Initialize"
PyRun_SimpleString%(script$z)="PyRun_SimpleString"
Py_InitModule4@*(name$z,methods@*,doc@*,PyObject@*,apiver%)="Py_InitModule4"
Py_Finalize%()="Py_Finalize"
PyArg_ParseTuple%(PyObject@*,name$z)="PyArg_ParseTuple"
Py_BuildValue@*(numtype$z,value%)="Py_BuildValue"
PYTHON_API_VERSION%=1012
METH_OLDARGS%=0
METH_VARARGS%=1
METH_KEYWORDS%=2
METH_NOARGS%=4
METH_O%=8
Py_InitModule@*(name$,methods@*)="axe_python_Py_InitModule"
