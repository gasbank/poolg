#pragma once
#include "StateManager.h"

/**
@brief PlayState 의 세부 상태를 관리하는 클래스

PlayState 는 특별히 게임을 플레이하고 있는 상태로서
세부적인 상태로 나눌 필요가 있다. 이러한 세부 상태를
WorldState라고 부르며 이러한 State 로는 BattleState 나 FieldState 등이 있다.

*/
class WorldStateManager : public StateManager, public Singleton<WorldStateManager>
{
public:
	WorldStateManager(void);
	~WorldStateManager(void);
	virtual void init();

	virtual HRESULT onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	void onLostDevice();
	
};
inline WorldStateManager& GetWorldStateManager() { return WorldStateManager::getSingleton(); }