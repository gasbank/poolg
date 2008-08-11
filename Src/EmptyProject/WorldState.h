#pragma once
#include "state.h"
#include "ArnFile.h"
#include "ArnSceneGraph.h"
#include "ArnNode.h"
#include "Picture.h"
#include "PictureMap.h"
#include "Sound.h"
#include "Unit.h"
#include "VideoMan.h"
#include "Character.h"
#include "Dialog.h"

typedef std::set<Unit*> UnitSet;
typedef std::list<Dialog*> DialogList;

class WorldState : public State
{
public:
	WorldState(void);
	~WorldState(void);

	virtual HRESULT enter();
	virtual HRESULT leave();

	virtual HRESULT frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual HRESULT frameMove(double fTime, float fElapsedTime);

	virtual HRESULT handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual HRESULT release();

	UINT addUnit(Unit* u);
	void detachAllUnits();
	const D3DXVECTOR3& getEnemyPos();
	const D3DXVECTOR3& getHeroPos();
	bool isCollide( const D3DXVECTOR3* vec0, const D3DXVECTOR3* vec1 );

	Unit* getHero() { return m_heroUnit; }
	Unit* getCurEnemy() { return m_curEnemyUnit; }
	void removeUnit( Unit* pUnit );

	void startDialog( int index );

private:
	void setupLight();
	void handleCollision( Unit* heroUnit, Unit* enemyUnit );

	ArnFileData*					m_afd;
	ArnSceneGraph*					m_sg;
	PictureMap						m_pic;
	Picture							m_picRhw;
	Picture							m_picSmiley;
	Picture							m_avatar;
	Sound							m_sound;
	//Dialog						m_dialog;
	DialogList						m_scriptedDialog;


	float							m_sampleTeapotMeshRot;
	LPD3DXMESH						m_aTile;

	Character*						m_heroUnit;
	Character*						m_curEnemyUnit;
	
	UnitSet							m_unitSet;
};
