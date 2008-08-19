#pragma once
#include "Tile.h"

#define s_xSize		(90)
#define s_ySize		(158)
#define s_tileSize	(2)


struct Point2Uint 
{
	UINT x, y;

	bool operator !=( const Point2Uint& rhs ) const
	{
		return (x != rhs.x || y != rhs.y );
	}

	bool operator ==( const Point2Uint& rhs ) const
	{
		return (x == rhs.x && y == rhs.y );
	}

	static Point2Uint ZERO;
};

class TileRegion
{
public:
	TileRegion() {}
	TileRegion( UINT startX, UINT startY, UINT endX, UINT endY ) { m_start.x = startX; m_start.y = startY; m_end.x = endX; m_end.y = endY; }
	TileRegion( const Point2Uint start, const Point2Uint end ) : m_start( start ), m_end( end ) {}
	~TileRegion() {}
	
	inline bool isExist( const Point2Uint& point ) const
	{
		return ( point.x >= m_start.x && point.x <= m_end.x && point.y >= m_start.y && point.y <= m_end.y);
	}

private:
	Point2Uint m_start;
	Point2Uint m_end;

};

//////////////////////////////////////////////////////////////////////////
class TileManager : public Singleton<TileManager>
{
public:
	TileManager(void);
	~TileManager(void);

	

// 	Tile* getTile( int x, int y );
// 	Tile* getTile( const Point2Uint& pos );
	int pos2TileX( const D3DXVECTOR3* pos );
	int pos2TileY( const D3DXVECTOR3* pos );

private:
	//Tile	m_tileArray[s_xSize][s_ySize];
};

inline TileManager& GetTileManager() { return TileManager::getSingleton(); }