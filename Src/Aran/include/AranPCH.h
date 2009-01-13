// AranPCH.h
// 2008 Geoyeob Kim (gasbank@gmail.com)

#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#include <CommCtrl.h>

#include <iostream>
#include <string>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <map>
#include <fstream>
#include <list>
#include <vector>

#include <d3dx9.h>
#include <dxerr9.h>
#include <CommCtrl.h>

#ifndef V
#define V(x)           { hr = (x); if( FAILED(hr) ) { return DXTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif


#include "Macros.h"

#include "Singleton.h"
#include "Log.h"
#include "Structs.h"
#include "MyError.h"

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
#endif // #ifndef _AFXDLL


#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds
