#pragma once
#include "Tile.h"

class TileManager
{
public:
	TileManager(void);
	~TileManager(void);

	static const int x = 25;
	static const int y = 25;

	Tile	tile[x][y];
};
