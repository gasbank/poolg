#pragma once
#include "BattleState.h"
#include <string>

enum SkillLocation { SL_FIRST, SL_SECOND, SL_THIRD, SL_FOURTH, SL_FIFTH};

class Skill
{
public:
	virtual void useSkill( BattleState* battleState );
	std::string			m_skillName;
};


class SkillSet
{
public:
	SkillSet();
	void setSkill( SkillLocation skillLocation, Skill* skill );
	void deleteSkill( SkillLocation skillLocation );
	void useSkill( SkillLocation skillLocation );
	std::string getSkillName( SkillLocation skillLocation );

	/*��� ���� �ݵ�� battleState�� ������ �־�� �Ѵ�.*/
	void setBattleState( BattleState* battleState ) { m_battleState = battleState; }

private:
	Skill*				m_skillSet[5];
	SkillLocation		m_skillLocation;
	BattleState*		m_battleState;

};

class NormalAttack : public Skill
{
};
