#pragma once
#include "Unit.h"


class DynamicMotion
{
public:
	virtual bool frameMove( float fElapsedTime );

	static DynamicMotion* createDMfireUniformly ( Unit* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist, float velocity );
	static DynamicMotion* createDMspinAround (Unit* target, const D3DXVECTOR3& fireDest, float radius, float radiusVelocity, float angularVelocity);


protected:
	Unit* m_target;
};


class DMfireUniformly : public DynamicMotion
{
public:
	virtual bool frameMove( float fElapsedTime );

	friend class DynamicMotion;

private:
	DMfireUniformly ( Unit* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist,  float velocity );

	D3DXVECTOR3 m_initPos;
	D3DXVECTOR3 m_fireDir;
	float m_retainDist;
	float m_velocity;
};

class DMspinAround :public DynamicMotion
{
public:
	virtual bool frameMove( float fElapsedTime );

	friend class DynamicMotion;

private:
	DMspinAround ( Unit* target, const D3DXVECTOR3& fireDest, float radius, float radiusVelocity, float angularVelocity );

	D3DXVECTOR3 m_fireDest;
	float m_radius;
	float m_radiusVelocity;
	float m_angle;
	float m_angularVelocity;
};
