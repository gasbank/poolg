#pragma once
#include "character.h"

class Hero :
	public Character
{
public:
	// Ctor and Dtor and create
	static Unit*				createHero( LPD3DXMESH mesh, int tileX, int tileY, float posZ );
	virtual						~Hero( void );

	// Virtual Methods
	virtual HRESULT				frameRender();
	virtual bool				frameMove( float fElapsedTime );
	virtual LRESULT				handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//virtual void pushUnitInFront( UnitInput dir );

private:
								Hero( void );
};

SCRIPT_FACTORY( Hero )
