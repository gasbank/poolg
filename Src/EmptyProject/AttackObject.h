#pragma once
#include "Unit.h"
#include <string>

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

	void setDamage(int value) {m_damage = value;}

private:
	Character* m_target;
	D3DXVECTOR3 m_initPos;
	D3DXVECTOR3 m_fireDir;
	float m_retainDist;
	int m_damage;
	//int m_manaConsumption;

	BallAttackObject( Character* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist );
};

class HealObject : public AttackObject
{
public:
	static AttackObject* createHealObject( Character* target );
	HealObject( Character* target );
	~HealObject();

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool frameMove( float fElapsedTime );

private:
	Character* m_target;
	D3DXVECTOR3 m_fireDir;
	int m_healPoint;
	float m_radius;
	float m_radiusVelocity;
	float m_angle;
	float m_angularVelocity;

};

class CsBurnObject : public AttackObject
{
public:
	static AttackObject* createCsBurnObject( Character* target );
	CsBurnObject( Character* target );
	~CsBurnObject();

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool frameMove( float fElapsedTime );

private:
	Character* m_target;
	D3DXVECTOR3 m_fireDir;
	float m_radius;
	float m_radiusVelocity;
	float m_angle;
	float m_angularVelocity;
};


class MeditationObject : public AttackObject
{
public:
	static AttackObject* createMeditationObject( Character* target );
	MeditationObject( Character* target );
	~MeditationObject();

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool frameMove( float fElapsedTime );

private:
	Character* m_target;
	D3DXVECTOR3 m_fireDir;
	int m_meditationPoint;
	float m_radius;
	float m_radiusVelocity;
	float m_angle;
	float m_angularVelocity;

};
/*
class MeditationObject : public AttackObject
{
public:
	static AttackObject* createMeditationObject( Character* target , int index);
	MeditationObject( Character* target );
	~MeditationObject();

	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool frameMove( float fElapsedTime );

private:
	Character* m_target;
	D3DXVECTOR3 m_fireDir;
	float m_velocity;
	float m_accelRate;
	int m_meditationPoint;
	int m_index;
}
*/