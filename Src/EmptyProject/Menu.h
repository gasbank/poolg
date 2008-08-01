#pragma once
#include "Box.h"

class Menu
{
public:
	void init (LPDIRECT3DDEVICE9& d3dDev, int scrWidth, int scrHeight);
	void frameMove (float fElapsedTime);
	void draw (int scrWidth, int scrHeight);
	void drawFixedText (int scrWidth, int scrHeight);
	void handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void release ();

	
private:
	
	LPD3DXFONT		m_lblHYnamL;
	LPD3DXFONT		m_lblREB;
	
	LPDIRECT3DDEVICE9	m_pDev;

	Box m_menu;
	Box m_selc;
};