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

	virtual void printDebugInfo() const { printf( "Debug Info not implemented on Trigger class\n" ); }
};
SCRIPT_FACTORY( Trigger )

//////////////////////////////////////////////////////////////////////////

class UnitPositionTrigger : public Trigger
{
public:
	UnitPositionTrigger( Unit* unit, TileRegion* region, UnitPositionTriggerType type );
	virtual ~UnitPositionTrigger();

	virtual bool check();
	virtual void printDebugInfo() const;

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

//////////////////////////////////////////////////////////////////////////

class IncidentTrigger : public Trigger
{
public:
	IncidentTrigger( Incident* incident );
	virtual ~IncidentTrigger();
	virtual bool check();

private:
	Incident* m_incident;
};

//////////////////////////////////////////////////////////////////////////

class UnitPositionWithTraceTrigger : public Trigger
{
public:
	UnitPositionWithTraceTrigger( Unit* unitA, Unit* unitB, UnitPositionTriggerType type );
	virtual ~UnitPositionWithTraceTrigger() {}

	virtual bool check();
	void reload();

private:
	Unit* m_unitA;
	Unit* m_unitB;
	TileRegion m_region;
	bool m_lastCheckResult;
	UnitPositionTriggerType m_type;
};

//////////////////////////////////////////////////////////////////////////

class ReverseTrigger : public Trigger
{
public:
	ReverseTrigger( Trigger* trigger );
	virtual ~ReverseTrigger() {}

	virtual bool check();

private:
	Trigger* m_trigger;
};

//////////////////////////////////////////////////////////////////////////

enum ScreenState { FADING_FINISHED };

class ScreenTrigger : public Trigger
{
public:
	ScreenTrigger( ScreenState ss );
	virtual ~ScreenTrigger() {}

	virtual bool check();

private:
	ScreenState m_ss;
};

//////////////////////////////////////////////////////////////////////////

class NullTrigger : public Trigger
{
public:
	NullTrigger() {}
	virtual ~NullTrigger() {}
	virtual bool check() { return true; }
};

//////////////////////////////////////////////////////////////////////////

class RemoveFlagTrigger : public Trigger
{
public:
	RemoveFlagTrigger( Character* unit );
	virtual ~RemoveFlagTrigger() {}

	virtual bool check();

private:
	Character* m_char;
};