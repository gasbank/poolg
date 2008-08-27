#include "EmptyProjectPCH.h"
#include "ProgressUi.h"

void ProgressUi::setRate( float fElapsedTime )
{
	float destRate = m_curVal / ( float )m_maxVal;
	if ( destRate > 1 )
		destRate = 1;
	else if (destRate < 0)
		destRate = 0;
	float movingRate;
	
	float minSpeed = 0.001f;



	if ( m_progressMode == PM_BAR )
	{
		movingRate = 3.0f;

		//���� ���̴� ��ġ�� �� ���� ��� (��ġ ���� ��)
		if ( destRate > m_rate )
		{

			m_rate += ( destRate - m_rate ) * movingRate * fElapsedTime + minSpeed;
		}
		//���� ���̴� ��ġ�� �� ���� ��� (��ġ ���� ��)
		else if ( destRate < m_rate )
		{
			setRatePerforce();
		}
	}
	else if ( m_progressMode == PM_ILLUSION)
	{
		movingRate = 3.0f;
				//���� ���̴� ��ġ�� �� ���� ��� (��ġ ���� ��)
		if ( destRate > m_rate )
		{
			setRatePerforce();
		}
		//���� ���̴� ��ġ�� �� ���� ��� (��ġ ���� ��) gypsy
		else if ( destRate < m_rate )
		{
			m_rate -= ( m_rate - destRate ) * movingRate * fElapsedTime + minSpeed;
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

float ProgressUi::getRate ()
{
	if (m_rate < 0)
		m_rate = 0;
	else if (m_rate > 1)
		m_rate = 1;
	return m_rate;
}