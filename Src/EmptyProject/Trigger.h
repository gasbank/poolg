#pragma once
#include "Character.h"
#include "Action.h"

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
	Action		m_action;
};
