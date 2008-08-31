#include "EmptyProjectPCH.h"
#include "SkillSet.h"
#include "Skill.h"
#include "BattleState.h"

const std::string		SkillSet::ms_emptySlotSkillName("-");
const std::string		SkillSet::ms_emptySlotSkillDescription("스킬이 없습니다.");

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

const std::string& SkillSet::getSkillName (SkillLocation skillLocation) const
{
	/*비었을 경우 NULL 반환*/
	if (m_skillSet[(int)skillLocation] == NULL)
		return ms_emptySlotSkillName;
	return m_skillSet[(int)skillLocation]->getSkillName();
}

bool SkillSet::useSkill ()
{
	if (m_skillSet[(int)this->m_skillLocation] == NULL)
		return false;

	//m_skillSet[(int)this->m_skillLocation]->useSkill( m_battleState );

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
		/*if (m_skillSet[i] != NULL)
			m_skillSet[i]->setUser (hero, enemy);*/
	}
}


const std::string& SkillSet::getDescription (SkillLocation skillLocation) const
{
	if (m_skillSet[(int)skillLocation] == NULL)
		return ms_emptySlotSkillDescription;
	return m_skillSet[(int)skillLocation]->getDescription();
}
