#include "EmptyProjectPCH.h"
#include "ProgressUi.h"

void ProgressUi::setRate( float fElapsedTime )
{
	float destRate = ( float )m_curVal / m_maxVal * 100;
	float movingRate;

	if ( m_progressMode == PM_BAR )
	{
		movingRate = 0.06f;

		//현재 보이는 수치가 더 적을 경우 (수치 증가 시)
		if ( destRate > m_rate )
		{
			m_rate += ( destRate - m_rate ) * movingRate;
		}
		//현재 보이는 수치가 더 많을 경우 (수치 감소 시)
		else if ( destRate < m_rate )
		{
			setRatePerforce();
		}
	}
	else if ( m_progressMode == PM_ILLUSION)
	{
		movingRate = 0.1f;
				//현재 보이는 수치가 더 적을 경우 (수치 증가 시)
		if ( destRate > m_rate )
		{
			setRatePerforce();
		}
		//현재 보이는 수치가 더 많을 경우 (수치 감소 시)
		else if ( destRate < m_rate )
		{
			m_rate -= ( m_rate - destRate ) * movingRate;
		}
	}
	/*
	float movingRate = 0.06f;
	//현재 보이는 HP가 더 적을 경우. (HP 증가시)
	if ((m_currentRate / m_maxRate) * m_fullWidth > m_width)
		m_width += ((m_currentRate / m_maxRate) * m_fullWidth - m_width) * movingRate;
	//현재 보이는 HP가 더 많을 경우. (HP 감소시)
	else if ((m_currentRate / m_maxRate) * m_fullWidth < m_width)
		m_width = (m_currentRate / m_maxRate) * m_fullWidth;
	*/
	/*
		float movingRate = 0.1f;
	//현재 보이는 HP가 더 적을 경우. (HP 증가시)
	if ((m_currentRate / m_maxRate) * m_fullWidth > m_width)
		m_width = (m_currentRate / m_maxRate) * m_fullWidth;
	//현재 보이는 HP가 더 많을 경우. (HP 감소시)
	else if ((m_currentRate / m_maxRate) * m_fullWidth < m_width)
		m_width +=  ((m_currentRate / m_maxRate) * m_fullWidth - m_width) * movingRate;

	*/
}