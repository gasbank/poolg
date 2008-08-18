#include "EmptyProjectPCH.h"
#include "Action.h"
#include "WorldManager.h"
#include "World.h"
#include "WorldStateManager.h"
#include "Enemy.h"
#include "Unit.h"
#include "ScriptManager.h"
#include "Sound.h"
#include "Unit.h"


Action::Action(void)
{
}

Action::~Action(void)
{
}

World* Action::getCurWorld() const
{
	return GetWorldManager().getCurWorld();
}

void Action::dialogAction( const char* dialogName )
{
	getCurWorld()->startDialog( dialogName );
}

void Action::battleAction( Enemy* oppCharacter )
{
	// Set current enemy unit and enter into BattleState
	getCurWorld()->setCurEnemy( (Character*)oppCharacter );

	getCurWorld()->getCurEnemyUnit()->setAttack(30);

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
		getCurWorld()->addUnit( EpCreateEnemy( x, y ) );
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
	assert ( GetWorldManager().getCurWorld()->getCurDialog() == 0 );
	GetWorldManager().getCurWorld()->startDialog( m_dialogName.c_str() );
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
	EP_SAFE_RELEASE( m_createUnit );
}

void UnitSpawnAction::activate()
{
	getCurWorld()->addUnit( m_createUnit );
	m_createUnit = 0; // Unit instance ownership moved to the world!
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



Action* EpCreateAction( ActionType at, ... )
{
	Action* ret = 0;
	va_list params;
	int count = 0;
	
	switch (at)
	{
	case AT_BATTLE:
		count = 2;
		va_start( params, count );
		ret = new BattleAction(
			va_arg( params, Unit*),
			va_arg( params, float) );
		break;
	}
	return ret;
}


void ScriptAction::activate()
{
	GetScriptManager().execute( m_scriptCommand.c_str() );
}
void ScriptAction::update()
{
}

Action* EpCreateScriptAction( const char* scriptCommand )
{
	ScriptAction* sa = new ScriptAction();
	sa->setScriptCommand( scriptCommand );
	return sa;
}  SCRIPT_CALLABLE_PV_PC( EpCreateScriptAction )

//////////////////////////////////////////////////////////////////////////

UnitMoveAction::UnitMoveAction( Unit* targetUnit, std::string input )
: m_targetUnit ( targetUnit ), m_input ( input )
{
}

UnitMoveAction::~UnitMoveAction()
{
}

void UnitMoveAction::activate()
{
	int i = 4;

	if ( m_input == "LEFT" )
		i = 2;
	else if ( m_input == "RIGHT" )
		i = 3;
	else if ( m_input == "UP" )
		i = 0;
	else if ( m_input == "DOWN" )
		i = 1;

	m_targetUnit->setForcedMove( i );

}

void UnitMoveAction::update()
{
}

Action* EpCreateUnitMoveAction( void* targetUnit, const char* input )
{
	Unit* u = reinterpret_cast<Unit*>( targetUnit );
	return new UnitMoveAction( u , input );
} SCRIPT_CALLABLE_PV_PV_PC( EpCreateUnitMoveAction )

//////////////////////////////////////////////////////////////////////////

START_SCRIPT_FACTORY( Action )
	CREATE_OBJ_COMMAND( EpCreateDialogAction )
	CREATE_OBJ_COMMAND( EpCreateSoundAction )
	CREATE_OBJ_COMMAND( EpCreateHealAction )
	CREATE_OBJ_COMMAND( EpCreateUnitSpawnAction )
	CREATE_OBJ_COMMAND( EpCreateScriptAction )
	CREATE_OBJ_COMMAND( EpCreateUnitMoveAction )
END_SCRIPT_FACTORY( Action )