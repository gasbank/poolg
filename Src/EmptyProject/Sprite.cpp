#include "EmptyProjectPCH.h"
#include "Sprite.h"

Sprite::Sprite( LPDIRECT3DTEXTURE9 d3dTex )
: m_d3dTex( d3dTex )
{

}
Sprite::~Sprite(void)
{
	
}

void Sprite::release()
{
	SAFE_RELEASE( m_d3dTex );
	EpSafeReleaseAll( m_drawReqList );
}

void Sprite::registerRect( const char* rectName, const RECT& rect )
{
	if ( m_rectMap.find( rectName ) == m_rectMap.end() )
		m_rectMap[ rectName ] = rect;
	else
		throw std::runtime_error( "Duplicate rectName already exist" );
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
DrawRequest* Sprite::drawRequest( const char* rectName, const D3DXVECTOR3* center, const D3DXVECTOR3* position, D3DCOLOR color )
{
	DrawRequest* dr = new DrawRequest();
	RectMap::iterator it = m_rectMap.find( rectName );
	if ( it != m_rectMap.end() )
		dr->srcRect = m_rectMap[ rectName ];
	else
		throw std::runtime_error( "Specified rectName does not exist" );
	
	if ( center ) dr->center = *center;
	else dr->center = DX_CONSTS::D3DXVEC3_ZERO;

	if ( position ) dr->position = *position;
	else dr->position = DX_CONSTS::D3DXVEC3_ZERO;

	dr->color = color;
	dr->xform = DX_CONSTS::D3DXMAT_IDENTITY;
		
	m_drawReqList.push_back( dr );
	return dr;
}

DrawRequest* Sprite::drawRequestXformable( const char* rectName )
{
	DrawRequest* dr = new DrawRequest();
	RectMap::iterator it = m_rectMap.find( rectName );
	if ( it != m_rectMap.end() )
		dr->srcRect = m_rectMap[ rectName ];
	else
		throw std::runtime_error( "Specified rectName does not exist" );
	
	dr->center = DX_CONSTS::D3DXVEC3_ZERO;
	dr->color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	dr->position = DX_CONSTS::D3DXVEC3_ZERO;
	dr->xform = DX_CONSTS::D3DXMAT_IDENTITY;

	m_drawReqXformableList.push_back( dr );
	return dr;
}

void Sprite::removeDrawRequest( DrawRequest*& dr )
{
	m_drawReqList.remove( dr );
	EP_SAFE_RELEASE( dr );
}