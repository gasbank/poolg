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
	D3DXVECTOR3 m_vPrevEye;
	D3DXVECTOR3 m_vPrevLookAt;
	D3DXVECTOR3 m_vPrevUp;

	bool			m_bCamManualMovement;
};
