#pragma once

class Skill;
class BattleState;
class Character;

typedef std::vector<const Skill*> ConstSkillVector;
typedef std::list<const Skill*> ConstSkillList;

enum SkillLocation { SL_FIRST = 0, SL_SECOND, SL_THIRD, SL_FOURTH, SL_FIFTH };

/**
@brief Character 가 가질 수 있는 Skill 목록을 관리하는 클래스
*/
class SkillSet
{
public:
									SkillSet( Character* owner );
									~SkillSet();

	bool							memorizeSkill( const Skill* skill );
	bool							equipSkill( UINT slot, const Skill* skill );

	void							setSkill( SkillLocation skillLocation, Skill* skill );
	const Skill*					getSkill( SkillLocation skillLoc ) const { return m_equippedSkills[(int)skillLoc]; }
	void							deleteSkill( SkillLocation skillLocation );
	void							deleteAllSkills();
	bool							useSkill( UINT slot, Character* user, Character* target ) const;
	const std::string&				getSkillName( UINT slot ) const;
	const std::string&				getDescription( UINT slot ) const;

	/*SkillLocation					getSkillLocation() const { return m_skillLocation; }*/
	/*void							moveSkillLocation( char mode );
	void							setCharacter( Character* hero, Character* enemy );*/

	///*사용 전엔 반드시 battleState를 설정해 주어야 한다.*/
	//void							setBattleState( BattleState* battleState ) { m_battleState = battleState; }

private:
	Character*						m_owner;

	ConstSkillVector				m_equippedSkills;		// Active skills drawn from memorized skills.
	ConstSkillList					m_memorizedSkills;		// Skills which an owner of this SkillSet is capable of using.

	static const std::string		ms_emptySlotSkillName;
	static const std::string		ms_emptySlotSkillDescription;
};
