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
	/*�Ѱ�ġ �̻��� �� ���� �����Ѵ�.*/
	if (m_currentRate + rateV > 100)
		m_currentRate = 100;
	else if (m_currentRate + rateV < 0)
		m_currentRate = 0;
	else
		m_currentRate += rateV;
	m_width = (m_currentRate / 100) * m_fullWidth;
}