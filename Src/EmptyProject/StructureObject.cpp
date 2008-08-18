#include "EmptyProjectPCH.h"
#include "StructureObject.h"

StructureObject::StructureObject(void)
: Unit ( UT_STRUCTREOBJECT )
{
}

StructureObject::~StructureObject(void)
{
}

Unit* StructureObject::createStructureObject( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool pushable )
{
	StructureObject* u = new StructureObject();
	u->init( GetG().m_dev, mesh );
	u->setControllable( false );
	u->setTilePos( tileX, tileY );
	u->setTileBufferPos( tileX, tileY );
	u->setPushable( pushable );
	GetTileManager().getTile( tileX, tileY )->b_movable = false;

	return u;
}

HRESULT StructureObject::frameRender()
{
	return Unit::frameRender();
}

LRESULT StructureObject::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return Unit::handleMessages( hWnd, uMsg, wParam, lParam );
}

bool StructureObject::frameMove( float fElapsedTime )
{
	return Unit::frameMove( fElapsedTime );
}

Unit* EpCreateStructureObject( int tileX, int tileY, int p )
{
	LPD3DXMESH d3dxMesh;
	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
	bool pushable;
	if ( p == 0 )
		pushable = false;
	else
		pushable = true;
	return StructureObject::createStructureObject( d3dxMesh, tileX, tileY, 0, pushable );
} SCRIPT_CALLABLE_PV_I_I_I( EpCreateStructureObject )

START_SCRIPT_FACTORY( StructureObject )
	CREATE_OBJ_COMMAND( EpCreateStructureObject )
END_SCRIPT_FACTORY( StructureObject )