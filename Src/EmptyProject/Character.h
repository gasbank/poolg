#pragma once
#include "Unit.h"
#include "TileManager.h"
#include "ArnConsts.h"

class SkillSet;
class SkillObject;
class Trigger;
class Skill;
class StructureObject;

typedef std::list<SkillObject*> SkillObjectList;
typedef std::list<StructureObject*> StructureObjectList;
typedef std::vector<StructureObject*> StructureObjectVector;

enum SkillLocation;

//고정적 스탯. 레벨 업 시만 오르는.(simon ornen)
struct Stat
{
	int health, will, coding, def, sense, immunity;
};


/**
@brief 주인공 및 적군, NPC 캐릭터의 베이스 클래스

Character 클래스는 보통 게임의 캐릭터가 가지는 공통된 속성을 가집니다. 예를 들어
HP, MP, 공격력, 방어력 등이 그것입니다.

캐릭터는 아이템을 소지 및 습득할 수 있으며, 스킬도 추가하거나 제거할 수 있습니다.
*/
class Character : public Unit
{
public:
	// Ctor and Dtor and create
	static Unit*						createCharacter( LPD3DXMESH mesh, int tileX, int tileY, float posZ );
	virtual								~Character();

	// Virtual Methods
	virtual HRESULT						frameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime );
	virtual bool						frameMove( double dTime, float fElapsedTime );
	virtual LRESULT						handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual const ArnVec3&				getPos() const { if ( m_moveImpulse != ArnConsts::D3DXVEC3_ZERO ) return m_curPos; else return Unit::getPos(); }
	virtual void						processUnitInFront( UnitInput dir );

	/* 스킬 관련 함수 */
	//void doNormalAttack(int type, Character* enemy);
	void								pushSkillObject( SkillObject* skillObj ) { m_skillObjects.push_back( skillObj ); }
	void								pushSkillObjectList( const SkillObjectList soList );
	const SkillSet*						getSkillSet() const { return m_skillSet; }
	bool								deleteSkill( SkillLocation skillLoc );
	bool								memorizeSkill( const Skill* skill );
	bool								equipSkill( UINT slot, const Skill* skill );

	void								doCsBurn();
	void								recoverCs ();
	void								heal (int point);

	

	void								setCurHp( int curHp );
	int									getCurHp() const			{ return m_curHp; }
	int									getMaxHp() const			{ return m_maxHp; }
	void								setCurCs( int curCs );
	int									getCurCs() const			{ return m_curCs; }
	int									getMaxCs() const			{ return m_maxCs; }

	void								damage( int point );
	bool								getMoving() const			{ return m_bMoving; }
	const Stat&							getStat() const				{ return m_stat; }
	bool								isDead() const				{ return (m_curHp<=0); }

	void								setDead();
	void								addMoveImpulse( const ArnVec3& impulse );
	
	void								setMoveDuration( float val ) { m_moveDuration = val; }	
	void								setBoundaryRect( LONG left, LONG top, LONG right, LONG bottom );
	

	void								setStat( int statHealth, int statWill, int statCoding, int statDef, int statSen, int statImmu );

	void								setControllable(bool bCtrl)		{ m_bControllable = bCtrl; }

	const std::string&					getItemThumbnailName( UINT idx ) const;
	UINT								getItemCount() const { return m_items.size(); }
	
protected:
										Character( UnitType type );
	
private:
	void								boundaryTesting( UnitInput );
	void								pickItem( StructureObject* structureObj );
	void								deleteAllSkills();
	void								deleteAllItems();

	bool								m_bMoving;
	float								m_fMovingTime;

	//유동적 스탯.
	int									m_maxHp;
	int									m_curHp;
	int									m_maxCs;
	int									m_curCs;

	float								m_moveDuration; // A time needed to move one time(tile) in seconds
	
	TileRegion							m_boundaryTileRect;

	// Move amount that applied instantly and decimated constantly on every frameMove()
	ArnVec3								m_moveImpulse;
	ArnVec3								m_curPos;

	Stat								m_stat;

	bool								m_bControllable;

	SkillSet*							m_skillSet;
	SkillObjectList						m_skillObjects;

	StructureObjectVector				m_items;
};


SCRIPT_FACTORY( Character )
