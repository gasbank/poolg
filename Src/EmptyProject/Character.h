#pragma once
#include "Unit.h"

class AttackObject;
class Trigger;

struct Stat
{
	int health, will, coding, def, sense, immunity;
};

class Character : public Unit
{
public:
	static Unit* createCharacter( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool bControllable );
	virtual ~Character();

	/*��ų �Լ�*/
	void attack(int type, Character* enemy);
	void throwHealBall();
	void csBurn();
	void meditation();

	void recoverCs();
	void heal (int point);

	virtual HRESULT frameRender();
	virtual bool frameMove( float fElapsedTime );
	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual const D3DXVECTOR3& getPos() const { return m_curPos; }

	void setCurHp( int curHp );
	int getCurHp() const { return m_curHp; }
	int getMaxHp() const { return m_maxHp; }
	void setCurCs( int curCs );
	int getCurCs() const { return m_curCs; }
	int getMaxCs() const { return m_maxCs; }
	void damage( int point );
	int getAttack() const { return m_attack; }
	int getInt() const { return m_intelligence; }
	bool getMoving() { return m_bMoving; }
	void setAttack (int point) { m_attack = point; }
	void setInt (int point) {m_intelligence = point; }
	const Stat& getStat() const { return m_stat; }
	bool isDead() { return (m_curHp<=0); }
	void setDead() { m_curHp = -1; }
	void addMoveImpulse( const D3DXVECTOR3& impulse ) { m_moveImpulse += impulse; }
	
	void setMoveDuration( float val ) { m_moveDuration = val; }
	

	void setBoundaryRect( LONG left, LONG top, LONG right, LONG bottom );

	virtual void pushUnitInFront( UnitInput dir );

	void setStat( int statHealth, int statWill, int statCoding, int statDef, int statSen, int statImmu );
protected:
	Character( UnitType type );

private:
	void boundaryTesting( UnitInput );

	typedef std::list<AttackObject*> AttackObjectList;
	AttackObjectList m_attackObjectList;

	bool					m_bMoving;
	float					m_fMovingTime;
	int						m_maxHp;
	int						m_curHp;
	int						m_maxCs;
	int						m_curCs;
	float					m_moveDuration; // A time needed to move one time(tile) in seconds
	
	int						m_attack;		// �Ϲ� ���� ��ų�� ����
	int						m_intelligence; // ���� ���� ��ų�� ����
	
	RECT					m_boundaryTileRect;

	// Move amount that applied instantly and decimated constantly on every frameMove()
	D3DXVECTOR3				m_moveImpulse;
	D3DXVECTOR3				m_curPos;

	Stat					m_stat;
};

Unit* EpCreateCharacter( int tileX, int tileY, int controllable );

SCRIPT_FACTORY( Character )
