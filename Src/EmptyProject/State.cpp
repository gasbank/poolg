#include "EmptyProjectPCH.h"
#include "State.h"

State::State(void)
{
	m_startTime = -1.0f;
}

State::~State(void)
{
}

double State::getStateTime(double fTime)
{
	if (m_startTime < 0.0f)
		m_startTime = fTime;
	return fTime - m_startTime;
}