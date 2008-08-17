#pragma once
#include "StateManager.h"

class World;

class TopStateManager : public StateManager, public Singleton<TopStateManager>
{
public:
	TopStateManager(void);
	~TopStateManager(void);

	virtual void init();

	
};
inline TopStateManager& GetTopStateManager() { return TopStateManager::getSingleton(); }
