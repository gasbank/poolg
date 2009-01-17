#pragma once
#include "WorldManager.h"

/**
@brief 단위 상태를 나타내는 클래스

예를 들어 로고를 보여주는 상태, 제작진을 보여주는 상태,
게임 메인 로직이 돌아가고 있는 상태 등을 정의할 때 사용되는 베이스 클래스입니다.
*/
class State
{
public:
						State(void);
	virtual				~State(void);

	// Pure Virtual Methods
	virtual HRESULT		enter( double dStartTime ) = 0;
	virtual HRESULT		leave() = 0;
	virtual HRESULT		frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime) = 0;
	virtual HRESULT		frameMove(double fTime, float fElapsedTime) = 0;
	virtual HRESULT		handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	
	virtual HRESULT		onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) = 0;
	virtual HRESULT		onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) = 0;
	virtual void		onLostDevice() = 0;

	virtual HRESULT		release() = 0;

	// Non-virtual Methods
	double				getStateTime( double dCurrentTime ) const { assert( m_startTime > 0 ); return dCurrentTime - m_startTime; }
	World*				getCurWorld() const { return GetWorldManager().getCurWorld(); }

private:
	double				m_startTime;

};
