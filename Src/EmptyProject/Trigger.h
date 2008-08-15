#pragma once
#include "Character.h"
#include "Action.h"

class WorldState;


class Trigger
{
public:
	Trigger();
	virtual ~Trigger(void);

	virtual bool check() = 0;


	void positionTrigger();
	void deadTrigger();
	void timeTrigger();

	void detectBattleAction();
	void detectTalkAction();
	void detectCreateAction();
	void detectHealAction();

private:
	WorldState* m_ws;
};

//////////////////////////////////////////////////////////////////////////

class UnitPositionTrigger : public Trigger
{
public:
	UnitPositionTrigger( Unit* unit, const TileRegion& region );
	virtual ~UnitPositionTrigger() {}

	virtual bool check();

private:
	Unit* m_unit;
	TileRegion m_region;
};

//////////////////////////////////////////////////////////////////////////

class CharHpTrigger : public Trigger
{
public:
	CharHpTrigger( Character* unit, int min, int max, bool bInclude = true );
	virtual ~CharHpTrigger() {}

	virtual bool check();

private:
	Character* m_char;
	int m_min;
	int m_max;
	bool m_bInclude;
};