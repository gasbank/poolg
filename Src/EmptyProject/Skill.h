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

	/*사용 전엔 반드시 battleState를 설정해 주어야 한다.*/
	void setBattleState( BattleState* battleState ) { m_battleState = battleState; }

private:
	Skill*				m_skillSet[5];
	SkillLocation		m_skillLocation;
	BattleState*		m_battleState;

};

class NormalAttack : public Skill
{
};
