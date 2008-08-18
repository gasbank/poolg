#pragma once
#include "Unit.h"


class DynamicMotion
{
public:
	virtual bool frameMove( float fElapsedTime );

	static DynamicMotion* createDMfireUniformly ( Unit* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist, float velocity );


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
