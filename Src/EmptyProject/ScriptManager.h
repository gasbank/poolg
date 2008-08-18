#pragma once
#include "SingletonCreators.h"

extern Tcl_Interp* g_consoleInterp;

class ScriptManager : public Singleton<ScriptManager>
{
public:
	ScriptManager(void);
	~ScriptManager(void);

	HRESULT init();
	HRESULT release();

	void execute(const char* command);
	void executeFile(const char* fileName);
	Tcl_Interp* getInterp() { assert(m_interp); return m_interp; }

	static void throwScriptErrorWithMessage( Tcl_Interp* interp );
	
	const char* readString( const char* variableName );
	bool readRect( const char* variableName, RECT& rect );
	int readInt( const char* variableName );
	Tcl_Obj* getObject( const char* variableName );
	bool readCharPtrList( const char* variableName, ConstCharList& strList );

	void initScriptBindings();
private:
	

	Tcl_Interp* m_interp;
};
inline ScriptManager& GetScriptManager() { return ScriptManager::getSingleton(); }



//////////////////////////////////////////////////////////////////////////

enum ArgumentType
{						// Equivalent C type

	AT_V	= 0,		// void                  ==used only at return type==
	AT_I	= 1,		// signed int or signed long
	AT_PC	= 2,		// const char*
	AT_D	= 3,		// double
	AT_PV	= 4,		// void*
	AT_OBJ	= 5,		// Tcl_Obj*

	AT_MAX	= 15,		// ---- SENTINEL (DO NOT CHANGE THIS VALUE) ---
};

union ScriptArgument
{
	int i;
	const char* pc;
	double d;
	void* pv;
	Tcl_Obj* obj;
};

typedef std::vector<ScriptArgument> ScriptArgumentList;

void ParseTclArgumentByTrait( DWORD trait, Tcl_Interp* interp, int objc, Tcl_Obj *CONST objv[], ScriptArgumentList& argList );
void SetTclResult(Tcl_Interp* interp, DWORD trait, Tcl_Obj* tcl_result, const ScriptArgumentList& argList);

#define SCRIPT_CALLABLE_END(funcName, traits)																				\
	static int _tcl_wrap_##funcName(ClientData clientData, Tcl_Interp *interp,	\
									int objc, Tcl_Obj *CONST objv[])			\
	{																			\
		Tcl_Obj* tcl_result = Tcl_GetObjResult(interp);							\
		ScriptArgumentList argList;												\
		ParseTclArgumentByTrait(_trait_##traits, interp, objc, objv, argList);	\
		_wrap_##funcName(argList);												\
		SetTclResult(interp, _trait_##traits, tcl_result, argList);				\
		return TCL_OK;															\
	}

//////////////////////////////////////////////////////////////////////////

//#define REGISTER_TRAIT_1(a1)									static const DWORD _trait_##a1										= (a1);
//#define REGISTER_TRAIT_2(a1, a2)								static const DWORD _trait_##a1_##a2									= (a1) | (a2<<4);
//#define REGISTER_TRAIT_3(a1, a2, a3)							static const DWORD _trait_##a1_##a2_##a3							= (a1) | (a2<<4) | (a3<<8);
//#define REGISTER_TRAIT_4(a1, a2, a3, a4)						static const DWORD _trait_##a1_##a2_##a3_##a4						= (a1) | (a2<<4) | (a3<<8) | (a4<<12);
//#define REGISTER_TRAIT_5(a1, a2, a3, a4, a5)					static const DWORD _trait_##a1_##a2_##a3_##a4_##a5					= (a1) | (a2<<4) | (a3<<8) | (a4<<12) | (a5<<16);
//#define REGISTER_TRAIT_6(a1, a2, a3, a4, a5, a6)				static const DWORD _trait_##a1_##a2_##a3_##a4_##a5_##a6				= (a1) | (a2<<4) | (a3<<8) | (a4<<12) | (a5<<16) | (a6<<20);
//#define REGISTER_TRAIT_7(a1, a2, a3, a4, a5, a6, a7)			static const DWORD _trait_##a1_##a2_##a3_##a4_##a5_##a6_##a7		= (a1) | (a2<<4) | (a3<<8) | (a4<<12) | (a5<<16) | (a6<<20) | (a7<<24);
//#define REGISTER_TRAIT_8(a1, a2, a3, a4, a5, a6, a7, a8)		static const DWORD _trait_##a1_##a2_##a3_##a4_##a5_##a6_##a7_##a8	= (a1) | (a2<<4) | (a3<<8) | (a4<<12) | (a5<<16) | (a6<<20) | (a7<<24) | (a7<<28);


static const DWORD _trait_I				= AT_I;
static const DWORD _trait_I_I			= AT_I | (AT_I << 4);
static const DWORD _trait_I_I_I			= AT_I | (AT_I << 4) | (AT_I << 8);
static const DWORD _trait_I_PC			= AT_I | (AT_PC << 4);
static const DWORD _trait_I_PV			= AT_I | (AT_PV << 4);
static const DWORD _trait_I_PV_I		= AT_I | (AT_PV << 4) | (AT_I << 8);
static const DWORD _trait_I_PV_I_I		= AT_I | (AT_PV << 4) | (AT_I << 8) | (AT_I << 12);
static const DWORD _trait_I_PV_I_I_I	= AT_I | (AT_PV << 4) | (AT_I << 8) | (AT_I << 12) | (AT_I << 16);
static const DWORD _trait_I_PV_I_I_I_I	= AT_I | (AT_PV << 4) | (AT_I << 8) | (AT_I << 12) | (AT_I << 16) | (AT_I << 20);
static const DWORD _trait_I_PV_D		= AT_I | (AT_PV << 4) | (AT_D << 8);
static const DWORD _trait_I_PV_PV		= AT_I | (AT_PV << 4) | (AT_PV << 8);
static const DWORD _trait_PV			= AT_PV;
static const DWORD _trait_PV_PV			= AT_PV | (AT_PV << 4);
static const DWORD _trait_PV_PC			= AT_PV | (AT_PC << 4);
static const DWORD _trait_PV_I_I		= AT_PV | (AT_I << 4) | (AT_I << 8);
static const DWORD _trait_PV_PV_I		= AT_PV | (AT_PV << 4) | (AT_I << 8);
static const DWORD _trait_PV_PV_PC		= AT_PV | (AT_PV << 4) | (AT_PC << 8);
static const DWORD _trait_PV_I_I_I		= AT_PV | (AT_I << 4) | (AT_I << 8) | (AT_I << 12);
static const DWORD _trait_PV_PV_PV		= AT_PV | (AT_PV << 4) | (AT_PV << 8) ;
static const DWORD _trait_PV_PV_PV_I	= AT_PV | (AT_PV << 4) | (AT_PV << 8) | (AT_I << 12);
static const DWORD _trait_PV_PV_I_I_I	= AT_PV | (AT_PV << 4) | (AT_I << 8) | (AT_I << 12) | (AT_I << 16);
static const DWORD _trait_PV_PV_I_I_I_I	= AT_PV | (AT_PV << 4) | (AT_I << 8) | (AT_I << 12) | (AT_I << 16) | (AT_I << 20);
static const DWORD _trait_D_PV			= AT_D | (AT_PV << 4);
static const DWORD _trait_PV_I_I_I_I_I_I= AT_I | (AT_PV << 4) | (AT_I << 8) | (AT_I << 12) | (AT_I << 16) | (AT_I << 20) | (AT_I << 24) | (AT_I << 28);
static const DWORD _trait_OBJ_PV		= AT_OBJ | (AT_PV << 4);

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

#define SCRIPT_CALLABLE_OBJ_PV(funcName)										\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].obj = funcName(args[1].pv);										\
	}																			\
	SCRIPT_CALLABLE_END(funcName, OBJ_PV)

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

#define SCRIPT_CALLABLE_I_PV_I(funcName)										\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].pv, args[2].i);							\
	}																			\
	SCRIPT_CALLABLE_END(funcName, I_PV_I)

#define SCRIPT_CALLABLE_I_PV_I_I(funcName)										\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].pv, args[2].i, args[3].i);					\
	}																			\
	SCRIPT_CALLABLE_END(funcName, I_PV_I_I)

#define SCRIPT_CALLABLE_I_PV_I_I_I(funcName)									\
	void _wrap_##funcName(ScriptArgumentList& args)								\
{																				\
	args[0].i = funcName(args[1].pv, args[2].i, args[3].i, args[4].i);			\
}																				\
	SCRIPT_CALLABLE_END(funcName, I_PV_I_I_I)

#define SCRIPT_CALLABLE_I_PV_I_I_I_I(funcName)									\
	void _wrap_##funcName(ScriptArgumentList& args)								\
{																				\
	args[0].i = funcName(args[1].pv, args[2].i, args[3].i, args[4].i, args[5].i);\
}																				\
	SCRIPT_CALLABLE_END(funcName, I_PV_I_I_I_I)

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


#define SCRIPT_CALLABLE_PV(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].pv = funcName();												\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV)



#define SCRIPT_CALLABLE_I_PV_I_I_I_I_I_I(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].i = funcName(args[1].pv, args[2].i, args[3].i, args[4].i, args[5].i, args[6].i, args[7].i);\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_I_I_I_I_I_I)


#define SCRIPT_CALLABLE_PV_PV_I_I_I(funcName)									\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].pv = funcName(args[1].pv, args[2].i, args[3].i, args[4].i);		\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_PV_I_I_I)

#define SCRIPT_CALLABLE_PV_PV_I_I_I_I(funcName)									\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
	args[0].pv = funcName(args[1].pv, args[2].i, args[3].i, args[4].i, args[5].i);		\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_PV_I_I_I_I)


#define SCRIPT_CALLABLE_PV_PC(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].pv = funcName(args[1].pc);										\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_PC)


#define SCRIPT_CALLABLE_PV_PV_PV(funcName)										\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].pv = funcName(args[1].pv, args[2].pv);							\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_PV_PV)

#define SCRIPT_CALLABLE_PV_PV(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
	args[0].pv = funcName(args[1].pv);											\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_PV)

#define SCRIPT_CALLABLE_PV_PV_I(funcName)											\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
	args[0].pv = funcName(args[1].pv, args[2].i);								\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_PV_PI)

#define SCRIPT_CALLABLE_PV_PV_PV_I(funcName)									\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
		args[0].pv = funcName(args[1].pv, args[2].pv, args[3].i);				\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_PV_PV_I)

#define SCRIPT_CALLABLE_PV_PV_PC(funcName)										\
	void _wrap_##funcName(ScriptArgumentList& args)								\
	{																			\
	args[0].pv = funcName(args[1].pv, args[2].pc);							\
	}																			\
	SCRIPT_CALLABLE_END(funcName, PV_PV_PC)


#define CREATE_OBJ_COMMAND_ENGINE(funcName)											\
	Tcl_CreateObjCommand(GetScriptManager().getInterp(), #funcName, _tcl_wrap_##funcName, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

#define CREATE_OBJ_COMMAND(funcName)																										\
	Tcl_CreateObjCommand(GetScriptManager().getInterp(), #funcName, _tcl_wrap_##funcName, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);	\
	assert(g_consoleInterp);																												\
	Tcl_CreateObjCommand(g_consoleInterp, #funcName, _tcl_wrap_##funcName, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

#define START_SCRIPT_FACTORY(className)											\
	void _script_factory_##className::init()									\
	{

#define END_SCRIPT_FACTORY(className)											\
	}