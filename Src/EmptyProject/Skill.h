#pragma once

class BattleState;
class Character;
class SkillObject;
class Unit;
class DynamicMotion;

typedef std::list<SkillObject*> SkillObjectList;

class Skill
{
public:
	static Skill*				createSkillByScript( const char* skillNsName );
								~Skill();

	const std::string&			getDescription() const { return m_skillDescription; }
	const std::string&			getSkillName() const { return m_skillName; }

	void						doSkill();

	void						release();
private:
								Skill( const char* name, const char* desc, int csEssentials );
	void						addSkillObject( SkillObject* so );

	std::string					m_skillName;
	std::string					m_skillDescription;
	int							m_csEssentials;			// Minimal CS requirement to use this skill
	
	SkillObjectList				m_skillObjects;
};

//
//class NormalAttack : public Skill
//{
//public:
//	NormalAttack()
//	{
//		m_skillName = L"N.A."; 
//		m_skillDescription =
//			L"[Normal Attack]\n"
//			L"Ÿ���� �ڵ��� ȥ�� ��� ���� �����մϴ�.\n";
//		m_csEssentials = 2;
//	}
//	virtual void useSkill( BattleState* battleState );
//};
//
//class Heal : public Skill
//{
//public:
//	Heal()
//	{
//		m_skillName = L"Heal";
//		m_skillDescription =
//			L"[Heal]\n"
//			L"����׸� ���Ͽ� HP�� ȸ���մϴ�.\n";
//		m_csEssentials = 8;
//	}
//	virtual void useSkill( BattleState* battleState );
//};
//
//class Meditation : public Skill
//{
//public:
//	Meditation()
//	{
//		m_skillName = L"Meditation";
//		m_skillDescription =
//			L"[Meditation]\n"
//			L"��A���� ��ħ�� ����� ����Ͽ� Ǯ����\n"
//			L"Ÿ���� ���Ǹ��� ����ø��ϴ�.\n";
//		m_csEssentials = 0;
//	}
//	virtual void useSkill( BattleState* battleState );
//};
//
//class MultiThread : public Skill
//{
//public:
//	MultiThread()
//	{
//		m_skillName = L"MultiThread";
//		m_skillDescription =
//			L"[MultiThread]\n"
//			L"���� ���� �����带 �����Ͽ� ������\n"
//			L"���������� �����մϴ�.\n";
//		m_csEssentials = 10;
//	}
//	virtual void useSkill( BattleState* battleState );
//};
//
//class Goto : public Skill
//{
//public:
//	Goto()
//	{
//		m_skillName = L"Goto";
//		m_skillDescription =
//			L"[Goto]\n"
//			L"���α׷��ӵ� ���̿��� �ǽ�, �ݱ��\n"
//			L"������ �Ծ��� ��ų�Դϴ�. ���� ����\n"
//			L"�ͷ��� �����Ͽ� ���ظ� �����ϴ�.\n";
//		m_csEssentials = 8;
//	}
//	virtual void useSkill( BattleState* battleState );
//};
//
