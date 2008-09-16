#include "EmptyProjectPCH.h"
#include "EpItem.h"

extern LPD3DXMESH						g_bst[BST_COUNT];

EpItem::EpItem( BasicShapeType bst )
: Unit( UT_ITEM )
{
	setMesh( g_bst[bst] );
}

EpItem::EpItem( const char* arnMeshName )
: Unit( UT_ITEM )
{
	setArnMeshName( arnMeshName );
}
EpItem::~EpItem(void)
{
}

//////////////////////////////////////////////////////////////////////////

Unit* EpCreateItem( int bst )
{
	Unit* item = new EpItem( (BasicShapeType)bst );
	return item;
} SCRIPT_CALLABLE_PV_I( EpCreateItem )

START_SCRIPT_FACTORY( EpItem )
	CREATE_OBJ_COMMAND( EpCreateItem )
END_SCRIPT_FACTORY( EpItem )
