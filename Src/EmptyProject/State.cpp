#include "EmptyProjectPCH.h"
#include "State.h"

State::State(void)
{
	m_startTime = -1.0;
}

State::~State(void)
{
}

HRESULT State::enter( double dStartTime )
{
	/* 스테이트의 진입을 알리는 enter() 함수는
	 * 반드시 State 객체 생성 후 처음 혹은
	 * leave() 함수가 호출 된 후 처음으로 호출되어야 한다. */
	assert( m_startTime < 0 );
	m_startTime = dStartTime;
	return S_OK;
}

HRESULT State::leave()
{
	assert( m_startTime > 0 );
	m_startTime = -1.0;
	return S_OK;
}
