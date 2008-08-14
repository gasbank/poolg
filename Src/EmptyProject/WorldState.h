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
#include "ShaderWrapper.h"
#include "Trigger.h"

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
	Character* getHeroUnit() { return m_heroUnit; }
	Unit* getCurEnemy() { return m_curEnemyUnit; }
	Character* getCurEnemyUnit() { return m_curEnemyUnit; }
	void setCurEnemy( Character* enemy ) { m_curEnemyUnit = enemy; }
	UnitSet* getUnitSet() { return &m_unitSet; }
	void removeUnit( Unit* pUnit );
	bool isInFightArea( Character* heroPt, Character* enemyPt );

	void startDialog( int index );
	
	ArnSceneGraph* getArnSceneGraphPt() { return m_sg; }
	void screenFlashing( float durationSec, float r, float g, float b );

	Unit* findUnitAtTile( UINT x, UINT y );
private:
	void setupLight();
	void handleCollision( Unit* heroUnit, Unit* opponentUnit );

	ArnFileData*					m_afd;
	ArnSceneGraph*					m_sg;
	ArnFileData*					m_afdRat;
	ArnSceneGraph*					m_sgRat;
	PictureMap						m_pic;
	Picture							m_picRhw;
	Picture							m_picSmiley;
	Picture							m_avatar;
	Sound							m_sound;
	//Dialog						m_dialog;
	DialogList						m_scriptedDialog;
	Trigger*						m_trigger;


	float							m_sampleTeapotMeshRot;
	LPD3DXMESH						m_aTile;

	Character*						m_heroUnit;
	Character*						m_curEnemyUnit;
	
	UnitSet							m_unitSet;

	AlphaShader*					m_alphaShader;
	LPD3DXMESH						m_testPolygon;
	LPD3DXMESH						m_testPolygonCloned;
	float							m_screenFlashAlphaAngle;
	float							m_screenFlashDurationSec;
	D3DXCOLOR						m_screenFlashCV;
};
