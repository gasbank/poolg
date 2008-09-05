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
//			L"타락한 코딩의 혼을 담아 적을 공격합니다.\n";
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
//			L"디버그를 통하여 HP를 회복합니다.\n";
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
//			L"공A관의 음침한 기운을 흡수하여 풀쥐의\n"
//			L"타락한 스피릿을 끌어올립니다.\n";
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
//			L"여러 개의 쓰레드를 생성하여 상대방을\n"
//			L"병렬적으로 공격합니다.\n";
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
//			L"프로그래머들 사이에서 악습, 금기로\n"
//			L"여겨져 왔었던 스킬입니다. 적을 향해\n"
//			L"맹렬히 돌진하여 피해를 입힙니다.\n";
//		m_csEssentials = 8;
//	}
//	virtual void useSkill( BattleState* battleState );
//};
//
