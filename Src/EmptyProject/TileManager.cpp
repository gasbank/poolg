#include "EmptyProjectPCH.h"
#include "TileManager.h"

TileManager::TileManager(void)
{
	int i, j;

	for(i = 0; i < x; i++)
	{
		for(j = 0; j < y; j++)
		{
			tile[i][j].movable = true;
			tile[i][j].talkable = false;
			tile[i][j].talkonetime = true;
		}
	}
}

TileManager::~TileManager(void)
{
}
