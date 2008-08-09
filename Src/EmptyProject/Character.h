#pragma once
#include "Unit.h"

class AttackObject;

class Character : public Unit
{
public:
	static Unit* createCharacter( LPD3DXMESH mesh, float posX = 0, float posY = 0, float posZ = 0, bool bCtrl = false );
	~Character();

	void attack(int type, Character* enemy);

	virtual HRESULT frameRender();
	virtual bool frameMove( float fElapsedTime );

	void setMaxAndCurHp( int maxHp, int curHp ) { assert( maxHp >= 0 && curHp >= 0 && maxHp >= curHp ); m_maxHp = maxHp; m_curHp = curHp; }
	int getCurHp() const { return m_curHp; }
	int getMaxHp() const { return m_maxHp; }
	void damage( int point ) { m_curHp -= point; }

protected:
	Character();

private:
	typedef std::list<AttackObject*> AttackObjectList;
	AttackObjectList m_attackObjectList;


	int						m_maxHp;
	int						m_curHp;
};
