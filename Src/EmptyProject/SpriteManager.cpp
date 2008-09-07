#include "EmptyProjectPCH.h"
#include "SpriteManager.h"
#include "Sprite.h"

IMPLEMENT_SINGLETON( SpriteManager )

SpriteManager::SpriteManager()
: m_d3dxSprite( 0 ), m_d3dxObjectSprite( 0 )
{
	init();
}

SpriteManager::~SpriteManager(void)
{
	release();
}


void SpriteManager::init()
{
	
}

void SpriteManager::release()
{
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
			if ( it->second->isCustomRendered() )
				continue;

			const Sprite::DrawRequestList& dr = it->second->getDrawRequestList();
			if ( !dr.empty() )
			{
				Sprite::DrawRequestList::const_iterator itDr = dr.begin();
				for ( ; itDr != dr.end(); ++itDr )
				{
					const DrawRequest* drawReq = *itDr;
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
			if ( it->second->isCustomRendered() )
				continue;

			const Sprite::DrawRequestList& dr = it->second->getDrawReqXformableList();
			if ( !dr.empty() )
			{
				Sprite::DrawRequestList::const_iterator itDr = dr.begin();
				
				for ( ; itDr != dr.end(); ++itDr )
				{
					const DrawRequest* drawReq = *itDr;
					D3DXMATRIX centerBiased = drawReq->xform;
					*((D3DXVECTOR3*)&centerBiased._41) -= drawReq->center;
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
	UNREFERENCED_PARAMETER( hr );

	SpriteMap::iterator it = m_spriteMap.find( spriteName );
	if ( it == m_spriteMap.end() )
	{
		Sprite* sprite = new Sprite( spriteFileName );
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

void SpriteManager::frameRenderSpecificSprite( const char* spriteName )
{
	m_d3dxSprite->Begin( D3DXSPRITE_ALPHABLEND );
	Sprite* sprite = m_spriteMap[ spriteName ];
	assert( sprite->isCustomRendered() );
	const Sprite::DrawRequestList& dr = sprite->getDrawRequestList();
	
	if ( !dr.empty() )
	{
		Sprite::DrawRequestList::const_iterator itDr = dr.begin();
		for ( ; itDr != dr.end(); ++itDr )
		{
			const DrawRequest* drawReq = *itDr;
			m_d3dxSprite->Draw( sprite->getTexture(), &drawReq->srcRect, &drawReq->center, &drawReq->position, drawReq->color );
		}
	}
	m_d3dxSprite->End();

}

HRESULT SpriteManager::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									 void* pUserContext )
{
	HRESULT hr = S_OK;

	V_RETURN( m_d3dxSprite->OnResetDevice() );
	V_RETURN( m_d3dxObjectSprite->OnResetDevice() );

	SpriteMap::iterator it = m_spriteMap.begin();
	for ( ; it != m_spriteMap.end(); ++it )
	{
		it->second->onResetDevice( m_dev, pBackBufferSurfaceDesc, pUserContext );
	}
	return hr;
}

void SpriteManager::onLostDevice()
{
	m_d3dxSprite->OnLostDevice();
	m_d3dxObjectSprite->OnLostDevice();

	SpriteMap::iterator it = m_spriteMap.begin();
	for ( ; it != m_spriteMap.end(); ++it )
	{
		it->second->onLostDevice();
	}
}

HRESULT SpriteManager::onCreateDevice( IDirect3DDevice9* pd3dDevice )
{
	HRESULT hr = S_OK;
	m_dev = pd3dDevice;

	V_RETURN( D3DXCreateSprite( m_dev, &m_d3dxSprite ) );
	V_RETURN( D3DXCreateSprite( m_dev, &m_d3dxObjectSprite ) );

	return hr;
}

void SpriteManager::onDestroyDevice()
{
	m_d3dxSprite->Release();
	m_d3dxObjectSprite->Release();
}