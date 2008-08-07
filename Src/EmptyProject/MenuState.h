#pragma once
#include "Box.h"
#include "State.h"

class MenuState : public State
{ 
public:
	MenuState();
	~MenuState();
	virtual HRESULT frameMove (double fTime, float fElapsedTime);
	virtual HRESULT frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime);
	//void drawFixedText (int scrWidth, int scrHeight);
	virtual HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual HRESULT release ();
	void select(int move);

	virtual HRESULT enter();
	virtual HRESULT leave();

	
private:
	
	LPD3DXFONT		m_lblHYnamL;
	LPD3DXFONT		m_lblREB;
	
	LPDIRECT3DDEVICE9	m_pDev;

	Box m_menu;
	Box m_selc;

	Box m_stdb;
	Box m_sadb;
};