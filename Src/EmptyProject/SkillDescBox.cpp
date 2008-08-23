#include "EmptyProjectPCH.h"
#include "SkillDescBox.h"

void SkillDescBox::setOnPos(float x, float y, float z)
{
	m_vPos.x = x + m_distance; m_vPos.y = y; m_vPos.z = z; m_onPosX = x;
}

void SkillDescBox::setOff()
{
	m_vPos.x = m_onPosX + m_distance;
	m_isOn = false;
	m_movingDirect = false;

	D3DXMATRIX mScaling, mTranslation;
	D3DXMatrixScaling(&mScaling, m_width, m_height, 1.0f);
	D3DXMatrixTranslation(&mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_localXform = mScaling * mTranslation;
}

void SkillDescBox::onBox()
{
	/*다른 상태일 때만 작동한다.*/
	if (m_isOn == false)
		m_movingDirect = true;
}

void SkillDescBox::offBox()
{
	if (m_isOn == true)
		m_movingDirect = false;
}

bool SkillDescBox::isOn()
{
	return m_isOn&&m_movingDirect;
}

void SkillDescBox::frameMove( float fElapsedTime )
{	
	float movingRate = 0.2f;
	float curDistance = m_vPos.x - m_onPosX; 

	/*왼쪽으로 움직인다.*/
	if (m_movingDirect == true && m_isOn == false)
	{
		if (curDistance <= 0.01f)
		{
			m_vPos.x = m_onPosX;
			m_isOn = true;
		}
		else
			m_vPos.x -= (m_vPos.x - m_onPosX) * movingRate + movingRate;
	}
	/*오른쪽으로 움직인다.*/
	else if (m_movingDirect == false && m_isOn == true)
	{
		if (curDistance >= m_distance - 0.01f)
		{
			m_vPos.x = m_onPosX + m_distance;
			m_isOn = false;
		}
		else
			m_vPos.x += (m_vPos.x - m_onPosX) * movingRate + movingRate;
	}



	D3DXMATRIX mScaling, mTranslation;
	D3DXMatrixScaling(&mScaling, m_width, m_height, 1.0f);
	D3DXMatrixTranslation(&mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_localXform = mScaling * mTranslation;
}