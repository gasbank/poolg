#pragma once
#include "StateManager.h"

class TopStateManager : public StateManager, public Singleton<TopStateManager>
{
public:
	TopStateManager(void);
	~TopStateManager(void);

	
	virtual void setNextState(GameState);
	virtual GameState curStateEnum();
	virtual GameState prevStateEnum();

	static const UINT TOTAL_STATE_COUNT = 10;

private:
	virtual void init();
	virtual void release();
};
inline TopStateManager& GetTopStateManager() { return TopStateManager::getSingleton(); }


SCRIPT_FACTORY( TopStateManager )
