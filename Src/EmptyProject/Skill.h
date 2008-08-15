#pragma once
#include "BattleState.h"
#include "Character.h"

#include <string>

enum SkillLocation { SL_FIRST, SL_SECOND, SL_THIRD, SL_FOURTH, SL_FIFTH };

class Skill
{
public:
	virtual void useSkill( BattleState* battleState );
	std::wstring getDescription() { return m_description; }
	void setUser(Character* hero, Character* enemy) { m_hero = hero; m_enemy = enemy; }
	std::wstring getSkillName() { return m_skillName; }
	std::wstring			m_skillName;
	std::wstring			m_description;
	Character*			m_hero;
	Character*			m_enemy;
	int					m_csEssentials;
};


class SkillSet
{
public:
	SkillSet();
	~SkillSet();
	void setSkill( SkillLocation skillLocation, Skill* skill );
	void deleteSkill( SkillLocation skillLocation );
	bool useSkill();
	std::wstring getSkillName( SkillLocation skillLocation );
	SkillLocation getSkillLocation() { return m_skillLocation; }
	void moveSkillLocation(char mode);
	void setCharacter (Character* hero, Character* enemy);

	std::wstring getDescription (SkillLocation skillLocation);

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
	NormalAttack()
	{
		m_skillName = L"N.A."; 
		m_description =
			L"[Normal Attack]\n"
			L"�Ϲ����� ������� ���� �����մϴ�.\n";
		m_csEssentials = 10;
	}
	virtual void useSkill( BattleState* battleState );
};

class Heal : public Skill
{
public:
	Heal()
	{
		m_skillName = L"Heal";
		m_description =
			L"[Heal]\n"
			L"����׸� ���Ͽ� HP�� ȸ���մϴ�.\n";
		m_csEssentials = 20;
	}
	virtual void useSkill( BattleState* battleState );
};

class Meditation : public Skill
{
public:
	Meditation()
	{
		m_skillName = L"Meditation";
		m_description =
			L"[Meditation]\n"
			L"��A���� ��ħ�� ����� ����Ͽ� Ǯ����\n"
			L"Ÿ���� ���Ǹ��� ����ø��ϴ�.\n";
		m_csEssentials = 0;
	}
	virtual void useSkill( BattleState* battleState );
};
