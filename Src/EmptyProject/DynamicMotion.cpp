#include "EmptyProjectPCH.h"
#include "DynamicMotion.h"

DynamicMotion::DynamicMotion( Unit* motionTarget )
: m_motionTarget( motionTarget )
, m_fireUnit( 0 )
, m_targetUnit( 0 )
, m_constVelocity( 0 )
{
}

DynamicMotion::DynamicMotion( const DynamicMotion& dm )
: m_motionTarget( dm.m_motionTarget )
, m_fireUnit( dm.m_fireUnit )
, m_targetUnit( dm.m_targetUnit )
, m_constVelocity( dm.m_constVelocity )
{
}

bool DynamicMotion::frameMove(float fElapsedTime)
{
	return true;
}

DynamicMotion* DynamicMotion::createDynamicMotion( DynamicMotionType dmt )
{
	// TODO DynamicMotion factory method
	return new RandomCurveDynamicMotion( ( Unit* )0 );
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FireUniformlyDynamicMotion::frameMove(float fElapsedTime)
{
	D3DXVECTOR3 newPos = getMotionTarget()->getPos() + m_fireDir * m_velocity * fElapsedTime;
	D3DXVECTOR3 vMoved = m_initPos - newPos;
	
	float distToInitPos = D3DXVec3Length( &vMoved );
//	m_target->setRotY(distToInitPos);
	if ( distToInitPos > m_retainDist )
	{
		return false;
	}
	getMotionTarget()->setPos(newPos);
	return true;
}

FireUniformlyDynamicMotion::FireUniformlyDynamicMotion ( Unit* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist, float velocity )
: DynamicMotion( target )
{
	m_initPos = initPos;
	m_fireDir = fireDir;
	m_retainDist = retainDist;
	m_velocity = velocity;

	getMotionTarget()->setPos(m_initPos);
}

FireUniformlyDynamicMotion::FireUniformlyDynamicMotion( const FireUniformlyDynamicMotion& dm )
: DynamicMotion( dm )
{
}

DynamicMotion* FireUniformlyDynamicMotion::clone() const
{
	return new FireUniformlyDynamicMotion( *this );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SpinAroundDynamicMotion::frameMove(float fElapsedTime)
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
	newPos.z = getMotionTarget()->getPos().z - (m_radiusVelocity/1.5f);

	if ( m_radius <= 0 )
	{
		// Hit to the target!
		return false;
	}

	getMotionTarget()->setPos( newPos );
	return true;
}

SpinAroundDynamicMotion::SpinAroundDynamicMotion ( Unit* target, const D3DXVECTOR3& fireDest, float radius, float radiusVelocity, float angularVelocity )
: DynamicMotion( target )
{
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

SpinAroundDynamicMotion::SpinAroundDynamicMotion( const SpinAroundDynamicMotion& dm )
: DynamicMotion( dm )
{
}

DynamicMotion* SpinAroundDynamicMotion::clone() const
{
	return new SpinAroundDynamicMotion( *this );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool PuffDynamicMotion::frameMove(float fElapsedTime)
{
	m_scaleX += m_puffSpeed;
	m_scaleY += m_puffSpeed;
	m_scaleZ += m_puffSpeed;

	getMotionTarget()->setScaleX(m_scaleX);
	getMotionTarget()->setScaleY(m_scaleY);
	getMotionTarget()->setScaleZ(m_scaleZ);

	if ( m_scaleX > m_puffRate )
	{
		// Hit to the target!
		return false;
	}

	return true;
}

PuffDynamicMotion::PuffDynamicMotion (Unit* target, const D3DXVECTOR3& initPos, float puffRate, float puffSpeed)
: DynamicMotion( target )
{
	m_puffRate = puffRate;
	m_puffSpeed = puffSpeed;

	getMotionTarget()->setPos (initPos);
	m_scaleX = 1;
	m_scaleY = 1;
	m_scaleZ = 1;
}

PuffDynamicMotion::PuffDynamicMotion( const PuffDynamicMotion& dm )
: DynamicMotion( dm )
{
}

DynamicMotion* PuffDynamicMotion::clone() const
{
	return new PuffDynamicMotion( *this );
}
//////////////////////////////////////////////////////////////////////////


RandomCurveDynamicMotion::RandomCurveDynamicMotion( Unit* motionTarget )
: DynamicMotion( motionTarget )
, m_dummy( 1985.56f )
{
}

RandomCurveDynamicMotion::RandomCurveDynamicMotion( const RandomCurveDynamicMotion& dm )
: DynamicMotion( dm )
, m_dummy( dm.m_dummy )
{
}

bool RandomCurveDynamicMotion::frameMove( float fElapsedTime )
{
	return true;
}

DynamicMotion* RandomCurveDynamicMotion::clone() const
{
	return new RandomCurveDynamicMotion( *this );
}