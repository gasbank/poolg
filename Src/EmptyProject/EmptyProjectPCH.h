#if defined(WIN32)
	#if (_MSC_VER < 1500) // Lower than VC9.0 (2008)
		#error "Visual Studio 2008 is minimum requirement of compilation of this project!"
	#endif
#else
	#error "WIN32 environment is minimum requirement of compilation of this project!"
#endif


#pragma warning(disable:4201)
#pragma warning(disable:4100)
#pragma warning(disable:4505) // disable 'unreferenced local function has been removed' warning
#pragma warning(disable:4189) // disable 'local variable is initialized but not referenced' warning

#include <tchar.h>
#include <string>
#include <assert.h>
#include <time.h>
#include <set>
#include <process.h>

#include "DXUT.h"
#include "AranPCH.h"
#include <xact3.h>
#include "DXUTcamera.h"
#include "tcl.h"

#include "G.h"


#ifndef EP_SAFE_RELEASE
#define EP_SAFE_RELEASE(p)      { if (p) { (p)->release(); SAFE_DELETE(p); } }
#endif

// Visual Studio memory leak detection C runtime library
// This feature is implemented somewhat different way in MFC library,
// therefore we should remove these on MFC related projects

#ifndef _AFXDLL
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////////

#define KEY_WAS_DOWN_MASK 0x80
#define KEY_IS_DOWN_MASK  0x01

inline bool IsKeyDown( BYTE key ) { return( (key & KEY_IS_DOWN_MASK) == KEY_IS_DOWN_MASK ); }
inline bool WasKeyDown( BYTE key ) { return( (key & KEY_WAS_DOWN_MASK) == KEY_WAS_DOWN_MASK ); }


#define SCRIPT_FACTORY(className) class _script_factory_##className { public: static void init(); };

typedef std::list<const char*> ConstCharList;


template<typename T> void EpSafeReleaseAll( T& obj ) {
	T::iterator it = obj.begin();
	for ( ; it != obj.end(); ++it )
	{
		EP_SAFE_RELEASE( *it );
	}
	obj.clear();	
};

#define FLOAT_POS_INF		(0x7f800000)
