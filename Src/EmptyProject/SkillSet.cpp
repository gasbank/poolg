#include "EmptyProjectPCH.h"
#include "SkillSet.h"
#include "Skill.h"
#include "BattleState.h"

const std::string		SkillSet::ms_emptySlotSkillName("-");
const std::string		SkillSet::ms_emptySlotSkillDescription("스킬이 없습니다.");

SkillSet::SkillSet()
{
	for (int i = 0; i < ms_skillCount; i++)
		m_skillSet[i] = NULL;
}

SkillSet::~SkillSet()
{
	/*for (int i = 0; i < ms_skillCount; i++)
	{
		if (m_skillSet[i] != NULL)
			delete m_skillSet[i];
	}*/
}



void SkillSet::setSkill (SkillLocation skillLocation, Skill* skill)
{
	//if (m_skillSet[(int)skillLocation] != (NULL))
	//	delete m_skillSet[(int)skillLocation];
	m_skillSet[(int)skillLocation] = skill;
}

void SkillSet::deleteSkill (SkillLocation skillLocation)
{
	//if (m_skillSet[(int)skillLocation] != (NULL))
	//	delete m_skillSet[(int)skillLocation];
	m_skillSet[(int)skillLocation] = NULL;
}

const std::string& SkillSet::getSkillName( SkillLocation skillLocation ) const
{
	if (m_skillSet[(int)skillLocation] == NULL)
		return ms_emptySlotSkillName;
	return m_skillSet[(int)skillLocation]->getSkillName();
}
//
//bool SkillSet::useSkill()
//{
//	if (m_skillSet[(int)this->m_skillLocation] == NULL)
//		return false;
//
//	//m_skillSet[(int)this->m_skillLocation]->useSkill( m_battleState );
//
//	return true;
//}

//void SkillSet::moveSkillLocation( char mode )
//{
//	switch (mode)
//	{
//		/*initialize*/
//	case 'i':
//		m_skillLocation = SL_FIRST;
//		break;
//		/*up*/
//	case 'u':
//		switch (m_skillLocation)
//		{
//		case SL_FIRST:
//			m_skillLocation = SL_FIFTH;
//			break;
//		case SL_SECOND:
//			m_skillLocation = SL_FIRST;
//			break;
//		case SL_THIRD:
//			m_skillLocation = SL_SECOND;
//			break;
//		case SL_FOURTH:
//			m_skillLocation = SL_THIRD;
//			break;
//		case SL_FIFTH:
//			m_skillLocation = SL_FOURTH;
//			break;
//		}
//		break;
//		/*down*/
//	case 'd':
//		switch (m_skillLocation)
//		{
//		case SL_FIRST:
//			m_skillLocation = SL_SECOND;
//			break;
//		case SL_SECOND:
//			m_skillLocation = SL_THIRD;
//			break;
//		case SL_THIRD:
//			m_skillLocation = SL_FOURTH;
//			break;
//		case SL_FOURTH:
//			m_skillLocation = SL_FIFTH;
//			break;
//		case SL_FIFTH:
//			m_skillLocation = SL_FIRST;
//			break;
//		}
//		break;
//	}
//}
//
//void SkillSet::setCharacter (Character* hero, Character* enemy)
//{
//	for (int i = 0; i < 5; i++)
//	{
//		/*if (m_skillSet[i] != NULL)
//			m_skillSet[i]->setUser (hero, enemy);*/
//	}
//}


const std::string& SkillSet::getDescription (SkillLocation skillLocation) const
{
	if (m_skillSet[(int)skillLocation] == NULL)
		return ms_emptySlotSkillDescription;
	return m_skillSet[(int)skillLocation]->getDescription();
}

void SkillSet::deleteAllSkills()
{
	m_skillSet[0] = 0;
	m_skillSet[1] = 0;
	m_skillSet[2] = 0;
	m_skillSet[3] = 0;
	m_skillSet[4] = 0;
}

bool SkillSet::acquireSkill( const Skill* skill )
{
	ConstSkillList::const_iterator cit = std::find( m_memorizedSkills.begin(), m_memorizedSkills.end(), skill );
	if ( skill && ( cit == m_memorizedSkills.end() ) )
	{
		m_memorizedSkills.push_back( skill );
		return true;
	}
	// Has skill already or skill param is null.
	return false;
}