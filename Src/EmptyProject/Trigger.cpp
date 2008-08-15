#include "EmptyProjectPCH.h"
#include "Trigger.h"
#include "TopStateManager.h"
#include "WorldState.h"
#include "Enemy.h"
#include "WorldStateManager.h"
#include "TileManager.h"
#include "Action.h"

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

UnitPositionTrigger::UnitPositionTrigger( Unit* unit, const TileRegion& region )
{
	m_unit		= unit;
	m_region	= region;
}


bool UnitPositionTrigger::check()
{
	return m_region.isExist( m_unit->getTilePos() );
}

