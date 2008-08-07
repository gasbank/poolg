#pragma once

#include "State.h"
#include "Picture.h"
#include "Sound.h" // 사운드 헤더파일

#define NUM_OF_LINES 20

class IntroState : public State
{
public:
	IntroState();
	~IntroState(void);

	virtual HRESULT enter();
	virtual HRESULT leave();

	virtual HRESULT frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual HRESULT frameMove(double fTime, float fElapsedTime);

	virtual HRESULT handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual HRESULT release();
	
private:

	HRESULT createTextMeshes( IDirect3DDevice9* pd3dDevice );
	HRESULT createD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPCWSTR str, LPD3DXMESH* ppTextMesh );
	void setupLight();
	void setupCamera();

	LPD3DXMESH m_pTextMeshes[NUM_OF_LINES];
	TCHAR* m_pStrs[NUM_OF_LINES];

	D3DXMATRIX m_matObjs[NUM_OF_LINES];	// Matrixes for transform of each text meshes
	
	Picture m_background;
	bool m_backgroundVisible;
	D3DXMATRIX m_matBackground;

	Picture m_pLogo;
	bool m_logoVisible;
	float m_logoFading;

	float m_mtrlControl;

	Picture m_pBlack;

	// Shape of text
	TCHAR* m_pStrFont;
	BOOL m_bBold;
	BOOL m_bItalic;
	DWORD m_dwFontSize;

	float m_velocity;

	Sound	m_sound; // 사운드 변수
};

/* SDKmisc.h
CDXUTTextHelper */