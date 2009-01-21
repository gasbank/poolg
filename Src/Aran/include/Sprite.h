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


/**
@brief 2D 이미지 하나를 나타내는 클래스

3D 메시에 사용할 텍스쳐와는 별개로 2D 이미지를 찍어야 할 경우 사용하는 클래스입니다.
물론 내부적으로는 사각형 메시로 2D이미지를 출력합니다. 주로 UI를 출력하거나 단순히
2D 이미지를 보여줘야할 경우가 있을 때 사용합니다.

본 클래스는 독립적으로 사용하는 경우는 없고 SpriteManager에 등록한 후에 사용하게 됩니다.
*/
class Sprite
{
public:
	Sprite( const char* texFileName );
	~Sprite(void);

	enum ScreenPosition {
		LEFT_TOP, MID_TOP, RIGHT_TOP,
		LEFT_MID, MID_MID, RIGHT_MID,
		LEFT_BOTTOM, MID_BOTTOM, RIGHT_BOTTOM
	};

	void release();
	void registerRect( const char* rectName, const RECT& rect );
	void registerRect( const char* rectName, long left, long top, long right, long bottom );
	void resizeRect( const char* rectName, long left, long top, long right, long bottom );
	DrawRequest* drawRequest( const char* rectName, const D3DXVECTOR3* center, const D3DXVECTOR3* position, D3DCOLOR color );
	DrawRequest* drawRequest( const char* rectName, const D3DXVECTOR3* center, int posX, int posY, int posZ, D3DCOLOR color );
	DrawRequest* drawRequest( const char* rectName, ScreenPosition spe, D3DCOLOR color );
	DrawRequest* drawRequestXformable( const char* rectName );
	LPDIRECT3DTEXTURE9 getTexture() const { assert( m_d3dTex ); return m_d3dTex; }

	void clearDrawRequest();
	void removeDrawRequest( DrawRequest*& dr );

	bool isCustomRendered() const { return m_bCustomRendered; }
	void setCustomRendered(bool val) { m_bCustomRendered = val; }

	HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		                   void* pUserContext);
	void onLostDevice();
private:
	friend class SpriteManager;
	
	typedef std::list<DrawRequest*> DrawRequestList;


	const DrawRequestList& getDrawRequestList() const { return m_drawReqList; }
	const DrawRequestList& getDrawReqXformableList() const { return m_drawReqXformableList; }

	RectMap m_rectMap;
	LPDIRECT3DTEXTURE9 m_d3dTex;
	
	DrawRequestList m_drawReqList;				// Screen space sprite drawing
	DrawRequestList m_drawReqXformableList;		// Object(3D) space sprite drawing
	bool m_bCustomRendered;

	const std::string m_texFileName;

	Sprite& operator = ( const Sprite& );
};
