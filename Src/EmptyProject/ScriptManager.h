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
	void throwScriptErrorWithMessage();

	Tcl_Interp* m_interpreter;
};
inline ScriptManager& GetScriptManager() { return ScriptManager::getSingleton(); }


//////////////////////////////////////////////////////////////////////////

enum ArgumentType
{
	AT_I = 1,
	AT_PC = 2,
	AT_D = 3,
	AT_PV = 4,
};
class SuperVoidPointer
{
public:
	SuperVoidPointer(void* realPtr = 0) { m_realPtr = realPtr; }
	template<class newType>
	operator newType()
	{
		return reinterpret_cast<newType>(m_realPtr);
	}
private:
	void* m_realPtr;
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

#define SCRIPT_CALLABLE_END(funcName, traits)																				\
	static int _tcl_wrap_##funcName(ClientData clientData, Tcl_Interp *interp,	\
									int objc, Tcl_Obj *CONST objv[])			\
	{																			\
		Tcl_Obj* tcl_result = Tcl_GetObjResult(interp);							\
		ScriptArgumentList argList;												\
		ParseTclArgumentByTrait(_trait_##traits, interp, objv, argList);		\
		_wrap_##funcName(argList);												\
		SetTclResult(_trait_##traits, tcl_result, argList);						\
		return TCL_OK;															\
	}

//////////////////////////////////////////////////////////////////////////

static const DWORD _trait_I			= AT_I;
static const DWORD _trait_I_I		= AT_I | (AT_I << 4);
static const DWORD _trait_I_I_I		= AT_I | (AT_I << 4) | (AT_I << 8);
static const DWORD _trait_I_PC		= AT_I | (AT_PC << 4);
static const DWORD _trait_I_PV		= AT_I | (AT_PV << 4);
static const DWORD _trait_I_PV_D	= AT_I | (AT_PV << 4) | (AT_D << 8);
static const DWORD _trait_I_PV_PV	= AT_I | (AT_PV << 4) | (AT_PV << 8);
static const DWORD _trait_PV		= AT_PV;
static const DWORD _trait_PV_I_I	= AT_PV | (AT_I << 4) | (AT_I << 8);
static const DWORD _trait_PV_I_I_I	= AT_PV | (AT_I << 4) | (AT_I << 8) | (AT_I << 12);
static const DWORD _trait_D_PV		= AT_D | (AT_PV << 4);


#define SCRIPT_CALLABLE_PV_I_I(funcName)										\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].pv = funcName(args[1].i, args[2].i);							\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_I_I)

#define SCRIPT_CALLABLE_I_PV(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].pv);										\
	}																			\
	SCRIPT_CALLABLE_END(funcName, I_PV)

#define SCRIPT_CALLABLE_I_I(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].i);										\
	}																			\
	SCRIPT_CALLABLE_END(funcName, I_I)

#define SCRIPT_CALLABLE_I_I_I(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].i, args[2].i);								\
	}																			\
	SCRIPT_CALLABLE_END(funcName, I_I_I)

#define SCRIPT_CALLABLE_I_PC(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].pc);										\
	}																			\
	SCRIPT_CALLABLE_END(funcName, I_PC)

#define SCRIPT_CALLABLE_I(funcName)												\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName();													\
	}																			\
	SCRIPT_CALLABLE_END(funcName, I)

#define SCRIPT_CALLABLE_PV_I_I_I(funcName)										\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].pv = funcName(args[1].i, args[2].i, args[3].i);					\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_I_I_I)

#define SCRIPT_CALLABLE_I_PV_PV(funcName)										\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].pv, args[2].pv);							\
	}																			\
	SCRIPT_CALLABLE_END(funcName, I_PV_PV)

#define SCRIPT_CALLABLE_I_PV_D(funcName)										\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].pv, args[2].d);							\
	}																			\
	SCRIPT_CALLABLE_END(funcName, I_PV_D)

#define SCRIPT_CALLABLE_D_PV(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].d = funcName(args[1].pv);										\
	}																			\
	SCRIPT_CALLABLE_END(funcName, D_PV)



#define CREATE_OBJ_COMMAND(funcName)	Tcl_CreateObjCommand(GetScriptManager().getInterp(), #funcName, _tcl_wrap_##funcName, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

#define START_SCRIPT_FACTORY(className)											\
	class _script_factory_##className											\
	{																			\
	public:																		\
		_script_factory_##className()											\
		{																		\
			CreateScriptManagerIfNotExist();

#define END_SCRIPT_FACTORY(className)											\
		}																		\
	};																			\
	static _script_factory_##className _script_factory_##className_instance_;

