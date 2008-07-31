#include "EmptyProjectPCH.h"
#include "StatusBar.h"


void HPBarPlayer::frameMove( float fElapsedTime )
{
	m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
	// Update acceleration vector based on keyboard state
	if( IsKeyDown( m_aKeys[PIC_MOVE_UP] ) )
		changeRate(-0.1f);
	if( IsKeyDown( m_aKeys[PIC_MOVE_DOWN] ) )
		changeRate(0.1f);
	if( IsKeyDown( m_aKeys[PIC_MOVE_RIGHT] ) )
		changeRate(5);
	if( IsKeyDown( m_aKeys[PIC_MOVE_LEFT] ) )
		changeRate(-5);

	// Update velocity
	m_vVelocity = m_vKeyboardDirection;

	// Simple euler method to calculate position delta
	D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;
	m_vPos += vPosDelta;

	D3DXMATRIX mScaling, mTranslation;
	D3DXMatrixScaling(&mScaling, m_width, m_height, 1.0f);
	D3DXMatrixTranslation(&mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_localXform = mScaling * mTranslation;
}

void MPBarPlayer::frameMove( float fElapsedTime )
{
	m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
	// Update acceleration vector based on keyboard state
	if( IsKeyDown( m_aKeys[PIC_MOVE_UP] ) )
		changeRate(-0.1f);
	if( IsKeyDown( m_aKeys[PIC_MOVE_DOWN] ) )
		changeRate(0.1f);
	if( IsKeyDown( m_aKeys[PIC_MOVE_RIGHT] ) )
		changeRate(2);
	if( IsKeyDown( m_aKeys[PIC_MOVE_LEFT] ) )
		changeRate(-3);

	// Update velocity
	m_vVelocity = m_vKeyboardDirection;

	// Simple euler method to calculate position delta
	D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;
	m_vPos += vPosDelta;

	D3DXMATRIX mScaling, mTranslation;
	D3DXMatrixScaling(&mScaling, m_width, m_height, 1.0f);
	D3DXMatrixTranslation(&mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_localXform = mScaling * mTranslation;
}

void EXPBarPlayer::frameMove( float fElapsedTime )
{
	m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
	// Update acceleration vector based on keyboard state
	if( IsKeyDown( m_aKeys[PIC_MOVE_UP] ) )
		changeRate(-0.1f);
	if( IsKeyDown( m_aKeys[PIC_MOVE_DOWN] ) )
		changeRate(0.1f);
	if( IsKeyDown( m_aKeys[PIC_MOVE_RIGHT] ) )
		changeRate(-7.3f);
	if( IsKeyDown( m_aKeys[PIC_MOVE_LEFT] ) )
		changeRate(11.5f);

	// Update velocity
	m_vVelocity = m_vKeyboardDirection;

	// Simple euler method to calculate position delta
	D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;
	m_vPos += vPosDelta;

	D3DXMATRIX mScaling, mTranslation;
	D3DXMatrixScaling(&mScaling, m_width, m_height, 1.0f);
	D3DXMatrixTranslation(&mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_localXform = mScaling * mTranslation;
}


void MPBarEnemy::frameMove( float fElapsedTime )
{
	m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
	// Update acceleration vector based on keyboard state
	if( IsKeyDown( m_aKeys[PIC_MOVE_UP] ) )
		changeRate(-0.1f);
	if( IsKeyDown( m_aKeys[PIC_MOVE_DOWN] ) )
		changeRate(0.1f);
	if( IsKeyDown( m_aKeys[PIC_MOVE_RIGHT] ) )
		changeRate(-5);
	if( IsKeyDown( m_aKeys[PIC_MOVE_LEFT] ) )
		changeRate(7);

	// Update velocity
	m_vVelocity = m_vKeyboardDirection;

	// Simple euler method to calculate position delta
	D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;
	m_vPos += vPosDelta;

	D3DXMATRIX mScaling, mTranslation;
	D3DXMatrixScaling(&mScaling, m_width, m_height, 1.0f);
	D3DXMatrixTranslation(&mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_localXform = mScaling * mTranslation;
}
