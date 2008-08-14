#include "EmptyProjectPCH.h"
#include "TileManager.h"

TileManager::TileManager(void)
{
	int i, j;

	for(i = 0; i < s_xSize; i++)
	{
		for(j = 0; j < s_ySize; j++)
		{
			m_tileArray[i][j].b_movable = true;
			m_tileArray[i][j].b_eventTalk = false;
		}
	}
}

TileManager::~TileManager(void)
{
}

Tile* TileManager::getTile( int x, int y )
{
	return &m_tileArray[x][y];
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