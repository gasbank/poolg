#pragma once
#include "StateManager.h"

class TopStateManager : public StateManager, public Singleton<TopStateManager>
{
public:
	TopStateManager(void);
	~TopStateManager(void);

	virtual void init();
	virtual void release();
	virtual void setNextState(GameState);
};
