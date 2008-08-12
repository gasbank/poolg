#include "EmptyProjectPCH.h"
#include "TileManager.h"

TileManager::TileManager(void)
{
	int i, j;

	for(i = 0; i < s_xSize; i++)
	{
		for(j = 0; j < s_ySize; j++)
		{
			m_tileArray[i][j].movable = true;
			m_tileArray[i][j].talkable = false;
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