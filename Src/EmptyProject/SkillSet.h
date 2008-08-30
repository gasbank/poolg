#pragma once

class Skill;
class BattleState;


enum SkillLocation { SL_FIRST, SL_SECOND, SL_THIRD, SL_FOURTH, SL_FIFTH };


class SkillSet
{
public:
	SkillSet();
	~SkillSet();
	void setSkill( SkillLocation skillLocation, Skill* skill );
	void deleteSkill( SkillLocation skillLocation );
	bool useSkill();
	const std::wstring& getSkillName( SkillLocation skillLocation ) const;
	SkillLocation getSkillLocation() const { return m_skillLocation; }
	void moveSkillLocation(char mode);
	void setCharacter (Character* hero, Character* enemy);

	const std::wstring& getDescription (SkillLocation skillLocation) const;

	/*사용 전엔 반드시 battleState를 설정해 주어야 한다.*/
	void setBattleState( BattleState* battleState ) { m_battleState = battleState; }

private:
	Skill*				m_skillSet[5];
	SkillLocation		m_skillLocation;
	BattleState*		m_battleState;

	static const std::wstring		ms_emptySlotSkillName;
	static const std::wstring		ms_emptySlotSkillDescription;
};
