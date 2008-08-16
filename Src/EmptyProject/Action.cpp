#include "EmptyProjectPCH.h"
#include "Action.h"
#include "TopStateManager.h"
#include "WorldState.h"
#include "WorldStateManager.h"
#include "Enemy.h"
#include "Unit.h"
#include "ScriptManager.h"


Action::Action(void)
{
	TopStateManager& m_tsm = TopStateManager::getSingleton();
	m_ws = reinterpret_cast<WorldState*>( m_tsm.getCurState() );
}

Action::~Action(void)
{
}

void Action::dialogAction( const char* dialogName )
{
	m_ws->startDialog( dialogName );
}

void Action::battleAction( Enemy* oppCharacter )
{
	// Set current enemy unit and enter into BattleState
	m_ws->setCurEnemy( (Character*)oppCharacter );

	m_ws->getCurEnemyUnit()->setAttack(30);

	if ( GetWorldStateManager().curStateEnum() == GAME_WORLD_STATE_FIELD )
		GetWorldStateManager().setNextState( GAME_WORLD_STATE_BATTLE );
}

void Action::soundAction( std::string sz )
{
	if ( sz == "Start Battle" )
	{
		GetAudioState().bBGMFade = true;
		GetAudioState().bMusicFade = false;
		GetAudioState().pEngine->Stop( GetAudioState().iMusicCategory, 0 );
		GetAudioState().pSoundBank->Play( GetAudioState().iBattle, 0, 0, NULL );
	}
	else if ( sz == "End Battle" )
	{
		GetAudioState().bBGMFade = false;
		GetAudioState().bMusicFade = true;
	}
}

void Action::createUnitAction( int x, int y, bool random )
{
	static int i = 0;
	if ( i == 0 )
		m_ws->addUnit( EpCreateEnemy( x, y ) );
	i++;
}

//////////////////////////////////////////////////////////////////////////

BattleAction::BattleAction( const Unit* targetUnit, float dist )
: m_targetUnit( targetUnit ), m_dist( dist )
{

}
BattleAction::~BattleAction( void )
{

}
void BattleAction::activate()
{
	// Do one-time init of this action
}

void BattleAction::update()
{

}

//////////////////////////////////////////////////////////////////////////

DialogAction::DialogAction( const char* dialogName )
: m_dialogName( dialogName )
{

}

DialogAction::~DialogAction( void )
{

}

void DialogAction::activate()
{
	assert ( getWs()->getCurDialog() == 0 );
	getWs()->startDialog( m_dialogName.c_str() );
}

void DialogAction::update()
{

}
Action* EpCreateDialogAction( const char* dialogName )
{
	return new DialogAction( dialogName );
} SCRIPT_CALLABLE_PV_PC( EpCreateDialogAction )

//////////////////////////////////////////////////////////////////////////

SoundAction::SoundAction( const char* soundName )
: m_soundName( soundName )
{
}

SoundAction::~SoundAction( void )
{
}

void SoundAction::activate()
{
	if ( m_soundName == "Start Battle" )
	{
		GetAudioState().bBGMFade = true;
		GetAudioState().bMusicFade = false;
		GetAudioState().pEngine->Stop( GetAudioState().iMusicCategory, 0 );
		GetAudioState().pSoundBank->Play( GetAudioState().iBattle, 0, 0, NULL );
	}
	else if ( m_soundName == "End Battle" )
	{
		GetAudioState().bBGMFade = false;
		GetAudioState().bMusicFade = true;
	}
}

void SoundAction::update()
{

}
Action* EpCreateSoundAction( const char* soundName )
{
	return new SoundAction( soundName );
}  SCRIPT_CALLABLE_PV_PC( EpCreateSoundAction )

//////////////////////////////////////////////////////////////////////////

HealAction::HealAction( Character* targetChar )
: m_targetChar ( targetChar )
{
}

HealAction::~HealAction()
{
}

void HealAction::activate()
{
	m_targetChar->heal( 9999 );
}

void HealAction::update()
{
}

Action* EpCreateHealAction( void* targetChar )
{
	Character* c = reinterpret_cast<Character*>( targetChar );
	return new HealAction( c );
}  SCRIPT_CALLABLE_PV_PV( EpCreateHealAction )

//////////////////////////////////////////////////////////////////////////

UnitSpawnAction::UnitSpawnAction( Unit* createUnit )
: m_createUnit ( createUnit )
{
}

UnitSpawnAction::~UnitSpawnAction()
{
}

void UnitSpawnAction::activate()
{
	getWs()->addUnit( m_createUnit );
}

void UnitSpawnAction::update()
{

}

Action* EpCreateUnitSpawnAction( void* createUnit )
{
	Unit* u = reinterpret_cast<Unit*>( createUnit );
	return new UnitSpawnAction( u );
}  SCRIPT_CALLABLE_PV_PV( EpCreateUnitSpawnAction )

//////////////////////////////////////////////////////////////////////////

START_SCRIPT_FACTORY( Action )
	CREATE_OBJ_COMMAND( EpCreateDialogAction )
	CREATE_OBJ_COMMAND( EpCreateSoundAction )
	CREATE_OBJ_COMMAND( EpCreateHealAction )
	CREATE_OBJ_COMMAND( EpCreateUnitSpawnAction ) 
END_SCRIPT_FACTORY( Action )