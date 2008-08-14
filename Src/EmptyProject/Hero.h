#pragma once
#include "character.h"

class Hero :
	public Character
{
public:
	Hero( void );
	virtual ~Hero( void );

	static Unit* createHero( LPD3DXMESH mesh, int tileX, int tileY, float posZ );

	virtual HRESULT frameRender();
	virtual bool frameMove( float fElapsedTime );
	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual void pushUnitInFront( UnitInput dir );
};
