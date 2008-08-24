#pragma once

class Sprite;

typedef std::map<std::string, Sprite*> SpriteMap;

class SpriteManager : public Singleton<SpriteManager>
{
public:
	SpriteManager( LPDIRECT3DDEVICE9 dev );
	~SpriteManager(void);
	
	void frameRender();
	void frameRenderSpecificSprite( const char* spriteName );
	Sprite* registerSprite( const char* spriteName, const char* spriteFileName );

	Sprite* getSprite( const char* spriteName ) const;

	LPD3DXSPRITE getSpriteRenerer() const { return m_d3dxSprite; }
private:
	void init();
	void release();

	LPDIRECT3DDEVICE9		m_dev;
	LPD3DXSPRITE			m_d3dxSprite;			// Screen space sprite
	LPD3DXSPRITE			m_d3dxObjectSprite;		// Object space sprite
	SpriteMap				m_spriteMap;			// Automatically rendered set of Sprites. Rendering is done at the last part of frameRender.
};

inline SpriteManager& GetSpriteManager() { return SpriteManager::getSingleton(); }