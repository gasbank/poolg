// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <windows.h>

#include <stdio.h>
#include <tchar.h>
#include <list>


// TODO: reference additional headers your program requires here

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

#ifdef _WIN32
#include "Kbhit.h"
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

#include "CommonStructs.h"
