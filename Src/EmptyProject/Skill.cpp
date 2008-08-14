#include "EmptyProjectPCH.h"
#include "Skill.h"

using std::string;

SkillSet::SkillSet()
{
	m_skillLocation = SL_FIRST;
}

void SkillSet::setSkill(SkillLocation skillLocation, Skill* skill)
{
	if (m_skillSet[(int)skillLocation] != (NULL))
		delete m_skillSet[(int)skillLocation];
	m_skillSet[(int)skillLocation] = skill;
}

void SkillSet::deleteSkill(SkillLocation skillLocation)
{
	if (m_skillSet[(int)skillLocation] != (NULL))
		delete m_skillSet[(int)skillLocation];
}

string SkillSet::getSkillName(SkillLocation skillLocation)
{
	/*비었을 경우 NULL 반환*/
	if (m_skillSet[(int)skillLocation] == NULL)
		return NULL;
	return m_skillSet[(int)skillLocation]->m_skillName;
}

void SkillSet::useSkill(SkillLocation skillLocation)
{
	if (m_skillSet[(int)skillLocation] == NULL)
		return;

	m_skillSet[(int)skillLocation]->useSkill(m_battleState);
}