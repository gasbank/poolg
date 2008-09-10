#pragma once



#define s_xSize		(90)
#define s_ySize		(158)
#define s_tileSize	(2)


struct Point3Float
{
	Point3Float() { x = 0; y = 0; z = 0; }
	Point3Float( float ax, float ay, float az ) { x = ax; y = ay; z = az; }
	float x, y, z;

	static Point3Float ZERO;
	static Point3Float ONE;
};
struct Mat4Float
{
	Mat4Float() { ZeroMemory( m, sizeof(float)*16 ); }

	float m[4][4];
};


struct Point2Uint 
{
	Point2Uint() { x = 0; y = 0; }
	Point2Uint( UINT ax, UINT ay ) { x = ax; y = ay; }
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
