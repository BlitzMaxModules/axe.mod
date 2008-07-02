Import axe.python

Strict

Function myfunc:Byte Ptr(PyObject:Byte Ptr,args:Byte Ptr)
	If Not PyArg_ParseTuple(args,":numargs") Return Null
	Return Py_BuildValue("i",220)
End Function

Local methods:Byte Ptr[64]

methods[0]=Byte Ptr("myfunc".toCString())
methods[1]=Byte Ptr(myfunc)
methods[2]=Byte Ptr(METH_VARARGS)
methods[3]=Byte Ptr("this is a test".toCString())

Py_InitModule "mymodule",methods

PyRun_SimpleString "import mymodule~nprint ~qmyfunc()=~q, mymodule.myfunc()"
