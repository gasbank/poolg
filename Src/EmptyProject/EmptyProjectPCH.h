#pragma once

#if defined(WIN32)
	#if (_MSC_VER < 1500) // Lower than VC9.0 (2008)
		#error "Visual Studio 2008 is minimum requirement of compilation of this project!"
	#endif
#else
	#error "WIN32 environment is minimum requirement of compilation of this project!"
#endif


// The following macros define the minimum required platform.  The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
// your application.  The macros work by enabling all features available on platform versions up to and 
// including the version specified.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif



#pragma warning(disable:4201)
#pragma warning(disable:4100)
#pragma warning(disable:4505) // disable 'unreferenced local function has been removed' warning
#pragma warning(disable:4189) // disable 'local variable is initialized but not referenced' warning



#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <map>
#include <fstream>
#include <list>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <tchar.h>
#include <string>
#include <assert.h>
#include <time.h>
#include <set>
#include <algorithm>
#include <process.h>


#include "DXUT.h"
#ifdef USE_XACT3
	#include <xact3.h>
#else
	#include <xact.h>
#endif
#include "DXUTcamera.h"
#include "tcl.h"


// Aran Lib
#include "AranPCH.h"
/*
#include "Macros.h"
#include "Singleton.h"
#include "Log.h"
#include "Structs.h"
#include "MyError.h"
*/

#include "G.h"



// RakNet
#include "RPC3.h"
#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "ReplicaManager2.h"
#include "FormatString.h"
#include "StringCompressor.h"
#include "StringTable.h"


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






#define FLOAT_POS_INF		(0x7f800000)

enum BasicShapeType { BST_UNKNOWN, BST_TEAPOT, BST_SPHERE, BST_CUBE, BST_PLANE, BST_COUNT };

void cdecl odprintf(const char *format, ...);

#include "Typedefs.h"
#include "CommonStructs.h"

#define DO_NOT_REDEFINE_NEW_KEYWORD
#include "AranPCH.h"



template<typename T> void SafeDeleteAll( T& obj ) {
	T::iterator it = obj.begin();
	for ( ; it != obj.end(); ++it )
	{
		SAFE_DELETE( *it );
	}
	obj.clear();
};

template<typename T> void EpSafeReleaseAllMap( T& obj ) {
	T::iterator it = obj.begin();
	for ( ; it != obj.end(); ++it )
	{
		EP_SAFE_RELEASE( it->second );
	}
	obj.clear();
};
