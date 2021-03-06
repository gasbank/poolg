﻿#pragma once
#include "state.h"

/**
@brief 필드 State

맵을 돌아다니고 있을 때의 상태이다.
*/
class FieldState :
	public State
{
public:
	FieldState(void);
	~FieldState(void);

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
	void setupCamera();

	bool		m_bCamManualMovement;
};
