// EmbeddingPython.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "EmbeddingPython.h"

static int numargs=0;
static PyObject *SpamError;

/* Return the number of arguments of the application command line */
static PyObject* emb_numargs(PyObject *self, PyObject *args)
{
	if(!PyArg_ParseTuple(args, ":numargs"))
		return NULL;
	return Py_BuildValue("i", numargs);
}
static PyObject* emb_printSomething(PyObject *self, PyObject *args)
{
	const wchar_t* str1;
	const wchar_t* str2;
	if (!PyArg_ParseTuple(args, "uu", &str1, &str2))
		return NULL;
	std::wcout.imbue(std::locale("korean"));
	std::wcout << L"C++ side 1: " << str1 << std::endl;
	std::wcout << L"C++ side 2: " << str2 << std::endl;
	return Py_BuildValue("i", 0);
}

static PyObject *
spam_system(PyObject *self, PyObject *args)
{
	const char *command;
	int sts;

	if (!PyArg_ParseTuple(args, "s", &command))
		return NULL;
	sts = system(command);
	return Py_BuildValue("i", sts);
}


static PyMethodDef EmbMethods[] = {
	{"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
	{"printSomething", emb_printSomething, METH_VARARGS, NULL},
	{NULL, NULL, 0, NULL}
};
static PyMethodDef SpamMethods[] = {
	{"system",  spam_system, METH_VARARGS,
	"Execute a shell command."},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

struct PyModuleDef spammodule = {
	PyModuleDef_HEAD_INIT,
	"spam",   /* name of module */
	NULL, /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
			  or -1 if the module keeps state in global variables. */
			  SpamMethods
};
PyMODINIT_FUNC
initspam(void)
{
	PyObject *m;

	m = PyModule_Create(&spammodule);
	if (m == NULL)
		return NULL;

	SpamError = PyErr_NewException("spam.error", NULL, NULL);
	Py_INCREF(SpamError);
	PyModule_AddObject(m, "error", SpamError);
	return m;

}
PyMODINIT_FUNC
PyInit_spam(void)
{
	return PyModule_Create(&spammodule);
}


int _tmain(int argc, _TCHAR* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;
	int i;

	//int* aa = new int[10];
	numargs = argc;
	if (argc < 3) {
		fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
		return 1;
	}

	wchar_t wc = 'H';
	char argv1[1024], argv2[1024];
	size_t cLen1, cLen2;
	wcstombs_s( &cLen1, argv1, argv[1], wcslen(argv[1]) );
	wcstombs_s( &cLen2, argv2, argv[2], wcslen(argv[2]) );

	char test[1024];
	wchar_t* testWc = L"가나다라마바사";
	size_t testLen;
	wcstombs_s( &testLen, test, testWc, wcslen(testWc) );

	PyImport_AppendInittab("spam", PyInit_spam);
	Py_SetProgramName(argv[0]);

	Py_Initialize();


	
	pName = PyUnicode_FromString(argv1);

	
	PyInit_spam();

	/* Error checking of pName left out */

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		pFunc = PyObject_GetAttrString(pModule, argv2);
		/* pFunc is a new reference */

		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(argc - 3);
			for (i = 0; i < argc - 3; ++i) {
				char argvx[1024];
				size_t cLenx;
				wcstombs_s(&cLenx, argvx, argv[i+3], wcslen(argv[i+3]));
				pValue = PyLong_FromLong(atoi(argvx));
				if (!pValue) {
					Py_DECREF(pArgs);
					Py_DECREF(pModule);
					fprintf(stderr, "Cannot convert argument\n");
					return 1;
				}
				/* pValue reference stolen here: */
				PyTuple_SetItem(pArgs, i, pValue);
			}
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			if (pValue != NULL) {
				printf("Result of call: %ld\n", PyLong_AsLong(pValue));
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr,"Call failed\n");
				return 1;
			}
		}
		else {
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		return 1;
	}



	Py_Finalize();
	return 0;


}

