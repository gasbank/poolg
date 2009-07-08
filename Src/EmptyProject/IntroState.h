﻿#pragma once

#include "State.h"
#include "Picture.h"
#include "Sound.h" // 사운드 헤더파일
#include "Sprite.h"
#include "StateManager.h"

#define NUM_OF_LINES 20

/**
@brief 새 게임 시작시에 소개를 보여주는 State
*/
class IntroState : public State
{
public:
	IntroState();
	~IntroState(void);

	virtual HRESULT enter( double dStartTime );
	virtual HRESULT leave();

	virtual HRESULT frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual HRESULT frameMove(double fTime, float fElapsedTime);

	virtual HRESULT handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual HRESULT onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	virtual HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	virtual void onLostDevice();
	virtual HRESULT release();
	
private:
	void setupLight();
	void setupCamera();
	
	bool m_backgroundVisible;
	ArnMatrix m_matBackground;

	Sprite* m_pLogo;
	DrawRequest* m_pLogoDrawRequest;
	Sprite* m_pGalaxy;
	DrawRequest* m_pGalaxyDrawRequest;
	Sprite* m_pPrologue;
	DrawRequest* m_pPrologueDrawRequest;

	float m_prologueInitZPos;

	bool m_logoVisible;
	float m_logoFading;

	float m_mtrlControl;

	Picture m_pBlack;

	float m_velocity;

	Sound	m_sound; // 사운드 변수

	float m_fFadeTimer;
	float m_fTextAlpha;

	GameState m_nextState;
};

/* SDKmisc.h
CDXUTTextHelper */