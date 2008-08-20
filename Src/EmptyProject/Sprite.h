#pragma once

class SpriteManager;

typedef std::map<std::string, RECT> RectMap;
class DrawRequest
{

public:
	DrawRequest( bool bXformable ) : bXformable( bXformable ) {}
	void release() {}
	const bool bXformable;
	RECT srcRect;
	D3DXVECTOR3 center;
	D3DXVECTOR3 position;
	D3DXMATRIX xform;
	D3DCOLOR color;
private:
	DrawRequest operator = ( const DrawRequest& rhs )
	{
		srcRect		= rhs.srcRect;
		center		= rhs.center;
		position	= rhs.position;
		xform		= rhs.xform;
		color		= rhs.color;
		return *this;
	}
};

class Sprite
{
public:
	Sprite( LPDIRECT3DTEXTURE9 d3dTex );
	~Sprite(void);

	void release();
	void registerRect( const char* rectName, const RECT& rect );
	void registerRect( const char* rectName, long left, long top, long right, long bottom );
	DrawRequest* drawRequest( const char* rectName, const D3DXVECTOR3* center, const D3DXVECTOR3* position, D3DCOLOR color );
	DrawRequest* drawRequestXformable( const char* rectName );
	LPDIRECT3DTEXTURE9 getTexture() const { assert( m_d3dTex ); return m_d3dTex; }

	void removeDrawRequest( DrawRequest*& dr );
private:
	friend class SpriteManager;

	
	typedef std::list<DrawRequest*> DrawRequestList;


	void draw();
	const DrawRequestList& getDrawRequestList() const { return m_drawReqList; }
	const DrawRequestList& getDrawReqXformableList() const { return m_drawReqXformableList; }

	RectMap m_rectMap;
	LPDIRECT3DTEXTURE9 m_d3dTex;
	
	DrawRequestList m_drawReqList;				// Screen space sprite drawing
	DrawRequestList m_drawReqXformableList;		// Object(3D) space sprite drawing

};
