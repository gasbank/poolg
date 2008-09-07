#include "EmptyProjectPCH.h"
#include "State.h"

State::State(void)
{
	m_startTime = -1.0f;
}

State::~State(void)
{
}

HRESULT State::enter( double dStartTime )
{
	assert( m_startTime < 0 );
	m_startTime = dStartTime;
	return S_OK;
}

HRESULT State::leave()
{
	assert( m_startTime > 0 );
	m_startTime = -1.0f;
	return S_OK;
}