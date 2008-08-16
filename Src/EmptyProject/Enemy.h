#pragma once
#include "character.h"

class Enemy :
	public Character
{
public:
	
	virtual ~Enemy(void);

	static Unit* createEnemy( LPD3DXMESH mesh, int tileX, int tileY, float posZ );

	virtual HRESULT frameRender();
	virtual bool frameMove( float fElapsedTime );
	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void setRandomWalkable( bool randomWalkable ) { m_bRandomWalkable = randomWalkable; }
	void setTalkable( bool talkable ) { m_bTalkable = talkable; }
	bool isTalkable() { return m_bTalkable; }
	void setFightRange( int i ) { m_iFightRange = i; }
	int	getFightRange() { return m_iFightRange; }

private:
	Enemy();
	void walkRandomly();

	bool m_bRandomWalkable;
	bool m_bTalkable;
	int m_iFightRange;
};

Unit* EpCreateEnemy( int tileX, int tileY );

SCRIPT_FACTORY( Enemy )
