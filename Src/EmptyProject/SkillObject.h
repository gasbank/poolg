#pragma once
#include "EffectObject.h"

class BattleState;

class SkillObject
{
public:
	virtual ~SkillObject();

protected:
	BattleState* getBattleState();
	EffectObject* m_effectObject;

private:
	float m_elapsedTime;
};

class SOnormalAttack : public SkillObject
{
public:
};