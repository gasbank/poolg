#pragma once
#include "Unit.h"
#include "TileManager.h"

class AttackObject;
class Trigger;

typedef std::list<AttackObject*> AttackObjectList;

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
	virtual HRESULT						frameRender();
	virtual bool						frameMove( float fElapsedTime );
	virtual LRESULT						handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual const D3DXVECTOR3&			getPos() { m_curPos = Unit::getPos() + m_moveImpulse; return m_curPos; }
	virtual void						pushUnitInFront( UnitInput dir );

	/*스킬 함수*/
	void								attack( int type, Character* enemy );
	void								throwHealBall();
	void								csBurn();
	void								meditation();

	void								recoverCs();
	void								heal( int point );

	

	void								setCurHp( int curHp );
	int									getCurHp() const			{ return m_curHp; }
	int									getMaxHp() const			{ return m_maxHp; }
	void								setCurCs( int curCs );
	int									getCurCs() const			{ return m_curCs; }
	int									getMaxCs() const			{ return m_maxCs; }
	void								damage( int point );
	int									getAttack() const			{ return m_attack; }
	int									getInt() const				{ return m_intelligence; }
	bool								getMoving() const			{ return m_bMoving; }
	void								setAttack (int point)		{ m_attack = point; }
	void								setInt (int point)			{ m_intelligence = point; }
	const Stat&							getStat() const				{ return m_stat; }
	bool								isDead() const				{ return (m_curHp<=0); }
	void								setDead();
	void								addMoveImpulse( const D3DXVECTOR3& impulse ) { m_moveImpulse += impulse; }
	
	void								setMoveDuration( float val ) { m_moveDuration = val; }	
	void								setBoundaryRect( LONG left, LONG top, LONG right, LONG bottom );
	

	void								setStat( int statHealth, int statWill, int statCoding, int statDef, int statSen, int statImmu );

	void								setControllable(bool bCtrl)		{ m_bControllable = bCtrl; }
	
protected:
										Character( UnitType type );

private:
	void								boundaryTesting( UnitInput );
	AttackObjectList					m_attackObjectList;

	bool								m_bMoving;
	float								m_fMovingTime;
	int									m_maxHp;
	int									m_curHp;
	int									m_maxCs;
	int									m_curCs;
	float								m_moveDuration; // A time needed to move one time(tile) in seconds
	
	int									m_attack;		// 일반 공격 스킬에 영향
	int									m_intelligence; // 힐링 등의 스킬에 영향
	
	TileRegion							m_boundaryTileRect;

	// Move amount that applied instantly and decimated constantly on every frameMove()
	D3DXVECTOR3							m_moveImpulse;
	D3DXVECTOR3							m_curPos;

	Stat								m_stat;

	bool								m_bControllable;
};


SCRIPT_FACTORY( Character )
