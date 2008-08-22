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

DynamicMotion* DynamicMotion::createDMspinAround (Unit* target, const D3DXVECTOR3& fireDest, float radius, float radiusVelocity, float angularVelocity)
{
	DynamicMotion* dm = new DMspinAround (target, fireDest, radius, radiusVelocity, angularVelocity);
	return dm;
}

DynamicMotion* DynamicMotion::createDMpuff (Unit* target, const D3DXVECTOR3& initPos, float puffRate, float puffSpeed)
{
	DynamicMotion* dm = new DMpuff (target, initPos, puffRate, puffSpeed);
	return dm;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DMfireUniformly::frameMove(float fElapsedTime)
{
	D3DXVECTOR3 newPos = m_target->getPos() + m_fireDir * m_velocity * fElapsedTime;
	D3DXVECTOR3 vMoved = m_initPos - newPos;
	
	float distToInitPos = D3DXVec3Length( &vMoved );
//	m_target->setRotY(distToInitPos);
	if ( distToInitPos > m_retainDist )
	{
		return false;
	}
	m_target->setPos(newPos);
	return true;
}

DMfireUniformly::DMfireUniformly ( Unit* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist, float velocity )
{
	m_target = target;
	m_initPos = initPos;
	m_fireDir = fireDir;
	m_retainDist = retainDist;
	m_velocity = velocity;

	m_target->setPos(m_initPos);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DMspinAround::frameMove(float fElapsedTime)
{
	/*
	D3DXVECTOR3 newPos = m_target->getPos() + m_fireDir * m_velocity;
	D3DXVECTOR3 vMoved = m_initPos - newPos;
	float distToInitPos = D3DXVec3Length( &vMoved );
	if ( distToInitPos > m_retainDist )
	{
		return false;
	}
	m_target->setPos(newPos);*/

	D3DXVECTOR3 newPos;
	m_radius -= m_radiusVelocity;
	m_angle += m_angularVelocity;
	float angle = D3DXToRadian (m_angle);
	newPos.x = m_fireDest.x + m_radius * cos (angle);
	newPos.y = m_fireDest.y + m_radius * sin (angle);
	newPos.z = m_target->getPos().z - (m_radiusVelocity/1.5f);

	if ( m_radius <= 0 )
	{
		// Hit to the target!
		return false;
	}

	m_target->setPos( newPos );
	return true;
}

DMspinAround::DMspinAround ( Unit* target, const D3DXVECTOR3& fireDest, float radius, float radiusVelocity, float angularVelocity )
{
	m_target = target;
	m_fireDest = fireDest;
	m_radius = radius;
	m_radiusVelocity = radiusVelocity;
	m_angle = 0;
	m_angularVelocity = angularVelocity;

	float angle = D3DXToRadian (m_angle);

	D3DXVECTOR3 newPos;

	newPos.x = target->getPos().x + m_radius * cos (angle);
	newPos.y = target->getPos().y + m_radius * sin (angle);
	newPos.z = target->getPos().z;
	//m_fireDest = target->getPos ();

	target->setPos( newPos );
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DMpuff::frameMove(float fElapsedTime)
{
	m_scaleX += m_puffSpeed;
	m_scaleY += m_puffSpeed;
	m_scaleZ += m_puffSpeed;

	m_target->setScaleX(m_scaleX);
	m_target->setScaleY(m_scaleY);
	m_target->setScaleZ(m_scaleZ);

	if ( m_scaleX > m_puffRate )
	{
		// Hit to the target!
		return false;
	}

	return true;
}

DMpuff::DMpuff (Unit* target, const D3DXVECTOR3& initPos, float puffRate, float puffSpeed)
{
	m_target = target;
	m_puffRate = puffRate;
	m_puffSpeed = puffSpeed;

	m_target->setPos (initPos);
	m_scaleX = 1;
	m_scaleY = 1;
	m_scaleZ = 1;
}


