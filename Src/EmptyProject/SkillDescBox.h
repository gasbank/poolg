#pragma once
#include "Picture.h"

class SkillDescBox : public Picture
{
public:

	void setOnPos (float x, float y, float z);
	void setDistance (float distance) { m_distance = distance; }
	void onBox();
	void offBox();
	bool isOn();

	void setOff ();

	void frameMove( float fElapsedTime );
private:
	float m_onPosX;
	float m_distance;
	bool m_movingDirect;
	bool m_isOn;
};
