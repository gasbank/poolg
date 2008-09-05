#pragma once
#include "Unit.h"

class BattleState;
class DynamicMotion;
class Action;

enum DynamicMotionType { DMT_UNKNOWN, DMT_FIRE_UNIFORMLY, DMT_RANDOM_CURVE };

class SkillObject : public Unit
{
public:
	static SkillObject*				createSkillObject( const char* bst, float size, D3DCOLOR color, const char* dmt );
									~SkillObject();

	// Virtual Methods
	virtual HRESULT					frameRender( double dTime, float fElapsedTime );
	virtual bool					frameMove( float fElapsedTime );

	/*static SkillObject*				createSOnormalAttack	(Character* user, Character* target, Unit* effectObject);
	static SkillObject*				createSOheal			(Character* user, Unit* effectObject);
	static SkillObject*				createSOmeditation		(Character* user, Unit* effectObject);
	static SkillObject*				createSOcsBurn			(Character* target, Unit* effectObject);
	static SkillObject*				createSOmtBullet		(Character* user, Character* target, Unit* effectObject, int leftNumber, int maxNumber);
	static SkillObject*				createSOgoto			(Character* user, Character* target, Unit* effectObject);*/

	void							setOnHitAction( Action* act ) { m_onHitAction = act; }
protected:
	BattleState*					getBattleState();

private:
									SkillObject( BasicShapeType bst, float size, D3DCOLOR color, DynamicMotionType dmt );
	Unit*							m_target;
	float							m_velocity;
	float							m_size;
	D3DCOLOR						m_color;
	DynamicMotion*					m_dm;
	int								m_onHitHpDamage;
	Action*							m_onHitAction;
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
