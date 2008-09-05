#pragma once
#include "Unit.h"
#include "TileManager.h"


class SkillSet;
class SkillObject;
class Trigger;

typedef std::list<SkillObject*> SkillObjectList;

enum SkillLocation;

//고정적 스탯. 레벨 업 시만 오르는.(simon ornen)
struct Stat
{
	int health, will, coding, def, sense, immunity;
};

class Character : public Unit
{
public:
	// Ctor and Dtor and create
	static Unit*						createCharacter( LPD3DXMESH mesh, int tileX, int tileY, float posZ );
	virtual								~Character();

	// Virtual Methods
	virtual HRESULT						frameRender( double dTime, float fElapsedTime );
	virtual bool						frameMove( float fElapsedTime );
	virtual LRESULT						handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual const D3DXVECTOR3&			getPos() const { if ( m_moveImpulse != DX_CONSTS::D3DXVEC3_ZERO ) return m_curPos; else return Unit::getPos(); }
	virtual void						pushUnitInFront( UnitInput dir );

	/*스킬 함수*/
	//void doNormalAttack(int type, Character* enemy);

	void								doCsBurn();
	void								recoverCs ();
	void								heal (int point);

	

	void								setCurHp( int curHp );
	int									getCurHp() const			{ return m_curHp; }
	int									getMaxHp() const			{ return m_maxHp; }
	void								setCurCs( int curCs );
	int									getCurCs() const			{ return m_curCs; }
	int									getMaxCs() const			{ return m_maxCs; }

	void								damage( int point );
	bool								getMoving() const			{ return m_bMoving; }
	const Stat&							getStat() const				{ return m_stat; }
	bool								isDead() const				{ return (m_curHp<=0); }

	void								setDead();
	void								addMoveImpulse( const D3DXVECTOR3& impulse );
	
	void								setMoveDuration( float val ) { m_moveDuration = val; }	
	void								setBoundaryRect( LONG left, LONG top, LONG right, LONG bottom );
	

	void								setStat( int statHealth, int statWill, int statCoding, int statDef, int statSen, int statImmu );

	void								setControllable(bool bCtrl)		{ m_bControllable = bCtrl; }

	void								pushSkillObject (SkillObject* skillObj);
	const SkillSet*						getSkillSet() const { return m_skillSet; }
	bool								deleteSkill( SkillLocation skillLoc );
	
protected:
										Character( UnitType type );
	
private:
	void								boundaryTesting( UnitInput );

	bool								m_bMoving;
	float								m_fMovingTime;

	//유동적 스탯.
	int									m_maxHp;
	int									m_curHp;
	int									m_maxCs;
	int									m_curCs;

	float								m_moveDuration; // A time needed to move one time(tile) in seconds
	
	TileRegion							m_boundaryTileRect;

	// Move amount that applied instantly and decimated constantly on every frameMove()
	D3DXVECTOR3							m_moveImpulse;
	D3DXVECTOR3							m_curPos;

	Stat								m_stat;

	bool								m_bControllable;

	SkillSet*							m_skillSet;
	SkillObjectList						m_skillObjectList;

};


SCRIPT_FACTORY( Character )
