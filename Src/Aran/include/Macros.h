// Macros.h
// 2007 Geoyeob Kim
//
// This file is shared between Aran and ModelExporter C++ Projects at VS2005 Solution.
// Should have dependency on d3dx9.h but NOT have on any 3ds Max related headers
// since Aran game engine itself must not rely on 3ds Max.
//
#pragma once

typedef std::string STRING;

#define V_OKAY(x) { HRESULT __hr__; if(FAILED(__hr__ = (x))) return DXTRACE_ERR_MSGBOX(_T("V_OKAY() FAILED"), __hr__); }
#define V_VERIFY(x) { if (FAILED(x)) throw MyError(MEE_GENERAL_VERIFICATION_FAILED); }
#define GLOBAL_TEXTURE_FILE_PATH			"Textures\\"
#define GLOBAL_ARN_FILE_PATH				"Models\\"
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if((p)!=0) { (p)->Release(); (p) = 0; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) if((p)!=0) { delete [] (p); (p) = 0; }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if((p)!=0) { delete (p); (p) = 0; }
#endif

namespace std {
#if defined _UNICODE || defined UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif
}

#define TCHARSIZE(x) (sizeof(x)/sizeof(TCHAR))


#if defined(DEBUG) | defined(_DEBUG)
#define ASSERTCHECK(x) \
	if (! (x)) \
{ \
	char lineNumber[8]; \
	_itoa_s(__LINE__, lineNumber, 10); \
	STRING ___file___Name___(__FILE__); \
	___file___Name___ += "("; \
	___file___Name___ += lineNumber; \
	___file___Name___ += ")\nFollowing statement is NOT TRUE or 0;\n"; \
	___file___Name___ += #x; \
	MessageBoxA(0, ___file___Name___.c_str(), "ASSERTION ERROR!", MB_OK | MB_ICONERROR); \
	return E_FAIL; \
}
#else
#define ASSERTCHECK(x)
#endif



