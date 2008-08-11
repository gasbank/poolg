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
			
			resultLog += "포인트 데미지를 받았다!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			getBattleState()->setNextTurnType(TT_PLAYER);
			getBattleState()->passTurn();
		}
		else
		{
			resultLog += "포인트 데미지를 입혔다!";
			getBattleState()->pushBattleLog(resultLog.c_str());
			getBattleState()->setNextTurnType(TT_COMPUTER);
			getBattleState()->passTurn();
			GetAudioState().pSoundBank->Play( GetAudioState().iSE, 0, 0, NULL );
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
