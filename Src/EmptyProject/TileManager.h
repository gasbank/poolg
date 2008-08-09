#pragma once
#include "Tile.h"
#define X 25
#define Y 25

class TileManager
{
public:
	TileManager(void);
	~TileManager(void);

	int x, y;

	Tile	tile[X][Y];
};
