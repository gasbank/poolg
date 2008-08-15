#include "EmptyProjectPCH.h"
#include "Character.h"
#include "AttackObject.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "TopStateManager.h"
#include "WorldStateManager.h"
#include "WorldState.h"
#include "ArnMesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Trigger.h"


extern TileManager tileManager;


void Character::attack( int type, Character* enemy )
{
	D3DXVECTOR3 fireDir = enemy->getPos() - getPos();
	float dist = D3DXVec3Length( &fireDir );

	D3DXVec3Normalize( &fireDir, &fireDir );
	AttackObject* ao = BallAttackObject::createBallAttackObject( enemy, getPos(), fireDir, dist );
	BallAttackObject* bao = (BallAttackObject*)ao;
	bao->setDamage (m_attack);
	m_attackObjectList.push_back(ao);
}

void Character::throwHealBall ()
{
	AttackObject* ao = HealObject::createHealObject (this);
	m_attackObjectList.push_back(ao);
}

void Character::heal (int point)
{
	if (m_curHp + point < m_maxHp)
		m_curHp += point;
	else if (m_curHp + point < 0)
		m_curHp = 0;
	else
		m_curHp = m_maxHp;
}

Unit* Character::createCharacter( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool bControllable )
{
	Character* u = new Character();
	u->init( GetG().m_dev, mesh );
	u->setControllable( bControllable );
	u->setTilePos( tileX, tileY );
	u->setTileBufferPos( tileX, tileY );
	return u;
}

Character::~Character()
{
	AttackObjectList::iterator it = m_attackObjectList.begin();
	for ( ; it != m_attackObjectList.end(); ++it )
	{
		EP_SAFE_RELEASE(*it);
	}
	m_attackObjectList.clear();
	delete m_trigger;
}

bool Character::frameMove( float fElapsedTime )
{
	if (m_bMoving == false)
	{
		m_fMovingTime = 0;


		UINT i;
		for ( i = 0; i < UNIT_MAX_KEYS; i++ )
		{
			if( IsKeyDown( m_aKeys[ (UnitInput)i ] ) )
			{
				m_bMovable = true;
				rayTesting( (UnitInput)i );
				boundaryTesting( (UnitInput)i );

				// if there is no obstacles
				if( m_bMovable )
				{
					int nextTileX = m_tileBufferX + g_moveAmount[ i ].x;
					int nextTileY = m_tileBufferY + g_moveAmount[ i ].y;
					Tile* nextTile = tileManager.getTile( nextTileX, nextTileY );
					assert( nextTile );
					if( nextTile->b_movable && (nextTileX  > -1 && nextTileX  < s_xSize) && (nextTileY  > -1 && nextTileY  < s_ySize) )
 					{
						m_bMoving = true;
						m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
						m_vKeyboardDirection.x += (float) g_moveAmount[ i ].x * s_tileSize;
						m_vKeyboardDirection.y += (float) g_moveAmount[ i ].y * s_tileSize;
						m_tileBufferX += g_moveAmount[ i ].x;
						m_tileBufferY += g_moveAmount[ i ].y;
					}
				}
				// 가는 방향으로 머리를 돌린다.
				this->setHeadDir( (UnitInput)i );

				pushUnitInFront( (UnitInput)i );

				break;
			}
		}
	}


	if (m_bMoving && m_fMovingTime <= m_moveDuration)
	{
		// Update velocity
		m_vVelocity = m_vKeyboardDirection / m_moveDuration;

		// Simple euler method to calculate position delta
		D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;
		m_fMovingTime += fElapsedTime;
		setPos(getPos() + vPosDelta);
	}
	else
	{
		m_bMoving = false;

		//타일에 맞도록 위치보정
		if( m_tileBufferX != m_tileX || m_tileBufferY != m_tileY )
		{
			enterTile( m_tileBufferX, m_tileBufferY );
			setTilePos( m_tileBufferX, m_tileBufferY );
		}
	}


	AttackObjectList::iterator it = m_attackObjectList.begin();
	for ( ; it != m_attackObjectList.end(); )
	{
		bool ret = (*it)->frameMove( fElapsedTime );
		if (!ret)
		{
			EP_SAFE_RELEASE( *it );
			it = m_attackObjectList.erase( it );
		}
		else
			++it;
	}

	return Unit::frameMove( fElapsedTime );
}

HRESULT Character::frameRender()
{
	AttackObjectList::iterator it = m_attackObjectList.begin();
	for ( ; it != m_attackObjectList.end(); ++it )
	{
		(*it)->frameRender();
	}
	Unit::frameRender();
	return S_OK;

}

LRESULT Character::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			// Map this key to a D3DUtil_CameraKeys enum and update the
			// state of m_aKeys[] by adding the KEY_WAS_DOWN_MASK|KEY_IS_DOWN_MASK mask
			// only if the key is not down
			UnitInput mappedKey = mapKey( ( UINT )wParam );
			if( mappedKey != UNIT_UNKNOWN )
			{
				if( FALSE == IsKeyDown( m_aKeys[mappedKey] ) )
				{
					m_aKeys[ mappedKey ] = KEY_WAS_DOWN_MASK | KEY_IS_DOWN_MASK;
					++m_cKeysDown;
				}
			}
			break;
		}

	case WM_KEYUP:
		{
			// Map this key to a D3DUtil_CameraKeys enum and update the
			// state of m_aKeys[] by removing the KEY_IS_DOWN_MASK mask.
			UnitInput mappedKey = mapKey( ( UINT )wParam );
			if( mappedKey != UNIT_UNKNOWN && ( DWORD )mappedKey < 8 )
			{
				m_aKeys[ mappedKey ] &= ~KEY_IS_DOWN_MASK;
				--m_cKeysDown;
			}
			break;
		}

	}

	return FALSE;
}


Character::Character()
{
	m_maxHp				= 5;
	m_curHp				= m_maxHp;
	m_bMoving			= false;
	m_fMovingTime		= 0;
	m_moveDuration		= 1.0f;
	m_boundaryTileRect.top   = 999999;
	m_boundaryTileRect.left  = -999999;
	m_boundaryTileRect.bottom= -999999;
	m_boundaryTileRect.right = 999999;
	m_trigger			= new Trigger;

	ZeroMemory( &m_stat, sizeof(Stat) );

	// Initialize random number
	srand ( (unsigned)time(NULL) );
}

void Character::setColor( int r, int g, int b )
{
	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));

	m_material.Ambient.r = (float)r / 255.0f;
	m_material.Ambient.g = (float)g / 255.0f;
	m_material.Ambient.b = (float)b / 255.0f;

	m_material.Diffuse.r = (float)r / 255.0f;
	m_material.Diffuse.g = (float)g / 255.0f;
	m_material.Diffuse.b = (float)b / 255.0f;

	m_material.Specular.r = (float)r / 255.0f;
	m_material.Specular.g = (float)g / 255.0f;
	m_material.Specular.b = (float)b / 255.0f;

	m_material.Ambient.a = m_material.Diffuse.a = m_material.Specular.a = 1.0f;
}

HRESULT Character::rayTesting( UnitInput mappedKey )
{
	//////////////////////////////////////////////////////////////////////////
	// Room Model MainWall intersection test

	HRESULT hr = S_OK;

	TopStateManager& tsm = TopStateManager::getSingleton();
	WorldState* ws = static_cast<WorldState*>( tsm.getCurState() );

	// Ray starting position as hero position
	D3DXVECTOR3 rayStartPos( getPos().x, getPos().y, getPos().z - 2.0f );

	// Direction data
	float dirArray[4][3] = { { 0.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } };

	BOOL hit;
	DWORD hitFaceIndex;
	float hitU, hitV;
	float hitDist;

	// Get mesh data
	ArnMesh* mainWallMesh = dynamic_cast<ArnMesh*>( ws->getArnSceneGraphPt()->getSceneRoot()->getNodeByName("MainWall") );

	// Select direction
	D3DXVECTOR3 rayDir( dirArray[mappedKey][0], dirArray[mappedKey][1], dirArray[mappedKey][2] );

	// Get intersection information
	V_RETURN( D3DXIntersect( 
		mainWallMesh->getD3DXMesh(), 
		&rayStartPos, 
		&rayDir,
		&hit, 
		&hitFaceIndex, 
		&hitU, 
		&hitV, 
		&hitDist, 
		0, 
		0 ) );

	// If there is collision between ray and face
	if ( hit )
	{
		//printf("Ray Testing test. (FaceIndex : %u, Dist : %f)\n", hitFaceIndex, hitDist );

		// 타일 1.5칸 이내에서 교차하면 그 방향으로 움직이지 않는다.
		if ( hitDist <= (float) 1.5 * s_tileSize )
			m_bMovable = false;
	}

	return hr;
}

void Character::setMaxAndCurHp( int maxHp, int curHp )
{
	if ( maxHp < curHp || maxHp <= 0 )
		throw std::runtime_error( "Logically incorrect value entered" );
	if ( curHp == -1 )
		curHp = maxHp;

	m_maxHp = maxHp;
	m_curHp = curHp;
}

// 지정된 사각형 경계 위에 캐릭터가 있을 때 경계 바깥으로 나가려고 하면 움직일 수 없게 한다.
void Character::boundaryTesting( UnitInput mappedKey )
{
	switch ( mappedKey )
	{
	case UNIT_MOVE_UP:
		if ( (UINT)m_boundaryTileRect.top == getTilePosY() )
			m_bMovable = false;
		break;
	case UNIT_MOVE_DOWN:
		if ( (UINT)m_boundaryTileRect.bottom == getTilePosY() )
			m_bMovable = false;
		break;
	case UNIT_MOVE_LEFT:
		if ( (UINT)m_boundaryTileRect.left == getTilePosX() )
			m_bMovable = false;
		break;
	case UNIT_MOVE_RIGHT:
		if ( (UINT)m_boundaryTileRect.right == getTilePosX() )
			m_bMovable = false;
		break;
	}
}

void Character::setBoundaryRect( LONG left, LONG top, LONG right, LONG bottom )
{
	m_boundaryTileRect.top = top; 
	m_boundaryTileRect.left = left; 
	m_boundaryTileRect.bottom = bottom; 
	m_boundaryTileRect.right = right;
}

void Character::damage( int point )
{
	TopStateManager& tsm = TopStateManager::getSingleton();
	WorldState* ws = static_cast<WorldState*>( tsm.getCurState() );

	ws->screenFlashing( 0.2f, 1.0f, 0.0f, 0.0f );

	m_curHp -= point;
}

void Character::enterTile( UINT tileX, UINT tileY )
{

	/*printf( "현재의 위치 : [%d, %d]\n", tileX, tileY );
	printf( "또다른 현재의 위치 : [%d, %d]\n",
		GetTileManager().pos2TileX( &getWorldState()->getHeroPos() ),
		GetTileManager().pos2TileY( &getWorldState()->getHeroPos() ) );*/

	m_trigger->positionTrigger();
}

void Character::setStat( int statHealth, int statWill, int statCoding, int statDef )
{
	m_stat.health	= statHealth;
	m_stat.will		= statWill;
	m_stat.coding	= statCoding;
	m_stat.def		= statDef;

	// Hp, Mp update goes here...
}


//////////////////////////////////////////////////////////////////////////


Unit* EpCreateCharacter( int tileX, int tileY, int controllable )
{
	LPD3DXMESH d3dxMesh;
	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
	return Character::createCharacter( d3dxMesh, tileX, tileY, 0, controllable?true:false );

} SCRIPT_CALLABLE_PV_I_I_I( EpCreateCharacter )

int EpCharacterSetMaxAndCurHp( void* ptr, int maxHp, int curHp )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setMaxAndCurHp( maxHp, curHp );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I( EpCharacterSetMaxAndCurHp )

int EpCharacterSetMoveDuration( void* ptr, double val )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setMoveDuration( (float)val );
	return 0;
} SCRIPT_CALLABLE_I_PV_D( EpCharacterSetMoveDuration )

int EpCharacterSetColor( void* ptr, int r, int g, int b )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setColor( r, g, b );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I_I( EpCharacterSetColor )

int EpCharacterSetBoundary( void* ptr, int left, int top, int right, int bottom )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setBoundaryRect( left, top, right, bottom );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I_I_I( EpCharacterSetBoundary )


int EpCharacterSetStat( void* ptr, int statHealth, int statWill, int statCoding, int statDef )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setStat( statHealth, statWill, statCoding, statDef );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I_I_I( EpCharacterSetStat )


START_SCRIPT_FACTORY(Character)
	CREATE_OBJ_COMMAND( EpCreateCharacter )
	CREATE_OBJ_COMMAND( EpCharacterSetMaxAndCurHp )
	CREATE_OBJ_COMMAND( EpCharacterSetMoveDuration )
	CREATE_OBJ_COMMAND( EpCharacterSetColor )
	CREATE_OBJ_COMMAND( EpCharacterSetBoundary )
	CREATE_OBJ_COMMAND( EpCharacterSetStat )

END_SCRIPT_FACTORY(Character)
