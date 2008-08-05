// EmbeddingTcl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <list>
#include <vector>

typedef unsigned long DWORD;

class Widget
{
public:
	static Widget* createWidget(int a = 0, int b = 0) { return new Widget(a, b); }
	~Widget()
	{
		std::cout << "Widget dtor() called" << std::endl;
	}
	int getA() { return a; }
	int getB() { return b; }
private:
	int a, b;
	Widget() {}
	Widget(int a, int b)
	{
		this->a = a;
		this->b = b;
		std::cout << "Widget ctor() called" << std::endl;
	}
};


//////////////////////////////////////////////////////////////////////////

enum ArgumentType
{
	AT_I = 1,
	AT_PC = 2,
	AT_D = 3,
	AT_PV = 4,
};

union ScriptArgument
{
	int i;
	const char* pc;
	double d;
	void* pv;
};
typedef std::vector<ScriptArgument> ScriptArgumentList;

void ParseTclArgumentByTrait( DWORD trait, Tcl_Interp* interp, Tcl_Obj *CONST objv[], ScriptArgumentList& argList )
{
	ScriptArgument sa;
	memset( &sa, 0, sizeof( ScriptArgument ) );
	argList.push_back(sa); // Return value set; type is not important at this point
	trait = trait >> 4;

	if (trait == 0)
		throw std::runtime_error("Trait value incorrect");

	unsigned int i = 1;
	int len = 0;
	while (trait)
	{
		switch (trait & 0xf)
		{
		case AT_I:
		case AT_PV:
			Tcl_GetIntFromObj( interp, objv[i], &sa.i );
			break;
		case AT_PC:
			sa.pc = Tcl_GetStringFromObj( objv[i], &len );
			break;
		case AT_D:
			Tcl_GetDoubleFromObj( interp, objv[i], &sa.d );
			break;
		default:
			throw std::runtime_error("Trait value incorrect");
		}
		argList.push_back(sa);
		
		trait = trait >> 4;
		i++;
	}

}
void SetTclResult(DWORD trait, Tcl_Obj* tcl_result, const ScriptArgumentList& argList)
{
	switch (trait & 0xf)
	{
	case AT_I:
		Tcl_SetIntObj( tcl_result, (long)argList[0].i );
		break;
	case AT_PV:
		Tcl_SetIntObj( tcl_result, reinterpret_cast<long>( argList[0].pv ) );
		break;
	case AT_PC:
		Tcl_SetStringObj( tcl_result, argList[0].pc, strlen( argList[0].pc ) );
		break;
	case AT_D:
		Tcl_SetDoubleObj( tcl_result, argList[0].d );
		break;
	default:
		throw std::runtime_error("Trait incorrect");
	}
}

//////////////////////////////////////////////////////////////////////////

#define SCRIPT_CALLABLE_0(arg0, funcName)										\
	static const DWORD _trait_##funcName = arg0;								\
	void funcName(ScriptArgumentList& args) {

#define SCRIPT_CALLABLE_1(arg0, funcName, arg1)									\
	static const DWORD _trait_##funcName = arg0 | (arg1 << 4);					\
	void funcName(ScriptArgumentList& args) {

#define SCRIPT_CALLABLE_2(arg0, funcName, arg1, arg2)							\
	static const DWORD _trait_##funcName = arg0 | (arg1 << 4) | (arg2 << 8);	\
	void funcName(ScriptArgumentList& args) {

#define SCRIPT_CALLABLE_END(funcName)	}																		\
	static int _wrap_##funcName(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])		\
	{																											\
		Tcl_Obj* tcl_result = Tcl_GetObjResult(interp);															\
		ScriptArgumentList argList;																				\
		ParseTclArgumentByTrait(_trait_##funcName, interp, objv, argList);										\
		funcName(argList);																						\
		SetTclResult(_trait_##funcName, tcl_result, argList);													\
		return TCL_OK;																							\
	}

//////////////////////////////////////////////////////////////////////////

SCRIPT_CALLABLE_2(AT_PV, createWidget, AT_I, AT_I)
	args[0].pv = Widget::createWidget(args[1].i, args[2].i);
SCRIPT_CALLABLE_END(createWidget)


SCRIPT_CALLABLE_1(AT_I, releaseWidget, AT_PV)
	delete reinterpret_cast<Widget*>( args[1].pv );
	args[0].i = 0;
SCRIPT_CALLABLE_END(releaseWidget)


SCRIPT_CALLABLE_1(AT_I, widgetGetA, AT_PV)
	args[0].i = reinterpret_cast<Widget*>( args[1].pv )->getA();
SCRIPT_CALLABLE_END(widgetGetA)


SCRIPT_CALLABLE_1(AT_I, widgetGetB, AT_PV)
	args[0].i = reinterpret_cast<Widget*>( args[1].pv )->getB();
SCRIPT_CALLABLE_END(widgetGetB)


SCRIPT_CALLABLE_1(AT_I, square, AT_I)
	args[0].i = args[1].i * args[1].i;
SCRIPT_CALLABLE_END(square)


SCRIPT_CALLABLE_2(AT_I, csum, AT_I, AT_I)
	args[0].i = args[1].i + args[2].i;
SCRIPT_CALLABLE_END(csum)

//////////////////////////////////////////////////////////////////////////


int Tcl_AppInit(Tcl_Interp *interp){
	if (Tcl_Init(interp) == TCL_ERROR)
		return TCL_ERROR;
	/* Now initialize our functions */
	Tcl_CreateObjCommand(interp, "square", _wrap_square, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "csum", _wrap_csum, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "createWidget", _wrap_createWidget, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "widgetGetA", _wrap_widgetGetA, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "widgetGetB", _wrap_widgetGetB, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "releaseWidget", _wrap_releaseWidget, (ClientData) "release~~~", (Tcl_CmdDeleteProc *) NULL);
	return TCL_OK;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	Tcl_Interp *interp;
	interp = Tcl_CreateInterp();
	Tcl_AppInit(interp);

	
	FILE       * infile   ;
	size_t       filesize ;
	char       * pstr   ;
	int          rc       =0;


	/* Read the configuration file with specific script code
	(in reality, I use a function that searches for the file and opens it!
	Hence I need to do more work ... If you have the file name, then use
	Tcl_EvalFile()!)
	*/
	infile = fopen( "script.tcl", "r" ) ;

	if ( infile == NULL )
	{
		return 1 ;
	}

	/* Now, read the whole file ... */
	fseek( infile, 0L, SEEK_END ) ;
	filesize = ftell( infile ) ;
	pstr     = (char *) malloc( (filesize+1) * sizeof(char) ) ;
	fseek( infile, 0L, SEEK_SET ) ;

	fread( pstr, filesize, 1, infile ) ;
	pstr[filesize] = '\0' ;

	rc = Tcl_EvalFile( interp, "script.tcl" ) ;
	if ( rc != TCL_OK )
	{
		fprintf( stderr, "Error loading script library\n" ) ;
		return 1 ;
	}

	rc = Tcl_Eval(interp, "printSquare 33");
	if ( rc != TCL_OK )
	{
		fprintf( stderr, "Error loading script library\n" ) ;
		return 1 ;
	}
	rc = Tcl_Eval(interp, "puts \"g_x value is $g_x \"");
	if ( rc != TCL_OK )
	{
		fprintf( stderr, "Error loading script library\n" ) ;
		return 1 ;
	}

	

	free( pstr ) ;

	std::cout << "Hello" << std::endl;
	Tcl_DeleteInterp(interp);
	Tcl_Finalize();
	return 0;
}

