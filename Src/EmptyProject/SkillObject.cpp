#include "EmptyProjectPCH.h"
#include "SkillObject.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "Character.h"
#include "DynamicMotion.h"
#include "Sound.h"
#include "ShaderWrapper.h"
#include "Unit.h"
#include "ScriptManager.h"
#include "Action.h"
#include "DynamicMotion.h"

extern BombShader*						g_bombShader;
extern LPD3DXMESH						g_bst[BST_COUNT];

SkillObject::SkillObject( BasicShapeType bst, float size, D3DCOLOR color, DynamicMotionType dmt )
: Unit( UT_SKILLOBJECT )
, m_bst( bst )
, m_target( 0 )
, m_velocity( 0 )
, m_dm( DynamicMotion::createDynamicMotion( dmt ) )
, m_size( size )
, m_color( color )
{
}

SkillObject::~SkillObject(void)
{
	//m_effectObject->release();
	//delete m_effectObject;

	EP_SAFE_RELEASE( m_dm );
	EpSafeReleaseAll( m_onHitActionList );

}

bool SkillObject::frameMove ( float fElapsedTime )
{
	//return this->m_effectObject->frameMove(fElapsedTime);
	return true;
}

HRESULT SkillObject::frameRender ( double dTime, float fElapsedTime )
{
	HRESULT f = 0;
	D3DPERF_BeginEvent(0, L"SkillObjRender");

	HRESULT hr = S_OK;
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	UINT iPass, cPasses;
	V( g_bombShader->setMainTechnique() );
	
	//V( g_bombShader->setWorldViewProj( dTime, fElapsedTime, &this->m_effectObject->getLocalXform(), GetG().m_camera.GetViewMatrix(), GetG().m_camera.GetProjMatrix() ) );

	V( g_bombShader->begin( &cPasses, 0 ) );
	for( iPass = 0; iPass < cPasses; iPass++ )
	{
		V( g_bombShader->beginPass( iPass ) );
		//f = this->m_effectObject->getMesh()->DrawSubset( 0 );
		V( g_bombShader->endPass() );
	}
	V( g_bombShader->end() );
	
	D3DPERF_EndEvent ();
	return f;
}

BattleState* SkillObject::getBattleState()
{
	return reinterpret_cast<BattleState*>( GetWorldStateManager().getCurState() );
}

SkillObject* SkillObject::createSkillObject( const char* bst, float size, D3DCOLOR color, const char* dmt )
{
	BasicShapeType bstEnum;
	if ( strcmp( bst, "SPHERE" ) == 0 ) bstEnum = BST_SPHERE;
	else if ( strcmp( bst, "CUBE" ) == 0 ) bstEnum = BST_CUBE;
	else if ( strcmp( bst, "PLANE" ) == 0 ) bstEnum = BST_PLANE;
	else bstEnum = BST_UNKNOWN;

	DynamicMotionType dmtEnum;
	if ( strcmp( bst, "FIRE_UNIFORMLY" ) == 0 ) dmtEnum = DMT_FIRE_UNIFORMLY;
	else if ( strcmp( bst, "RANDOM_CURVE" ) == 0 ) dmtEnum = DMT_RANDOM_CURVE;
	else dmtEnum = DMT_UNKNOWN;

	SkillObject* so = new SkillObject( bstEnum, size, color, dmtEnum );
	return so;
}


//////////////////////////////////////////////////////////////////////////

SkillObject* EpCreateSkillObject( const char* bst, double size, int color /* ARGB */, const char* dmt )
{
	return SkillObject::createSkillObject( bst, (float)size, (D3DCOLOR)color, dmt );
} SCRIPT_CALLABLE_PV_PC_D_I_PC( EpCreateSkillObject )

int EpSkillObjectAddOnHitAction( void* soPtr, void* actPtr )
{
	SkillObject* so = reinterpret_cast<SkillObject*>( soPtr );
	Action* act = reinterpret_cast<Action*>( actPtr );
	so->addOnHitAction( act );
	return 0;
} SCRIPT_CALLABLE_I_PV_PV( EpSkillObjectAddOnHitAction )



START_SCRIPT_FACTORY( SkillObject )
	CREATE_OBJ_COMMAND( EpCreateSkillObject )
	CREATE_OBJ_COMMAND( EpSkillObjectAddOnHitAction )
END_SCRIPT_FACTORY( SkillObject )



//////////////////////////////////////////////////////////////////////////


//SkillObject* SkillObject::createSOnormalAttack(Character* user, Character* target, Unit* effectObject)
//{
//	SOnormalAttack* so = new SOnormalAttack (user, target, effectObject);
//	return so;
//}
//
//SkillObject* SkillObject::createSOheal(Character* user, Unit* effectObject)
//{
//	SOheal* so = new SOheal (user, effectObject);
//	return so;
//}
//
//SkillObject* SkillObject::createSOmeditation(Character* user, Unit* effectObject)
//{
//	SOmeditation* so = new SOmeditation (user, effectObject);
//	return so;
//}
//
//SkillObject* SkillObject::createSOcsBurn(Character* target, Unit* effectObject)
//{
//	SOcsBurn* so = new SOcsBurn (target, effectObject);
//	return so;
//}
//
//SkillObject* SkillObject::createSOmtBullet(Character* user, Character* target, Unit* effectObject, int leftNumber, int maxNumber)
//{
//	SOmtBullet* so = new SOmtBullet (user, target, effectObject, leftNumber, maxNumber);
//	return so;
//}
//
//SkillObject* SkillObject::createSOgoto (Character* user, Character* target, Unit* effectObject)
//{
//	SOgoto* so = new SOgoto (user, target, effectObject);
//	return so;
//}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

//SOnormalAttack::~SOnormalAttack(void)
//{
//}
//
//bool SOnormalAttack::frameMove (float fElapsedTime)
//{
//
//	if ( !m_effectObject->frameMove ( fElapsedTime ) )
//	{
//		//스크립트 함수를 호출한다.
//		//onObjectHitTarget(---);
//
//
//		// Hit to the target!
//		int damage = m_user->getStat().coding- m_target->getStat().def + 5;
//
//		m_target->damage(damage);
//
//		D3DXVECTOR3 posDiff = m_target->getPos() - m_user->getPos();
//		D3DXVec3Normalize( &posDiff, &posDiff );
//		m_target->addMoveImpulse( posDiff / 3 ); // Attacked unit shows startled shake
//
//		char stringBuffer[20];
//		_itoa_s (damage, stringBuffer, 10);
//		std::string resultLog = stringBuffer;
//
//		if ( m_target->getType() == UT_HERO )
//		{ 
//
//			resultLog += "포인트 데미지를 받았다!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("HP가 모두 소진되었습니다. 게임 오버입니다.");
//				getBattleState()->pushBattleLog("순순히 F5키를 누르고 종료하시죠.");
//				m_target->setDead();
//				getBattleState()->setNextTurnType(TT_NATURAL);
//				getBattleState()->passTurn();
//			}
//			else
//			{
//				getBattleState()->setNextTurnType(TT_PLAYER);
//				getBattleState()->passTurn();
//			}
//		}
//		else
//		{
//			resultLog += "포인트 데미지를 입혔다!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("대상을 섬멸하였습니다! (Enter key로 월드로 복귀)");
//				m_target->setDead();
//				getBattleState()->setNextTurnType(TT_NATURAL);
//				GetAudioState().pSoundBank->Play( GetAudioState().iAttack, 0, 0, NULL );
//			}
//			else
//			{
//				getBattleState()->setNextTurnType(TT_COMPUTER);
//				getBattleState()->passTurn();
//				GetAudioState().pSoundBank->Play( GetAudioState().iAttack, 0, 0, NULL );
//			}
//		}
//
//		return false;
//	}
//	return true;
//}
//
//SOnormalAttack::SOnormalAttack (Character* user, Character* target, Unit* effectObject)
//{
//	m_user = user;
//	m_target = target;
//	m_effectObject = effectObject;
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//SOheal::~SOheal(void)
//{
//}
//
//bool SOheal::frameMove (float fElapsedTime)
//{
//	if ( !m_effectObject->frameMove( fElapsedTime ))
//	{
//		int healPoint = m_user->getStat().sense * 50;
//		m_user->heal (healPoint);
//
//		char stringBuffer[20];
//		_itoa_s (healPoint, stringBuffer, 10);
//		std::string resultLog = stringBuffer;
//		resultLog += "포인트 HP가 회복됩니다.";
//		getBattleState()->pushBattleLog(resultLog.c_str());
//
//		if ( m_user->getType() == UT_HERO )
//		{
//			getBattleState()->setNextTurnType(TT_COMPUTER);
//			getBattleState()->passTurn();
//			GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
//		}
//		else
//		{
//			getBattleState()->setNextTurnType(TT_PLAYER);
//			getBattleState()->passTurn();
//		}
//		return false;
//
//	}
//	return true;
//}
//
//SOheal::SOheal (Character* user, Unit* effectObject)
//{
//	m_user = user;
//	m_effectObject = effectObject;
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//SOmeditation::~SOmeditation(void)
//{
//}
//
//bool SOmeditation::frameMove (float fElapsedTime)
//{
//	if ( !m_effectObject->frameMove( fElapsedTime ))
//	{
//		for (int i=0; i<3; i++)
//		{
//			m_user->recoverCs();
//		}
//
//		getBattleState()->pushBattleLog("청명한 기운을 느끼며 CS가 30% 회복됩니다.");
//
//		if ( m_user->getType() == UT_HERO )
//		{
//			getBattleState()->setNextTurnType(TT_COMPUTER);
//			getBattleState()->passTurn();
//			GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
//		}
//		else
//		{
//			getBattleState()->setNextTurnType(TT_PLAYER);
//			getBattleState()->passTurn();
//		}
//		return false;
//	}
//	return true;
//}
//
//SOmeditation::SOmeditation (Character* user, Unit* effectObject)
//{
//	m_user = user;
//	m_effectObject = effectObject;
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//SOcsBurn::~SOcsBurn(void)
//{
//}
//
//bool SOcsBurn::frameMove (float fElapsedTime)
//{
//	if ( !m_effectObject->frameMove( fElapsedTime ))
//	{
//		// Hit to the target!
//		m_target->setCurCs(0);
//
//		getBattleState()->pushBattleLog("스피릿이 모두 소진되었습니다.");
//
//		int m_burnDamage = m_target->getCurHp() / 2;
//
//		m_target->damage(m_burnDamage);
//
//		getBattleState()->pushBattleLog("스피릿 소진에 의한 현기증으로 인해");
//		getBattleState()->pushBattleLog("현재 HP가 반으로 줄어듭니다.");
//
//
//
//		if ( m_target->getType() == UT_HERO )
//		{
//			getBattleState()->setNextTurnType(TT_COMPUTER);
//			getBattleState()->passTurn();
//			GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
//		}
//		else
//		{
//			getBattleState()->setNextTurnType(TT_PLAYER);
//			getBattleState()->passTurn();
//		}
//		return false;
//
//
//	}
//	return true;
//}
//
//SOcsBurn::SOcsBurn (Character* target, Unit* effectObject)
//{
//	m_target = target;
//	m_effectObject = effectObject;
//}
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//SOmtBullet::~SOmtBullet(void)
//{
//}
//
//bool SOmtBullet::frameMove (float fElapsedTime)
//{
//	float interval = 1;
//	m_elapsedTime += fElapsedTime;
//	if (m_elapsedTime < interval * (m_maxNumber - m_leftNumber - 1))
//	{
//		return true;
//	}
//	else if (m_target->isDead())
//		return false;
//
//	if ( !m_effectObject->frameMove ( fElapsedTime ) )
//	{
//		int damage;
//		// Hit to the target!
//		if (m_leftNumber != 0)
//			damage = ( m_user->getStat().coding - m_target->getStat().def + 5) / 3;
//		else
//			//방어 무시
//			damage = m_user->getStat().coding / 2;
//
//		m_target->damage(damage);
//
//		D3DXVECTOR3 posDiff = m_target->getPos() - m_user->getPos();
//		D3DXVec3Normalize( &posDiff, &posDiff );
//		if (m_leftNumber != 0)
//			m_target->addMoveImpulse( posDiff / 3 ); // Attacked unit shows startled shake
//		else
//			m_target->addMoveImpulse( posDiff * 3 );
//
//		char stringBuffer[20];
//		_itoa_s (damage, stringBuffer, 10);
//		std::string resultLog = stringBuffer;
//
//		if ( m_target->getType() == UT_HERO )
//		{ 
//
//			resultLog += "포인트 쓰레드 데미지!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("HP가 모두 소진되었습니다. 게임 오버입니다.");
//				getBattleState()->pushBattleLog("순순히 F5키를 누르고 종료하시죠.");
//				m_target->setDead();
//
//				getBattleState()->setNextTurnType(TT_NATURAL);
//				getBattleState()->passTurn();
//
//			}
//			else
//			{
//				if (m_leftNumber == 0)
//				{
//					getBattleState()->setNextTurnType(TT_PLAYER);
//					getBattleState()->passTurn();
//				}
//			}
//		}
//		else
//		{
//			resultLog += "포인트 쓰레드 데미지!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("대상을 섬멸하였습니다! (Enter key로 월드로 복귀)");
//				m_target->setDead();
//				getBattleState()->setNextTurnType(TT_NATURAL);
//				GetAudioState().pSoundBank->Play( GetAudioState().iAttack, 0, 0, NULL );
//			}
//			else
//			{
//				if (m_leftNumber == 0)
//				{
//					getBattleState()->setNextTurnType(TT_COMPUTER);
//					getBattleState()->passTurn();
//					GetAudioState().pSoundBank->Play( GetAudioState().iAttack, 0, 0, NULL );
//				}
//			}
//		}
//
//		return false;
//	}
//	return true;
//}
//
//SOmtBullet::SOmtBullet (Character* user, Character* target, Unit* effectObject, int leftNumber, int maxNumber)
//{
//	m_user = user;
//	m_target = target;
//	m_effectObject = effectObject;
//	m_leftNumber = leftNumber;
//	m_maxNumber = maxNumber;
//	m_elapsedTime = 0;
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//SOgoto::~SOgoto(void)
//{
//	m_illusion->release();
//	delete m_illusion;
//}
//
//bool SOgoto::frameMove (float fElapsedTime)
//{
//	// 상대방에게 다가가는 중일 경우
//	if (m_phase == 0)
//	{
//		if (m_illusion->frameMove(fElapsedTime))
//			return true;
//		m_phase = 1;
//		m_illusion->setDynamicMotion(NULL);
//		return true;
//	}
//	// 공격 이펙트가 진행되는 경우
//	else if (m_phase == 1)
//	{
//		if (m_effectObject->frameMove (fElapsedTime))
//			return true;
//
//		// Hit to the target!
//		// 자신의 공격력 + 자신의 방어력 - 상대방 공격력
//		int damage = m_user->getStat().coding + m_user->getStat().def - m_target->getStat().def;
//
//		m_target->damage(damage);
//
//		D3DXVECTOR3 posDiff = m_target->getPos() - m_user->getPos();
//		D3DXVec3Normalize( &posDiff, &posDiff );
//		m_target->addMoveImpulse( posDiff / 3 ); // Attacked unit shows startled shake
//
//		char stringBuffer[20];
//		_itoa_s (damage, stringBuffer, 10);
//		std::string resultLog = stringBuffer;
//
//		if ( m_target->getType() == UT_HERO )
//		{ 
//			resultLog += "포인트 데미지를 받았다!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("HP가 모두 소진되었습니다. 게임 오버입니다.");
//				getBattleState()->pushBattleLog("순순히 F5키를 누르고 종료하시죠.");
//				m_target->setDead();
//				getBattleState()->setNextTurnType(TT_NATURAL);
//				getBattleState()->passTurn();
//				return false;
//			}
//		}
//		else
//		{
//			resultLog += "포인트 데미지를 입혔다!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("대상을 섬멸하였습니다! (Enter key로 월드로 복귀)");
//				m_target->setDead();
//				getBattleState()->setNextTurnType(TT_NATURAL);
//				GetAudioState().pSoundBank->Play( GetAudioState().iAttack, 0, 0, NULL );
//				return false;
//			}
//		}
//
//		m_phase = 2;
//		D3DXVECTOR3 fireDir = m_originPoint - m_user->getPos();
//		float dist = D3DXVec3Length( &fireDir );
//		D3DXVec3Normalize( &fireDir, &fireDir );
//
//		m_effectObject->setVisible(false);
//		m_illusion->setDynamicMotion (DynamicMotion::createDMfireUniformly (m_user, m_user->getPos(), fireDir, dist, 10));
//
//		return true;	
//	}
//	//복귀하는 과정
//	else if (m_phase == 2)
//	{
//		if (m_illusion->frameMove(fElapsedTime))
//			return true;
//
//		m_illusion->setDynamicMotion (NULL);
//
//		if ( m_target->getType() == UT_HERO )
//		{
//			getBattleState()->setNextTurnType(TT_PLAYER);
//			getBattleState()->passTurn();
//		}
//		else
//		{
//			getBattleState()->setNextTurnType(TT_COMPUTER);
//			getBattleState()->passTurn();
//			GetAudioState().pSoundBank->Play( GetAudioState().iAttack, 0, 0, NULL );
//		}
//
//		return false;
//	}
//
//	return true;
//}
//
//
//SOgoto::SOgoto (Character* user, Character* target, Unit* effectObject)
//{
//	m_user = user;
//	m_target = target;
//	m_effectObject = effectObject;
//	m_phase = 0;
//	m_originPoint = m_user->getPos();
//
//	LPD3DXMESH mesh;
//	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0);
//
//	m_illusion = Character::createCharacter(mesh, 0, 0, 0);
//	//m_illusion = Unit::createUnit(mesh, 0, 0, 0);
//
//	D3DXVECTOR3 fireDir = m_target->getPos() - m_user->getPos();
//	float dist = D3DXVec3Length( &fireDir );
//	D3DXVec3Normalize( &fireDir, &fireDir );
//
//
//	m_illusion->setDynamicMotion (DynamicMotion::createDMfireUniformly (m_user, m_user->getPos(), fireDir, dist, 10));
//}
