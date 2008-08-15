#include "EmptyProjectPCH.h"
#include "Action.h"
#include "TopStateManager.h"
#include "WorldState.h"
#include "WorldStateManager.h"
#include "Enemy.h"

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
		m_ws->addUnit (EpCreateEnemy( x, y ) );
	i++;
}