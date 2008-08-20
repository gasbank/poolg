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
