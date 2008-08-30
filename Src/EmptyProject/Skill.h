#pragma once

class BattleState;
class Character;
class SkillObject;
class Unit;
class DynamicMotion;

class Skill
{
public:
	Skill() {}
	virtual ~Skill() {}

	virtual void				useSkill( BattleState* battleState );

	const std::wstring&			getDescription() const { return m_description; }
	void						setUser(Character* hero, Character* enemy) { m_hero = hero; m_enemy = enemy; }
	const std::wstring&			getSkillName() const { return m_skillName; }

protected:
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

