#include "EmptyProjectPCH.h"
#include "SpriteManager.h"
#include "Sprite.h"

IMPLEMENT_SINGLETON( SpriteManager )

SpriteManager::SpriteManager( LPDIRECT3DDEVICE9 dev )
: m_dev( dev ), m_d3dxSprite( 0 ), m_d3dxObjectSprite( 0 )
{
	init();
}

SpriteManager::~SpriteManager(void)
{
	release();
}


void SpriteManager::init()
{
	D3DXCreateSprite( m_dev, &m_d3dxSprite );
	D3DXCreateSprite( m_dev, &m_d3dxObjectSprite );
}

void SpriteManager::release()
{
	SAFE_RELEASE( m_d3dxSprite );
	SAFE_RELEASE( m_d3dxObjectSprite );

	SpriteMap::iterator it = m_spriteMap.begin();
	for ( ; it != m_spriteMap.end(); ++it )
	{
		EP_SAFE_RELEASE( it->second );
	}
	m_spriteMap.clear();
}

void SpriteManager::frameRender()
{

	m_d3dxSprite->Begin( D3DXSPRITE_ALPHABLEND );
	{
		SpriteMap::iterator it = m_spriteMap.begin();	
		for ( ; it != m_spriteMap.end(); ++it )
		{
			const Sprite::DrawRequestList& dr = it->second->getDrawRequestList();
			if ( dr.size() )
			{
				Sprite::DrawRequestList::const_iterator itDr = dr.begin();
				const DrawRequest* drawReq = *itDr;
				for ( ; itDr != dr.end(); ++itDr )
				{
					m_d3dxSprite->Draw( it->second->getTexture(), &drawReq->srcRect, &drawReq->center, &drawReq->position, drawReq->color );
				}
			}

		}
	}
	m_d3dxSprite->End();

	//////////////////////////////////////////////////////////////////////////
	
	m_d3dxObjectSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE );
	{
		m_dev->SetTransform( D3DTS_VIEW, GetG().m_camera.GetViewMatrix() );
		m_dev->SetTransform( D3DTS_PROJECTION, GetG().m_camera.GetProjMatrix() );
		SpriteMap::iterator it = m_spriteMap.begin();
		for ( ; it != m_spriteMap.end(); ++it )
		{
			const Sprite::DrawRequestList& dr = it->second->getDrawReqXformableList();
			if ( dr.size() )
			{
				Sprite::DrawRequestList::const_iterator itDr = dr.begin();
				const DrawRequest* drawReq = *itDr;
				for ( ; itDr != dr.end(); ++itDr )
				{
					m_dev->SetTransform( D3DTS_WORLD, &drawReq->xform );
					m_d3dxObjectSprite->Draw( it->second->getTexture(), &drawReq->srcRect, &drawReq->center, &drawReq->position, drawReq->color );
				}
			}
		}
	}	
	m_d3dxObjectSprite->End();

}

Sprite* SpriteManager::registerSprite( const char* spriteName, const char* spriteFileName )
{
	HRESULT hr;

	SpriteMap::iterator it = m_spriteMap.find( spriteName );
	if ( it == m_spriteMap.end() )
	{
		LPDIRECT3DTEXTURE9 d3dTex = 0;
		V( D3DXCreateTextureFromFileA( m_dev, spriteFileName, &d3dTex ) );
		Sprite* sprite = new Sprite( d3dTex );
		m_spriteMap[ spriteName ] = sprite;
		return sprite;
	}	
	else
		throw std::runtime_error( "Duplicate sprite name already exist!" );
	
}


Sprite* SpriteManager::getSprite( const char* spriteName ) const
{
	SpriteMap::const_iterator it = m_spriteMap.find( spriteName );
	if ( it != m_spriteMap.end() )
		return it->second;
	else
		throw std::runtime_error( "Requested sprite name does not exist" );
}
