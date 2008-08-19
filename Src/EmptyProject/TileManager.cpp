#include "EmptyProjectPCH.h"
#include "TileManager.h"

Point2Uint Point2Uint::ZERO = { 0, 0 };


//////////////////////////////////////////////////////////////////////////

TileManager::TileManager(void)
{
	int xSize = s_xSize;
	int ySize = s_ySize;
	int tileSize = s_tileSize;

	if ( xSize % 2 != 0 || ySize % 2 != 0 || xSize <= 0 || ySize <= 0 || tileSize <= 0)
	{
		printf( " - Tile SIze Error! Should be positive and even number\n" );
		system( "pause" );
		exit( -10 );
	}
}

TileManager::~TileManager(void)
{
}

Tile* TileManager::getTile( int x, int y )
{
	return &m_tileArray[x][y];
}

Tile* TileManager::getTile( const Point2Uint& pos )
{
	return getTile( pos.x, pos.y );
}
int TileManager::pos2TileX( const D3DXVECTOR3* pos )
{
	for ( int i = 0; i < s_xSize; i++ )
	{
		if ( ((float) ( s_xSize / 2 ) + (pos->x / (float)s_tileSize)) >= ((float)i -  ((float)s_tileSize / 2 ))
			&& ((float) ( s_xSize / 2 ) + (pos->x / (float)s_tileSize)) < ((float)i +  ((float)s_tileSize / 2 )) )
			return i;
	}
	return 0;
}

int TileManager::pos2TileY( const D3DXVECTOR3* pos )
{
	for ( int i = 0; i < s_ySize; i++ )
	{
		if ( ((float) ( s_ySize / 2 ) + (pos->y / (float)s_tileSize)) >= ((float)i -  ((float)s_tileSize / 2 ))
			&& ((float) ( s_ySize / 2 ) + (pos->y / (float)s_tileSize)) < ((float)i +  ((float)s_tileSize / 2 )) )
			return i;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
