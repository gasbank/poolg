#pragma once
#include "Unit.h"

enum DynamicMotionType { DMT_UNKNOWN, DMT_FIRE_UNIFORMLY, DMT_RANDOM_CURVE };

class DynamicMotion
{
public:
	virtual bool frameMove( float fElapsedTime );

	static DynamicMotion* createDynamicMotion( DynamicMotionType dmt );

	static DynamicMotion* createDMfireUniformly ( Unit* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist, float velocity );
	static DynamicMotion* createDMspinAround	( Unit* target, const D3DXVECTOR3& fireDest, float radius, float radiusVelocity, float angularVelocity );
	static DynamicMotion* createDMpuff			( Unit* target, const D3DXVECTOR3& initPos, float puffRate, float puffSpeed );
	static DynamicMotion* createRandomCurve		() { return 0; }

	

	virtual void release() {}

	void setFireAndTargetUnit( const Unit* fireUnit, const Unit* targetUnit ) { m_fireUnit = fireUnit; m_targetUnit = targetUnit; }
	void setConstantVelocity( float constVelocity ) { m_constVelocity = constVelocity; }

protected:
	Unit* m_motionTarget;			// This unit's animation(motion) will be governed by this DynamicMotion object.

private:
	const Unit* m_fireUnit;
	const Unit* m_targetUnit;
	
	float m_constVelocity;
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

class DMpuff : public DynamicMotion
{
public:
	virtual bool frameMove( float fElapsedTime);
	
	friend class DynamicMotion;

private:
	DMpuff (Unit* target, const D3DXVECTOR3& initPos, float puffRate, float puffSpeed);
	float m_puffRate;
	float m_puffSpeed;
	float m_scaleX;
	float m_scaleY;
	float m_scaleZ;
};

class RandomCurveDynamicMotion : public DynamicMotion
{
public:
	virtual bool frameMove( float fElapsedTime );
};