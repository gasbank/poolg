#include "EmptyProjectPCH.h"
#include "Character.h"
#include "AttackObject.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "TopStateManager.h"
#include "WorldStateManager.h"
#include "World.h"
#include "ArnMesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Trigger.h"
#include "StructureObject.h"


extern TileManager tileManager;


void Character::attack( int type, Character* enemy )
{
	D3DXVECTOR3 fireDir = enemy->getPos() - getPos();
	float dist = D3DXVec3Length( &fireDir );

	D3DXVec3Normalize( &fireDir, &fireDir );
	AttackObject* ao = BallAttackObject::createBallAttackObject( enemy, getPos(), fireDir, dist );
	BallAttackObject* bao = (BallAttackObject*)ao;
	/*일반공격 데미지 계산*/
	int damage = m_stat.coding + 10 - enemy->getStat().def;
	bao->setDamage (damage);
	m_attackObjectList.push_back(ao);
}

void Character::throwHealBall ()
{
	AttackObject* ao = HealObject::createHealObject (this);
	m_attackObjectList.push_back(ao);
}

void Character::csBurn ()
{
	AttackObject* ao = CsBurnObject::createCsBurnObject(this);
	m_attackObjectList.push_back(ao);
}

void Character::meditation ()
{
	AttackObject* ao = MeditationObject::createMeditationObject(this);
	m_attackObjectList.push_back(ao);
}


void Character::recoverCs()
{
	int recoverCsPoint = (int)(m_maxCs * 0.1f);
	if (m_curCs + recoverCsPoint > m_maxCs)
		m_curCs = m_maxCs;
	else
		m_curCs += recoverCsPoint;
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

Unit* Character::createCharacter( LPD3DXMESH mesh, int tileX, int tileY, float posZ )
{
	Character* u = new Character( UT_CHARACTER );
	u->init( GetG().m_dev, mesh );
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
				setMovable( true );
				rayTesting( (UnitInput)i );
				boundaryTesting( (UnitInput)i );

				// if there is no obstacles
				if( getMovable() )
				{
					TileRegion entireRegion( 0, 0, s_xSize - 1, s_ySize - 1);

					Point2Uint nextTilePos(
						getTileBufferPos().x + g_moveAmount[ i ].x,
						getTileBufferPos().y + g_moveAmount[ i ].y );

//  					Tile* nextTile = tileManager.getTile( nextTilePos );
//  					assert( nextTile );
					if( !GetWorldManager().getCurWorld()->findUnitAtTile( nextTilePos.x, nextTilePos.y ) && entireRegion.isExist( nextTilePos ) )
 					{
						m_bMoving = true;
						m_vKeyboardDirection.x = (float) g_moveAmount[ i ].x * s_tileSize;
						m_vKeyboardDirection.y = (float) g_moveAmount[ i ].y * s_tileSize;
						m_vKeyboardDirection.z = 0;

						setTileBufferPos(
							getTileBufferPos().x + g_moveAmount[ i ].x,
							getTileBufferPos().y + g_moveAmount[ i ].y );
					}
				}
				// 가는 방향으로 머리를 돌린다.
				this->setHeadDir( (UnitInput)i );
				// 앞에 있는 유닛을 민다.
				this->pushUnitInFront( (UnitInput)i );

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
		if( getTileBufferPos() != getTilePos() )
		{
			setTilePos( getTileBufferPos() );
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

	const D3DXVECTOR3& unitPos = Unit::getPos();
	m_curPos = unitPos + m_moveImpulse;
	if ( m_moveImpulse != DX_CONSTS::D3DXVEC3_ZERO )
	{
		const float decimate = 0.5f;
		m_moveImpulse *= decimate;
		if ( D3DXVec3Length( &m_moveImpulse ) < 1e-4 )
			m_moveImpulse = DX_CONSTS::D3DXVEC3_ZERO;

		//printf( "Char FM: %.2f %.2f %.2f\n", m_curPos.x, m_curPos.y, m_curPos.z );
		setLocalXformDirty();
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
			if( mappedKey != UNIT_UNKNOWN && m_bControllable )
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


Character::Character( UnitType type )
: Unit( type ), m_moveImpulse( DX_CONSTS::D3DXVEC3_ZERO )
{
	m_maxHp				= 5;
	m_curHp				= m_maxHp;
	m_bMoving			= false;
	m_fMovingTime		= 0;
	m_moveDuration		= 1.0f;
	m_bControllable		= true;
	m_boundaryTileRect.setStart( Point2Uint( 0, 0 ) );
	m_boundaryTileRect.setEnd( Point2Uint( s_xSize - 1, s_ySize - 1 ) );
	ZeroMemory( &m_stat, sizeof(Stat) );
	
	// Initialize random number
	srand ( (unsigned)time(NULL) );
}

void Character::setCurHp( int curHp )
{
	if ( curHp == -1 )
		m_curHp = m_maxHp;
	else if ( curHp > m_maxHp )
		m_curHp = m_maxHp;
	else if ( curHp < 0)
		m_curHp = 0;
	else
		m_curHp = curHp;
}

void Character::setCurCs( int curCs )
{
	if ( curCs == -1 )
		m_curCs = m_maxCs;
	else if ( curCs > m_maxCs )
		m_curCs = m_maxCs;
	else if ( curCs < 0)
		m_curCs = 0;
	else
		m_curCs = curCs;
}
/*void Character::setMaxAndCurHp( int maxHp, int curHp )
{
	if ( maxHp < curHp || maxHp <= 0 )
		throw std::runtime_error( "Logically incorrect value entered" );
	maxHp = m_stat.health * m_stat.health / 4 + 50;
	if ( curHp == -1 )
		curHp = maxHp;

	m_maxHp = maxHp;
	m_curHp = curHp;
}

void Character::setMaxAndCurCs( int maxCs, int curCs )
{
	if ( maxCs < curCs || maxCs <= 0 )
		throw std::runtime_error( "Logically incorrect value entered" );
	maxCs = m_stat.will * m_stat.will / 10 + 20;
	if ( curCs == -1 )
		curCs = maxCs;

	m_maxCs = maxCs;
	m_curCs = curCs;
}*/

// 지정된 사각형 경계 위에 캐릭터가 있을 때 경계 바깥으로 나가려고 하면 움직일 수 없게 한다.
void Character::boundaryTesting( UnitInput mappedKey )
{
	if ( !m_boundaryTileRect.isExist( getTilePos() ) )
		setMovable( false );
}

void Character::setBoundaryRect( LONG x0, LONG y0, LONG x1, LONG y1 )
{
	m_boundaryTileRect = TileRegion( x0, y0, x1, y1 );
}

void Character::damage( int point )
{
	m_curHp -= point;

	printf("체력은? %d\n", m_curHp);
}


void Character::setStat( int statHealth, int statWill, int statCoding, int statDef, int statSen, int statImmu )
{
	m_stat.health	= statHealth;
	m_stat.will		= statWill;
	m_stat.coding	= statCoding;
	m_stat.def		= statDef;
	m_stat.sense	= statSen;
	m_stat.immunity	= statImmu;

	m_maxHp = m_stat.health * m_stat.health / 4 + 50;
	m_maxCs = m_stat.will * m_stat.will / 10 + 20;


	// Hp, Mp update goes here...
}

void Character::pushUnitInFront( UnitInput dir )
{
	if ( this->getType() == UT_HERO )
	{
		Unit* u = GetWorldManager().getCurWorld()->findUnitAtTile( 
		getTileBufferPos().x + g_moveAmount[ (int)dir ].x,
		getTileBufferPos().y + g_moveAmount[ (int)dir ].y );

		if ( u )
		{
			if ( u->getType() == UT_STRUCTREOBJECT )
			{
				StructureObject* s = reinterpret_cast<StructureObject*>(u);
				if ( s->getPushable() )
				{
					s->setForcedMove( (int)dir );
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////


Unit* EpCreateCharacter( int tileX, int tileY )
{
	LPD3DXMESH d3dxMesh;
	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
	return Character::createCharacter( d3dxMesh, tileX, tileY, 0 );

} SCRIPT_CALLABLE_PV_I_I( EpCreateCharacter )

int EpCharacterSetCurHp( void* ptr, int curHp )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setCurHp( curHp );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpCharacterSetCurHp )

int EpCharacterSetCurCs( void* ptr, int curCs )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setCurCs( curCs );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpCharacterSetCurCs )

int EpCharacterSetMoveDuration( void* ptr, double val )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setMoveDuration( (float)val );
	return 0;
} SCRIPT_CALLABLE_I_PV_D( EpCharacterSetMoveDuration )


int EpCharacterSetBoundary( void* ptr, int left, int top, int right, int bottom )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setBoundaryRect( left, top, right, bottom );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I_I_I( EpCharacterSetBoundary )


int EpCharacterSetStat( void* ptr, int statHealth, int statWill, int statCoding, int statDef, int statSen, int statImmu )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setStat( statHealth, statWill, statCoding, statDef, statSen, statImmu );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I_I_I_I_I( EpCharacterSetStat )


START_SCRIPT_FACTORY(Character)
	CREATE_OBJ_COMMAND( EpCreateCharacter )
	CREATE_OBJ_COMMAND( EpCharacterSetCurHp )
	CREATE_OBJ_COMMAND( EpCharacterSetCurCs )
	CREATE_OBJ_COMMAND( EpCharacterSetMoveDuration )
	CREATE_OBJ_COMMAND( EpCharacterSetBoundary )
	CREATE_OBJ_COMMAND( EpCharacterSetStat )
END_SCRIPT_FACTORY(Character)
