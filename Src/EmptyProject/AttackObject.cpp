#include "EmptyProjectPCH.h"
#include "AttackObject.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "Character.h"
#include "Sound.h"

AttackObject::~AttackObject(void)
{
}

BattleState* AttackObject::getBattleState()
{
	return reinterpret_cast<BattleState*>( GetWorldStateManager().getCurState() );
}
//////////////////////////////////////////////////////////////////////////


BallAttackObject::~BallAttackObject()
{

}


LRESULT BallAttackObject::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

bool BallAttackObject::frameMove( float fElapsedTime )
{
	D3DXVECTOR3 newPos = getPos() + m_fireDir/4;
	D3DXVECTOR3 vMoved = m_initPos - newPos;
	float distToInitPos = D3DXVec3Length( &vMoved );
	if ( distToInitPos > m_retainDist )
	{
		// Hit to the target!
		m_target->damage(m_damage);

		char stringBuffer[20];
		_itoa_s (m_damage, stringBuffer, 10);
		std::string resultLog = stringBuffer;
		
		if (m_target->isControllable())
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

	setPos( newPos );
	return Unit::frameMove( fElapsedTime );
}

AttackObject* BallAttackObject::createBallAttackObject( Character* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist )
{
	BallAttackObject* ao = new BallAttackObject( target, initPos, fireDir, retainDist );
	LPD3DXMESH mesh;
	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0 );
	ao->init( GetG().m_dev, mesh );
	ao->setControllable( false );
	ao->setPos( initPos );
	return ao;
}

BallAttackObject::BallAttackObject( Character* target, const D3DXVECTOR3& initPos, const D3DXVECTOR3& fireDir, float retainDist )
{
	m_target = target;
	m_initPos = initPos;
	m_fireDir = fireDir;
	m_retainDist = retainDist;
	m_damage = 1;
}

HealObject::HealObject( Character* target )
{
	m_target = target;
	D3DXVECTOR3 initialDistance;
	initialDistance.x = initialDistance.y = initialDistance.z = 5;
	m_radius = 5;
	m_radiusVelocity = 0.03f;
	m_angle = 0;
	m_angularVelocity = 10;

	float angle = D3DXToRadian (m_angle);

	D3DXVECTOR3 newPos;

	newPos.x = target->getPos().x + m_radius * cos (angle);
	newPos.y = target->getPos().y + m_radius * sin (angle);
	newPos.z = target->getPos().z;
	m_fireDir = target->getPos ();

	setPos( newPos );

	/*���� ���� �ִ� ��*/
	m_healPoint = target->getInt();
}

HealObject::~HealObject()
{

}

AttackObject* HealObject::createHealObject( Character* target )
{
	HealObject* ho = new HealObject( target );
	LPD3DXMESH mesh;
	//D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0 );
	
	D3DXCreateBox(GetG().m_dev, 2, 2, 2, &mesh, 0);
	//D3DXCreateTeapot( GetG().m_dev, mesh, 0);
	//D3DXCreateTeapot( GetG().m_dev, &mesh, 0);
	ho->init( GetG().m_dev, mesh );
	ho->setControllable( false );
	return ho;
}

LRESULT HealObject::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

bool HealObject::frameMove( float fElapsedTime )
{
	D3DXVECTOR3 newPos;
	m_radius -= m_radiusVelocity;
	m_angle += m_angularVelocity;
	float angle = D3DXToRadian (m_angle);
	newPos.x = m_fireDir.x + m_radius * cos (angle);
	newPos.y = m_fireDir.y + m_radius * sin (angle);
	newPos.z = getPos().z - (m_radiusVelocity/1.5f);

	if ( m_radius <= 0 )
	{
		// Hit to the target!
		m_target->heal (m_healPoint);

		char stringBuffer[20];
		_itoa_s (m_healPoint, stringBuffer, 10);
		std::string resultLog = stringBuffer;
		resultLog += "����Ʈ HP�� ȸ���˴ϴ�.";
		getBattleState()->pushBattleLog(resultLog.c_str());

		if (m_target->isControllable())
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

	setPos( newPos );
	return Unit::frameMove( fElapsedTime );
}


CsBurnObject::CsBurnObject( Character* target )
{
	m_target = target;
	D3DXVECTOR3 initialDistance;
	initialDistance.x = initialDistance.y = initialDistance.z = 5;
	m_radius = 5;
	m_radiusVelocity = 0.03f;
	m_angle = 0;
	m_angularVelocity = 50;

	float angle = D3DXToRadian (m_angle);

	D3DXVECTOR3 newPos;

	newPos.x = target->getPos().x + m_radius * cos (angle);
	newPos.y = target->getPos().y + m_radius * sin (angle);
	newPos.z = target->getPos().z;
	m_fireDir = target->getPos ();

	setPos( newPos );

	/*���� ���� �ִ� ��*/
}

CsBurnObject::~CsBurnObject()
{

}

AttackObject* CsBurnObject::createCsBurnObject( Character* target )
{
	CsBurnObject* ho = new CsBurnObject( target );
	LPD3DXMESH mesh;
	//D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0 );
	
	D3DXCreateBox(GetG().m_dev, 5, 5, 1, &mesh, 0);
	//D3DXCreateTeapot( GetG().m_dev, mesh, 0);
	//D3DXCreateTeapot( GetG().m_dev, &mesh, 0);
	ho->init( GetG().m_dev, mesh );
	ho->setControllable( false );
	return ho;
}

LRESULT CsBurnObject::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

bool CsBurnObject::frameMove( float fElapsedTime )
{
	D3DXVECTOR3 newPos;
	m_radius -= m_radiusVelocity;
	m_angle += m_angularVelocity;
	float angle = D3DXToRadian (m_angle);
	newPos.x = m_fireDir.x + m_radius * cos (angle);
	newPos.y = m_fireDir.y + m_radius * sin (angle);
	newPos.z = getPos().z - (m_radiusVelocity/1.5f);

	if ( m_radius <= 0 )
	{
		if (getPos().z < m_fireDir.z)
		{
			newPos.x = m_fireDir.x;
			newPos.y = m_fireDir.y;
			newPos.z = getPos().z - m_radius;
			setPos( newPos );
			return Unit::frameMove( fElapsedTime );
		}
		// Hit to the target!
		m_target->setCurCs(0);

		getBattleState()->pushBattleLog("���Ǹ��� ��� �����Ǿ����ϴ�.");

		int m_burnDamage = m_target->getCurHp() / 2;

		m_target->damage(m_burnDamage);

		getBattleState()->pushBattleLog("���Ǹ� ������ ���� ���������� ����");
		getBattleState()->pushBattleLog("���� HP�� ������ �پ��ϴ�.");

		

		if (m_target->isControllable())
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

	setPos( newPos );
	return Unit::frameMove( fElapsedTime );
}


MeditationObject::MeditationObject( Character* target )
{
	m_target = target;
	D3DXVECTOR3 initialDistance;
	initialDistance.x = initialDistance.y = initialDistance.z = 5;
	m_radius = 5;
	m_radiusVelocity = 0.03f;
	m_angle = 0;
	m_angularVelocity = 10;

	float angle = D3DXToRadian (m_angle);

	D3DXVECTOR3 newPos;

	newPos.x = target->getPos().x + m_radius * cos (angle);
	newPos.y = target->getPos().y + m_radius * sin (angle);
	newPos.z = target->getPos().z;
	m_fireDir = target->getPos ();

	setPos( newPos );

}

MeditationObject::~MeditationObject()
{

}

AttackObject* MeditationObject::createMeditationObject( Character* target )
{
	MeditationObject* ho = new MeditationObject( target );
	LPD3DXMESH mesh;
	//D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0 );
	
	D3DXCreateBox(GetG().m_dev, 2, 2, 2, &mesh, 0);
	//D3DXCreateTeapot( GetG().m_dev, mesh, 0);
	//D3DXCreateTeapot( GetG().m_dev, &mesh, 0);
	ho->init( GetG().m_dev, mesh );
	ho->setControllable( false );
	return ho;
}

LRESULT MeditationObject::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

bool MeditationObject::frameMove( float fElapsedTime )
{
	D3DXVECTOR3 newPos;
	m_radius -= m_radiusVelocity;
	m_angle += m_angularVelocity;
	float angle = D3DXToRadian (m_angle);
	newPos.x = m_fireDir.x + m_radius * cos (angle);
	newPos.y = m_fireDir.y + m_radius * sin (angle);
	newPos.z = getPos().z - (m_radiusVelocity/1.5f);

	if ( m_radius <= 0 )
	{
		// Hit to the target!
		for (int i=0; i<3; i++)
		{
			m_target->recoverCs();
		}

		getBattleState()->pushBattleLog("û���� ����� ������ CS�� 30% ȸ���˴ϴ�.");

		if (m_target->isControllable())
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

	setPos( newPos );
	return Unit::frameMove( fElapsedTime );
}
