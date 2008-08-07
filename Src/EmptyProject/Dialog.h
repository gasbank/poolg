#pragma once
#include "Box.h"

class Dialog
{
public:\
	   	bool OK;

	Dialog(void);
	~Dialog(void);
	HRESULT init();
	HRESULT release();
	void Toggle();

	HRESULT frameMove (double fTime, float fElapsedTime);
	HRESULT frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime);
	HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static LPD3DXFONT pFont;
	static RECT rc;
private:
	
	
	Box dial;

	LPDIRECT3DDEVICE9	m_pDev;
};