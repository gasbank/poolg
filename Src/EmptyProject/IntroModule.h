#pragma once

#include "Picture.h"

#define NUM_OF_LINES 20

class IntroModule
{
public:
	IntroModule();
	~IntroModule(void);

	HRESULT CreateTextMeshes( IDirect3DDevice9* pd3dDevice );
	void SetCameraAndLight( IDirect3DDevice9* pd3dDevice, 
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		CFirstPersonCamera* pCamera );
	void frameMove( double fTime );
	void draw( IDirect3DDevice9* pd3dDevice, CFirstPersonCamera* pCamera );
	void release();
	
private:
	HRESULT CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPCWSTR str, LPD3DXMESH* ppTextMesh );
	

	LPD3DXMESH m_pTextMeshes[NUM_OF_LINES];
	TCHAR* m_pStrs[NUM_OF_LINES];

	D3DXMATRIXA16 m_matObjs[NUM_OF_LINES];	// Matrixes for transform of each text meshes
	
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
};

/* SDKmisc.h
CDXUTTextHelper */