#pragma once
#include "Unit.h"
#include "TileManager.h"


class SkillSet;
class SkillObject;
class Trigger;

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
	virtual const D3DXVECTOR3&			getPos() const { if ( m_moveImpulse != DX_CONSTS::D3DXVEC3_ZERO ) return m_curPos; else return Unit::getPos(); }
	virtual void						pushUnitInFront( UnitInput dir );

	/*��ų �Լ�*/
	void doNormalAttack(int type, Character* enemy);
	void doHeal();
	void doCsBurn();
	void doMeditation();
	void doMultiThread(int frequency, Character* enemy);
	void doGoto(Character* enemy);


	void recoverCs();
	void heal (int point);

	

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

	SkillSet* getSkillSet () const { return m_skillSet; }
	
	
protected:
										Character( UnitType type );
	SkillSet*				m_skillSet;

private:
	void								boundaryTesting( UnitInput );

	typedef std::list<SkillObject*> SkillObjectList;
	SkillObjectList m_skillObjectList;

	bool					m_bMoving;
	float					m_fMovingTime;

	//����.
	int						m_maxHp;
	int						m_curHp;
	int						m_maxCs;
	int						m_curCs;
	int						m_maxExp;
	int						m_curExp;
	int						m_level;


	float					m_moveDuration; // A time needed to move one time(tile) in seconds
	
	
	TileRegion							m_boundaryTileRect;

	// Move amount that applied instantly and decimated constantly on every frameMove()
	D3DXVECTOR3							m_moveImpulse;
	D3DXVECTOR3							m_curPos;

	Stat								m_stat;

	bool								m_bControllable;

	
};


SCRIPT_FACTORY( Character )
