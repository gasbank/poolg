#pragma once
#include "state.h"

class FieldState :
	public State
{
public:
	FieldState(void);
	~FieldState(void);

	virtual HRESULT enter();
	virtual HRESULT leave();

	virtual HRESULT frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual HRESULT frameMove(double fTime, float fElapsedTime);

	virtual HRESULT handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual HRESULT release();

private:
	void setupCamera();

	bool		m_bCamManualMovement;
};
