#include "EmptyProjectPCH.h"
#include "SkillObject.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "Character.h"
#include "Sound.h"

SkillObject::~SkillObject(void)
{
	delete m_effectObject;
}

bool SkillObject::frameMove ( float fElapsedTime )
{
	return this->m_effectObject->frameMove(fElapsedTime);
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
