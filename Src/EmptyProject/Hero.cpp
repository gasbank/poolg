#include "EmptyProjectPCH.h"
#include "Hero.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "WorldState.h"

Hero::Hero(void)
{
}

Hero::~Hero(void)
{
}

Unit* Hero::createHero( LPD3DXMESH mesh, int tileX, int tileY, float posZ )
{
	Hero* u = new Hero();
	u->init( GetG().m_dev, mesh );
	u->setControllable( true );
	u->setTilePos( tileX, tileY );
	u->setTileBufferPos( tileX, tileY );

	return u;
}

HRESULT Hero::frameRender()
{
	return Character::frameRender();
}

bool Hero::frameMove( float fElapsedTime )
{
	return Character::frameMove( fElapsedTime );
}

LRESULT Hero::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return Character::handleMessages( hWnd, uMsg, wParam, lParam );
}

void Hero::pushUnitInFront( UnitInput dir )
{
	/*Unit* u = getWorldState()->findUnitAtTile(
		m_tileBufferX + g_moveAmount[ (int)dir ].x,
		m_tileBufferY + g_moveAmount[ (int)dir ].y );

	if ( u )
	{
		UINT targetTileX = u->getTilePosX() + g_moveAmount[dir].x;
		UINT targetTileY = u->getTilePosY() + g_moveAmount[dir].y;
		u->setTilePos( targetTileX, targetTileY );
	}*/
}
Unit* EpCreateHero( int tileX, int tileY )
{
	LPD3DXMESH d3dxMesh;
	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
	return Hero::createHero( d3dxMesh, tileX, tileY, 0 );

} SCRIPT_CALLABLE_PV_I_I( EpCreateHero )

START_SCRIPT_FACTORY(Hero)
	CREATE_OBJ_COMMAND( EpCreateHero )
END_SCRIPT_FACTORY(Hero)