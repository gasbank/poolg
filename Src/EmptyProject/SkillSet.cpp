#include "EmptyProjectPCH.h"
#include "SkillSet.h"
#include "Skill.h"
#include "BattleState.h"

const std::string		SkillSet::ms_emptySlotSkillName("-");
const std::string		SkillSet::ms_emptySlotSkillDescription("스킬이 없습니다.");

SkillSet::SkillSet()
{
	m_equippedSkills.resize( 5 );
}

SkillSet::~SkillSet()
{
}

void SkillSet::setSkill (SkillLocation skillLocation, Skill* skill)
{
	//if (m_equippedSkills[(int)skillLocation] != (NULL))
	//	delete m_equippedSkills[(int)skillLocation];
	m_equippedSkills[(int)skillLocation] = skill;
}

void SkillSet::deleteSkill (SkillLocation skillLocation)
{
	//if (m_equippedSkills[(int)skillLocation] != (NULL))
	//	delete m_equippedSkills[(int)skillLocation];
	m_equippedSkills[(int)skillLocation] = NULL;
}

const std::string& SkillSet::getSkillName( SkillLocation skillLocation ) const
{
	if (m_equippedSkills[(int)skillLocation] == NULL)
		return ms_emptySlotSkillName;
	return m_equippedSkills[(int)skillLocation]->getSkillName();
}
//
//bool SkillSet::useSkill()
//{
//	if (m_equippedSkills[(int)this->m_skillLocation] == NULL)
//		return false;
//
//	//m_equippedSkills[(int)this->m_skillLocation]->useSkill( m_battleState );
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
//		/*if (m_equippedSkills[i] != NULL)
//			m_equippedSkills[i]->setUser (hero, enemy);*/
//	}
//}


const std::string& SkillSet::getDescription (SkillLocation skillLocation) const
{
	if (m_equippedSkills[(int)skillLocation] == NULL)
		return ms_emptySlotSkillDescription;
	return m_equippedSkills[(int)skillLocation]->getDescription();
}

void SkillSet::deleteAllSkills()
{
	m_equippedSkills[0] = 0;
	m_equippedSkills[1] = 0;
	m_equippedSkills[2] = 0;
	m_equippedSkills[3] = 0;
	m_equippedSkills[4] = 0;
}

bool SkillSet::memorizeSkill( const Skill* skill )
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

bool SkillSet::equipSkill( UINT slot, const Skill* skill )
{
	if ( slot >= m_equippedSkills.size() )
		throw std::runtime_error( "Slot index is larger than a number of skill slots." );

	ConstSkillList::const_iterator memorized = std::find( m_memorizedSkills.begin(), m_memorizedSkills.end(), skill );
	ConstSkillVector::const_iterator equipped = std::find( m_equippedSkills.begin(), m_equippedSkills.end(), skill );
	if ( memorized != m_memorizedSkills.end() && equipped == m_equippedSkills.end() )
	{
		m_equippedSkills[slot] = skill;
		return true;
	}
	// Not memorized or already equipped skill.
	return false;
}