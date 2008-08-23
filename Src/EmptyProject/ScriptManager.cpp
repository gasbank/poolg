#include "EmptyProjectPCH.h"
#include "ScriptManager.h"
#include "TopStateManager.h"
#include "Unit.h"
#include "Character.h"
#include "Hero.h"
#include "Enemy.h"
#include "World.h"
#include "Incident.h"
#include "NonblockingActionIncident.h"
#include "BlockingActionIncident.h"
#include "Trigger.h"
#include "Action.h"
#include "StructureObject.h"
#include "TileManager.h"
#include "SequentialIncident.h"

int Tcl_AppInit(Tcl_Interp *interp);

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_SINGLETON(ScriptManager);

ScriptManager::ScriptManager(void)
{	
	m_interp = 0;
}

ScriptManager::~ScriptManager(void)
{
}

HRESULT ScriptManager::init()
{
	m_interp = Tcl_CreateInterp();
	if ( Tcl_AppInit( m_interp ) != TCL_OK ) throw std::runtime_error( "Script init error" );
	
	return S_OK;
}

HRESULT ScriptManager::release()
{
	Tcl_DeleteInterp( m_interp );
	Tcl_Finalize();
	return S_OK;
}

void ScriptManager::execute( const char* command )
{
	if ( Tcl_Eval( m_interp, command ) != TCL_OK )
		throwScriptErrorWithMessage( m_interp );
}

void ScriptManager::executeFile( const char* fileName )
{
#ifdef DEBUG
	FILE* f;
	fopen_s( &f, fileName, "r" );
	if (!f)
		throw std::runtime_error("Script file opening failed");
#endif

	if ( Tcl_EvalFile( m_interp, fileName ) != TCL_OK )
		throwScriptErrorWithMessage( m_interp );
}

void ScriptManager::throwScriptErrorWithMessage( Tcl_Interp* interp )
{
	// Caution: 'trace' may contain wrong message of you call another script command afterwards
	const char* trace = Tcl_GetVar( interp, "errorInfo", TCL_GLOBAL_ONLY );

	OutputDebugStringA( "\n@@@------------------------- SCRIPT ERROR -------------------------@@@\n" );
	OutputDebugStringA( trace );
	OutputDebugStringA( "\n@@@------------------------- SCRIPT ERROR -------------------------@@@\n\n" );
		
	throw std::runtime_error( trace );
}

bool ScriptManager::readRect( const char* variableName, TileRegion& rect )
{
	Tcl_Obj* retObj;
	Tcl_Obj* aObj;
	int retObjLength;

	retObj = Tcl_GetVar2Ex( m_interp, variableName, 0, 0 );
	Tcl_ListObjLength( m_interp, retObj, &retObjLength );
	assert(retObjLength == 4);
	long x0, y0, x1, y1;
	Tcl_ListObjIndex( m_interp, retObj, 0, &aObj );
	Tcl_GetLongFromObj( m_interp, aObj, &x0 );
	Tcl_ListObjIndex( m_interp, retObj, 1, &aObj );
	Tcl_GetLongFromObj( m_interp, aObj, &y0 );
	Tcl_ListObjIndex( m_interp, retObj, 2, &aObj );
	Tcl_GetLongFromObj( m_interp, aObj, &x1 );
	Tcl_ListObjIndex( m_interp, retObj, 3, &aObj );
	Tcl_GetLongFromObj( m_interp, aObj, &y1 );

	rect = TileRegion( x0, y0, x1, y1 );
	return true;
}

Tcl_Obj* ScriptManager::getObject( const char* variableName )
{
	return Tcl_GetVar2Ex( m_interp, variableName, 0, 0 );
}

const char* ScriptManager::readString( const char* variableName )
{
	Tcl_Obj* obj = Tcl_GetVar2Ex( m_interp, variableName, 0, 0 );
	int stringLen;
	return Tcl_GetStringFromObj( obj, &stringLen );
}
int ScriptManager::readInt( const char* variableName )
{
	Tcl_Obj* obj = Tcl_GetVar2Ex( m_interp, variableName, 0, 0 );
	int ret;
	Tcl_GetIntFromObj( m_interp, obj, &ret );
	return ret;
}

bool ScriptManager::readCharPtrList( const char* variableName, ConstCharList& strList )
{
	Tcl_Obj* retObj;
	Tcl_Obj* aObj;	
	int retObjLength;	
	retObj = Tcl_GetVar2Ex( m_interp, variableName, 0, 0 );
	Tcl_ListObjLength( m_interp, retObj, &retObjLength );
	assert( strList.size() == 0 );
	UINT i;
	for ( i = 0; i < (UINT)retObjLength; ++i )
	{
		int len;
		Tcl_ListObjIndex( m_interp, retObj, i, &aObj );
		strList.push_back( Tcl_GetStringFromObj( aObj, &len ) );
	}
	return true;
}

void ScriptManager::readTwoIntObj( Tcl_Obj* obj, int& v1, int& v2 )
{
	int objLength = 0;
	Tcl_ListObjLength( m_interp, obj, &objLength );
	if ( objLength != 2 )
		throw std::runtime_error( "Tcl_Obj* parse error" );
	
	Tcl_Obj* elemObj;
	Tcl_ListObjIndex( m_interp, obj, 0, &elemObj );
	Tcl_GetIntFromObj( m_interp, elemObj, &v1 );
	Tcl_ListObjIndex( m_interp, obj, 1, &elemObj );
	Tcl_GetIntFromObj( m_interp, elemObj, &v2 );
}

#define INIT_BINDING(className) _script_factory_##className::init()
void ScriptManager::initScriptBindings()
{
	INIT_BINDING( WorldManager );
	INIT_BINDING( World );
	INIT_BINDING( Unit );
	INIT_BINDING( Character );
	INIT_BINDING( Hero );
	INIT_BINDING( Enemy );
	INIT_BINDING( Incident );
	INIT_BINDING( NonblockingActionIncident );
	INIT_BINDING( BlockingActionIncident );
	INIT_BINDING( Action );
	INIT_BINDING( Trigger );
	INIT_BINDING( StructureObject );
	INIT_BINDING( SequentialIncident );
}
#undef INIT_BINDING

//////////////////////////////////////////////////////////////////////////

int EpSetNextState(int stateID)
{
	GetTopStateManager().setNextState( (GameState)stateID );
	return 0;

} SCRIPT_CALLABLE_I_I( EpSetNextState )



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
	CREATE_OBJ_COMMAND_ENGINE( EpSetNextState );
	CREATE_OBJ_COMMAND_ENGINE( EpSetWindowSize );
	return TCL_OK;
}

//////////////////////////////////////////////////////////////////////////

void PrintTclCallerArgumentDebugInfo( Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[] )
{
	char debugInfo[512];
	OutputDebugStringA( "\n@@@------------------------- SCRIPT ERROR -------------------------@@@\n" );
	OutputDebugStringA( "Debug Info about procedure arguments invoked by Tcl script side function\n" );
	StringCchPrintfA( debugInfo, 512, " - Argument count: %d\n", objc ); OutputDebugStringA( debugInfo );
	int i;
	for ( i = 0; i < objc; ++i )
	{
		StringCchPrintfA( debugInfo, 512,
			" - Arg %d %s: Byte '%s' / Int %d (0x%p)\n",
			i, (objv[i]->typePtr)?objv[i]->typePtr->name:"Unknown type", objv[i]->bytes, objv[i]->internalRep.longValue, objv[i]->internalRep.otherValuePtr );
		OutputDebugStringA( debugInfo );
	}
	OutputDebugStringA( "\n@@@------------------------- SCRIPT ERROR -------------------------@@@\n\n" );
	//Tcl_SetErrorCode( interp, "XXX", 0 );
	//Tcl_SetErrno( 100 );
	//Tcl_PosixError( interp );
	//Tcl_Obj* dicObj = Tcl_NewDictObj();
	//Tcl_DictObjPut( interp, dicObj, Tcl_NewStringObj( "-errorInfo", 10 ), Tcl_NewStringObj( "-errorinfo", 10 ) );
	//Tcl_SetReturnOptions(interp, Tcl_NewListObj(objc-1, objv+1));
	//Tcl_AddErrorInfo( interp, "Ep related procedure's argument parsing error" );
	//ScriptManager::throwScriptErrorWithMessage( interp );
}

void ParseTclArgumentByTrait( DWORD trait, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[], ScriptArgumentList& argList )
{
	try
	{
		if ( trait == 0 || (trait & 0xf) == 0 )
			throw std::runtime_error("Trait value incorrect");
		DWORD traitCopy = trait;
		int argCount = 0;
		while ( traitCopy & 0xf )
		{
			++argCount;
			traitCopy >>= 4;
		}

		// Check for script caller's argument count and script binder's argument count.
		// 'objc' includes a script caller's function name itself, so it has argument count + 1.
		// argCount includes a script binder return type, so it has argument count +1.
		// Since argCount and objc have both +1 value, we can compare directly.
		if ( argCount != objc )
		{
			throw std::runtime_error( "Trait value and script caller's argument does not match" );
		}

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
				{
					/*
					 * If you caught this error, you probably missed '$' character
					 * on the first place of a variable name in the script file.
					 * Note that tcl recognize a variable name without a preceding '$' to
					 * simple lexical string.
					 */
					throw std::runtime_error("Integer argument access failed");
				}
				break;
			case AT_PV:
				if ( Tcl_GetLongFromObj( interp, objv[i], (long*)&sa.i ) != TCL_OK )
				{
					throw std::runtime_error("Long(raw pointer) argument access failed");
				}
				break;
			case AT_PC:
				sa.pc = Tcl_GetStringFromObj( objv[i], &len );
				if (sa.pc == 0)
				{
					throw std::runtime_error("char* argument access failed");
				}
				break;
			case AT_D:
				if ( Tcl_GetDoubleFromObj( interp, objv[i], &sa.d ) != TCL_OK )
				{
					throw std::runtime_error("Double argument access failed");
				}
				break;
			case AT_OBJ:
				sa.obj = objv[i];
				break;
			default:
				throw std::runtime_error("Trait value incorrect");
			}
			argList.push_back(sa);

			trait = trait >> 4;
			i++;
		}
	}
	catch ( std::runtime_error& err )
	{
		PrintTclCallerArgumentDebugInfo( interp, objc, objv );
		OutputDebugStringA( err.what() );
		throw std::runtime_error( __FUNCTION__ );
	}
}
void SetTclResult(Tcl_Interp* interp, DWORD trait, Tcl_Obj* tcl_result, const ScriptArgumentList& argList)
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
	case AT_OBJ:
		Tcl_SetObjResult( interp, argList[0].obj );
		break;
	default:
		throw std::runtime_error("Trait incorrect");
	}
}