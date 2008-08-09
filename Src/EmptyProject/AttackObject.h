#pragma once
#include "Unit.h"

class BattleState;

class AttackObject : public Unit
{
public:
	virtual ~AttackObject(void);

protected:
	BattleState* getBattleState();
private:
	float m_elapsedTime;
};

class BallAttackObject : public AttackObject
{
public:
	static AttackObject* createBallAttackObject( Character* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist );
	~BallAttackObject();

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool frameMove( float fElapsedTime );

private:
	Character* m_target;
	D3DXVECTOR3 m_initPos;
	D3DXVECTOR3 m_fireDir;
	float m_retainDist;

	BallAttackObject( Character* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist );
};
