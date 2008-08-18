#include "EmptyProjectPCH.h"
#include "DynamicMotion.h"

bool DynamicMotion::frameMove(float fElapsedTime)
{
	return true;
}

DynamicMotion* DynamicMotion::createDMfireUniformly ( Unit* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist, float velocity )
{
	DynamicMotion* dm = new DMfireUniformly (target, initPos, fireDir, retainDist, velocity);
	return dm;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DMfireUniformly::frameMove(float fElapsedTime)
{
	return true;
}

DMfireUniformly::DMfireUniformly ( Unit* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist, float velocity )
{
	m_target = target;
	m_initPos = initPos;
	m_fireDir = fireDir;
	m_retainDist = retainDist;
	m_velocity = velocity;
}
