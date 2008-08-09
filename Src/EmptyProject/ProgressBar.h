#pragma once
#include "Picture.h"

class ProgressBar : public Picture
{
public:
	virtual ~ProgressBar(void) {}

	void setSize(float width, float height);
	void initRate(float maxRate = 100.0f) { m_maxRate = m_currentRate = maxRate; setRate(maxRate); }
	void changeRate (float rateV);
	void setRate (float rateV);
protected:
	float m_maxRate;
	float m_currentRate;
	float m_fullWidth;
};
