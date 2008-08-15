#include "EmptyProjectPCH.h"
#include "Skill.h"

using std::string;

SkillSet::SkillSet()
{
	m_skillLocation = SL_FIRST;

	for (int i = 0; i < 5; i++)
		m_skillSet[i] = NULL;
}

SkillSet::~SkillSet()
{
	for (int i = 0; i < 5; i++)
	{
		if (m_skillSet[i] != NULL)
			delete m_skillSet[i];
	}
}

void Skill::useSkill (BattleState* battleState)
{

}

void SkillSet::setSkill (SkillLocation skillLocation, Skill* skill)
{
	if (m_skillSet[(int)skillLocation] != (NULL))
		delete m_skillSet[(int)skillLocation];
	m_skillSet[(int)skillLocation] = skill;
}

void SkillSet::deleteSkill (SkillLocation skillLocation)
{
	if (m_skillSet[(int)skillLocation] != (NULL))
		delete m_skillSet[(int)skillLocation];
}

string SkillSet::getSkillName (SkillLocation skillLocation)
{
	/*비었을 경우 NULL 반환*/
	if (m_skillSet[(int)skillLocation] == NULL)
		return NULL;
	return m_skillSet[(int)skillLocation]->m_skillName;
}

void SkillSet::useSkill (SkillLocation skillLocation)
{
	if (m_skillSet[(int)skillLocation] == NULL)
		return;

	m_skillSet[(int)skillLocation]->useSkill( m_battleState );
}

void SkillSet::moveSkillLocation (char mode)
{
	switch (mode)
	{
	/*initialize*/
	case 'i':
		m_skillLocation = SL_FIRST;

	/*up*/
	case 'u':
		switch (m_skillLocation)
		{
		case SL_FIRST:
			m_skillLocation = SL_FIFTH;
			break;
		case SL_SECOND:
			m_skillLocation = SL_FIRST;
			break;
		case SL_THIRD:
			m_skillLocation = SL_SECOND;
			break;
		case SL_FOURTH:
			m_skillLocation = SL_THIRD;
			break;
		case SL_FIFTH:
			m_skillLocation = SL_FOURTH;
			break;
		}
	/*down*/
	case 'd':
		switch (m_skillLocation)
		{
		case SL_FIRST:
			m_skillLocation = SL_SECOND;
			break;
		case SL_SECOND:
			m_skillLocation = SL_THIRD;
			break;
		case SL_THIRD:
			m_skillLocation = SL_FOURTH;
			break;
		case SL_FOURTH:
			m_skillLocation = SL_FIFTH;
			break;
		case SL_FIFTH:
			m_skillLocation = SL_FIRST;
			break;
		}
	}
}
/*
Skill 행하는 순서
턴 제어 to TT_NATURAL
스킬 행하는 멘트
AttackObject기반으로 인한 수치변경 or 자기자신 효과(buff). (공격 or 자가)
공격일 경우 공격 object가 대사출력, 자가일 경우 
*/

void NormalAttack::useSkill(BattleState *bs)
{
/*
	m_battleLog.push_back(std::string("일반 공격을 사용하였습니다."));
	getHero()->attack( 0, getEnemy() );
	/*

	bs->pushBattleLog("힐링을 사용하였습니다.");
	int healPoint = m_hero->getInt();
	
	char stringBuffer[20];
	_itoa_s (healPoint, stringBuffer, 10);
	std::string resultLog = stringBuffer;
	resultLog += "포인트 HP를 회복하였습니다.";
	bs->pushBattleLog(resultLog.c_str());
	m_hero->heal (healPoint);
	//bs->setNextTurnType(TT_COMPUTER);
	//bs->passTurn();
	*/
}
