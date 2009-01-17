#pragma once
#include "ScriptManager.h"
#include "Unit.h"

class Character;


/**
@brief 맵 상의 상자나 아이템을 나타내는 클래스

맵에 존재하지만 살아있지 않은 물체를 나타내는 클래스입니다.
현재는 밀어서 움직일 수 있는 상자로만 사용하고 있습니다.
*/
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
