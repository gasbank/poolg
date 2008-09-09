#include "WSAStartupSingleton.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

int WSAStartupSingleton::refCount;
	
WSAStartupSingleton::WSAStartupSingleton() {refCount=0;}
WSAStartupSingleton::~WSAStartupSingleton() {}
void WSAStartupSingleton::AddRef(void)
{
#ifdef _WIN32

	refCount++;
	
	if (refCount!=1)
		return;
		
	WSADATA winsockInfo;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &winsockInfo ) != 0 )
	{
#if !defined(_XBOX360) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "WSAStartup failed:Error code - %d\n%s", dwIOError, messageBuffer );
		//Free the buffer.
		LocalFree( messageBuffer );
#endif
	}

#endif
}
void WSAStartupSingleton::Deref(void)
{
#ifdef _WIN32
	if (refCount==0)
		return;
		
	if (refCount>1)
	{
		refCount--;
		return;
	}
	
	WSACleanup();
	
	refCount=0;
#endif
}
