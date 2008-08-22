#include "EmptyProjectPCH.h"
#include "SkillObject.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "Character.h"
#include "DynamicMotion.h"
#include "Sound.h"

SkillObject::~SkillObject(void)
{
	m_effectObject->release();
	delete m_effectObject;
}

bool SkillObject::frameMove ( float fElapsedTime )
{
	return this->m_effectObject->frameMove(fElapsedTime);
}

HRESULT SkillObject::frameRender ()
{
	D3DPERF_BeginEvent(0, L"SkillObjRender");
	HRESULT f = this->m_effectObject->frameRender();
	D3DPERF_EndEvent ();
	return f;
}

BattleState* SkillObject::getBattleState()
{
	return reinterpret_cast<BattleState*>( GetWorldStateManager().getCurState() );
}

SkillObject* SkillObject::createSOnormalAttack(Character* user, Character* target, Unit* effectObject)
{
	SOnormalAttack* so = new SOnormalAttack (user, target, effectObject);
	return so;
}

SkillObject* SkillObject::createSOheal(Character* user, Unit* effectObject)
{
	SOheal* so = new SOheal (user, effectObject);
	return so;
}

SkillObject* SkillObject::createSOmeditation(Character* user, Unit* effectObject)
{
	SOmeditation* so = new SOmeditation (user, effectObject);
	return so;
}

SkillObject* SkillObject::createSOcsBurn(Character* target, Unit* effectObject)
{
	SOcsBurn* so = new SOcsBurn (target, effectObject);
	return so;
}

SkillObject* SkillObject::createSOmtBullet(Character* user, Character* target, Unit* effectObject, int leftNumber, int maxNumber)
{
	SOmtBullet* so = new SOmtBullet (user, target, effectObject, leftNumber, maxNumber);
	return so;
}

SkillObject* SkillObject::createSOgoto (Character* user, Character* target, Unit* effectObject)
{
	SOgoto* so = new SOgoto (user, target, effectObject);
	return so;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

SOnormalAttack::~SOnormalAttack(void)
{
}

bool SOnormalAttack::frameMove (float fElapsedTime)
{

	if ( !m_effectObject->frameMove ( fElapsedTime ) )
	{
		// Hit to the target!
		int damage = m_user->getStat().coding - m_target->getStat().def + 20;

		m_target->damage(damage);

		D3DXVECTOR3 posDiff = m_target->getPos() - m_user->getPos();
		D3DXVec3Normalize( &posDiff, &posDiff );
		m_target->addMoveImpulse( posDiff / 3 ); // Attacked unit shows startled shake

		char stringBuffer[20];
		_itoa_s (damage, stringBuffer, 10);
		std::string resultLog = stringBuffer;

		if ( m_target->getType() == UT_HERO )
		{ 

			resultLog += "포인트 데미지를 받았다!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("HP가 모두 소진되었습니다. 게임 오버입니다.");
				getBattleState()->pushBattleLog("순순히 F5키를 누르고 종료하시죠.");
				m_target->setDead();
				getBattleState()->setNextTurnType(TT_NATURAL);
				getBattleState()->passTurn();
			}
			else
			{
				getBattleState()->setNextTurnType(TT_PLAYER);
				getBattleState()->passTurn();
			}
		}
		else
		{
			resultLog += "포인트 데미지를 입혔다!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("대상을 섬멸하였습니다! (Enter key로 월드로 복귀)");
				m_target->setDead();
				getBattleState()->setNextTurnType(TT_NATURAL);
				GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
			}
			else
			{
				getBattleState()->setNextTurnType(TT_COMPUTER);
				getBattleState()->passTurn();
				GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
			}
		}

		return false;
	}
	return true;
}

SOnormalAttack::SOnormalAttack (Character* user, Character* target, Unit* effectObject)
{
	m_user = user;
	m_target = target;
	m_effectObject = effectObject;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

SOheal::~SOheal(void)
{
}

bool SOheal::frameMove (float fElapsedTime)
{
	if ( !m_effectObject->frameMove( fElapsedTime ))
	{
		int healPoint = m_user->getStat().sense * 50;
		m_user->heal (healPoint);

		char stringBuffer[20];
		_itoa_s (healPoint, stringBuffer, 10);
		std::string resultLog = stringBuffer;
		resultLog += "포인트 HP가 회복됩니다.";
		getBattleState()->pushBattleLog(resultLog.c_str());

		if ( m_user->getType() == UT_HERO )
		{
			getBattleState()->setNextTurnType(TT_COMPUTER);
			getBattleState()->passTurn();
			GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
		}
		else
		{
			getBattleState()->setNextTurnType(TT_PLAYER);
			getBattleState()->passTurn();
		}
		return false;

	}
	return true;
}

SOheal::SOheal (Character* user, Unit* effectObject)
{
	m_user = user;
	m_effectObject = effectObject;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

SOmeditation::~SOmeditation(void)
{
}

bool SOmeditation::frameMove (float fElapsedTime)
{
	if ( !m_effectObject->frameMove( fElapsedTime ))
	{
		for (int i=0; i<3; i++)
		{
			m_user->recoverCs();
		}

		getBattleState()->pushBattleLog("청명한 기운을 느끼며 CS가 30% 회복됩니다.");

		if ( m_user->getType() == UT_HERO )
		{
			getBattleState()->setNextTurnType(TT_COMPUTER);
			getBattleState()->passTurn();
			GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
		}
		else
		{
			getBattleState()->setNextTurnType(TT_PLAYER);
			getBattleState()->passTurn();
		}
		return false;
	}
	return true;
}

SOmeditation::SOmeditation (Character* user, Unit* effectObject)
{
	m_user = user;
	m_effectObject = effectObject;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////


SOcsBurn::~SOcsBurn(void)
{
}

bool SOcsBurn::frameMove (float fElapsedTime)
{
	if ( !m_effectObject->frameMove( fElapsedTime ))
	{
		// Hit to the target!
		m_target->setCurCs(0);

		getBattleState()->pushBattleLog("스피릿이 모두 소진되었습니다.");

		int m_burnDamage = m_target->getCurHp() / 2;

		m_target->damage(m_burnDamage);

		getBattleState()->pushBattleLog("스피릿 소진에 의한 현기증으로 인해");
		getBattleState()->pushBattleLog("현재 HP가 반으로 줄어듭니다.");



		if ( m_target->getType() == UT_HERO )
		{
			getBattleState()->setNextTurnType(TT_COMPUTER);
			getBattleState()->passTurn();
			GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
		}
		else
		{
			getBattleState()->setNextTurnType(TT_PLAYER);
			getBattleState()->passTurn();
		}
		return false;


	}
	return true;
}

SOcsBurn::SOcsBurn (Character* target, Unit* effectObject)
{
	m_target = target;
	m_effectObject = effectObject;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////


SOmtBullet::~SOmtBullet(void)
{
}

bool SOmtBullet::frameMove (float fElapsedTime)
{
	float interval = 1;
	m_elapsedTime += fElapsedTime;
	if (m_elapsedTime < interval * (m_maxNumber - m_leftNumber - 1))
	{
		return true;
	}
	else if (m_target->isDead())
		return false;

	if ( !m_effectObject->frameMove ( fElapsedTime ) )
	{
		int damage;
		// Hit to the target!
		if (m_leftNumber != 0)
			damage = m_user->getStat().coding - m_target->getStat().def + 10;
		else
			//방어 무시
			damage = m_user->getStat().coding + 10 ;

		m_target->damage(damage);

		D3DXVECTOR3 posDiff = m_target->getPos() - m_user->getPos();
		D3DXVec3Normalize( &posDiff, &posDiff );
		if (m_leftNumber != 0)
			m_target->addMoveImpulse( posDiff / 3 ); // Attacked unit shows startled shake
		else
			m_target->addMoveImpulse( posDiff * 3 );

		char stringBuffer[20];
		_itoa_s (damage, stringBuffer, 10);
		std::string resultLog = stringBuffer;

		if ( m_target->getType() == UT_HERO )
		{ 

			resultLog += "포인트 쓰레드 데미지!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("HP가 모두 소진되었습니다. 게임 오버입니다.");
				getBattleState()->pushBattleLog("순순히 F5키를 누르고 종료하시죠.");
				m_target->setDead();

				getBattleState()->setNextTurnType(TT_NATURAL);
				getBattleState()->passTurn();

			}
			else
			{
				if (m_leftNumber == 0)
				{
					getBattleState()->setNextTurnType(TT_PLAYER);
					getBattleState()->passTurn();
				}
			}
		}
		else
		{
			resultLog += "포인트 쓰레드 데미지!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("대상을 섬멸하였습니다! (Enter key로 월드로 복귀)");
				m_target->setDead();
				getBattleState()->setNextTurnType(TT_NATURAL);
				GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
			}
			else
			{
				if (m_leftNumber == 0)
				{
					getBattleState()->setNextTurnType(TT_COMPUTER);
					getBattleState()->passTurn();
					GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
				}
			}
		}

		return false;
	}
	return true;
}

SOmtBullet::SOmtBullet (Character* user, Character* target, Unit* effectObject, int leftNumber, int maxNumber)
{
	m_user = user;
	m_target = target;
	m_effectObject = effectObject;
	m_leftNumber = leftNumber;
	m_maxNumber = maxNumber;
	m_elapsedTime = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////


SOgoto::~SOgoto(void)
{
	m_illusion->release();
	delete m_illusion;
}

bool SOgoto::frameMove (float fElapsedTime)
{
	// 상대방에게 다가가는 중일 경우
	if (m_phase == 0)
	{
		if (m_illusion->frameMove(fElapsedTime))
			return true;
		m_phase = 1;
		m_illusion->setDynamicMotion(NULL);
		return true;
	}
	// 공격 이펙트가 진행되는 경우
	else if (m_phase == 1)
	{
		if (m_effectObject->frameMove (fElapsedTime))
			return true;

		// Hit to the target!
		// 자신의 공격력 + 자신의 방어력 - 상대방 공격력 + 10
		int damage = m_user->getStat().coding + m_user->getStat().def - m_target->getStat().def + 10;

		m_target->damage(damage);

		D3DXVECTOR3 posDiff = m_target->getPos() - m_user->getPos();
		D3DXVec3Normalize( &posDiff, &posDiff );
		m_target->addMoveImpulse( posDiff / 3 ); // Attacked unit shows startled shake

		char stringBuffer[20];
		_itoa_s (damage, stringBuffer, 10);
		std::string resultLog = stringBuffer;

		if ( m_target->getType() == UT_HERO )
		{ 
			resultLog += "포인트 데미지를 받았다!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("HP가 모두 소진되었습니다. 게임 오버입니다.");
				getBattleState()->pushBattleLog("순순히 F5키를 누르고 종료하시죠.");
				m_target->setDead();
				getBattleState()->setNextTurnType(TT_NATURAL);
				getBattleState()->passTurn();
				return false;
			}
		}
		else
		{
			resultLog += "포인트 데미지를 입혔다!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("대상을 섬멸하였습니다! (Enter key로 월드로 복귀)");
				m_target->setDead();
				getBattleState()->setNextTurnType(TT_NATURAL);
				GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
				return false;
			}
		}

		m_phase = 2;
		D3DXVECTOR3 fireDir = m_originPoint - m_user->getPos();
		float dist = D3DXVec3Length( &fireDir );
		D3DXVec3Normalize( &fireDir, &fireDir );

		m_effectObject->setVisible(false);
		m_illusion->setDynamicMotion (DynamicMotion::createDMfireUniformly (m_user, m_user->getPos(), fireDir, dist, 10));

		return true;	
	}
	//복귀하는 과정
	else if (m_phase == 2)
	{
		if (m_illusion->frameMove(fElapsedTime))
			return true;

		m_illusion->setDynamicMotion (NULL);

		if ( m_target->getType() == UT_HERO )
		{
			getBattleState()->setNextTurnType(TT_PLAYER);
			getBattleState()->passTurn();
		}
		else
		{
			getBattleState()->setNextTurnType(TT_COMPUTER);
			getBattleState()->passTurn();
			GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
		}

		return false;
	}

	return true;
}


SOgoto::SOgoto (Character* user, Character* target, Unit* effectObject)
{
	m_user = user;
	m_target = target;
	m_effectObject = effectObject;
	m_phase = 0;
	m_originPoint = m_user->getPos();

	LPD3DXMESH mesh;
	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0);
	m_illusion = Character::createCharacter(mesh, 0, 0, 0);
	//m_illusion = Unit::createUnit(mesh, 0, 0, 0);

	D3DXVECTOR3 fireDir = m_target->getPos() - m_user->getPos();
	float dist = D3DXVec3Length( &fireDir );
	D3DXVec3Normalize( &fireDir, &fireDir );


	m_illusion->setDynamicMotion (DynamicMotion::createDMfireUniformly (m_user, m_user->getPos(), fireDir, dist, 10));
}
