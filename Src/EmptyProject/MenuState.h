﻿#pragma once
#include "Picture.h"
#include "State.h"

class Sprite;
class DrawRequest;

/**
@brief 게임 플레이 중 플레이어가 메뉴를 불렀을 때 사용되는 State
*/
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

	virtual HRESULT enter( double dStartTime );
	virtual HRESULT leave();

	virtual HRESULT onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	virtual HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	virtual void onLostDevice();
	
private:
	Sprite*			m_sprite;

	DrawRequest*	m_menuBoxDr;
	DrawRequest*	m_menuBoxDr1;
	DrawRequest*	m_menuBoxDr2;
	DrawRequest*	m_menuBoxDr3;
	DrawRequest*	m_menuBoxDr4;

	DrawRequest*	m_menuDr[5];
	int				m_curSelMenu;

	/*
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
	Picture m_expbar;
	Picture m_hpbg;
	Picture m_mpbg;
	Picture m_expbg;

	Picture m_slb0;
	Picture m_slb1;
	Picture m_slb2;
	Picture m_slb3;
	*/
};