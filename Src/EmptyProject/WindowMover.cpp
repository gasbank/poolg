#include "EmptyProjectPCH.h"
#include "WindowMover.h"
#include "ArnMath.h"

void WindowMover::setPos( ArnVec3 onPos, ArnVec3 offPos )
{
	m_onPos = onPos;
	m_offPos = offPos;
	m_curPos = offPos;

	m_movingDirect = false;
	m_isOn = false;
}

void WindowMover::onBox()
{
	if (m_isOn == false)
		m_movingDirect = true;
}

void WindowMover::offBox()
{
	if (m_isOn == true)
		m_movingDirect = false;
}

void WindowMover::setOff()
{
	m_isOn = false;
	m_movingDirect = false;
	m_curPos = m_offPos;
}

void WindowMover::frameMove( float fElapsedTime )
{
	float movingRate = 5.0f * fElapsedTime;


	/*왼쪽(활성화 쪽)으로 움직인다.*/
	if (m_movingDirect == true && m_isOn == false)
	{
		const ArnVec3 gap = m_onPos - m_curPos;
		float curDistance = ArnVec3Length( &gap );
		ArnVec3 normalizedVector;
		ArnVec3Normalize (&normalizedVector, &gap);

		if (curDistance <= 1)
		{
			m_curPos = m_onPos;
			m_isOn = true;
		}
		else
			m_curPos += ( m_onPos - m_curPos ) * movingRate + normalizedVector;// + movingRate * ( m_onPos - m_offPos );
	}
	/*오른쪽(비활성화 쪽)으로 움직인다.*/
	else if (m_movingDirect == false && m_isOn == true)
	{
		const ArnVec3 gap = m_offPos - m_curPos;
		float curDistance = ArnVec3Length( &gap );
		ArnVec3 normalizedVector;
		ArnVec3Normalize (&normalizedVector, &gap);

		if (curDistance <= 1)
		{
			m_curPos = m_offPos;
			m_isOn = false;
		}
		else
			m_curPos += ( m_offPos - m_curPos ) * movingRate + normalizedVector;// + movingRate * ( m_offPos - m_onPos );
	}


}

ArnVec3 WindowMover::getPos()
{
	return m_curPos;
}