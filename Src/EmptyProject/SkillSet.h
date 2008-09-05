#pragma once

class Skill;
class BattleState;

typedef std::vector<const Skill*> ConstSkillVector;
typedef std::list<const Skill*> ConstSkillList;

enum SkillLocation { SL_FIRST = 0, SL_SECOND, SL_THIRD, SL_FOURTH, SL_FIFTH };


class SkillSet
{
public:
									SkillSet();
									~SkillSet();

	bool							memorizeSkill( const Skill* skill );
	bool							equipSkill( UINT slot, const Skill* skill );

	void							setSkill( SkillLocation skillLocation, Skill* skill );
	const Skill*					getSkill( SkillLocation skillLoc ) const { return m_equippedSkills[(int)skillLoc]; }
	void							deleteSkill( SkillLocation skillLocation );
	void							deleteAllSkills();
	//bool							useSkill();
	const std::string&				getSkillName( SkillLocation skillLocation ) const;
	const std::string&				getDescription( SkillLocation skillLocation ) const;

	/*SkillLocation					getSkillLocation() const { return m_skillLocation; }*/
	/*void							moveSkillLocation( char mode );
	void							setCharacter( Character* hero, Character* enemy );*/

	///*��� ���� �ݵ�� battleState�� ������ �־�� �Ѵ�.*/
	//void							setBattleState( BattleState* battleState ) { m_battleState = battleState; }

private:
	//static const UINT				ms_skillCount = 5;
	ConstSkillVector				m_equippedSkills;		// Active skills drawn from memorized skills.
	ConstSkillList					m_memorizedSkills;		// Skills which an owner of this SkillSet is capable of using.

	static const std::string		ms_emptySlotSkillName;
	static const std::string		ms_emptySlotSkillDescription;
};
