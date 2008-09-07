#include "EmptyProjectPCH.h"
#include "SkillManager.h"
#include "Skill.h"

IMPLEMENT_SINGLETON( SkillManager )

SkillManager::SkillManager(void)
{
}

SkillManager::~SkillManager(void)
{
	unregisterAllSkills();
}

void SkillManager::registerSkill( Skill* skill )
{
	if ( m_skillMap.find( skill->getSkillName() ) == m_skillMap.end() )
	{
		m_skillMap[ skill->getSkillName() ] = skill;
	}
	else
	{
		throw std::runtime_error( "Duplicate skill name is registered." );
	}
}

void SkillManager::unregisterAllSkills()
{
	EpSafeReleaseAllMap( m_skillMap );
}

const Skill* SkillManager::getSkill( const char* skillName ) const
{
	SkillMap::const_iterator cit = m_skillMap.find( skillName );
	if ( cit != m_skillMap.end() )
		return cit->second;
	else
		return 0;
}