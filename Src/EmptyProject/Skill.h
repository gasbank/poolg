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
			L"Ÿ���� �ڵ��� ȥ�� ��� ���� �����մϴ�.\n";
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
			L"����׸� ���Ͽ� HP�� ȸ���մϴ�.\n";
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
			L"��A���� ��ħ�� ����� ����Ͽ� Ǯ����\n"
			L"Ÿ���� ���Ǹ��� ����ø��ϴ�.\n";
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
			L"���� ���� �����带 �����Ͽ� ������\n"
			L"���������� �����մϴ�.\n";
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
			L"���α׷��ӵ� ���̿��� �ǽ�, �ݱ��\n"
			L"������ �Ծ��� ��ų�Դϴ�. ���� ����\n"
			L"�ͷ��� �����Ͽ� ���ظ� �����ϴ�.\n";
		m_csEssentials = 8;
	}
	virtual void useSkill( BattleState* battleState );
};

