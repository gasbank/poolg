#include "EmptyProjectPCH.h"
#include "DynamicMotion.h"
#include "ScriptManager.h"
#include "ArnMath.h"
#include "ArnConsts.h"

DynamicMotion::DynamicMotion( Unit* motionTarget )
: m_motionTarget( motionTarget )
, m_fireUnit( 0 )
, m_targetUnit( 0 )
{
}

DynamicMotion::DynamicMotion( const DynamicMotion& dm )
: m_motionTarget( dm.m_motionTarget )
, m_fireUnit( dm.m_fireUnit )
, m_targetUnit( dm.m_targetUnit )
{
}

DynamicMotion* DynamicMotion::createDynamicMotion( DynamicMotionType dmt )
{
	// TODO DynamicMotion factory method
	return new RandomCurveDynamicMotion( ( Unit* )0, 1.0f );
}

DynamicMotion* DynamicMotion::createRandomCurveDynamicMotion( float duration )
{
	return new RandomCurveDynamicMotion( ( Unit* )0, duration );
}

//////////////////////////////////////////////////////////////////////////

DynamicMotion* EpCreateRandomCurveDynamicMotion( double duration )
{
	return DynamicMotion::createRandomCurveDynamicMotion( (float)duration );
} SCRIPT_CALLABLE_PV_D( EpCreateRandomCurveDynamicMotion )

START_SCRIPT_FACTORY( DynamicMotion )
	CREATE_OBJ_COMMAND( EpCreateRandomCurveDynamicMotion )
END_SCRIPT_FACTORY( DynamicMotion )




/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FireUniformlyDynamicMotion::frameMove(float fElapsedTime)
{
	ArnVec3 newPos = getMotionTarget()->getPos() + m_fireDir * m_velocity * fElapsedTime;
	ArnVec3 vMoved = m_initPos - newPos;
	
	float distToInitPos = ArnVec3Length( &vMoved );
//	m_target->setRotY(distToInitPos);
	if ( distToInitPos > m_retainDist )
	{
		return false;
	}
	getMotionTarget()->setPos( newPos.x, newPos.y, newPos.z );
	return true;
}

FireUniformlyDynamicMotion::FireUniformlyDynamicMotion ( Unit* target, const ArnVec3& initPos, const ArnVec3& fireDir, float retainDist, float velocity )
: DynamicMotion( target )
{
	m_initPos = initPos;
	m_fireDir = fireDir;
	m_retainDist = retainDist;
	m_velocity = velocity;

	getMotionTarget()->setPos( m_initPos.x, m_initPos.y, m_initPos.z );
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
	ArnVec3 newPos = m_target->getPos() + m_fireDir * m_velocity;
	ArnVec3 vMoved = m_initPos - newPos;
	float distToInitPos = ArnVec3Length( &vMoved );
	if ( distToInitPos > m_retainDist )
	{
		return false;
	}
	m_target->setPos(newPos);*/

	ArnVec3 newPos;
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

	getMotionTarget()->setPos( newPos.x, newPos.y, newPos.z );
	return true;
}

SpinAroundDynamicMotion::SpinAroundDynamicMotion ( Unit* target, const ArnVec3& fireDest, float radius, float radiusVelocity, float angularVelocity )
: DynamicMotion( target )
{
	m_fireDest = fireDest;
	m_radius = radius;
	m_radiusVelocity = radiusVelocity;
	m_angle = 0;
	m_angularVelocity = angularVelocity;

	float angle = D3DXToRadian (m_angle);

	ArnVec3 newPos;

	newPos.x = target->getPos().x + m_radius * cos (angle);
	newPos.y = target->getPos().y + m_radius * sin (angle);
	newPos.z = target->getPos().z;
	//m_fireDest = target->getPos ();

	target->setPos( newPos.x, newPos.y, newPos.z );
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

PuffDynamicMotion::PuffDynamicMotion (Unit* target, const ArnVec3& initPos, float puffRate, float puffSpeed)
: DynamicMotion( target )
{
	m_puffRate = puffRate;
	m_puffSpeed = puffSpeed;

	getMotionTarget()->setPos( initPos.x, initPos.y, initPos.z );
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

RandomCurveDynamicMotion::RandomCurveDynamicMotion( Unit* motionTarget, float duration )
: DynamicMotion( motionTarget )
, m_totalElapsedTime( 0 )
, m_duration( duration )
{
}

RandomCurveDynamicMotion::RandomCurveDynamicMotion( const RandomCurveDynamicMotion& dm )
: DynamicMotion( dm )
, m_totalElapsedTime( dm.m_totalElapsedTime )
, m_duration( dm.m_duration )
{
	assert( m_duration > 0 );
}

bool RandomCurveDynamicMotion::frameMove( float fElapsedTime )
{
	assert( getMotionTarget() );
	assert( getMotionTarget()->getType() == UT_SKILLOBJECT );

	m_totalElapsedTime += fElapsedTime;
	ArnVec3 targetPos = ArnConsts::D3DXVEC3_ZERO;
	if ( m_totalElapsedTime < m_duration )
	{
		float zeroToOne = m_totalElapsedTime / m_duration;
		ArnVec3Lerp( &targetPos, &getFireUnit()->getPos(), &getTargetUnit()->getPos(), zeroToOne );
		
		ArnVec3 zDelta( 0, 0, 3.0f * zeroToOne * (zeroToOne - 1 ) );
		

		ArnVec3 fireDir = getTargetUnit()->getPos() - getFireUnit()->getPos();
		ArnMatrix mat;
		ArnMatrixRotationAxis( &mat, &fireDir, D3DXToRadian( (zeroToOne * 360 - 180) * 2 ) );
		ArnVec3TransformCoord( &zDelta, &zDelta, &mat );

		targetPos += zDelta;
	}
	else
	{
		targetPos = getTargetUnit()->getPos();
	}

	getMotionTarget()->setPos( targetPos.x, targetPos.y, targetPos.z );
	return true;
}

DynamicMotion* RandomCurveDynamicMotion::clone() const
{
	return new RandomCurveDynamicMotion( *this );
}

//////////////////////////////////////////////////////////////////////////
