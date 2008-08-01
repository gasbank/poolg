// Log.h
// 2006 Geoyeob, Kim (First Revision: MapModel Project)
// 2008 Geoyoeb, Kim (Second Revision: Aran Project)

// Log management class
#pragma once

#ifndef LOG_OKAY
#define LOG_OKAY true
#endif
#ifndef LOG_FAIL
#define LOG_FAIL false
#endif



class LOGMANAGER : public Singleton<LOGMANAGER>
{
public:
	LOGMANAGER();
	~LOGMANAGER();
	LOGMANAGER(const char* filename);
	LOGMANAGER(const char* filename, bool bAppend);

	void NewLog(const TCHAR* sourcefilename, const TCHAR* funcname, int line, const TCHAR* message, bool bOkay);
	void NewLog(const TCHAR* message, bool bOkay);
	void NewLog(const TCHAR* message);
	int GetFailCount() { return failCount; };

private:
	int okayCount;
	int failCount;
	//std::ofstream fout;
	std::wofstream fout;
	

	TCHAR debugBuf[256];
};

#define QUOTEME(x) #x

#ifndef _LogWrite
#define _LogWrite(___msg,___okay)															\
{																							\
	LOGMANAGER::getSingleton().NewLog(_T(__FILE__), _T(__FUNCTION__), __LINE__, ___msg, ___okay);	\
}
#endif
