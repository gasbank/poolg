#include "EmptyProjectPCH.h"

/**
@brief Visual Studio 출력 창에 디버그 정보를 출력하는 함수
*/
void __cdecl odprintf(const char *format, ...)
{
	int nSize = 0;
	char buf[4096];
	va_list	args;
	va_start(args, format);
	nSize = vsnprintf_s( buf, sizeof(buf), _TRUNCATE, format, args);
	OutputDebugStringA(buf);
}

