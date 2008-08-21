#include "EmptyProjectPCH.h"
#include "SkillObject.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "Character.h"
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
		//m_target->addMoveImpulse( -m_fireDir/2 ); // Attacked unit shows startled shake

		char stringBuffer[20];
		_itoa_s (damage, stringBuffer, 10);
		std::string resultLog = stringBuffer;
		
		if ( m_target->getType() == UT_HERO )
		{ 

			resultLog += "����Ʈ �������� �޾Ҵ�!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("HP�� ��� �����Ǿ����ϴ�. ���� �����Դϴ�.");
				getBattleState()->pushBattleLog("������ F5Ű�� ������ �����Ͻ���.");
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
			resultLog += "����Ʈ �������� ������!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("����� �����Ͽ����ϴ�! (Enter key�� ����� ����)");
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
		resultLog += "����Ʈ HP�� ȸ���˴ϴ�.";
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

		getBattleState()->pushBattleLog("û���� ����� ������ CS�� 30% ȸ���˴ϴ�.");

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

		getBattleState()->pushBattleLog("���Ǹ��� ��� �����Ǿ����ϴ�.");

		int m_burnDamage = m_target->getCurHp() / 2;

		m_target->damage(m_burnDamage);

		getBattleState()->pushBattleLog("���Ǹ� ������ ���� ���������� ����");
		getBattleState()->pushBattleLog("���� HP�� ������ �پ��ϴ�.");

		

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
			//��� ����
			damage = m_user->getStat().coding + 10 ;

		m_target->damage(damage);
		//m_target->addMoveImpulse( -m_fireDir/2 ); // Attacked unit shows startled shake

		char stringBuffer[20];
		_itoa_s (damage, stringBuffer, 10);
		std::string resultLog = stringBuffer;
		
		if ( m_target->getType() == UT_HERO )
		{ 

			resultLog += "����Ʈ ������ ������!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("HP�� ��� �����Ǿ����ϴ�. ���� �����Դϴ�.");
				getBattleState()->pushBattleLog("������ F5Ű�� ������ �����Ͻ���.");
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
			resultLog += "����Ʈ ������ ������!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			if (m_target->isDead())
			{
				getBattleState()->pushBattleLog("����� �����Ͽ����ϴ�! (Enter key�� ����� ����)");
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
