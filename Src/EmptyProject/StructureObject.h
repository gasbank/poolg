#pragma once
#include "ScriptManager.h"
#include "Unit.h"

class StructureObject : public Unit
{
public:
	static Unit*	createStructureObject( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool pushable, bool pickable );
					~StructureObject(void);
	
	virtual bool	frameMove( double dTime, float fElapsedTime );

	bool			getPushable() const		{ return m_bPushable; }
	void			setPushable(bool val)	{ m_bPushable = val; }

	bool			getPickable() const		{ return m_bPickable; }
	void			setPickable(bool val)	{ m_bPickable = val; }
private:
					StructureObject(void);
	
	bool			m_bPushable;
	bool			m_bPickable;
	bool			m_bMoving;
	float			m_fMovingTime;
	float			m_moveDuration;
};

SCRIPT_FACTORY( StructureObject )
