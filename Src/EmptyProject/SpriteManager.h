#pragma once

class Sprite;

typedef std::map<std::string, Sprite*> SpriteMap;

class SpriteManager : public Singleton<SpriteManager>
{
public:
	SpriteManager( LPDIRECT3DDEVICE9 dev );
	~SpriteManager(void);
	
	void frameRender();
	Sprite* registerSprite( const char* spriteName, const char* spriteFileName );

	Sprite* getSprite( const char* spriteName ) const;

private:
	void init();
	void release();

	LPDIRECT3DDEVICE9		m_dev;
	LPD3DXSPRITE			m_d3dxSprite;			// Screen space sprite
	LPD3DXSPRITE			m_d3dxObjectSprite;		// Object space sprite
	SpriteMap				m_spriteMap;
};
