#pragma once
#include "StateManager.h"

class WorldStateManager : public StateManager, public Singleton<WorldStateManager>
{
public:
	WorldStateManager(void);
	~WorldStateManager(void);

	virtual void init();
	virtual void release();
	virtual void setNextState(GameState);
};