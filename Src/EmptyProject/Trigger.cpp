#include "EmptyProjectPCH.h"
#include "Trigger.h"
#include "TopStateManager.h"
#include "World.h"
#include "Enemy.h"
#include "WorldStateManager.h"
#include "TileManager.h"
#include "Action.h"
#include "ScriptManager.h"
#include "Unit.h"
#include "Utility.h"

Trigger::Trigger(void)
{
}

Trigger::~Trigger(void)
{
}

//////////////////////////////////////////////////////////////////////////

UnitPositionTrigger::UnitPositionTrigger( Unit* unit, TileRegion* region, UnitPositionTriggerType type )
{
	m_unit		= unit;
	m_region	= region;
	m_type		= type;
}


bool UnitPositionTrigger::check()
{
	bool checkResult = m_region->isExist( m_unit->getTilePos() );
	bool retValue = false;

	if ( m_type & UPTT_ENTER )
	{
		if ( !m_lastCheckResult && checkResult )
			retValue =  true;
	}
	if ( m_type & UPTT_LEAVE )
	{
		if ( m_lastCheckResult && !checkResult )
			retValue =  true;
	}
	if ( m_type & UPTT_STAY)
	{
		if ( m_lastCheckResult && checkResult )
			retValue =  true;
	}
	
	m_lastCheckResult = checkResult;
	return retValue;
}

void UnitPositionTrigger::printDebugInfo() const
{
	printf( "UnitPositionTrigger:\n" );
	m_unit->printDebugInfo();
	printf( "lastcheck=%d trigType=0x%x region=", m_lastCheckResult, m_type ); Utility::printValue( *m_region );
	printf( "\n" );
}
Trigger* EpCreateUnitPositionTrigger( void* unit, int x0, int y0, int x1, int y1, int type )
{
	Unit* u = reinterpret_cast<Character*>( unit );
	Point2Uint start;
	Point2Uint end;
	start.x = x0;
	start.y = y0;
	end.x = ( x1 == -1 ) ? x0 : x1;
	end.y = ( y1 == -1 ) ? y0 : y1;
	TileRegion* region = new TileRegion( start, end );
	return new UnitPositionTrigger( u, region, (UnitPositionTriggerType)type );
} SCRIPT_CALLABLE_PV_PV_I_I_I_I_I( EpCreateUnitPositionTrigger )


//////////////////////////////////////////////////////////////////////////

CharHpTrigger::CharHpTrigger( Character* c, int min, int max, bool bInclude /*= true */ )
: m_char( c ), m_min( min ), m_max( max ), m_bInclude( bInclude )
{
}

bool CharHpTrigger::check()
{
	if ( m_bInclude && m_char->getCurHp() >= m_min && m_char->getCurHp() <= m_max )
	{
		// HP is within the range
		return true;
	}
	else if ( !m_bInclude && m_char->getCurHp() < m_min && m_char->getCurHp() > m_max )
	{
		// HP is not within the range
		return true;
	}

	return false;
}

Trigger* EpCreateCharHpTrigger( void* ptr, int min, int max, int bInclude )
{
	Character* c = reinterpret_cast<Character*>( ptr );
	return new CharHpTrigger( c, min, max, bInclude?true:false );
} SCRIPT_CALLABLE_PV_PV_I_I_I( EpCreateCharHpTrigger )


//////////////////////////////////////////////////////////////////////////

TotalAnnihilationTrigger::TotalAnnihilationTrigger( World* pWs )
{
	m_pUs = pWs->getUnitSet();
}

TotalAnnihilationTrigger::~TotalAnnihilationTrigger()
{

}

bool TotalAnnihilationTrigger::check()
{
	int i = 0;
	// 말을 못하는 Enemy, 즉 죽일 수 있는 적이 남은 경우 TotalAnnihilation 실패로 간주.
	for ( UnitSet::iterator it = m_pUs->begin(); it != m_pUs->end(); it++ )
	{
		if ( (*it)->getType() == UT_ENEMY )
			if ( !static_cast<Enemy*>(*it)->isTalkable() && !static_cast<Enemy*>(*it)->getRemoveFlag() )
				return false;
	}

	return true;
}

Trigger* EpCreateTotalAnnihilationTrigger( void* pWorld )
{
	return new TotalAnnihilationTrigger( reinterpret_cast<World*>(pWorld) );
} SCRIPT_CALLABLE_PV_PV( EpCreateTotalAnnihilationTrigger )

//////////////////////////////////////////////////////////////////////////

UnTotalAnnihilationTrigger::UnTotalAnnihilationTrigger( World* pWs )
{
	m_pUs = pWs->getUnitSet();
}

UnTotalAnnihilationTrigger::~UnTotalAnnihilationTrigger()
{

}

bool UnTotalAnnihilationTrigger::check()
{
	int i = 0;
	// 말을 못하는 Enemy, 즉 죽일 수 있는 적이 남은 경우 TotalAnnihilation 실패로 간주.
	for ( UnitSet::iterator it = m_pUs->begin(); it != m_pUs->end(); it++ )
	{
		if ( (*it)->getType() == UT_ENEMY )
			if ( !static_cast<Enemy*>(*it)->isTalkable() && !static_cast<Enemy*>(*it)->getRemoveFlag() )
				return true;
	}

	return false;
}

Trigger* EpCreateUnTotalAnnihilationTrigger( void* pWorld )
{
	return new UnTotalAnnihilationTrigger( reinterpret_cast<World*>( pWorld ) );
} SCRIPT_CALLABLE_PV_PV( EpCreateUnTotalAnnihilationTrigger )

//////////////////////////////////////////////////////////////////////////

IncidentTrigger::IncidentTrigger( Incident* incident )
: m_incident( incident )
{
}

IncidentTrigger::~IncidentTrigger()
{
}

bool IncidentTrigger::check()
{
	if ( m_incident->getLeastOnetime() )
		return true;
	else
		return false;
}

Trigger* EpCreateIncidentTrigger( void* incident )
{
	return new IncidentTrigger( reinterpret_cast<Incident*>( incident ) );
} SCRIPT_CALLABLE_PV_PV( EpCreateIncidentTrigger )

//////////////////////////////////////////////////////////////////////////

UnitPositionWithTraceTrigger::UnitPositionWithTraceTrigger( Unit* unitA, Unit* unitB, UnitPositionTriggerType type )
{
	m_unitA		= unitA;
	m_unitB		= unitB;
	m_type		= type;

	reload();
}


bool UnitPositionWithTraceTrigger::check()
{
	reload();

	bool checkResult = m_region.isExist( m_unitA->getTilePos() );
	bool retValue = false;

	if ( m_type & UPTT_ENTER )
	{
		if ( !m_lastCheckResult && checkResult )
			retValue =  true;
	}
	if ( m_type & UPTT_LEAVE )
	{
		if ( m_lastCheckResult && !checkResult )
			retValue =  true;
	}
	if ( m_type & UPTT_STAY)
	{
		if ( m_lastCheckResult && checkResult )
			retValue =  true;
	}

	m_lastCheckResult = checkResult;
	return retValue;
}

void UnitPositionWithTraceTrigger::reload()
{
	Point2Uint start, end;
	
	start.x = m_unitB->getTilePos().x - 1;
	start.y = m_unitB->getTilePos().y - 1;
	end.x = m_unitB->getTilePos().x + 1;
	end.y = m_unitB->getTilePos().y + 1;
	
	m_region.setStart( start );
	m_region.setEnd( end );
}

Trigger* EpCreateUnitPositionWithTraceTrigger( void* unitA, void* unitB, int type )
{
	Unit* uA = reinterpret_cast<Character*>( unitA );
	Unit* uB = reinterpret_cast<Character*>( unitB );
	
	return new UnitPositionWithTraceTrigger( uA, uB, (UnitPositionTriggerType)type );
} SCRIPT_CALLABLE_PV_PV_PV_I( EpCreateUnitPositionWithTraceTrigger )

//////////////////////////////////////////////////////////////////////////

START_SCRIPT_FACTORY( Trigger )
	CREATE_OBJ_COMMAND( EpCreateCharHpTrigger )
	CREATE_OBJ_COMMAND( EpCreateUnitPositionTrigger )
	CREATE_OBJ_COMMAND( EpCreateTotalAnnihilationTrigger )
	CREATE_OBJ_COMMAND( EpCreateUnTotalAnnihilationTrigger )
	CREATE_OBJ_COMMAND( EpCreateIncidentTrigger )
	CREATE_OBJ_COMMAND( EpCreateUnitPositionWithTraceTrigger )
END_SCRIPT_FACTORY( Trigger )

