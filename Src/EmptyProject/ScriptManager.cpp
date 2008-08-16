#include "EmptyProjectPCH.h"
#include "ScriptManager.h"
#include "TopStateManager.h"
#include "Unit.h"
#include "Character.h"
#include "Hero.h"
#include "Enemy.h"
#include "World.h"
#include "Incident.h"
#include "Trigger.h"
#include "Action.h"

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
		throwScriptErrorWithMessage();
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
		throwScriptErrorWithMessage();
}

void ScriptManager::throwScriptErrorWithMessage()
{
	char lineNo[32];
	StringCchPrintfA( lineNo, 32, "Line: %d\n", m_interp->errorLine );
	OutputDebugStringA( "\n@@@------------------------- SCRIPT ERROR -------------------------@@@\n" );
	OutputDebugStringA( lineNo );
	OutputDebugStringA( m_interp->result );
	OutputDebugStringA( "\n@@@------------------------- SCRIPT ERROR -------------------------@@@\n\n" );
	throw std::runtime_error( m_interp->result );
}

bool ScriptManager::readRect( const char* variableName, RECT& rect )
{
	Tcl_Obj* retObj;
	Tcl_Obj* aObj;	
	int retObjLength;	
	retObj = Tcl_GetVar2Ex( m_interp, variableName, 0, 0 );
	Tcl_ListObjLength( m_interp, retObj, &retObjLength );
	assert(retObjLength == 4);
	Tcl_ListObjIndex( m_interp, retObj, 0, &aObj );
	Tcl_GetLongFromObj( m_interp, aObj, &rect.left );
	Tcl_ListObjIndex( m_interp, retObj, 1, &aObj );
	Tcl_GetLongFromObj( m_interp, aObj, &rect.top );
	Tcl_ListObjIndex( m_interp, retObj, 2, &aObj );
	Tcl_GetLongFromObj( m_interp, aObj, &rect.right );
	Tcl_ListObjIndex( m_interp, retObj, 3, &aObj );
	Tcl_GetLongFromObj( m_interp, aObj, &rect.bottom );

	
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



void ScriptManager::initBoundings()
{
	_script_factory_World::init();
	_script_factory_Unit::init();
	_script_factory_Character::init();
	_script_factory_Hero::init();
	_script_factory_Enemy::init();
	_script_factory_TopStateManager::init();
	_script_factory_Incident::init();
	_script_factory_Action::init();
	_script_factory_Trigger::init();

}

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