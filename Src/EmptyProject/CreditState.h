#pragma once

#include "State.h"

class CreditState : public State
{
public:
	CreditState(void);
	~CreditState(void);

	virtual HRESULT enter();
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


	D3DMATERIAL9					m_textMat;
	float							m_textMatAlpha;

	D3DXCOLOR						m_textColor;
	float							m_creditStartTime;

	double							m_startTime;

	static const float				s_period;
	static const float				s_fadeIn;
	static const float				s_fadeOut;
};
