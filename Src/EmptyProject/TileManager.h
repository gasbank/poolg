#pragma once
#include "Tile.h"

static const int s_xSize = 90;
static const int s_ySize = 158;
static const int s_tileSize = 2;

class TileManager : public Singleton<TileManager>
{
public:
	TileManager(void);
	~TileManager(void);

	

	Tile* getTile( int x, int y );
	int pos2TileX( const D3DXVECTOR3* pos );
	int pos2TileY( const D3DXVECTOR3* pos );

private:
	Tile	m_tileArray[s_xSize][s_ySize];
};

inline TileManager& GetTileManager() { return TileManager::getSingleton(); }