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
	/*����� ��� NULL ��ȯ*/
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
Skill ���ϴ� ����
�� ���� to TT_NATURAL
��ų ���ϴ� ��Ʈ
AttackObject������� ���� ��ġ���� or �ڱ��ڽ� ȿ��(buff). (���� or �ڰ�)
������ ��� ���� object�� ������, �ڰ��� ��� 
*/

void NormalAttack::useSkill(BattleState *bs)
{
/*
	m_battleLog.push_back(std::string("�Ϲ� ������ ����Ͽ����ϴ�."));
	getHero()->attack( 0, getEnemy() );
	/*

	bs->pushBattleLog("������ ����Ͽ����ϴ�.");
	int healPoint = m_hero->getInt();
	
	char stringBuffer[20];
	_itoa_s (healPoint, stringBuffer, 10);
	std::string resultLog = stringBuffer;
	resultLog += "����Ʈ HP�� ȸ���Ͽ����ϴ�.";
	bs->pushBattleLog(resultLog.c_str());
	m_hero->heal (healPoint);
	//bs->setNextTurnType(TT_COMPUTER);
	//bs->passTurn();
	*/
}
