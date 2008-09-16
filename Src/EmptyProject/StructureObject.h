#pragma once
#include "ScriptManager.h"
#include "Unit.h"

class Character;

class StructureObject : public Unit
{
public:
	static Unit*			createStructureObject( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool pushable, bool pickable );
							~StructureObject(void);
	
	virtual bool			frameMove( double dTime, float fElapsedTime );

	bool					isPushable() const		{ return m_bPushable; }
	void					setPushable(bool val)	{ m_bPushable = val; }

	bool					isPickable() const		{ return m_bPickable; }
	void					setPickable(bool val)	{ m_bPickable = val; }

	const std::string&		getThumbnailName() const { return m_thumbnailName; }

	Character*				getOwner() const { return m_bOwner; }
	void					setOwner( Character* val ) { m_bOwner = val; }

private:
							StructureObject(void);
	
	Character*				m_bOwner;
	bool					m_bPushable;
	bool					m_bPickable;
	bool					m_bMoving;
	float					m_fMovingTime;
	float					m_moveDuration;
	std::string				m_thumbnailName;
};

SCRIPT_FACTORY( StructureObject )
