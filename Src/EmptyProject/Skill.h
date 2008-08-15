#pragma once
#include "BattleState.h"
#include "Character.h"

#include <string>

enum SkillLocation { SL_FIRST, SL_SECOND, SL_THIRD, SL_FOURTH, SL_FIFTH };

class Skill
{
public:
	void useSkill( BattleState* battleState );
	void setUser (Character* hero, Character* enemy) { m_hero = hero; m_enemy = enemy; }
	std::string getSkillName () { return m_skillName; }
	std::string			m_skillName;
	Character*			m_hero;
	Character*			m_enemy;
};


class SkillSet
{
public:
	SkillSet();
	~SkillSet();
	void setSkill( SkillLocation skillLocation, Skill* skill );
	void deleteSkill( SkillLocation skillLocation );
	void useSkill( SkillLocation skillLocation );
	std::string getSkillName( SkillLocation skillLocation );
	SkillLocation getSkillLocation() { return m_skillLocation; }
	void moveSkillLocation(char mode);

	/*��� ���� �ݵ�� battleState�� ������ �־�� �Ѵ�.*/
	void setBattleState( BattleState* battleState ) { m_battleState = battleState; }

private:
	Skill*				m_skillSet[5];
	SkillLocation		m_skillLocation;
	BattleState*		m_battleState;

};

class NormalAttack : public Skill
{
public:
	NormalAttack() { m_skillName = "NormalAttack"; }
	void useSkill( BattleState* battleState );
};

class Heal : public Skill
{
public:
	Heal() { m_skillName = "Heal"; }
	void useSkill( BattleState* battleState );
};
