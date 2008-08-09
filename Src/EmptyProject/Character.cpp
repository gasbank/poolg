#include "EmptyProjectPCH.h"
#include "Character.h"
#include "AttackObject.h"
#include "ScriptManager.h"

void Character::attack( int type, Character* enemy )
{
	D3DXVECTOR3 fireDir = enemy->getPos() - getPos();
	float dist = D3DXVec3Length( &fireDir );

	D3DXVec3Normalize( &fireDir, &fireDir );
	AttackObject* ao = BallAttackObject::createBallAttackObject( enemy, getPos(), fireDir, dist );
	m_attackObjectList.push_back(ao);
}

Unit* Character::createCharacter( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool bControllable )
{
	Character* u = new Character();
	u->init( GetG().m_dev, mesh );
	u->setControllable( bControllable );
	u->setTilePos( tileX, tileY );
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

		if( IsKeyDown( m_aKeys[UNIT_MOVE_UP] ) )
		{
			m_bMoving = true;
			m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
			m_vKeyboardDirection.y += 2.0f;
			m_tileY++;
			GetScriptManager().execute("EpUnitOnMove 0");
		}
		else if( IsKeyDown( m_aKeys[UNIT_MOVE_DOWN] ) )
		{
			m_bMoving = true;
			m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
			m_vKeyboardDirection.y -= 2.0f;
			m_tileY--;
			GetScriptManager().execute("EpUnitOnMove 1");
		}
		else if( IsKeyDown( m_aKeys[UNIT_MOVE_RIGHT] ) )
		{
			m_bMoving = true;
			m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
			m_vKeyboardDirection.x += 2.0f;
			m_tileX++;
			GetScriptManager().execute("EpUnitOnMove 2");
		}
		else if( IsKeyDown( m_aKeys[UNIT_MOVE_LEFT] ) )
		{
			m_bMoving = true;
			m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
			m_vKeyboardDirection.x -= 2.0f;
			m_tileX--;
			GetScriptManager().execute("EpUnitOnMove 3");
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
		setTilePos( m_tileX, m_tileY );
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

Character::Character()
{
	m_maxHp				= 5;
	m_curHp				= m_maxHp;
	m_bMoving			= false;
	m_tileX				= 0;
	m_tileY				= 0;

}
void Character::setTilePos( int tileX, int tileY )
{
	m_tileX = tileX;
	m_tileY = tileY;

	setPos( D3DXVECTOR3( (float)(tileX - 13) * G::s_tileSize, (float)(tileY  - 13) * G::s_tileSize, 0 ) );
}



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



START_SCRIPT_FACTORY(Character)
	CREATE_OBJ_COMMAND( EpCreateCharacter )
	CREATE_OBJ_COMMAND( EpCharacterSetMaxAndCurHp )
		CREATE_OBJ_COMMAND( EpCharacterSetMoveDuration )
END_SCRIPT_FACTORY(Character)