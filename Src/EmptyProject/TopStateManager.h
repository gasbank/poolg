#pragma once
#include "StateManager.h"

class World;

/**
@brief 게임 내 최상위 레벨의 상태를 관리하는 클래스
*/
class TopStateManager : public StateManager, public Singleton<TopStateManager>
{
public:
	TopStateManager(void);
	~TopStateManager(void);

	virtual void init();

	virtual HRESULT onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	void onLostDevice();
	
};
inline TopStateManager& GetTopStateManager() { return TopStateManager::getSingleton(); }
