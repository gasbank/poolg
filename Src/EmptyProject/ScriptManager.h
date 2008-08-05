#pragma once

class ScriptManager : public Singleton<ScriptManager>
{
public:
	ScriptManager(void);
	~ScriptManager(void);

	HRESULT init();
	HRESULT release();

	HRESULT execute(const char* command);

private:
	Tcl_Interp* m_interpreter;
};
