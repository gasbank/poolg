#pragma once

class Dialog
{
public:\
	   	bool OK;

	Dialog(void);
	~Dialog(void);
	HRESULT init();
	void release();
	void print(WCHAR debugBuffer[256]);
	void Toggle();

private:
	RECT rc;
	LPD3DXFONT pFont;
};
