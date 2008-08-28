#pragma once
#include "StateManager.h"

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