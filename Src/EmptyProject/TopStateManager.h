#pragma once
#include "StateManager.h"

class TopStateManager : public StateManager, public Singleton<TopStateManager>
{
public:
	TopStateManager(void);
	~TopStateManager(void);

private:
	virtual void init();
};
inline TopStateManager& GetTopStateManager() { return TopStateManager::getSingleton(); }


SCRIPT_FACTORY( TopStateManager )
