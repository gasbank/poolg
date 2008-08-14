#pragma once
#include "Character.h"
class WorldState;

class Trigger
{
public:
	Trigger(void);
	~Trigger(void);

	void positionTrigger();
	void deadTrigger();
	void timeTrigger();

	void detectBattleAction();
	void detectTalkAction();
	void detectHealAction();
private:
	WorldState* m_ws;
};
