#pragma once

class World;

class WorldManager : public Singleton<WorldManager>
{
public:
	WorldManager(void);
	~WorldManager(void);

	void addWorld( World* ws );
	void release();

	World* getWorld( const char* worldName ) { return m_worlds[worldName]; }
	World* getCurWorld() const { return m_curWorld; }

	void setNextWorld( World* nextWorld );
	void setNextWorld( const char* nextWorldName );
	void changeToNextWorldIfExist();
private:
	void detachAllWorlds();
	std::map<std::string, World*> m_worlds; // WorldState pool

	World* m_curWorld;
	World* m_nextWorld;
};
inline WorldManager& GetWorldManager() { return WorldManager::getSingleton(); }