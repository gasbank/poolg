#pragma once
#include "BattleState.h"
#include "Character.h"

#include <string>

enum SkillLocation { SL_FIRST, SL_SECOND, SL_THIRD, SL_FOURTH, SL_FIFTH };

class Skill
{
public:
//	Skill ( std::wstring skillName, std::wstring description, int csEssentials,  );

	virtual void useSkill( BattleState* battleState );
	std::wstring getDescription() { return m_description; }
	void setUser(Character* hero, Character* enemy) { m_hero = hero; m_enemy = enemy; }
	std::wstring getSkillName() { return m_skillName; }

	std::wstring		m_skillName;
	std::wstring		m_description;
	int					m_csEssentials;
	int					m_skillObjectCount;
	SkillObject*		m_so;


	Unit*				m_effectObject;
	DynamicMotion*		m_dm;


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
	bool useSkill();
	std::wstring getSkillName( SkillLocation skillLocation );
	SkillLocation getSkillLocation() { return m_skillLocation; }
	void moveSkillLocation(char mode);
	void setCharacter (Character* hero, Character* enemy);

	std::wstring getDescription (SkillLocation skillLocation);

	/*사용 전엔 반드시 battleState를 설정해 주어야 한다.*/
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
			L"타락한 코딩의 혼을 담아 적을 공격합니다.\n";
		m_csEssentials = 2;
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
			L"디버그를 통하여 HP를 회복합니다.\n";
		m_csEssentials = 8;
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
			L"공A관의 음침한 기운을 흡수하여 풀쥐의\n"
			L"타락한 스피릿을 끌어올립니다.\n";
		m_csEssentials = 0;
	}
	virtual void useSkill( BattleState* battleState );
};

class MultiThread : public Skill
{
public:
	MultiThread()
	{
		m_skillName = L"MultiThread";
		m_description =
			L"[MultiThread]\n"
			L"여러 개의 쓰레드를 생성하여 상대방을\n"
			L"병렬적으로 공격합니다.\n";
		m_csEssentials = 10;
	}
	virtual void useSkill( BattleState* battleState );
};

class Goto : public Skill
{
public:
	Goto()
	{
		m_skillName = L"Goto";
		m_description =
			L"[Goto]\n"
			L"프로그래머들 사이에서 악습, 금기로\n"
			L"여겨져 왔었던 스킬입니다. 적을 향해\n"
			L"맹렬히 돌진하여 피해를 입힙니다.\n";
		m_csEssentials = 8;
	}
	virtual void useSkill( BattleState* battleState );
};

