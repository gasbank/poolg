#include "EmptyProjectPCH.h"
#include "TileManager.h"

TileManager::TileManager(void)
{
	int i, j;

	x = X;
	y = Y;

	for(i = 0; i < X; i++)
	{
		for(j = 0; j < Y; j++)
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
