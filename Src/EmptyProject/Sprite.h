#pragma once

class SpriteManager;
class ArnTexture;

typedef std::map<std::string, RECT> RectMap;
class DrawRequest
{

public:
	DrawRequest( bool bXformable ) : bXformable( bXformable ), bRender(true) {}
	void release() {}
	const bool bXformable;
	RECT srcRect;
	ArnVec3 center;
	ArnVec3 position;
	ArnMatrix xform;
	ArnColorValue color;
	bool bRender;
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
@brief Representing a single 2D image. (Textured quad)

See SpriteManager
*/
class Sprite
{
public:
	enum ScreenPosition {
		LEFT_TOP, MID_TOP, RIGHT_TOP,
		LEFT_MID, MID_MID, RIGHT_MID,
		LEFT_BOTTOM, MID_BOTTOM, RIGHT_BOTTOM
	};
									Sprite( const char* texFileName );
									~Sprite(void);
	void							release();
	void							registerRect( const char* rectName, const RECT& rect );
	void							registerRect( const char* rectName, long left, long top, long right, long bottom );
	void							resizeRect( const char* rectName, long left, long top, long right, long bottom );
	DrawRequest*					drawRequest( const char* rectName, const ArnVec3* center, const ArnVec3* position, ArnColorValue color );
	DrawRequest*					drawRequest( const char* rectName, const ArnVec3* center, int posX, int posY, int posZ, ArnColorValue color );
	DrawRequest*					drawRequest( const char* rectName, ScreenPosition spe, ArnColorValue color );
	DrawRequest*					drawRequestXformable( const char* rectName );
	ArnTexture*						getTexture() const { assert( m_d3dTex ); return m_d3dTex; }
	const ArnRenderableObject*		getRenderableObject() const { return m_d3dTex->getRenderableObject(); }
	void							clearDrawRequest();
	void							removeDrawRequest( DrawRequest*& dr );
	bool							isCustomRendered() const { return m_bCustomRendered; }
	void							setCustomRendered(bool val) { m_bCustomRendered = val; }
	HRESULT							onResetDevice();
	void							onLostDevice();
private:
	friend class SpriteManager;

	typedef std::list<DrawRequest*> DrawRequestList;

	Sprite&							operator = ( const Sprite& );
	const DrawRequestList&			getDrawRequestList() const { return m_drawReqList; }
	const DrawRequestList&			getDrawReqXformableList() const { return m_drawReqXformableList; }
	RectMap							m_rectMap;
	ArnTexture*						m_d3dTex;
	DrawRequestList					m_drawReqList;				// Screen space sprite drawing
	DrawRequestList					m_drawReqXformableList;		// Object(3D) space sprite drawing
	bool							m_bCustomRendered;
	const std::string				m_texFileName;
};
