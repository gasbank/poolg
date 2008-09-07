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

SkillObject::SkillObject( BasicShapeType bst, float size, D3DCOLOR color, DynamicMotion* dm )
: Unit( UT_SKILLOBJECT )
, m_bst( bst )
, m_user( 0 )
, m_target( 0 )
, m_velocity( 0 )
, m_dm( dm )
, m_size( size )
, m_color( color )
{
}

SkillObject::SkillObject( const SkillObject& so )
: Unit( UT_SKILLOBJECT )
, m_bst( so.m_bst )
, m_user( so.m_user )
, m_target( so.m_target )
, m_velocity( so.m_velocity )
, m_dm( so.m_dm->clone() )
, m_size( so.m_size )
, m_color( so.m_color )
{
	ActionList::const_iterator cit = so.m_onHitActionList.begin();
	for ( ; cit != so.m_onHitActionList.end(); ++cit )
	{
		Action* act = (*cit)->clone();
		addOnHitAction( act );
	}
	m_dm->setMotionTarget( this );
}

SkillObject::~SkillObject(void)
{
	//m_effectObject->release();
	//delete m_effectObject;

	EP_SAFE_RELEASE( m_dm );
	EpSafeReleaseAll( m_onHitActionList );

}

bool SkillObject::frameMove( double dTime, float fElapsedTime )
{
	// This bool indicates that whether the present frameMove() is the last(final) call
	// or another frameMove() will be called after this call.
	// If SkillObject hits(collides) to target unit, than this will be last call,
	// otherwise there will be one or more frameMove() until collision occurs.
	bool frameMoveInProgress = true;

	// 'm_dm' governs the movement of this SkillObject.
	m_dm->frameMove( fElapsedTime );

	D3DXVECTOR3 objToTarget = getPos() - m_target->getPos();
	float dist = D3DXVec3Length( &objToTarget );
	if ( dist < 0.1f )
	{
		assert( m_target );
		
		m_target->addMoveImpulse( DX_CONSTS::D3DXVEC3_Z / 2.5f ); // Attacked unit shows startled shake
		
		ActionList::iterator it = m_onHitActionList.begin();
		UINT updateInProgressCount = 0;
		for ( ; it != m_onHitActionList.end(); ++it )
		{
			bool updateInProgress = (*it)->update( dTime, fElapsedTime );
			if ( updateInProgress )
				++updateInProgress;
		}
		if ( updateInProgressCount > 0 )
			frameMoveInProgress = true; // SkillObject is collided with target unit, however hit actions are not completed yet.
		else
			frameMoveInProgress = false; // No more frameMove() is needed.
	}

	Unit::frameMove( dTime, fElapsedTime );
	return frameMoveInProgress;
}

HRESULT SkillObject::frameRender ( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime )
{
	HRESULT f = 0;
	D3DPERF_BeginEvent( 0, L"SkillObject Render" );

	HRESULT hr = S_OK;
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );
	UINT iPass, cPasses;
	V( g_bombShader->setMainTechnique() );
	
	V( g_bombShader->setWorldViewProj( dTime, fElapsedTime, &getLocalXform(), GetG().m_camera.GetViewMatrix(), GetG().m_camera.GetProjMatrix() ) );

	V( g_bombShader->begin( &cPasses, 0 ) );
	for( iPass = 0; iPass < cPasses; iPass++ )
	{
		V( g_bombShader->beginPass( iPass ) );

		// TODO Mesh pointing should be done here? not OnResetDevice?
		if ( getMesh() == 0 )
		{
			setMesh( g_bst[ m_bst ] );
		}

		f = getMesh()->DrawSubset( 0 );
		
		V( g_bombShader->endPass() );
	}
	V( g_bombShader->end() );
	
	D3DPERF_EndEvent();
	return f;
}

BattleState* SkillObject::getBattleState() const
{
	return reinterpret_cast<BattleState*>( GetWorldStateManager().getCurState() );
}

SkillObject* SkillObject::createSkillObject( const char* bst, float size, D3DCOLOR color, DynamicMotion* dm )
{
	BasicShapeType bstEnum;
	if ( strcmp( bst, "SPHERE" ) == 0 ) bstEnum = BST_SPHERE;
	else if ( strcmp( bst, "CUBE" ) == 0 ) bstEnum = BST_CUBE;
	else if ( strcmp( bst, "PLANE" ) == 0 ) bstEnum = BST_PLANE;
	else bstEnum = BST_UNKNOWN;

	SkillObject* so = new SkillObject( bstEnum, size, color, dm );
	return so;
}

SkillObject* SkillObject::clone() const
{
	return new SkillObject( *this );
}

void SkillObject::setUserAndTarget( Character* user, Character* target )
{
	m_user = user;
	m_target = target;
	m_dm->setFireAndTargetUnit( user, target );

	ActionList::iterator it = m_onHitActionList.begin();
	for ( ; it != m_onHitActionList.end(); ++it )
	{
		CharacterAction* ca = dynamic_cast<CharacterAction*>( *it );
		if ( ca )
		{
			ca->setCharacter( m_target );
		}	
	}
}
//////////////////////////////////////////////////////////////////////////

SkillObject* EpCreateSkillObject( const char* bst, double size, int color /* ARGB */, void* dmPtr )
{
	DynamicMotion* dm = reinterpret_cast<DynamicMotion*>( dmPtr );
	return SkillObject::createSkillObject( bst, (float)size, (D3DCOLOR)color, dm );
} SCRIPT_CALLABLE_PV_PC_D_I_PV( EpCreateSkillObject )

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
//		//��ũ��Ʈ �Լ��� ȣ���Ѵ�.
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
//			resultLog += "����Ʈ �������� �޾Ҵ�!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("HP�� ��� �����Ǿ����ϴ�. ���� �����Դϴ�.");
//				getBattleState()->pushBattleLog("������ F5Ű�� ������ �����Ͻ���.");
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
//			resultLog += "����Ʈ �������� ������!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("����� �����Ͽ����ϴ�! (Enter key�� ����� ����)");
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
//		resultLog += "����Ʈ HP�� ȸ���˴ϴ�.";
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
//		getBattleState()->pushBattleLog("û���� ����� ������ CS�� 30% ȸ���˴ϴ�.");
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
//		getBattleState()->pushBattleLog("���Ǹ��� ��� �����Ǿ����ϴ�.");
//
//		int m_burnDamage = m_target->getCurHp() / 2;
//
//		m_target->damage(m_burnDamage);
//
//		getBattleState()->pushBattleLog("���Ǹ� ������ ���� ���������� ����");
//		getBattleState()->pushBattleLog("���� HP�� ������ �پ��ϴ�.");
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
//			//��� ����
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
//			resultLog += "����Ʈ ������ ������!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("HP�� ��� �����Ǿ����ϴ�. ���� �����Դϴ�.");
//				getBattleState()->pushBattleLog("������ F5Ű�� ������ �����Ͻ���.");
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
//			resultLog += "����Ʈ ������ ������!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("����� �����Ͽ����ϴ�! (Enter key�� ����� ����)");
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
//	// ���濡�� �ٰ����� ���� ���
//	if (m_phase == 0)
//	{
//		if (m_illusion->frameMove(fElapsedTime))
//			return true;
//		m_phase = 1;
//		m_illusion->setDynamicMotion(NULL);
//		return true;
//	}
//	// ���� ����Ʈ�� ����Ǵ� ���
//	else if (m_phase == 1)
//	{
//		if (m_effectObject->frameMove (fElapsedTime))
//			return true;
//
//		// Hit to the target!
//		// �ڽ��� ���ݷ� + �ڽ��� ���� - ���� ���ݷ�
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
//			resultLog += "����Ʈ �������� �޾Ҵ�!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("HP�� ��� �����Ǿ����ϴ�. ���� �����Դϴ�.");
//				getBattleState()->pushBattleLog("������ F5Ű�� ������ �����Ͻ���.");
//				m_target->setDead();
//				getBattleState()->setNextTurnType(TT_NATURAL);
//				getBattleState()->passTurn();
//				return false;
//			}
//		}
//		else
//		{
//			resultLog += "����Ʈ �������� ������!";
//			getBattleState()->pushBattleLog(resultLog.c_str());
//			if (m_target->isDead())
//			{
//				getBattleState()->pushBattleLog("����� �����Ͽ����ϴ�! (Enter key�� ����� ����)");
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
//	//�����ϴ� ����
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
