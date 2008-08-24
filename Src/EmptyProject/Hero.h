#pragma once
#include "character.h"
enum SkillLocation;

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
	int							gainExp (int expPoint);
	void						levelUp ();
	int							getCurExp () const	{ return m_curExp; }
	int							getMaxExp () const	{ return m_maxExp; }
	int							getLevel () const	{ return m_level; }

	int						m_maxExp;
	int						m_curExp;
	int						m_expGap;
	int						m_level;

	//virtual void pushUnitInFront( UnitInput dir );

	void						addToSkillSet( SkillLocation sl );

private:
								Hero( void );
};

SCRIPT_FACTORY( Hero )
