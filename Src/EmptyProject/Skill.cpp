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
	int curCs = m_hero->getCurCs();

	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("���Ǹ��� �����մϴ�.");
		bs->pushBattleLog("����� ��ҵǸ� �ڵ����Ǹ� ���� �Ͼ�ϴ�.");
		m_hero->doCsBurn();
		return;
	}

	m_hero->setCurCs (curCs - m_csEssentials);

	bs->pushBattleLog("�Ϲ� ������ ����Ͽ����ϴ�.");
	m_hero->doNormalAttack(0, m_enemy);

}

void Heal::useSkill(BattleState *bs)
{
	int curCs = m_hero->getCurCs();
	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("���Ǹ��� �����մϴ�.");
		bs->pushBattleLog("����� ��ҵǸ� �ڵ����Ǹ� ���� �Ͼ�ϴ�.");
		m_hero->doCsBurn();
		return;
	}

	m_hero->setCurCs (curCs - m_csEssentials);


	bs->pushBattleLog("������ ����Ͽ����ϴ�.");
	m_hero->doHeal();
}

void Meditation::useSkill(BattleState *bs)
{
	bs->pushBattleLog("�޵����̼��� ����Ͽ����ϴ�.");
	m_hero->doMeditation();
}

void MultiThread::useSkill(BattleState *bs)
{
	int curCs = m_hero->getCurCs();
	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("���Ǹ��� �����մϴ�.");
		bs->pushBattleLog("����� ��ҵǸ� �ڵ����Ǹ� ���� �Ͼ�ϴ�.");
		m_hero->doCsBurn();
		return;
	}
	
	m_hero->setCurCs (curCs - m_csEssentials);

	bs->pushBattleLog("��Ƽ�����带 ����Ͽ����ϴ�.");
	m_hero->doMultiThread(3, m_enemy);
}
