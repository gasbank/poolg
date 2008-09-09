// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here

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
