#include "AranPCH.h"
#include <tchar.h>
#include "DXUT.h"
#include <xact3.h>
#include "DXUTcamera.h"
#include "G.h"
#include "tcl.h"

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