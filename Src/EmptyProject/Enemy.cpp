#include "EmptyProjectPCH.h"
#include "Enemy.h"
#include "WorldStateManager.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "TopStateManager.h"
#include "PlayState.h"
#include "ArnSceneGraph.h"
#include "ArnMesh.h"

Enemy::Enemy()
: Character( UT_ENEMY )
{
	m_bRandomWalkable	= false;
	m_bTalkable			= false;
	m_iFightRange		= 1;
}

Enemy::~Enemy(void)
{
}

Unit* Enemy::createEnemy( LPD3DXMESH mesh, int tileX, int tileY, float posZ )
{
	Enemy* u = new Enemy();
	u->init( GetG().m_dev, mesh );
	u->setTilePos( tileX, tileY );
	u->setTileBufferPos( tileX, tileY );
		
	// As default, enemy's model is GwengYi
	PlayState* ps = static_cast<PlayState*>(GetTopStateManager().getState( GAME_TOP_STATE_PLAY ));
	ArnSceneGraph* charSceneGraph = ps->getCharacterSceneGraph();
	ArnMesh* arnMesh = static_cast<ArnMesh*>(charSceneGraph->getSceneRoot()->getNodeByName( "GwengYiModel" ));
	u->setArnMesh( arnMesh );

	return u;
}

HRESULT Enemy::frameRender()
{
	return Character::frameRender();
}

bool Enemy::frameMove( float fElapsedTime )
{
	WorldStateManager& rWsm = GetWorldStateManager();

	/*if ( m_bRandomWalkable && rWsm.curStateEnum() == GAME_WORLD_STATE_FIELD )
		walkRandomly();*/

	return Character::frameMove( fElapsedTime );
}

LRESULT Enemy::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return Character::handleMessages( hWnd, uMsg, wParam, lParam );
}

void Enemy::walkRandomly()
{
	// Move per minute
	float movePerMin = 0.5;

	int randomNumber = rand() % ((int)(60 / movePerMin) * 4);

	// If random number is 0~3, move.

	if ( 0 <= randomNumber && randomNumber <= 3 )
	{
		// Map key virtually by random number
		UnitInput mappedKey = (UnitInput)randomNumber ;
		if( mappedKey != UNIT_UNKNOWN )
		{
			if( FALSE == IsKeyDown( m_aKeys[mappedKey] ) )
			{
				m_aKeys[ mappedKey ] = KEY_WAS_DOWN_MASK | KEY_IS_DOWN_MASK;
				++m_cKeysDown;
			}
		}
	}
	else
	{
		UINT i;
		// Pull virtually pushed key at 33% probability
		if ( (rand() / (float)RAND_MAX) < 0.1f )
		{
			for ( i = 0; i < UNIT_MAX_KEYS; i++ )
			{
				if( IsKeyDown( m_aKeys[ (UnitInput)i ] ) )
				{

					UnitInput mappedKey = (UnitInput)i;
					if( mappedKey != UNIT_UNKNOWN && ( DWORD )mappedKey < 8 )
					{
						m_aKeys[ mappedKey ] &= ~KEY_IS_DOWN_MASK;
						--m_cKeysDown;
					}
				}
			}
		}
	}
}

Unit* EpCreateEnemy( int tileX, int tileY )
{
	LPD3DXMESH d3dxMesh;
	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
	return Enemy::createEnemy( 
		d3dxMesh, 
		tileX, 
		tileY, 
		0 );

} SCRIPT_CALLABLE_PV_I_I( EpCreateEnemy )

int EpEnemySetRandomWalkable( void* ptr, int randomWalkable )
{
	Enemy* instance = reinterpret_cast<Enemy*>( ptr );
	instance->Enemy::setRandomWalkable( randomWalkable?true:false );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpEnemySetRandomWalkable )

int EpEnemySetTalkable( void* ptr, int talkable )
{
	Enemy* instance = reinterpret_cast<Enemy*>( ptr );
	instance->Enemy::setTalkable( talkable?true:false );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpEnemySetTalkable )

int EpEnemySetFightRange( void* ptr, int fightRange )
{
	Enemy* instance = reinterpret_cast<Enemy*>( ptr );
	instance->Enemy::setFightRange( fightRange );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpEnemySetFightRange )

START_SCRIPT_FACTORY(Enemy)
	CREATE_OBJ_COMMAND( EpCreateEnemy )
	CREATE_OBJ_COMMAND( EpEnemySetRandomWalkable )
	CREATE_OBJ_COMMAND( EpEnemySetTalkable )
	CREATE_OBJ_COMMAND(	EpEnemySetFightRange )
END_SCRIPT_FACTORY(Enemy)