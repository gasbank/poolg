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
	/* ������Ʈ�� ������ �˸��� enter() �Լ���
	 * �ݵ�� State ��ü ���� �� ó�� Ȥ��
	 * leave() �Լ��� ȣ�� �� �� ó������ ȣ��Ǿ�� �Ѵ�. */
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
