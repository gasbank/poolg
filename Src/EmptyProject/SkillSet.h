#pragma once

class Skill;
class BattleState;


enum SkillLocation { SL_FIRST = 0, SL_SECOND, SL_THIRD, SL_FOURTH, SL_FIFTH };


class SkillSet
{
public:
									SkillSet();
									~SkillSet();

	void							setSkill( SkillLocation skillLocation, Skill* skill );
	const Skill*					getSkill( SkillLocation skillLoc ) const { return m_skillSet[(int)skillLoc]; }
	void							deleteSkill( SkillLocation skillLocation );
	void							deleteAllSkills();
	//bool							useSkill();
	const std::string&				getSkillName( SkillLocation skillLocation ) const;
	const std::string&				getDescription( SkillLocation skillLocation ) const;

	/*SkillLocation					getSkillLocation() const { return m_skillLocation; }*/
	/*void							moveSkillLocation( char mode );
	void							setCharacter( Character* hero, Character* enemy );*/

	///*사용 전엔 반드시 battleState를 설정해 주어야 한다.*/
	//void							setBattleState( BattleState* battleState ) { m_battleState = battleState; }

private:
	static const UINT				ms_skillCount = 5;
	const Skill*					m_skillSet[ms_skillCount];

	/*SkillLocation					m_skillLocation;*/
	/*BattleState*					m_battleState;*/

	static const std::string		ms_emptySlotSkillName;
	static const std::string		ms_emptySlotSkillDescription;
};
