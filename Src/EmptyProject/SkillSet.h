#pragma once

class Skill;
class BattleState;

typedef std::list<const Skill*> ConstSkillList;

enum SkillLocation { SL_FIRST = 0, SL_SECOND, SL_THIRD, SL_FOURTH, SL_FIFTH };


class SkillSet
{
public:
									SkillSet();
									~SkillSet();

	bool							acquireSkill( const Skill* skill );

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
	const Skill*					m_skillSet[ms_skillCount];	// Equipped Skills: Five active skills drawn from memorized skills.
	ConstSkillList					m_memorizedSkills;			// Memorized Skills: Skills which an owner of this SkillSet is capable of using.

	static const std::string		ms_emptySlotSkillName;
	static const std::string		ms_emptySlotSkillDescription;
};
