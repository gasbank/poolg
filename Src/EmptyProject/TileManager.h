#pragma once
#include "Tile.h"
#define X 25
#define Y 25

class TileManager
{
public:
	int x, y;

	Tile	tile[X][Y];

	TileManager(void);
	~TileManager(void);
};
