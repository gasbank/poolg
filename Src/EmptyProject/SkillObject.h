#pragma once
#include "Unit.h"

class BattleState;
class DynamicMotion;
class Action;

/**
@brief 전투 상태에서 기술을 사용했을 때 생성되는 Unit

예를 들어 전투 중에 캐릭터가 파이어볼 기술을 사용했을 때 파이어볼에
해당하는 오브젝트가 있어야 한다. 이 클래스는 그러한 오브젝트에 대한
인터페이스를 제공한다.
*/
class SkillObject : public Unit
{
public:
	static SkillObject*				createSkillObject( const char* bst, float size, D3DCOLOR color, DynamicMotion* dmt );
									~SkillObject();

	// Virtual Methods
	virtual HRESULT					frameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime );
	virtual bool					frameMove( double dTime, float fElapsedTime );

	/*static SkillObject*				createSOnormalAttack	(Character* user, Character* target, Unit* effectObject);
	static SkillObject*				createSOheal			(Character* user, Unit* effectObject);
	static SkillObject*				createSOmeditation		(Character* user, Unit* effectObject);
	static SkillObject*				createSOcsBurn			(Character* target, Unit* effectObject);
	static SkillObject*				createSOmtBullet		(Character* user, Character* target, Unit* effectObject, int leftNumber, int maxNumber);
	static SkillObject*				createSOgoto			(Character* user, Character* target, Unit* effectObject);*/

	SkillObject*					clone() const;
	void							addOnHitAction( Action* act ) { m_onHitActionList.push_back( act ); }
	void							setUserAndTarget( Character* user, Character* target );

private:
									SkillObject( const SkillObject& so );
									SkillObject( BasicShapeType bst, float size, D3DCOLOR color, DynamicMotion* dm );
	BattleState*					getBattleState() const;

	BasicShapeType					m_bst;
	Character*						m_user;
	Character*						m_target;
	float							m_velocity;
	float							m_size;
	D3DCOLOR						m_color;
	DynamicMotion*					m_dm;
	ActionList						m_onHitActionList;
};

SCRIPT_FACTORY( SkillObject )


//class SOnormalAttack : public SkillObject
//{
//public:
//	~SOnormalAttack();
//	virtual bool frameMove ( float fElapsedTime );
//
//	friend class SkillObject;
//
//protected:
//	Character* m_user;
//	Character* m_target;
//
//private:
//	SOnormalAttack (Character* user, Character* target, Unit* effectObject);
//};
//
//class SOheal : public SkillObject
//{
//public:
//	~SOheal();
//	virtual bool frameMove ( float fElapsedTime );
//
//	friend class SkillObject;
//
//protected:
//	Character* m_user;
//
//private:
//	SOheal (Character* user, Unit* effectObject);
//};
//
//class SOmeditation : public SkillObject
//{
//public:
//	~SOmeditation();
//	virtual bool frameMove ( float fElapsedTime );
//
//	friend class SkillObject;
//
//protected:
//	Character* m_user;
//
//private:
//	SOmeditation (Character* user, Unit* effectObject);
//};
//
//class SOcsBurn : public SkillObject
//{
//public:
//	~SOcsBurn();
//	virtual bool frameMove ( float fElapsedTime );
//
//	friend class SkillObject;
//
//protected:
//	Character* m_target;
//
//private:
//	SOcsBurn (Character* target, Unit* effectObject);
//};
//
//
////MultiThreadBullet
//class SOmtBullet : public SkillObject
//{
//public:
//	~SOmtBullet();
//	virtual bool frameMove ( float fElapsedTime );
//
//	friend class SkillObject;
//
//protected:
//	Character* m_user;
//	Character* m_target;
//	int m_leftNumber;
//	int m_maxNumber;
//	float m_elapsedTime;
//
//private:
//	SOmtBullet (Character* user, Character* target, Unit* effectObject, int leftNumber, int maxNumber);
//};
//
//class SOgoto : public SkillObject
//{
//public:
//	~SOgoto();
//	virtual bool frameMove ( float fElapsedTime );
//	
//	friend class SkillObject;
//
//protected:
//	Character* m_user;
//	Character* m_target;
//	Unit* m_illusion;
//	int m_phase;
//	D3DXVECTOR3 m_originPoint;
//
//private:
//	SOgoto (Character* user, Character* target, Unit* effectObject);
//};
