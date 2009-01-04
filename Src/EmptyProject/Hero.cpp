#include "EmptyProjectPCH.h"
#include "Hero.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "World.h"
#include "TopStateManager.h"
#include "PlayState.h"
#include "ArnMesh.h"
#include "Skill.h"
#include "SkillSet.h"
#include "Sound.h"
#include "SkillManager.h"
#include "EpUser.h"

extern NetworkIDManager					g_networkIDManager;
extern RakNet::ReplicaManager2*			g_replicaManager;

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
	int a = 10;
	a = a;
}

Unit* Hero::createHero( LPD3DXMESH mesh, int tileX, int tileY, float posZ )
{
	Hero* u = new Hero();
	u->setTilePos( tileX, tileY );
	u->setTileBufferPos( tileX, tileY );
	u->setArnMeshName( "PoolGModel" );

	// Acquire the most basic skill to our hero!
	//u->addToSkillSet( SL_FIRST );
	u->memorizeSkill( GetSKillManager().getSkill( "NormalAttack" ) );
	u->equipSkill( 0, GetSKillManager().getSkill( "NormalAttack" ) );

	u->memorizeSkill( GetSKillManager().getSkill( "TestSkill" ) );
	u->equipSkill( 1, GetSKillManager().getSkill( "TestSkill" ) );

	// RakNet related

	/*NetworkID nid;
	nid.localSystemAddress = 100;
	u->SetNetworkIDManager( &g_networkIDManager );
	u->SetNetworkID( nid );*/

	if ( g_replicaManager )
	{
		// Spawn a new unit
		UnitBase::myUnit = u;
		// In order to use any networked member functions of Replica2, you must first call SetReplicaManager
		UnitBase::myUnit->SetReplicaManager( g_replicaManager );
		// Tell the system to automatically serialize our data members every 100 milliseconds (if changed)
		// This way if we change the unit's name we don't have to call unit::myUnit->BroadcastSerialize();
		UnitBase::myUnit->AddAutoSerializeTimer( 100 );
		// Send out this new user to all systems. Unlike the old system (ReplicaManager) all sends are done immediately.
		UnitBase::myUnit->BroadcastConstruction();
	}
	
	return u;
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


bool Hero::frameMove( double dTime, float fElapsedTime )
{
	//printf( "Tile X, Y : %d, %d \n", getTilePosX(), getTilePosY() );
	return Character::frameMove( dTime, fElapsedTime );
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

//int EpAddSkillToHero( int skillNo )
//{
//	Hero* hero = (Hero*)GetWorldManager().getCurWorld()->getHeroUnit();
//	hero->addToSkillSet( (SkillLocation)skillNo );
//	return 0;
//} SCRIPT_CALLABLE_I_I( EpAddSkillToHero )
//
//int EpDeleteSkillFromHero( int skillNo )
//{
//	Hero* hero = (Hero*)GetWorldManager().getCurWorld()->getHeroUnit();
//	hero->deleteSkill(  ( SkillLocation )skillNo );
//	return 0;
//} SCRIPT_CALLABLE_I_I( EpDeleteSkillFromHero )

int EpHeroSetEncounterEnemy( int b )
{
	Hero* hero = (Hero*)GetWorldManager().getCurWorld()->getHeroUnit();
	hero->setEncounterEnemy( b?true:false );
	return 0;
} SCRIPT_CALLABLE_I_I( EpHeroSetEncounterEnemy )


START_SCRIPT_FACTORY(Hero)
	CREATE_OBJ_COMMAND( EpCreateHero )
	/*CREATE_OBJ_COMMAND( EpAddSkillToHero )
	CREATE_OBJ_COMMAND( EpDeleteSkillFromHero )*/
	CREATE_OBJ_COMMAND( EpHeroSetEncounterEnemy )
END_SCRIPT_FACTORY(Hero)
