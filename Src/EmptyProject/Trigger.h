#pragma once
#include "Character.h"
#include "Action.h"

class World;
class Unit;

enum UnitPositionTriggerType {
	UPTT_ENTER  = 0x001,
	UPTT_LEAVE  = 0x010,
	UPTT_STAY	= 0x100 
};

typedef std::set<Unit*> UnitSet;

class Trigger
{
public:
	Trigger();
	virtual ~Trigger(void);

	virtual bool check() = 0;
	virtual void release() {}
};
SCRIPT_FACTORY( Trigger )

//////////////////////////////////////////////////////////////////////////

class UnitPositionTrigger : public Trigger
{
public:
	UnitPositionTrigger( Unit* unit, TileRegion* region, UnitPositionTriggerType type );
	virtual ~UnitPositionTrigger() {}

	virtual bool check();

private:
	Unit* m_unit;
	TileRegion* m_region;
	bool m_lastCheckResult;
	UnitPositionTriggerType m_type;
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
	TotalAnnihilationTrigger( World* );
	virtual ~TotalAnnihilationTrigger();
	virtual bool check();

private:
	UnitSet* m_pUs;
};

//////////////////////////////////////////////////////////////////////////

class UnTotalAnnihilationTrigger : public Trigger
{
public:
	UnTotalAnnihilationTrigger( World* );
	virtual ~UnTotalAnnihilationTrigger();
	virtual bool check();

private:
	UnitSet* m_pUs;
};