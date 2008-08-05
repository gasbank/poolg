#include "EmptyProjectPCH.h"
#include "ScriptManager.h"
#include "TopStateManager.h"


int square (int i)
{
	return i*i;
}
int csum(int a, int b)
{

	return a+b;
}
int EpSetNextState(int stateID)
{
	TopStateManager::getSingleton().setNextState((GameState)stateID);
	return 0;
}
int EpOutputDebugString(const char* msg)
{
	OutputDebugStringA(msg);
	return 0;
}
static int _wrap_csum(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	int  _result;
	int  _arg0, _arg1;
	Tcl_Obj * tcl_result;
	int tempint1, tempint2;

	clientData = clientData; objv = objv;
	tcl_result = Tcl_GetObjResult(interp);
	if ((objc < 3) || (objc > 3)) {
		Tcl_SetStringObj(tcl_result,"Wrong # args. csum a b ",-1);
		return TCL_ERROR;
	}
	if (Tcl_GetIntFromObj(interp,objv[1],&tempint1) == TCL_ERROR) return TCL_ERROR;
	if (Tcl_GetIntFromObj(interp,objv[2],&tempint2) == TCL_ERROR) return TCL_ERROR;
	_arg0 = (int ) tempint1;
	_arg1 = (int ) tempint2;
	_result = (int )csum(_arg0, _arg1);
	tcl_result = Tcl_GetObjResult(interp);
	Tcl_SetIntObj(tcl_result,(long) _result);
	return TCL_OK;
}
static int _wrap_EpOutputDebugString(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	int  _result;
	Tcl_Obj * tcl_result;
	
	clientData = clientData; objv = objv;
	tcl_result = Tcl_GetObjResult(interp);
	if ((objc < 2) || (objc > 2)) {
		Tcl_SetStringObj(tcl_result,"Wrong # args. square i ",-1);
		return TCL_ERROR;
	}
	const char* msg;
	int msgLen;
	msg = Tcl_GetStringFromObj( objv[1], &msgLen );
	_result = (int )EpOutputDebugString(msg);
	tcl_result = Tcl_GetObjResult(interp);
	Tcl_SetIntObj(tcl_result,(long) _result);
	return TCL_OK;
}
static int _wrap_square(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	int  _result;
	int  _arg0;
	Tcl_Obj * tcl_result;
	int tempint;

	clientData = clientData; objv = objv;
	tcl_result = Tcl_GetObjResult(interp);
	if ((objc < 2) || (objc > 2)) {
		Tcl_SetStringObj(tcl_result,"Wrong # args. square i ",-1);
		return TCL_ERROR;
	}
	if (Tcl_GetIntFromObj(interp,objv[1],&tempint) == TCL_ERROR) return TCL_ERROR;
	_arg0 = (int ) tempint;
	_result = (int )square(_arg0);
	tcl_result = Tcl_GetObjResult(interp);
	Tcl_SetIntObj(tcl_result,(long) _result);
	return TCL_OK;
}
static int _wrap_EpSetNextState(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
	int  _result;
	int  _arg0;
	Tcl_Obj * tcl_result;
	int tempint;

	clientData = clientData; objv = objv;
	tcl_result = Tcl_GetObjResult(interp);
	if ((objc < 2) || (objc > 2)) {
		Tcl_SetStringObj(tcl_result,"Wrong # args. square i ",-1);
		return TCL_ERROR;
	}
	if (Tcl_GetIntFromObj(interp,objv[1],&tempint) == TCL_ERROR) return TCL_ERROR;
	_arg0 = (int ) tempint;
	_result = (int )EpSetNextState(_arg0);
	tcl_result = Tcl_GetObjResult(interp);
	Tcl_SetIntObj(tcl_result,(long) _result);
	return TCL_OK;
}

int Tcl_AppInit(Tcl_Interp *interp){
	if (Tcl_Init(interp) == TCL_ERROR)
		return TCL_ERROR;
	/* Now initialize our functions */
	Tcl_CreateObjCommand(interp, "square", _wrap_square, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "csum", _wrap_csum, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "EpSetNextState", _wrap_EpSetNextState, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	Tcl_CreateObjCommand(interp, "EpOutputDebugString", _wrap_EpOutputDebugString, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
	return TCL_OK;
}

//////////////////////////////////////////////////////////////////////////
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
	Tcl_AppInit( m_interpreter );
	assert( Tcl_EvalFile( m_interpreter, "library/EpInitScript.tcl" ) == TCL_OK );
	assert( Tcl_Eval( m_interpreter, "EpInitGame" ) == TCL_OK );
	return S_OK;
}

HRESULT ScriptManager::release()
{
	Tcl_DeleteInterp( m_interpreter );
	Tcl_Finalize();
	return S_OK;
}

HRESULT ScriptManager::execute( const char* command )
{
	return Tcl_Eval( m_interpreter, command );
}