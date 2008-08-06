#pragma once

class ScriptManager : public Singleton<ScriptManager>
{
public:
	ScriptManager(void);
	~ScriptManager(void);

	HRESULT init();
	HRESULT release();

	void execute(const char* command);
	void executeFile(const char* fileName);
	Tcl_Interp* getInterp() { assert(m_interpreter); return m_interpreter; }
private:
	Tcl_Interp* m_interpreter;
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

void ParseTclArgumentByTrait( DWORD trait, Tcl_Interp* interp, Tcl_Obj *CONST objv[], ScriptArgumentList& argList );
void SetTclResult(DWORD trait, Tcl_Obj* tcl_result, const ScriptArgumentList& argList);

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
	static int _tcl_wrap_##funcName(ClientData clientData, Tcl_Interp *interp,	\
									int objc, Tcl_Obj *CONST objv[])			\
	{																			\
		Tcl_Obj* tcl_result = Tcl_GetObjResult(interp);							\
		ScriptArgumentList argList;												\
		ParseTclArgumentByTrait(_trait_##funcName, interp, objv, argList);		\
		_wrap_##funcName(argList);												\
		SetTclResult(_trait_##funcName, tcl_result, argList);					\
		return TCL_OK;															\
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

#define SCRIPT_CALLABLE_I_PC(funcName)											\
	static const DWORD _trait_##funcName = AT_I | (AT_PC << 4);					\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].pc);										\
	}																			\
	SCRIPT_CALLABLE_END(funcName)

#define SCRIPT_CALLABLE_I(funcName)												\
	static const DWORD _trait_##funcName = AT_I;								\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName();													\
	}																			\
	SCRIPT_CALLABLE_END(funcName)


#define CREATE_OBJ_COMMAND(funcName)	Tcl_CreateObjCommand(ScriptManager::getSingleton().getInterp(), #funcName, _tcl_wrap_##funcName, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL)
//////////////////////////////////////////////////////////////////////////