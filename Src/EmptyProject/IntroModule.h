#pragma once


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
	void draw( IDirect3DDevice9* pd3dDevice );
	void release();
	
private:
	HRESULT CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPCWSTR str, LPD3DXMESH* ppTextMesh );
	

	LPD3DXMESH m_pTextMeshes[NUM_OF_LINES];
	TCHAR* m_pStrs[NUM_OF_LINES];

	D3DXMATRIXA16 m_matObjs[NUM_OF_LINES];	// Matrixes for transform of each text meshes

	// Shape of text
	TCHAR* m_pStrFont;
	BOOL m_bBold;
	BOOL m_bItalic;
	DWORD m_dwFontSize;
};

/* SDKmisc.h
CDXUTTextHelper */