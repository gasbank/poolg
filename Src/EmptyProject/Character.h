#pragma once
#include "Unit.h"

class AttackObject;
class Trigger;



class Character : public Unit
{
public:
	static Unit* createCharacter( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool bControllable );
	virtual ~Character();

	void attack(int type, Character* enemy);
	void heal (int point);

	virtual HRESULT frameRender();
	virtual bool frameMove( float fElapsedTime );
	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void setMaxAndCurHp( int maxHp, int curHp );
	int getCurHp() const { return m_curHp; }
	int getMaxHp() const { return m_maxHp; }
	void damage( int point );
	int getAttack() const { return m_attack; }
	int getInt() const { return m_intelligence; }
	bool getMoving() { return m_bMoving; }
	void setAttack (int point) { m_attack = point; }
	void setInt (int point) {m_intelligence = point; }

	bool isDead() { return (m_curHp<=0); }

	
	void setMoveDuration( float val ) { m_moveDuration = val; }
	void setColor( int r, int g, int b );

	void setBoundaryRect( LONG left, LONG top, LONG right, LONG bottom );

	virtual void pushUnitInFront( UnitInput dir ) {}

	virtual void enterTile( UINT tileX, UINT tileY);
protected:
	Character();

private:
	HRESULT rayTesting( UnitInput );
	void boundaryTesting( UnitInput );

	typedef std::list<AttackObject*> AttackObjectList;
	AttackObjectList m_attackObjectList;

	bool					m_bMoving;
	bool					m_bMovable;
	float					m_fMovingTime;
	int						m_maxHp;
	int						m_curHp;
	float					m_moveDuration; // A time needed to move one time(tile) in seconds
	
	int						m_attack;		// 일반 공격 스킬에 영향
	int						m_intelligence; // 힐링 등의 스킬에 영향
	
	RECT					m_boundaryTileRect;
	Trigger*				m_trigger;
};

Unit* EpCreateCharacter( int tileX, int tileY, int controllable );