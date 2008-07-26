#include "EmptyProjectPCH.h"
#include "HPBar.h"

void HPBar::frameMove( float fElapsedTime )
{
	m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
	// Update acceleration vector based on keyboard state
	if( IsKeyDown( m_aKeys[PIC_MOVE_UP] ) )
		changeRate(-1);
	if( IsKeyDown( m_aKeys[PIC_MOVE_DOWN] ) )
		changeRate(1);
	if( IsKeyDown( m_aKeys[PIC_MOVE_RIGHT] ) )
		m_vKeyboardDirection.x += 0.5f;
	if( IsKeyDown( m_aKeys[PIC_MOVE_LEFT] ) )
		m_vKeyboardDirection.x -= 0.5f;

	// Update velocity
	m_vVelocity = m_vKeyboardDirection;

	// Simple euler method to calculate position delta
	D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;
	m_vPos += vPosDelta;

	D3DXMATRIX mScaling, mTranslation;
	D3DXMatrixScaling(&mScaling, m_width, m_height, 1.0f);
	D3DXMatrixTranslation(&mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_localXform = mTranslation * mScaling;
}