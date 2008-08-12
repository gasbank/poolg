#include "EmptyProjectPCH.h"
#include "ProgressBar.h"


void ProgressBar::setSize(float width, float height)
{
	m_width = width;
	m_fullWidth = width;
	m_height = height;
}


void ProgressBar::changeRate(float rateV)
{
	if (rateV > m_maxRate)
		m_currentRate = m_maxRate;
	else if (rateV < 0)
		m_currentRate = 0;
	else
		m_currentRate = rateV;

	//m_width = (m_currentRate / m_maxRate) * m_fullWidth;
}

void ProgressBar::setRate(float rateV)
{
	if (rateV > m_maxRate)
		m_currentRate = m_maxRate;
	else if (rateV < 0)
		m_currentRate = 0;
	else
		m_currentRate = rateV;

	m_width = (m_currentRate / m_maxRate) * m_fullWidth;
}

void ProgressBar::frameMove( float fElapsedTime )
{
	m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );

	//// Update acceleration vector based on keyboard state
	
	//if( IsKeyDown( m_aKeys[PIC_MOVE_UP] ) )
	//	m_vKeyboardDirection.y += 0.5f;
	//if( IsKeyDown( m_aKeys[PIC_MOVE_DOWN] ) )
	//	m_vKeyboardDirection.y -= 0.5f;
	//if( IsKeyDown( m_aKeys[PIC_MOVE_RIGHT] ) )
	//	m_vKeyboardDirection.x += 0.5f;
	//if( IsKeyDown( m_aKeys[PIC_MOVE_LEFT] ) )
	//	m_vKeyboardDirection.x -= 0.5f;

	float movingRate = 0.06f;
	//현재 보이는 HP가 더 적을 경우. (HP 증가시)
	if ((m_currentRate / m_maxRate) * m_fullWidth > m_width)
		m_width += ((m_currentRate / m_maxRate) * m_fullWidth - m_width) * movingRate;
	//현재 보이는 HP가 더 많을 경우. (HP 감소시)
	else if ((m_currentRate / m_maxRate) * m_fullWidth < m_width)
		m_width = (m_currentRate / m_maxRate) * m_fullWidth;

	

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

void IllusionBar::frameMove( float fElapsedTime )
{
	m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );

	//// Update acceleration vector based on keyboard state
	
	//if( IsKeyDown( m_aKeys[PIC_MOVE_UP] ) )
	//	m_vKeyboardDirection.y += 0.5f;
	//if( IsKeyDown( m_aKeys[PIC_MOVE_DOWN] ) )
	//	m_vKeyboardDirection.y -= 0.5f;
	//if( IsKeyDown( m_aKeys[PIC_MOVE_RIGHT] ) )
	//	m_vKeyboardDirection.x += 0.5f;
	//if( IsKeyDown( m_aKeys[PIC_MOVE_LEFT] ) )
	//	m_vKeyboardDirection.x -= 0.5f;

	float movingRate = 0.1f;
	//현재 보이는 HP가 더 적을 경우. (HP 증가시)
	if ((m_currentRate / m_maxRate) * m_fullWidth > m_width)
		m_width = (m_currentRate / m_maxRate) * m_fullWidth;
	//현재 보이는 HP가 더 많을 경우. (HP 감소시)
	else if ((m_currentRate / m_maxRate) * m_fullWidth < m_width)
		m_width +=  ((m_currentRate / m_maxRate) * m_fullWidth - m_width) * movingRate;

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
