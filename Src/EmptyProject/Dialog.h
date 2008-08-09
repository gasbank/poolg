#pragma once

#include "Picture.h"

class Dialog
{
public:
	bool dlg_ON;
	bool startTalk;
	bool endTalk;

	Dialog(void);
	~Dialog(void);
	HRESULT init();
	HRESULT release();
	void Toggle(bool *OK);
	void printDialog();
	void printName();
	void NextDialog();

	HRESULT frameMove (double fTime, float fElapsedTime);
	HRESULT frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime);
	HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static LPD3DXFONT pFont;
	static LPD3DXFONT qFont;
	static RECT pRc;
	static RECT qRc;
private:
	Picture dialog;
	Picture name;

	LPDIRECT3DDEVICE9	m_pDev;

	std::vector<std::string> debugBuffer;
	std::vector<std::string>::iterator bufferPointer;
	std::string nameBuffer;
	std::string* namePointer;
};