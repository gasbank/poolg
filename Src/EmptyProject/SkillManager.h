#pragma once

class Skill;

typedef std::map<std::string, Skill*> SkillMap;

class SkillManager : public Singleton<SkillManager>
{
public:
	SkillManager(void);
	~SkillManager(void);

	void registerSkill( Skill* skill );

	void unregisterAllSkills();

private:
	SkillMap m_skillMap;
};
inline SkillManager& GetSKillManager() { return SkillManager::getSingleton(); }