#pragma once
#include "Unit.h"

class AttackObject;

class Character : public Unit
{
public:
	static Unit* createCharacter( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool bControllable );
	~Character();

	void attack(int type, Character* enemy);
	void heal (int point);

	virtual HRESULT frameRender();
	virtual bool frameMove( float fElapsedTime );
	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void setMaxAndCurHp( int maxHp, int curHp );
	int getCurHp() const { return m_curHp; }
	int getMaxHp() const { return m_maxHp; }
	void damage( int point ) { m_curHp -= point; }
	int getAttack() const { return m_attack; }
	int getInt() const { return m_intelligence; }
	bool getMoving() { return m_bMoving; }
	void setAttack (int point) { m_attack = point; }
	void setInt (int point) {m_intelligence = point; }

	bool isDead() { return (m_curHp<=0); }


	void setTilePos(int tileX, int tileY);
	void setTileBufferPos(int tileX, int tileY) { m_tileBufferX = tileX; m_tileBufferY = tileY; }
	int getTilePosX() { return m_tileX; }
	int getTilePosY() { return m_tileY; }
	void enterTile(int tileX, int tileY);

	void setMoveDuration( float val ) { m_moveDuration = val; }
	void setColor( int r, int g, int b );

	void setTalkable( bool talkable ) { m_bTalkable = talkable; }
	bool isTalkable() { return m_bTalkable; }

	void setRandomWalkable( bool randomWalkable ) { m_bRandomWalkable = randomWalkable; }
protected:
	Character();

private:
	HRESULT rayTesting( UnitInput );
	void walkRandomly();

	typedef std::list<AttackObject*> AttackObjectList;
	AttackObjectList m_attackObjectList;

	bool					m_bMoving;
	bool					m_bMovable;
	bool					m_bTalkable;
	bool					m_bRandomWalkable;
	float					m_fMovingTime;
	int						m_maxHp;
	int						m_curHp;
	float					m_moveDuration; // A time needed to move one time(tile) in seconds
	int						m_tileX;
	int						m_tileY;
	int						m_attack;		// 일반 공격 스킬에 영향
	int						m_intelligence; // 힐링 등의 스킬에 영향
	int						m_tileBufferX;
	int						m_tileBufferY;
};
