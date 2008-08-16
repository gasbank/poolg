#pragma once
#include "Character.h"
#include "Action.h"

class WorldState;
class Unit;

typedef std::set<Unit*> UnitSet;

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
SCRIPT_FACTORY( Trigger )

//////////////////////////////////////////////////////////////////////////

class UnitPositionTrigger : public Trigger
{
public:
	UnitPositionTrigger( Unit* unit, TileRegion* region );
	virtual ~UnitPositionTrigger() {}

	virtual bool check();

private:
	Unit* m_unit;
	TileRegion* m_region;
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

//////////////////////////////////////////////////////////////////////////

class TotalAnnihilationTrigger : public Trigger
{
public:
	TotalAnnihilationTrigger( WorldState* );
	virtual ~TotalAnnihilationTrigger();
	virtual bool check();

private:
	UnitSet* m_pUs;
};

//////////////////////////////////////////////////////////////////////////

class UnTotalAnnihilationTrigger : public Trigger
{
public:
	UnTotalAnnihilationTrigger( WorldState* );
	virtual ~UnTotalAnnihilationTrigger();
	virtual bool check();

private:
	UnitSet* m_pUs;
};