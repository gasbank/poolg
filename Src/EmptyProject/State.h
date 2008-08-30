#pragma once
#include "WorldManager.h"

class State
{
public:
	State(void);
	virtual ~State(void);

	virtual HRESULT enter() = 0;
	virtual HRESULT leave() = 0;

	virtual HRESULT frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime) = 0;
	virtual HRESULT frameMove(double fTime, float fElapsedTime) = 0;

	virtual HRESULT handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	
	virtual HRESULT onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									void* pUserContext ) = 0;
	virtual HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									void* pUserContext ) = 0;
	virtual void onLostDevice() = 0;

	virtual HRESULT release() = 0;

	double getStateTime(double fTime);
	World* getCurWorld() const { return GetWorldManager().getCurWorld(); }

	
protected:
	double m_startTime;
};
