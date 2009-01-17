#pragma once

class Skill;

typedef std::map<std::string, Skill*> SkillMap;

/**
@brief Skill 인스턴스를 관리하는 클래스
*/
class SkillManager : public Singleton<SkillManager>
{
public:
	SkillManager(void);
	~SkillManager(void);

	void registerSkill( Skill* skill );

	void unregisterAllSkills();

	const Skill* getSkill( const char* skillName ) const;
private:
	SkillMap m_skillMap;
};
inline SkillManager& GetSKillManager() { return SkillManager::getSingleton(); }