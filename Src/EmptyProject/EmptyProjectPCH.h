#pragma warning(disable:4201)
#pragma warning(disable:4100)
#pragma warning(disable:4505) // disable 'unreferenced local function has been removed' warning

#include "AranPCH.h"
#include <tchar.h>
#include <set>
#include "DXUT.h"
#include <xact3.h>
#include "DXUTcamera.h"
#include "G.h"
#include "tcl.h"
#include <string>

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