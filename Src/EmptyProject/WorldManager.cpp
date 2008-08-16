#include "EmptyProjectPCH.h"
#include "WorldManager.h"
#include "World.h"

IMPLEMENT_SINGLETON( WorldManager )

WorldManager::WorldManager(void)
{
	m_curWorld = 0;
	m_nextWorld = 0;
}

WorldManager::~WorldManager(void)
{
	release();
}

void WorldManager::addWorld( World* ws )
{
	m_worlds[ ws->getWorldName() ] = ws;
}


void WorldManager::changeToNextWorldIfExist()
{
	if ( m_nextWorld && m_nextWorld != m_curWorld )
	{
		if ( m_curWorld )
			m_curWorld->release();

		m_curWorld = m_nextWorld;
		m_curWorld->init();
		m_nextWorld = 0;
	}
}

void WorldManager::setNextWorld( const char* nextWorldName )
{
	if ( m_worlds.find( nextWorldName ) != m_worlds.end() )
		setNextWorld( m_worlds[ nextWorldName ] );
	else
		throw std::runtime_error( "World name not found" );
}

void WorldManager::setNextWorld( World* nextWorld )
{
	m_nextWorld = nextWorld;
}

void WorldManager::detachAllWorlds()
{
	std::map<std::string, World*>::iterator it = m_worlds.begin();
	for ( ; it != m_worlds.end(); ++it )
	{
		EP_SAFE_RELEASE( it->second );
	}
	m_worlds.clear();
}

void WorldManager::release()
{
	detachAllWorlds();
}