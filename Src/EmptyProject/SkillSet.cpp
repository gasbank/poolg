#include "EmptyProjectPCH.h"
#include "SkillSet.h"
#include "Skill.h"
#include "BattleState.h"
#include "Character.h"

const std::string SkillSet::ms_emptySlotSkillName("-");
const std::string SkillSet::ms_emptySlotSkillDescription("스킬이 없습니다.");

SkillSet::SkillSet( Character* owner )
: m_owner( owner )
{
	assert( m_owner );
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

const std::string& SkillSet::getSkillName( UINT slot ) const
{
	if (m_equippedSkills.size() <= slot || m_equippedSkills[slot] == NULL)
		return ms_emptySlotSkillName;
	return m_equippedSkills[slot]->getSkillName();
}

const std::string& SkillSet::getDescription( UINT slot ) const
{
	if (m_equippedSkills.size() <= slot || m_equippedSkills[slot] == NULL)
		return ms_emptySlotSkillDescription;
	return m_equippedSkills[slot]->getDescription();
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

bool SkillSet::useSkill( UINT slot, Character* target ) const
{
	if ( slot >= m_equippedSkills.size() )
		throw std::runtime_error( "Slot index is larger than a number of skill slots." );

	const Skill* skill = m_equippedSkills[slot];
	if ( skill )
	{
		// TODO: Clone the skill's skill objects and register to owner(type of Character)'s m_skillObjects.
		SkillObjectList soList;
		if ( skill->getClonedSkillObjects( soList, target ) )
		{
			assert( !soList.empty() );
			m_owner->pushSkillObjectList( soList );
			return true;
		}
		else
			return false;
	}
	// Selected skill slot is empty.
	return false;
}