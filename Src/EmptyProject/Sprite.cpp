#include "EmptyProjectPCH.h"
#include "Sprite.h"
#include "ArnConsts.h"
#include "ArnTextureDx9.h"

Sprite::Sprite( const char* texFileName )
: m_d3dTex (0)
, m_bCustomRendered (false)
, m_texFileName (texFileName)
{
	m_d3dTex = ArnTexture::createFrom (m_texFileName.c_str ());
	m_d3dTex->init();
}

Sprite::~Sprite(void)
{
	delete m_d3dTex;
}

void Sprite::release()
{
	EpSafeReleaseAll (m_drawReqList);
	EpSafeReleaseAll (m_drawReqXformableList);
}

void Sprite::registerRect( const char* rectName, const RECT& rect )
{
	if ( m_rectMap.find( rectName ) == m_rectMap.end() )
		m_rectMap[ rectName ] = rect;
	else
		throw std::runtime_error( "Duplicate rectName already exist" );
}

void Sprite::resizeRect( const char* rectName, long left, long top, long right, long bottom )
{
	RECT rect;
	rect.left	= left;
	rect.top	= top;
	rect.right	= right;
	rect.bottom = bottom;

	if ( m_rectMap.find( rectName ) == m_rectMap.end() )
		throw std::runtime_error( "There`s no rectName exist" );
	else
		m_rectMap[ rectName ] = rect;

}

void Sprite::registerRect( const char* rectName, long left, long top, long right, long bottom )
{
	RECT rect;
	rect.left	= left;
	rect.top	= top;
	rect.right	= right;
	rect.bottom = bottom;

	registerRect( rectName, rect );
}
DrawRequest* Sprite::drawRequest( const char* rectName, const ArnVec3* center, const ArnVec3* position, ArnColorValue color )
{
	DrawRequest* dr = new DrawRequest( false );
	RectMap::iterator it = m_rectMap.find( rectName );
	if ( it != m_rectMap.end() )
		dr->srcRect = m_rectMap[ rectName ];
	else
		throw std::runtime_error( "Specified rectName does not exist" );

	if ( center ) dr->center = *center;
	else dr->center = ArnConsts::ARNVEC3_ZERO;

	if ( position ) dr->position = *position;
	else dr->position = ArnConsts::ARNVEC3_ZERO;

	dr->color = color;
	dr->xform = ArnConsts::ARNMAT_IDENTITY;

	m_drawReqList.push_back( dr );
	return dr;
}

DrawRequest* Sprite::drawRequest( const char* rectName, const ArnVec3* center, int posX, int posY, int posZ, ArnColorValue color )
{
	ArnVec3 vPos = CreateArnVec3( (float)posX, (float)posY, (float)posZ );
	return drawRequest( rectName, center, &vPos, color );
}

DrawRequest* Sprite::drawRequest( const char* rectName, ScreenPosition spe, ArnColorValue color )
{
	// TODO: Screen width and height
	int scrWidth = 800;
	int scrHeight = 600;
	ArnVec3 vPos = ArnConsts::ARNVEC3_ZERO;
	const RECT& imgRect = m_rectMap[ rectName ];
	int imgWidth = imgRect.right - imgRect.left;
	int imgHeight = imgRect.bottom - imgRect.top;
	assert( imgWidth > 0 && imgHeight > 0 );

	switch ( spe )
	{
	case LEFT_TOP:		vPos.x = 0;								vPos.y = 0;									break;
	case MID_TOP:		vPos.x = (scrWidth-imgWidth)/2.0f;		vPos.y = 0;									break;
	case RIGHT_TOP:		vPos.x = (float)(scrWidth - imgWidth);	vPos.y = 0;									break;
	case LEFT_MID:		vPos.x = 0;								vPos.y = (scrHeight - imgHeight)/2.0f;		break;
	case MID_MID:		vPos.x = (scrWidth-imgWidth)/2.0f;		vPos.y = (scrHeight - imgHeight)/2.0f;		break;
	case RIGHT_MID:		vPos.x = (float)(scrWidth - imgWidth);	vPos.y = (scrHeight - imgHeight)/2.0f;		break;
	case LEFT_BOTTOM:	vPos.x = 0;								vPos.y = (float)(scrHeight - imgHeight);	break;
	case MID_BOTTOM:	vPos.x = (scrWidth-imgWidth)/2.0f;		vPos.y = (float)(scrHeight - imgHeight);	break;
	case RIGHT_BOTTOM:	vPos.x = (float)(scrWidth - imgWidth);	vPos.y = (float)(scrHeight - imgHeight);	break;
	}

	return drawRequest( rectName, 0, &vPos, color );
}
DrawRequest* Sprite::drawRequestXformable( const char* rectName )
{
	DrawRequest* dr = new DrawRequest( true );
	RectMap::iterator it = m_rectMap.find( rectName );
	if ( it != m_rectMap.end() )
		dr->srcRect = m_rectMap[ rectName ];
	else
		throw std::runtime_error( "Specified rectName does not exist" );

	dr->center.x = (dr->srcRect.right - dr->srcRect.left + 1) * 0.5f;
	dr->center.y = (dr->srcRect.bottom - dr->srcRect.top + 1) * 0.5f;
	dr->center.z = 0;
	dr->color = 0xffffffff;
	dr->position = ArnConsts::ARNVEC3_ZERO;
	dr->xform = ArnConsts::ARNMAT_IDENTITY;

	m_drawReqXformableList.push_back( dr );
	return dr;
}

void Sprite::removeDrawRequest( DrawRequest*& dr )
{
	if ( !dr->bXformable )
		m_drawReqList.remove( dr );
	else
		m_drawReqXformableList.remove( dr );

	EP_SAFE_release( dr );
}

HRESULT Sprite::onResetDevice()
{
	ArnDetachRenderableObjects (m_d3dTex);
	assert(m_d3dTex);
	ArnRenderableObject *renderable = ArnTextureDx9::createFrom (m_d3dTex);
	assert (renderable);
	m_d3dTex->attachChild (renderable);
	return 0;
}

void Sprite::onLostDevice()
{
	ArnDetachRenderableObjects (m_d3dTex);
}

void Sprite::clearDrawRequest()
{
	EpSafeReleaseAll( m_drawReqList );
}
