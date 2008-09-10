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
	Mat4Float() { ZeroMemory( m, sizeof( float ) * 16 ); }
	static void MakeIdentity( Mat4Float* m4f )
	{
		ZeroMemory( m4f, sizeof( float ) * 16 );
		m4f->m[0][0] = m4f->m[1][1] = m4f->m[2][2] = m4f->m[3][3] = 1.0f;
	}
	static void MakeTranslation( Mat4Float* m4f, float x, float y, float z )
	{
		MakeIdentity( m4f );
		m4f->m[3][0] = x;
		m4f->m[3][1] = y;
		m4f->m[3][2] = z;
	}
	static void MakeRotationZ( Mat4Float* m4f, float rad )
	{
		m4f->m[0][0] = cosf( rad );
		m4f->m[0][1] = sinf( rad );
		m4f->m[0][2] = 0;
		m4f->m[0][3] = 0;

		m4f->m[1][0] = - m4f->m[0][1];
		m4f->m[1][1] = m4f->m[0][0];
		m4f->m[1][2] = 0;
		m4f->m[1][3] = 0;

		m4f->m[2][0] = 0;
		m4f->m[2][1] = 0;
		m4f->m[2][2] = 1;
		m4f->m[2][3] = 0;

		m4f->m[3][0] = 0;
		m4f->m[3][1] = 0;
		m4f->m[3][2] = 0;
		m4f->m[3][3] = 1;
	}
	static void MakeMultiplication( Mat4Float* m4f, const Mat4Float* m1, const Mat4Float* m2 )
	{
		UINT row, col;
		Mat4Float result;
		for ( row = 0; row < 4; ++row )
		{
			for ( col = 0; col < 4; ++col )
			{
				result.m[row][col] =	m1->m[row][0] * m2->m[0][col]
									+	m1->m[row][1] * m2->m[1][col]
									+	m1->m[row][2] * m2->m[2][col]
									+	m1->m[row][3] * m2->m[3][col];
			}
		}
		*m4f = result;
	}

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
