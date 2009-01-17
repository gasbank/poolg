#pragma once

class World;
class Dialog;

typedef std::list<Dialog*> DialogList;

/**
@brief 모든 게임내 World를 관리하는 클래스

게임 초기화시에 스크립트에 정의된 모든 World를 초기화 및 로드합니다.
또한 실행 중에는 다음으로 바꿀 World를 설정하고, 종료시에는 로드된 World를
해제합니다.
*/
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
	UINT getWorldCount() const { return m_worlds.size(); }

	HRESULT onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		                   void* pUserContext );
	void onLostDevice();

private:
	void detachAllWorlds();
	std::map<std::string, World*> m_worlds; // WorldState pool

	World* m_curWorld;
	World* m_nextWorld;

	DialogList m_globalDialogs;

};
inline WorldManager& GetWorldManager() { return WorldManager::getSingleton(); }

SCRIPT_FACTORY( WorldManager )