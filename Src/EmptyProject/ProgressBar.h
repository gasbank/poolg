#pragma once
#include "Box.h"

class ProgressBar : public Box
{
public:
	void setSize(float width, float height);
	void initRate() {m_currentRate = 100.0f;}
	void changeRate (float rateV);
protected:
	float m_currentRate;
	float m_fullWidth;
};
