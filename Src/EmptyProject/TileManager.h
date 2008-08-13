#pragma once
#include "Tile.h"

class TileManager
{
public:
	TileManager(void);
	~TileManager(void);

	static const int s_xSize = 90;
	static const int s_ySize = 158;
	static const int s_tileSize = 2;	

	Tile* getTile( int x, int y );

private:
	Tile	m_tileArray[s_xSize][s_ySize];
};
