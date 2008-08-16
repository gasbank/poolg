#pragma once
#include "Picture.h"
#include "State.h"

class MenuState : public State
{ 
public:
	MenuState();
	~MenuState();
	virtual HRESULT frameMove (double fTime, float fElapsedTime);
	virtual HRESULT frameRender(IDirect3DDevice9* pd3dDevice,  double fTime, float fElapsedTime);
	void drawFixedText (int scrWidth, int scrHeight);
	virtual HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual HRESULT release ();
	void select(int move);
	void saveloadselect(int select);

	virtual HRESULT enter();
	virtual HRESULT leave();

	
private:
	
	LPD3DXFONT		m_lblHYnamL;
	LPD3DXFONT		m_lblREB;
	LPD3DXFONT		m_lblsREB;
	
	LPDIRECT3DDEVICE9	m_pDev;

	Picture m_menu;

	Picture m_stub;
	Picture m_saub;
	Picture m_loub;
	Picture m_seub;
	Picture m_exub;

	Picture m_stdb;
	Picture m_sadb;
	Picture m_lodb;
	Picture m_sedb;
	Picture m_exdb;

	Picture m_sanub;

	Picture m_stwin;
	Picture m_sawin;
	Picture m_lowin;

	Picture m_hpbar;
	Picture m_mpbar;
	Picture m_hpbg;
	Picture m_mpbg;
	Picture m_expbg;

	Picture m_slb0;
	Picture m_slb1;
	Picture m_slb2;
	Picture m_slb3;
};