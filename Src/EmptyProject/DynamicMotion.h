#pragma once
#include "Unit.h"

enum DynamicMotionType { DMT_UNKNOWN, DMT_FIRE_UNIFORMLY, DMT_RANDOM_CURVE };

class DynamicMotion
{
public:	
	static DynamicMotion*		createDynamicMotion( DynamicMotionType dmt, Unit* motionTarget );
	virtual						~DynamicMotion() {}

	virtual DynamicMotion*		clone() const = 0;
	virtual bool				frameMove( float fElapsedTime );
	virtual void				release() {}
	
	void						setFireAndTargetUnit( const Unit* fireUnit, const Unit* targetUnit ) { m_fireUnit = fireUnit; m_targetUnit = targetUnit; }
	void						setConstantVelocity( float constVelocity ) { m_constVelocity = constVelocity; }
	void						setMotionTarget(Unit* val) { m_motionTarget = val; }

protected:
								DynamicMotion( Unit* motionTarget );
								DynamicMotion( const DynamicMotion& dm );
	Unit*						getMotionTarget() const { return m_motionTarget; }
	
	const Unit*					getFireUnit() const { return m_fireUnit; }
	const Unit*					getTargetUnit() const { return m_targetUnit; }

private:
	Unit*						m_motionTarget;			// This unit's animation(motion) is governed by this DynamicMotion object.
	const Unit*					m_fireUnit;
	const Unit*					m_targetUnit;
	float						m_constVelocity;
};


class FireUniformlyDynamicMotion : public DynamicMotion
{
public:
	virtual DynamicMotion*		clone() const;
	virtual bool				frameMove( float fElapsedTime );

protected:
								FireUniformlyDynamicMotion( Unit* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist,  float velocity );
								FireUniformlyDynamicMotion( const FireUniformlyDynamicMotion& dm );
private:
	D3DXVECTOR3					m_initPos;
	D3DXVECTOR3					m_fireDir;
	float						m_retainDist;
	float						m_velocity;
};

class SpinAroundDynamicMotion : public DynamicMotion
{
public:
	virtual DynamicMotion*		clone() const;
	virtual bool				frameMove( float fElapsedTime );

protected:
								SpinAroundDynamicMotion( Unit* target, const D3DXVECTOR3& fireDest, float radius, float radiusVelocity, float angularVelocity );
								SpinAroundDynamicMotion( const SpinAroundDynamicMotion& dm );
private:
	D3DXVECTOR3					m_fireDest;
	float						m_radius;
	float						m_radiusVelocity;
	float						m_angle;
	float						m_angularVelocity;
};

class PuffDynamicMotion : public DynamicMotion
{
public:
	virtual DynamicMotion*		clone() const;
	virtual bool				frameMove( float fElapsedTime);
	
protected:
								PuffDynamicMotion( Unit* target, const D3DXVECTOR3& initPos, float puffRate, float puffSpeed );
								PuffDynamicMotion( const PuffDynamicMotion& dm );
private:
	float						m_puffRate;
	float						m_puffSpeed;
	float						m_scaleX;
	float						m_scaleY;
	float						m_scaleZ;
};

class RandomCurveDynamicMotion : public DynamicMotion
{
public:
								RandomCurveDynamicMotion( Unit* motionTarget );
	virtual DynamicMotion*		clone() const;
	virtual bool				frameMove( float fElapsedTime );

protected:
								RandomCurveDynamicMotion( const RandomCurveDynamicMotion& dm );

private:
	float						m_totalElapsedTime;
	float						m_duration;
};