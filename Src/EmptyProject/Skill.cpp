#include "EmptyProjectPCH.h"
#include "Skill.h"

using std::wstring;

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

void Skill::useSkill (BattleState* bs)
{
	bs->pushBattleLog("�׳�..");
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
	m_skillSet[(int)skillLocation] = NULL;
}

wstring SkillSet::getSkillName (SkillLocation skillLocation)
{
	/*����� ��� NULL ��ȯ*/
	if (m_skillSet[(int)skillLocation] == NULL)
		return L"-";
	return m_skillSet[(int)skillLocation]->m_skillName;
}

bool SkillSet::useSkill ()
{
	if (m_skillSet[(int)this->m_skillLocation] == NULL)
		return false;

	m_skillSet[(int)this->m_skillLocation]->useSkill( m_battleState );
	
	return true;
}

void SkillSet::moveSkillLocation (char mode)
{
	switch (mode)
	{
	/*initialize*/
	case 'i':
		m_skillLocation = SL_FIRST;
		break;
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
		break;
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
		break;
	}
}

void SkillSet::setCharacter (Character* hero, Character* enemy)
{
	for (int i = 0; i < 5; i++)
	{
		if (m_skillSet[i] != NULL)
			m_skillSet[i]->setUser (hero, enemy);
	}

}


wstring SkillSet::getDescription (SkillLocation skillLocation)
{
	if (m_skillSet[(int)skillLocation] == NULL)
		return L"��ų�� �����ϴ�.";
	return m_skillSet[(int)skillLocation]->getDescription();
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
	bs->pushBattleLog("�Ϲ� ������ ����Ͽ����ϴ�.");
	m_hero->attack(0, m_enemy);

}

void Heal::useSkill(BattleState *bs)
{
	bs->pushBattleLog("������ ����Ͽ����ϴ�.");
	m_hero->throwHealBall();
}

