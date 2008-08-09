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

Unit* Character::createCharacter( LPD3DXMESH mesh, float posX /*= 0*/, float posY /*= 0*/, float posZ /*= 0*/, bool bCtrl /*= false */ )
{
	Unit* u = new Character();
	u->init( GetG().m_dev, mesh );
	u->setControllable( bCtrl );
	u->setPos( D3DXVECTOR3( posX, posY, posZ ) );
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
}
Unit* EpCreateCharacter( int tileX, int tileY, int controllable )
{
	LPD3DXMESH d3dxMesh;
	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
	return Character::createCharacter( d3dxMesh, (float)tileX, (float)tileY, 0, controllable?true:false );

} SCRIPT_CALLABLE_PV_I_I_I( EpCreateCharacter )

int EpCharacterSetMaxAndCurHp( void* ptr, int maxHp, int curHp )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setMaxAndCurHp( maxHp, curHp );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I( EpCharacterSetMaxAndCurHp )


START_SCRIPT_FACTORY(Character)
	CREATE_OBJ_COMMAND( EpCreateCharacter )
	CREATE_OBJ_COMMAND( EpCharacterSetMaxAndCurHp )
END_SCRIPT_FACTORY(Character)