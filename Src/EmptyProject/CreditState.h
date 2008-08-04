#pragma once

#include "State.h"

class CreditState : public State
{
public:
	CreditState(void);
	~CreditState(void);

	virtual void enter();
	virtual void leave();

	virtual void frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual void frameMove(double fTime, float fElapsedTime);

	virtual void release();

private:
	LPD3DXFONT						m_d3dxFont;
	LPD3DXFONT						m_d3dxFontBig;
};
