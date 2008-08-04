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

	virtual HRESULT release();

private:
	void setupLight();

	LPD3DXFONT						m_d3dxFont;
	LPD3DXFONT						m_d3dxFontBig;

	double							m_startTime;
};
