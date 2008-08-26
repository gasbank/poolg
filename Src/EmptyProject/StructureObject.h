#pragma once
#include "ScriptManager.h"
#include "Unit.h"

class StructureObject : public Unit
{
public:
	StructureObject(void);
	~StructureObject(void);
	static Unit* createStructureObject( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool pushable );

	virtual bool frameMove(float fElapsedTime);

	bool getPushable() const { return m_bPushable; }
	void setPushable(bool val) { m_bPushable = val; }

private:
	bool	m_bPushable;
	bool	m_bMoving;
	float	m_fMovingTime;
	float	m_moveDuration;
	
};

SCRIPT_FACTORY( StructureObject )