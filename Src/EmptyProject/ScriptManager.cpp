#include "EmptyProjectPCH.h"
#include "ScriptManager.h"
#include "TopStateManager.h"

int Tcl_AppInit(Tcl_Interp *interp);

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_SINGLETON(ScriptManager);

ScriptManager::ScriptManager(void)
{	
	m_interpreter = 0;
}

ScriptManager::~ScriptManager(void)
{
}

HRESULT ScriptManager::init()
{
	m_interpreter = Tcl_CreateInterp();
	if ( Tcl_AppInit( m_interpreter ) != TCL_OK ) throw std::runtime_error( "Script init error" );
	
	return S_OK;
}

HRESULT ScriptManager::release()
{
	Tcl_DeleteInterp( m_interpreter );
	Tcl_Finalize();
	return S_OK;
}

void ScriptManager::execute( const char* command )
{
	if ( Tcl_Eval( m_interpreter, command ) != TCL_OK )
		throwScriptErrorWithMessage();
}

void ScriptManager::executeFile( const char* fileName )
{
	if ( Tcl_EvalFile( m_interpreter, fileName ) != TCL_OK )
		throwScriptErrorWithMessage();
}

void ScriptManager::throwScriptErrorWithMessage()
{
	char lineNo[32];
	StringCchPrintfA( lineNo, 32, "Line: %d\n", m_interpreter->errorLine );
	OutputDebugStringA( "\n@@@------------------------- SCRIPT ERROR -------------------------@@@\n" );
	OutputDebugStringA( lineNo );
	OutputDebugStringA( m_interpreter->result );
	OutputDebugStringA( "\n@@@------------------------- SCRIPT ERROR -------------------------@@@\n\n" );
	throw std::runtime_error( m_interpreter->result );
}

//////////////////////////////////////////////////////////////////////////

int EpSetNextState(int stateID)
{
	TopStateManager::getSingleton().setNextState((GameState)stateID);
	return 0;

} SCRIPT_CALLABLE_I_I( EpSetNextState )

int EpOutputDebugString( const char* msg )
{
	OutputDebugStringA( msg );
	return 0;

} SCRIPT_CALLABLE_I_PC( EpOutputDebugString )

int EpSetWindowSize(int w, int h)
{
	GetG().m_scrWidth = w;
	GetG().m_scrHeight = h;
	OutputDebugStringW(L" - Window size set\n");
	return 0;

} SCRIPT_CALLABLE_I_I_I( EpSetWindowSize )

//////////////////////////////////////////////////////////////////////////


int Tcl_AppInit(Tcl_Interp *interp)
{
	if (Tcl_Init(interp) == TCL_ERROR)
		return TCL_ERROR;
	/* Now initialize our functions */
	CREATE_OBJ_COMMAND( EpSetNextState );
	CREATE_OBJ_COMMAND( EpOutputDebugString );
	CREATE_OBJ_COMMAND( EpSetWindowSize );
	return TCL_OK;
}

//////////////////////////////////////////////////////////////////////////


void ParseTclArgumentByTrait( DWORD trait, Tcl_Interp* interp, Tcl_Obj *CONST objv[], ScriptArgumentList& argList )
{
	if (trait == 0)
		throw std::runtime_error("Trait value incorrect");

	ScriptArgument sa;
	memset( &sa, 0, sizeof( ScriptArgument ) );
	argList.push_back(sa); // Return value set; type is not important at this point
	trait = trait >> 4;

	unsigned int i = 1;
	int len = 0;
	while (trait)
	{
		switch (trait & 0xf)
		{
		case AT_I:
			if ( Tcl_GetIntFromObj( interp, objv[i], &sa.i ) != TCL_OK )
				throw std::runtime_error("Integer argument access failed");
			break;
		case AT_PV:
			if ( Tcl_GetLongFromObj( interp, objv[i], (long*)&sa.i ) != TCL_OK )
				throw std::runtime_error("Long(raw pointer) argument access failed");
			break;
		case AT_PC:
			sa.pc = Tcl_GetStringFromObj( objv[i], &len );
			if (sa.pc == 0)
				throw std::runtime_error("char* argument access failed");
			break;
		case AT_D:
			if ( Tcl_GetDoubleFromObj( interp, objv[i], &sa.d ) != TCL_OK )
				throw std::runtime_error("Double argument access failed");
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