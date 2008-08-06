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
	void _wrap_##funcName(ScriptArgumentList& args)

#define SCRIPT_CALLABLE_1(arg0, funcName, arg1)									\
	static const DWORD _trait_##funcName = arg0 | (arg1 << 4);					\
	void _wrap_##funcName(ScriptArgumentList& args)

#define SCRIPT_CALLABLE_2(arg0, funcName, arg1, arg2)							\
	static const DWORD _trait_##funcName = arg0 | (arg1 << 4) | (arg2 << 8);	\
	void _wrap_##funcName(ScriptArgumentList& args)

#define SCRIPT_CALLABLE_END(funcName)																				\
	static int _tcl_wrap_##funcName(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])		\
	{																												\
		Tcl_Obj* tcl_result = Tcl_GetObjResult(interp);																\
		ScriptArgumentList argList;																					\
		ParseTclArgumentByTrait(_trait_##funcName, interp, objv, argList);											\
		_wrap_##funcName(argList);																					\
		SetTclResult(_trait_##funcName, tcl_result, argList);														\
		return TCL_OK;																								\
	}
#define SCRIPT_CALLABLE_PV_I_I(funcName)										\
	static const DWORD _trait_##funcName = AT_PV | (AT_I << 4) | (AT_I << 8);	\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].pv = funcName(args[1].i, args[2].i);							\
	}																			\
	SCRIPT_CALLABLE_END(funcName)

#define SCRIPT_CALLABLE_I_PV(funcName)											\
	static const DWORD _trait_##funcName = AT_I | (AT_PV << 4);					\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].pv);										\
	}																			\
	SCRIPT_CALLABLE_END(funcName)

#define SCRIPT_CALLABLE_I_I(funcName)											\
	static const DWORD _trait_##funcName = AT_I | (AT_I << 4);					\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].i);										\
	}																			\
	SCRIPT_CALLABLE_END(funcName)

#define SCRIPT_CALLABLE_I_I_I(funcName)											\
	static const DWORD _trait_##funcName = AT_I | (AT_I << 4) | (AT_I << 8);	\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].i, args[2].i);								\
	}																			\
	SCRIPT_CALLABLE_END(funcName)


//////////////////////////////////////////////////////////////////////////

Widget* createWidget( int a, int b )
{
	return Widget::createWidget(a, b);

} SCRIPT_CALLABLE_PV_I_I(createWidget)

int releaseWidget( void* w )
{
	delete reinterpret_cast<Widget*>( w );
	return 0;

} SCRIPT_CALLABLE_I_PV(releaseWidget)

int widgetGetA( void* w )
{
	return reinterpret_cast<Widget*>( w )->getA();

} SCRIPT_CALLABLE_I_PV(widgetGetA)

int widgetGetB( void* w )
{
	return reinterpret_cast<Widget*>( w )->getB();

} SCRIPT_CALLABLE_I_PV(widgetGetB)

int square( int a )
{
	return a * a;

} SCRIPT_CALLABLE_I_I(square)

int csum( int a, int b )
{
	return a + b;

} SCRIPT_CALLABLE_I_I_I(csum)

//////////////////////////////////////////////////////////////////////////


int Tcl_AppInit(Tcl_Interp *interp){
	if (Tcl_Init(interp) == TCL_ERROR)
		return TCL_ERROR;
	/* Now initialize our functions */

#define CREATE_SCRIPT_METHOD(funcName)	Tcl_CreateObjCommand(interp, #funcName, _tcl_wrap_##funcName, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL)
	CREATE_SCRIPT_METHOD(square);
	CREATE_SCRIPT_METHOD(csum);
	CREATE_SCRIPT_METHOD(createWidget);
	CREATE_SCRIPT_METHOD(releaseWidget);
	CREATE_SCRIPT_METHOD(widgetGetA);
	CREATE_SCRIPT_METHOD(widgetGetB);
#undef CREATE_SCRIPT_METHOD

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

