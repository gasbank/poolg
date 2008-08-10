#pragma once
#include "StateManager.h"

class WorldStateManager : public StateManager, public Singleton<WorldStateManager>
{
public:
	WorldStateManager(void);
	~WorldStateManager(void);

	
	virtual void setNextState(GameState);
	virtual GameState curStateEnum();
	virtual GameState prevStateEnum();

	static const UINT TOTAL_STATE_COUNT = 10;

private:
	virtual void init();
	virtual void release();
};
inline WorldStateManager& GetWorldStateManager() { return WorldStateManager::getSingleton(); }