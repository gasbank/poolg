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
	bs->pushBattleLog("그냥..");
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
	/*비었을 경우 NULL 반환*/
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
		return L"스킬이 없습니다.";
	return m_skillSet[(int)skillLocation]->getDescription();
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
	int curCs = m_hero->getCurCs();

	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("스피릿이 부족합니다.");
		bs->pushBattleLog("기술은 취소되며 코딩스피릿 번이 일어납니다.");
		m_hero->doCsBurn();
		return;
	}

	m_hero->setCurCs (curCs - m_csEssentials);

	bs->pushBattleLog("일반 공격을 사용하였습니다.");
	m_hero->doNormalAttack(0, m_enemy);

}

void Heal::useSkill(BattleState *bs)
{
	int curCs = m_hero->getCurCs();
	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("스피릿이 부족합니다.");
		bs->pushBattleLog("기술은 취소되며 코딩스피릿 번이 일어납니다.");
		m_hero->doCsBurn();
		return;
	}

	m_hero->setCurCs (curCs - m_csEssentials);


	bs->pushBattleLog("힐링을 사용하였습니다.");
	m_hero->doHeal();
}

void Meditation::useSkill(BattleState *bs)
{
	bs->pushBattleLog("메디테이션을 사용하였습니다.");
	m_hero->doMeditation();
}

void MultiThread::useSkill(BattleState *bs)
{
	int curCs = m_hero->getCurCs();
	if (curCs < m_csEssentials)
	{
		bs->pushBattleLog("스피릿이 부족합니다.");
		bs->pushBattleLog("기술은 취소되며 코딩스피릿 번이 일어납니다.");
		m_hero->doCsBurn();
		return;
	}
	
	m_hero->setCurCs (curCs - m_csEssentials);

	bs->pushBattleLog("멀티쓰레드를 사용하였습니다.");
	m_hero->doMultiThread(3, m_enemy);
}
