#include "EmptyProjectPCH.h"
#include "ProgressUi.h"

void ProgressUi::setRate( float fElapsedTime )
{
	float destRate = ( float )m_curVal / m_maxVal * 100;
	float movingRate;

	if ( m_progressMode == PM_BAR )
	{
		movingRate = 0.06f;

		//���� ���̴� ��ġ�� �� ���� ��� (��ġ ���� ��)
		if ( destRate > m_rate )
		{
			m_rate += ( destRate - m_rate ) * movingRate;
		}
		//���� ���̴� ��ġ�� �� ���� ��� (��ġ ���� ��)
		else if ( destRate < m_rate )
		{
			setRatePerforce();
		}
	}
	else if ( m_progressMode == PM_ILLUSION)
	{
		movingRate = 0.1f;
				//���� ���̴� ��ġ�� �� ���� ��� (��ġ ���� ��)
		if ( destRate > m_rate )
		{
			setRatePerforce();
		}
		//���� ���̴� ��ġ�� �� ���� ��� (��ġ ���� ��)
		else if ( destRate < m_rate )
		{
			m_rate -= ( m_rate - destRate ) * movingRate;
		}
	}
	/*
	float movingRate = 0.06f;
	//���� ���̴� HP�� �� ���� ���. (HP ������)
	if ((m_currentRate / m_maxRate) * m_fullWidth > m_width)
		m_width += ((m_currentRate / m_maxRate) * m_fullWidth - m_width) * movingRate;
	//���� ���̴� HP�� �� ���� ���. (HP ���ҽ�)
	else if ((m_currentRate / m_maxRate) * m_fullWidth < m_width)
		m_width = (m_currentRate / m_maxRate) * m_fullWidth;
	*/
	/*
		float movingRate = 0.1f;
	//���� ���̴� HP�� �� ���� ���. (HP ������)
	if ((m_currentRate / m_maxRate) * m_fullWidth > m_width)
		m_width = (m_currentRate / m_maxRate) * m_fullWidth;
	//���� ���̴� HP�� �� ���� ���. (HP ���ҽ�)
	else if ((m_currentRate / m_maxRate) * m_fullWidth < m_width)
		m_width +=  ((m_currentRate / m_maxRate) * m_fullWidth - m_width) * movingRate;

	*/
}