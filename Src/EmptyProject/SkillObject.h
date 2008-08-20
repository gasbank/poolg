#pragma once
#include "Unit.h"

class BattleState;

class SkillObject
{
public:
	virtual ~SkillObject();
	virtual bool frameMove ( float fElapsedTime );
	SkillObject* createSOnormalAttack (Character* user, Character* target, Unit* effectObject);


protected:
	BattleState* getBattleState();
	Unit* m_effectObject;
};

class SOnormalAttack : public SkillObject
{
public:
	~SOnormalAttack();
	virtual bool frameMove ( float fElapsedTime );

	friend class SkillObject;

protected:
	Character* m_user;
	Character* m_target;

private:
	SOnormalAttack (Character* user, Character* target, Unit* effectObject);
};
