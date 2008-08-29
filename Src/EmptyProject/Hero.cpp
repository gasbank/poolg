#include "EmptyProjectPCH.h"
#include "Hero.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "World.h"
#include "TopStateManager.h"
#include "PlayState.h"
#include "ArnMesh.h"
#include "Skill.h"
#include "Sound.h"

Hero::Hero(void)
: Character( UT_HERO )
{
	m_curExp = 0;
	m_maxExp = 6;
	m_expGap = 3;
	m_level = 1;
	m_bEncounterEnemy = true;
}

Hero::~Hero(void)
{
}

Unit* Hero::createHero( LPD3DXMESH mesh, int tileX, int tileY, float posZ )
{
	Hero* u = new Hero();
	u->init();
	u->setTilePos( tileX, tileY );
	u->setTileBufferPos( tileX, tileY );

	// As default, hero's model is PoolG
	PlayState* ps = static_cast<PlayState*>(GetTopStateManager().getState( GAME_TOP_STATE_PLAY ));
	ArnSceneGraph* charSceneGraph = ps->getCharacterSceneGraph();
	//ArnMesh* arnMesh = static_cast<ArnMesh*>(charSceneGraph->getSceneRoot()->getNodeByName( "PoolGModel" ));
	u->setArnMeshName( "PoolGModel" );
	
	u->addToSkillSet( SL_FIRST );

	return u;
}

void Hero::addToSkillSet( SkillLocation sl )
{
	SkillSet* skillSet = getSkillSet();

	switch ( sl )
	{
	case SL_FIRST: skillSet->setSkill (SL_FIRST, (Skill*) new NormalAttack()); break;
	case SL_SECOND: skillSet->setSkill (SL_SECOND, (Skill*) new Heal()); break;
	case SL_THIRD: skillSet->setSkill (SL_THIRD, (Skill*) new Goto()); break;
	case SL_FOURTH: skillSet->setSkill (SL_FOURTH, (Skill*) new MultiThread());; break;
	case SL_FIFTH: skillSet->setSkill (SL_FIFTH, (Skill*) new Meditation()); break;
	}
}

void Hero::levelUp()
{
	m_curExp = 0;
	m_maxExp += m_expGap;
	m_level++;
}

int Hero::gainExp( int expPoint )
{

	//초과할 경우
	if ( m_maxExp < m_curExp + expPoint ) 
	{
		int retExp = m_curExp + expPoint - m_maxExp;
		m_curExp = m_maxExp;
		return retExp;
	}
	//딱 맞을 경우
	else if (m_maxExp == m_curExp + expPoint)
	{
		m_curExp = m_maxExp;
		return -1;
	}
	//부족할 경우
	else
	{
		m_curExp += expPoint;
		return 0;
	}
}


bool Hero::frameMove( float fElapsedTime )
{
	//printf( "Tile X, Y : %d, %d \n", getTilePosX(), getTilePosY() );
	return Character::frameMove( fElapsedTime );
}

LRESULT Hero::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( wParam )
	{
	case 'W' :
	case 'A' :
	case 'S' :
	case 'D' :
		// TODO Why check this assert?
		//assert( GetAudioState().pZeroLatencyWalkCue );

		if ( GetAudioState().pZeroLatencyWalkCue )
		{
			DWORD dwState;
			GetAudioState().pZeroLatencyWalkCue->GetState( &dwState );
			if( ( dwState & ( XACT_CUESTATE_PREPARING | XACT_CUESTATE_PREPARED ) ) != 0 )
				GetAudioState().pZeroLatencyWalkCue->Play();
		}
		break;
	default :
		break;
	}
	return Character::handleMessages( hWnd, uMsg, wParam, lParam );
}

///////////////////////////////////////////////////////////////////////////////////////

Unit* EpCreateHero( Tcl_Obj* tilePos )
{
	int tileX = 0, tileY = 0;
	GetScriptManager().readTwoIntObj( tilePos, tileX, tileY );
	return Hero::createHero( 0, tileX, tileY, 0 );

} SCRIPT_CALLABLE_PV_OBJ( EpCreateHero )

int EpAddSkillToHero( int skillNo )
{
	Hero* hero = (Hero*)GetWorldManager().getCurWorld()->getHeroUnit();
	hero->addToSkillSet( (SkillLocation)skillNo );
	return 0;
} SCRIPT_CALLABLE_I_I( EpAddSkillToHero )

int EpDeleteSkillFromHero( int skillNo )
{
	Hero* hero = (Hero*)GetWorldManager().getCurWorld()->getHeroUnit();
	hero->getSkillSet()->deleteSkill(  ( SkillLocation )skillNo );
	return 0;
} SCRIPT_CALLABLE_I_I( EpDeleteSkillFromHero )

int EpHeroSetEncounterEnemy( int b )
{
	Hero* hero = (Hero*)GetWorldManager().getCurWorld()->getHeroUnit();
	hero->setEncounterEnemy( b?true:false );
	return 0;
} SCRIPT_CALLABLE_I_I( EpHeroSetEncounterEnemy )


START_SCRIPT_FACTORY(Hero)
	CREATE_OBJ_COMMAND( EpCreateHero )
	CREATE_OBJ_COMMAND( EpAddSkillToHero )
	CREATE_OBJ_COMMAND( EpDeleteSkillFromHero )
	CREATE_OBJ_COMMAND( EpHeroSetEncounterEnemy )
END_SCRIPT_FACTORY(Hero)
