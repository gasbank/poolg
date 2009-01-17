#pragma once
#include "Character.h"
#include "Action.h"

class World;




/**
@brief 게임의 단위 조건에 대해 정의하는 클래스

본 클래스는 게임에서 확인할 수 있는 각종 조건에 대해 정의하는 클래스입니다.
예를 들어 어떤 유닛이 어디에 있는지, 현재 월드에 존재하는 적군 수가 몇인지 등이
확인할 수 있는 조건에 해당합니다.

이러한 조건은 독립적으로 쓰이지 않고 Incident 클래스를 통해서 사용하게 됩니다.
*/
class Trigger
{
public:
	Trigger();
	virtual ~Trigger(void);

	virtual bool check() const = 0;
	virtual void release() {}

	virtual void printDebugInfo() const { printf( "Debug Info not implemented on Trigger class\n" ); }

	/**
	@brief UnitPositionWithTraceTrigger 에서 사용하는 enum
	*/
	enum UnitPositionTriggerType {
		UPTT_ENTER  = 0x001, //< Unit 이 해당 영역에 들어왔을 때 발동
		UPTT_LEAVE  = 0x010, //< Unit 이 해당 영역을 빠져나갔을 때 발동
		UPTT_STAY	= 0x100 //< Unit이 해당 영역에 머물렀을 때 발동
	};
};
SCRIPT_FACTORY( Trigger )

//////////////////////////////////////////////////////////////////////////

/**
@brief Unit 이 특정 위치에 있을 때 발동하는 Trigger
*/
class UnitPositionTrigger : public Trigger
{
public:
	UnitPositionTrigger( Unit* unit, TileRegion* region, UnitPositionTriggerType type );
	virtual ~UnitPositionTrigger();

	virtual bool check() const;
	virtual void printDebugInfo() const;

private:
	Unit* m_unit;
	TileRegion* m_region;
	mutable bool m_lastCheckResult;
	UnitPositionTriggerType m_type;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief Unit 의 HP가 특정 범위에 있을 때 발동하는 Trigger
*/
class CharHpTrigger : public Trigger
{
public:
	CharHpTrigger( Character* unit, int min, int max, bool bInclude = true );
	virtual ~CharHpTrigger() {}

	virtual bool check() const;

private:
	Character* m_char;
	int m_min;
	int m_max;
	bool m_bInclude;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief 현재 World 의 적군이 모두 죽었는지 확인하는 Trigger
*/
class TotalAnnihilationTrigger : public Trigger
{
public:
	TotalAnnihilationTrigger( World* );
	virtual ~TotalAnnihilationTrigger();
	virtual bool check() const;

private:
	UnitSet* m_pUs;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief 현재 World 의 적군이 하나라도 남았는지 확인하는 Trigger
*/
class UnTotalAnnihilationTrigger : public Trigger
{
public:
	UnTotalAnnihilationTrigger( World* );
	virtual ~UnTotalAnnihilationTrigger();
	virtual bool check() const;

private:
	UnitSet* m_pUs;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief Incident 가 단 한번이라도 수행된 적이 있는지 확인하는 Trigger

해당 Incident에 포함된 Trigger 가 모두 만족된 적이 한번 이라도 있다면
본 Trigger 가 발동됩니다.
*/
class IncidentTrigger : public Trigger
{
public:
	IncidentTrigger( Incident* incident );
	virtual ~IncidentTrigger();
	virtual bool check() const;

private:
	Incident* m_incident;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief 움직이는 Unit 근처에 갔을 때 발동하는 Trigger

*/
class UnitPositionWithTraceTrigger : public Trigger
{
public:
	/**
	@param unitA 접근하는 Unit
	@param unitB 움직이는 Unit
	@param type 타입
	*/
	UnitPositionWithTraceTrigger( Unit* unitA, Unit* unitB, UnitPositionTriggerType type );
	virtual ~UnitPositionWithTraceTrigger() {}

	virtual bool check() const;

private:
	Unit* m_unitA;
	Unit* m_unitB;
	mutable bool m_lastCheckResult;
	UnitPositionTriggerType m_type;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief Trigger 의 발동과는 반대로 발동하는 Trigger
*/
class ReverseTrigger : public Trigger
{
public:
	ReverseTrigger( Trigger* trigger );
	virtual ~ReverseTrigger() {}

	virtual bool check() const;

private:
	Trigger* m_trigger;
};

//////////////////////////////////////////////////////////////////////////

enum ScreenState { FADING_FINISHED };

/**
@brief 화면 상태에 대한 Trigger

현재 지원되는 조건은 '페이드 인 종료됨' 뿐이다.
*/
class ScreenTrigger : public Trigger
{
public:
	ScreenTrigger( ScreenState ss );
	virtual ~ScreenTrigger() {}

	virtual bool check() const;

private:
	ScreenState m_ss;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief 무조건 발동되는 Trigger

어떤 Action 을 무조건 발동시키게 하는 Incident 를 만들고 싶을 때
사용하는 Trigger 이다. Action 을 독립적으로 사용할 수는 없기 때문에
이러한 Trigger 가 필요하다.
*/
class NullTrigger : public Trigger
{
public:
	NullTrigger() {}
	virtual ~NullTrigger() {}
	virtual bool check() const { return true; }
};

//////////////////////////////////////////////////////////////////////////

/**
@brief Character 의 Remove Flag가 켜지면 발동되는 Trigger

Remove Flag는 캐릭터가 죽거나 다른 어떤 이유때문에 화면에 더이상
렌더링 되면 안되는 경우에 설정된다. 이 값을 확인하여 참이면
본 Trigger 가 발동된다.
*/
class RemoveFlagTrigger : public Trigger
{
public:
	RemoveFlagTrigger( Character* unit );
	virtual ~RemoveFlagTrigger() {}

	virtual bool check() const;

private:
	Character* m_char;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief 적이 일정 수에 도달하면 발동되는 Trigger
*/
class EnemyCountTrigger : public Trigger
{
public:
	EnemyCountTrigger( World* pWorld, int _numOfEnemyToTrigger );
	virtual ~EnemyCountTrigger() {}

	virtual bool check() const;

private:
	UnitSet* m_pUnitSet;
	int m_numOfEnemyToTrigger;
};

//////////////////////////////////////////////////////////////////////////