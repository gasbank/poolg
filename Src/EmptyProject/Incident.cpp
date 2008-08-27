#include "EmptyProjectPCH.h"
#include "Incident.h"
#include "ScriptManager.h"

Incident::Incident( int trigCount )
: m_bActivated( false )
, m_trigCount( trigCount )
, m_LeastOnetime( false )
{
}

Incident::~Incident(void)
{
}


void Incident::release()
{
}

void Incident::activate()
{
	if ( isActivated() )
		throw std::runtime_error( "Incident::activate() is called after prior activation" );
	m_bActivated = true;
}

void Incident::deactivate()
{
	if ( !isActivated() )
		throw std::runtime_error( "Incident::deactivate() is called during not activated state" );
	m_bActivated = false;
}

//////////////////////////////////////////////////////////////////////////


int EpIncidentSetName( void* pv1, const char* pv2 )
{
	Incident* inci = reinterpret_cast<Incident*>( pv1 );
	inci->setName( pv2 );
	return 0;
} SCRIPT_CALLABLE_I_PV_PC( EpIncidentSetName )


START_SCRIPT_FACTORY( Incident )
	CREATE_OBJ_COMMAND( EpIncidentSetName )
END_SCRIPT_FACTORY( Incident )