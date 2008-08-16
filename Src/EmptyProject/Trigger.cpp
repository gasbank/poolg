#include "EmptyProjectPCH.h"
#include "Trigger.h"
#include "TopStateManager.h"
#include "WorldState.h"
#include "Enemy.h"
#include "WorldStateManager.h"
#include "TileManager.h"
#include "Action.h"
#include "ScriptManager.h"
#include "Unit.h"

Trigger::Trigger(void)
{
	TopStateManager& tsm = TopStateManager::getSingleton();
	m_ws = reinterpret_cast<WorldState*>( tsm.getCurState() );
}

Trigger::~Trigger(void)
{
}

void Trigger::positionTrigger()
{
	detectBattleAction();
	detectHealAction();
	detectTalkAction();
	detectCreateAction();
}

void Trigger::detectBattleAction()
{
	// Detect battle event.
	// If current selected unit isn't hero unit, and isn't talkable,
	// regard as enemy. And if hero is in the fight area of enemy, start battle.
	UnitSet::iterator it = m_ws->getUnitSet()->begin();
	for ( ; it != m_ws->getUnitSet()->end(); ++it )
	{
		if ( (*it) != m_ws->getHeroUnit() )
		{
			Enemy* oppCharacter = dynamic_cast<Enemy*>( *it );
			if ( oppCharacter->isTalkable() == false && oppCharacter != NULL )
			{
				if ( m_ws->isInFightArea( m_ws->getHeroUnit() , oppCharacter ) == true )
				{
					//m_action.battleAction( oppCharacter );
				}
			}
		}	
	}
}

void Trigger::detectTalkAction()
{
/*
	if ( GetTileManager().getTile( GetTileManager().pos2TileX( &m_ws->getHeroPos() ) , GetTileManager().pos2TileY( &m_ws->getHeroPos() ) )->b_eventTalk )
		m_action.dialogAction( "EpDialog1" );*/
}

void Trigger::detectHealAction()
{
	if ( GetTileManager().getTile( GetTileManager().pos2TileX( &m_ws->getHeroPos() ) , GetTileManager().pos2TileY( &m_ws->getHeroPos() ) )->b_heal )
	{
		m_ws->getHeroUnit()->heal( 9999 );
	}
}

void Trigger::detectCreateAction()
{
	//if ( GetTileManager().getTile( GetTileManager().pos2TileX( &m_ws->getHeroPos() ) , GetTileManager().pos2TileY( &m_ws->getHeroPos() ) )->b_createEnemy )
	//	m_action.createUnitAction( 34, 57, 0 );
}


//////////////////////////////////////////////////////////////////////////

UnitPositionTrigger::UnitPositionTrigger( Unit* unit, TileRegion* region )
{
	m_unit		= unit;
	m_region	= region;
}


bool UnitPositionTrigger::check()
{
	return m_region->isExist( m_unit->getTilePos() );
}

Trigger* EpCreateUnitPositionTrigger( void* unit, int x0, int y0, int x1, int y1 )
{
	Unit* u = reinterpret_cast<Character*>( unit );
	Point2Uint start;
	Point2Uint end;
	start.x = x0;
	start.y = y0;
	end.x = x1;
	end.y = y1;
	TileRegion* region = new TileRegion( start, end );
	return new UnitPositionTrigger( u, region );
} SCRIPT_CALLABLE_PV_PV_I_I_I_I( EpCreateUnitPositionTrigger )


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

//////////////////////////////////////////////////////////////////////////

TotalAnnihilationTrigger::TotalAnnihilationTrigger( WorldState* pWs )
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
	return new TotalAnnihilationTrigger( reinterpret_cast<WorldState*>(pWorld) );
} SCRIPT_CALLABLE_PV_PV( EpCreateTotalAnnihilationTrigger )

//////////////////////////////////////////////////////////////////////////

UnTotalAnnihilationTrigger::UnTotalAnnihilationTrigger( WorldState* pWs )
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
	return new UnTotalAnnihilationTrigger( reinterpret_cast<WorldState*>(pWorld) );
} SCRIPT_CALLABLE_PV_PV( EpCreateUnTotalAnnihilationTrigger )

//////////////////////////////////////////////////////////////////////////

Trigger* EpCreateCharHpTrigger( void* ptr, int min, int max, int bInclude )
{
	Character* c = reinterpret_cast<Character*>( ptr );
	return new CharHpTrigger( c, min, max, bInclude?true:false );
} SCRIPT_CALLABLE_PV_PV_I_I_I( EpCreateCharHpTrigger )


START_SCRIPT_FACTORY( Trigger )
	CREATE_OBJ_COMMAND( EpCreateCharHpTrigger )
	CREATE_OBJ_COMMAND( EpCreateUnitPositionTrigger )
	CREATE_OBJ_COMMAND( EpCreateTotalAnnihilationTrigger )
	CREATE_OBJ_COMMAND( EpCreateUnTotalAnnihilationTrigger )
END_SCRIPT_FACTORY( Trigger )
