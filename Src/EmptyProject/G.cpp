#include "EmptyProjectPCH.h"
#include "G.h"

IMPLEMENT_SINGLETON(G);

G::G(void)
{
	m_scrWidth = 160 * 5;
	m_scrHeight = 90 * 5;
}

G::~G(void)
{
}
