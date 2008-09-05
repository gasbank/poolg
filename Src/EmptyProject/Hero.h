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
	virtual bool				frameMove( float fElapsedTime );
	virtual LRESULT				handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	int							gainExp (int expPoint);
	void						levelUp ();
	int							getCurExp () const	{ return m_curExp; }
	int							getMaxExp () const	{ return m_maxExp; }
	int							getLevel () const	{ return m_level; }

//	void						addToSkillSet( SkillLocation sl );
	bool						isEncounterEnemy() const { return m_bEncounterEnemy; }
	void						setEncounterEnemy(bool val) { m_bEncounterEnemy = val; }
private:
								Hero( void );

	int							m_maxExp;
	int							m_curExp;
	int							m_expGap;
	int							m_level;
	bool						m_bEncounterEnemy;
	
};

SCRIPT_FACTORY( Hero )
