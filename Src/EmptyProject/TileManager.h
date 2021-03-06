﻿#pragma once
#include "Tile.h"
#include "CommonStructs.h"


/**
@brief 특정 타일 영역을 정의하는 클래스

맵은 타일 기반으로 이루어져 있습니다. 맵의 일정 영역을 정의하기 위해서는 타일 영역을
정의할 필요가 있는데 본 클래스는 그러한 영역 하나를 정의합니다.
*/
class TileRegion
{
public:
	TileRegion() {}
	TileRegion( UINT startX, UINT startY, UINT endX, UINT endY )
	{
		m_start.x = startX;
		m_start.y = startY;
		m_end.x = endX;
		m_end.y = endY;
		if ( endX == -1 )
			m_end.x = m_start.x;
		if ( endY == -1 )
			m_end.y = m_start.y;		
	}
	TileRegion( const Point2Uint start, const Point2Uint end ) : m_start( start ), m_end( end ) {}
	~TileRegion() {}
	
	inline bool isExist( const Point2Uint& point ) const
	{
		UINT xmin, xmax, ymin, ymax;
		if (m_start.x < m_end.x)
		{
			xmin = m_start.x;
			xmax = m_end.x;
		}
		else
		{
			xmin = m_end.x;
			xmax = m_start.x;
		}
		if (m_start.y < m_end.y)
		{
			ymin = m_start.y;
			ymax = m_end.y;
		}
		else
		{
			ymin = m_end.y;
			ymax = m_start.y;
		}
		return (point.x >= xmin && point.x <= xmax && point.y >= ymin && point.y <= ymax);
	}

	TileRegion &TileRegion::operator =(const TileRegion &rhs) { m_start = rhs.m_start; m_end = rhs.m_end; return *this; }

	Point2Uint getStart() const { return m_start; }
	void setStart(Point2Uint val) { m_start = val; }
	Point2Uint getEnd() const { return m_end; }
	void setEnd(Point2Uint val) { m_end = val; }

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
	int pos2TileX( const ArnVec3* pos );
	int pos2TileY( const ArnVec3* pos );

private:
	//Tile	m_tileArray[s_xSize][s_ySize];
};

inline TileManager& GetTileManager() { return TileManager::getSingleton(); }