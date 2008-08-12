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

	void setMaxAndCurHp( int maxHp, int curHp ) { assert( maxHp >= 0 && curHp >= 0 && maxHp >= curHp ); m_maxHp = maxHp; m_curHp = curHp; }
	int getCurHp() const { return m_curHp; }
	int getMaxHp() const { return m_maxHp; }
	void damage( int point ) { m_curHp -= point; }
	int getAttack() const { return m_attack; }
	void setAttack (int point) { m_attack = point; }

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
protected:
	Character();

private:
	HRESULT rayTesting( UnitInput );

	typedef std::list<AttackObject*> AttackObjectList;
	AttackObjectList m_attackObjectList;

	bool					m_bMoving;
	bool					m_bTalkable;
	float					m_fMovingTime;
	int						m_maxHp;
	int						m_curHp;
	float					m_moveDuration; // A time needed to move one time(tile) in seconds
	int						m_tileX;
	int						m_tileY;
	int						m_attack;
	int						m_tileBufferX;
	int						m_tileBufferY;
};
