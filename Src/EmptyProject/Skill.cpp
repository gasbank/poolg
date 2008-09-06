#include "EmptyProjectPCH.h"
#include "Skill.h"
#include "DynamicMotion.h"
#include "SkillObject.h"
#include "BattleState.h"
#include "Character.h"
#include "ScriptManager.h"


/*
Skill 행하는 순서
턴 제어 to TT_NATURAL
스킬 행하는 멘트
AttackObject기반으로 인한 수치변경 or 자기자신 효과(buff). (공격 or 자가)
공격일 경우 공격 object가 대사출력, 자가일 경우 
*/

//
//void Skill::useSkill (BattleState *bs)
//{
//	int curCs = m_hero->getCurCs();
//
//	if (curCs < m_csEssentials)
//	{
//		bs->pushBattleLog("스피릿이 부족합니다.");
//		bs->pushBattleLog("기술은 취소되며 코딩스피릿 번이 일어납니다.");
//		m_hero->doCsBurn();
//		return;
//	}
//
//	m_hero->setCurCs (curCs - m_csEssentials);
//
//}

Skill* Skill::createSkillByScript( const char* skillNsName )
{
	char tempBuf[256];
	StringCchPrintfA( tempBuf, 256, "%s::name", skillNsName );
	const char* skillName = GetScriptManager().readString( tempBuf );
	StringCchPrintfA( tempBuf, 256, "%s::description", skillNsName );
	const char* skillDescription = GetScriptManager().readString( tempBuf );
	StringCchPrintfA( tempBuf, 256, "%s::csEssentials", skillNsName );
	int csEssentials = GetScriptManager().readInt( tempBuf );

	Skill* ret = new Skill( skillName, skillDescription, csEssentials );

	StringCchPrintfA( tempBuf, 256, "%s::registerSkillObjects", skillNsName );
	Tcl_Obj* skillObjects = GetScriptManager().execute( tempBuf );

	int skillObjectsCount = 0;
	Tcl_Interp* interp = GetScriptManager().getInterp();
	Tcl_ListObjLength( interp, skillObjects, &skillObjectsCount );
	
	int i;
	for ( i = 0; i < skillObjectsCount; ++i )
	{
		Tcl_Obj* elem;
		long soPtrVal = 0;
		SkillObject* so = 0;
		Tcl_ListObjIndex( interp, skillObjects, i, &elem );
		Tcl_GetLongFromObj( interp, elem, &soPtrVal );
		so = reinterpret_cast<SkillObject*>( soPtrVal );
		if ( so->getType() == UT_SKILLOBJECT )
			ret->addSkillObject( so );
		else
			throw std::runtime_error( "Serious error on script file." );
	}

	return ret;
}

Skill::Skill( const char* name, const char* desc, int csEssentials )
{
	m_skillName			= name;
	m_skillDescription	= desc;
	m_csEssentials		= csEssentials;
}

void Skill::addSkillObject( SkillObject* so )
{
	m_skillObjects.push_back( so );
}

Skill::~Skill()
{
	release();
}

void Skill::release()
{
	EpSafeReleaseAll( m_skillObjects );
}

bool Skill::getClonedSkillObjects( SkillObjectList& soList, Character* target ) const
{
	SkillObjectList::const_iterator cit = m_skillObjects.begin();
	for ( ; cit != m_skillObjects.end(); ++cit )
	{
		SkillObject* so = SkillObject::clone( **cit );
		so->setTarget( target );
		soList.push_back( so );
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////


//void NormalAttack::useSkill(BattleState *bs)
//{
//	int curCs = m_hero->getCurCs();
//
//	if (curCs < m_csEssentials)
//	{
//		bs->pushBattleLog("스피릿이 부족합니다.");
//		bs->pushBattleLog("기술은 취소되며 코딩스피릿 번이 일어납니다.");
//		m_hero->doCsBurn();
//		return;
//	}
//
//	m_hero->setCurCs (curCs - m_csEssentials);
//
//	bs->pushBattleLog("일반 공격을 사용하였습니다.");
//	//m_hero->doNormalAttack(0, m_enemy);
//
//	D3DXVECTOR3 fireDir = m_enemy->getPos() - m_hero->getPos();
//	float dist = D3DXVec3Length( &fireDir );
//
//	D3DXVec3Normalize( &fireDir, &fireDir );
//
//
//
//	LPD3DXMESH mesh;
//	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0 );
//	Unit* missile = Unit::createUnit(mesh, 0, 0, 0);
//
//	missile->setDynamicMotion(DynamicMotion::createDMfireUniformly
//		(missile, m_hero->getPos(), fireDir, dist, 40.0f ));
//
//	SkillObject* so = SkillObject::createSOnormalAttack( m_hero, m_enemy, missile);
//
//	m_hero->pushSkillObject( so );
//
//	//m_skillObjectList.push_back(so);
//
//}
//
////////////////////////////////////////////////////////////////////////////
//
//
//void Heal::useSkill(BattleState *bs)
//{
//	int curCs = m_hero->getCurCs();
//	if (curCs < m_csEssentials)
//	{
//		bs->pushBattleLog("스피릿이 부족합니다.");
//		bs->pushBattleLog("기술은 취소되며 코딩스피릿 번이 일어납니다.");
//		m_hero->doCsBurn();
//		return;
//	}
//
//	m_hero->setCurCs (curCs - m_csEssentials);
//
//
//	bs->pushBattleLog("힐링을 사용하였습니다.");
//
//	LPD3DXMESH mesh;
//	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0);
//	Unit* healBall = Unit::createUnit(mesh, 0, 0, 0);
//
//	healBall->setDynamicMotion(DynamicMotion::createDMspinAround
//		(healBall, m_hero->getPos(), 5, 0.03f, 10));
//
//	SkillObject* so = SkillObject::createSOheal(m_hero, healBall);
//	m_hero->pushSkillObject( so );
//
//}
//
////////////////////////////////////////////////////////////////////////////
//
//
//void Meditation::useSkill(BattleState *bs)
//{
//	bs->pushBattleLog("메디테이션을 사용하였습니다.");
//	LPD3DXMESH mesh;
//	D3DXCreateSphere( GetG().m_dev, 0.7f, 16, 16, &mesh, 0);
//	Unit* healBall = Unit::createUnit(mesh, 0, 0, 0);
//
//	healBall->setDynamicMotion(DynamicMotion::createDMspinAround
//		(healBall, m_hero->getPos(), 5, 0.03f, 2));
//    
//	SkillObject* so = SkillObject::createSOmeditation( m_hero, healBall );
//	m_hero->pushSkillObject ( so );
//}
//
////////////////////////////////////////////////////////////////////////////
//
//void MultiThread::useSkill(BattleState *bs)
//{
//	int curCs = m_hero->getCurCs();
//	if (curCs < m_csEssentials)
//	{
//		bs->pushBattleLog("스피릿이 부족합니다.");
//		bs->pushBattleLog("기술은 취소되며 코딩스피릿 번이 일어납니다.");
//		m_hero->doCsBurn();
//		return;
//	}
//	
//	m_hero->setCurCs (curCs - m_csEssentials);
//
//	bs->pushBattleLog("멀티쓰레드를 사용하였습니다.");
//	//m_hero->doMultiThread(3, m_enemy);
//
//	D3DXVECTOR3 fireDir = m_enemy->getPos() - m_hero->getPos();
//	float dist = D3DXVec3Length( &fireDir );
//
//	D3DXVec3Normalize( &fireDir, &fireDir );
//
//	int frequency = m_hero->getStat().sense / 5;
//
//	for (int i=frequency-1; i >= 0; i--)
//	{
//		LPD3DXMESH mesh;
//		D3DXCreateBox( GetG().m_dev, 0.5f, 8, 0.5f, &mesh, 0);
//		Unit* effectObj = Unit::createUnit(mesh, 0, 0, 0);
//
//		effectObj->setDynamicMotion(DynamicMotion::createDMfireUniformly
//		(effectObj, m_hero->getPos(), fireDir, dist, 4.0f ));
//
//		SkillObject* so = SkillObject::createSOmtBullet( m_hero, m_enemy, effectObj, i, frequency );
//		m_hero->pushSkillObject( so );
//	}
//
//}
//
////////////////////////////////////////////////////////////////////////////
//
//void Goto::useSkill(BattleState *bs)
//{
//	int curCs = m_hero->getCurCs();
//	if (curCs < m_csEssentials)
//	{
//		bs->pushBattleLog("스피릿이 부족합니다.");
//		bs->pushBattleLog("기술은 취소되며 코딩스피릿 번이 일어납니다.");
//		m_hero->doCsBurn();
//		return;
//	}
//	
//	m_hero->setCurCs (curCs - m_csEssentials);
//
//	bs->pushBattleLog("금기의 Goto를 사용하였습니다.");
//	//m_hero->doGoto (m_enemy);
//
//	LPD3DXMESH mesh;
//	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0);
//	Unit* effectObj = Unit::createUnit(mesh, 0, 0, 0);
//
//	effectObj->setDynamicMotion(DynamicMotion::createDMpuff
//		(effectObj, m_enemy->getPos(), 25, 0.2f));
//
//	SkillObject* so = SkillObject::createSOgoto(m_hero, m_enemy, effectObj);
//	m_hero->pushSkillObject( so );
//}
