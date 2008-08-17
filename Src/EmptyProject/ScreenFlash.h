#pragma 

#include "ShaderWrapper.h"

class ScreenFlash
{
public:
	ScreenFlash(void);
	~ScreenFlash(void);

	void setup();
	HRESULT frameRender();
	HRESULT frameMove( double fTime, float fElapsedTime );
	HRESULT handleMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void reset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	void release();
	void screenFlashing( float durationSec, float r, float g, float b );

private:
	AlphaShader*					m_alphaShader;
	LPD3DXMESH						m_testPolygon;
	LPD3DXMESH						m_testPolygonCloned;
	float							m_screenFlashAlphaAngle;
	float							m_screenFlashDurationSec;
	D3DXCOLOR						m_screenFlashCV;
	bool							m_bStop;
};
