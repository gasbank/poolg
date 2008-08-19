#pragma once
#include "state.h"
#include "ArnFile.h"
#include "ArnSceneGraph.h"
#include "ArnNode.h"
#include "Picture.h"
#include "PictureMap.h"
#include "VideoMan.h"
#include "Character.h"
#include "ShaderWrapper.h"
#include "Trigger.h"

class Incident;
class Unit;
class Dialog;
class Sound;
class ArnMesh;

typedef std::set<Unit*> UnitSet;
typedef std::list<Dialog*> DialogList;
typedef std::list<Incident*> IncidentList;

class World
{
public:
	static World*		createWorld( const char* worldName, TCHAR* modelFilePath );
						~World(void);

	HRESULT				init();
	HRESULT				frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	HRESULT				frameMove(double fTime, float fElapsedTime);
	HRESULT				handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT				release();

	const std::string&	getWorldName() { return m_worldName; }
	UINT				addUnit( Unit* u );
	UINT				addIncident( Incident* inc );
	UINT				addDialog( Dialog* dlg ) { m_scriptedDialog.push_back( dlg ); return m_scriptedDialog.size(); }

	UnitSet::iterator	removeUnit( Unit* pUnit );
	
	void				setCurEnemy( Character* enemy ) { m_curEnemyUnit = enemy; }
	
	const D3DXVECTOR3&	getEnemyPos();
	const D3DXVECTOR3&	getHeroPos();
	bool				isCollide( const D3DXVECTOR3* vec0, const D3DXVECTOR3* vec1 );

	Unit*				getHero() { return m_heroUnit; }
	Character*			getHeroUnit() { return m_heroUnit; }

	Unit*				getCurEnemy() { return m_curEnemyUnit; }
	Character*			getCurEnemyUnit() { return m_curEnemyUnit; }
	UnitSet*			getUnitSet() { return &m_unitSet; }
	
	bool				isInFightArea( Character* heroPt, Character* enemyPt );

	Dialog*				getDialogByName( const char* dialogName );	
	ArnSceneGraph*		getArnSceneGraphPt() { return m_modelSg; }

	Dialog*				startDialog( const char* dialogName );
	Unit*				findUnitAtTile( UINT x, UINT y );
	const Dialog*		getCurDialog() const { return m_curDialog; }



	void				setModelFilePath( const TCHAR* modelFilePath ) { m_modelFilePath = modelFilePath; }
	const std::tstring& getModelFilePath() const { return m_modelFilePath; }
	
	ArnNode*			getNode( const char* nodeName ) const;
private:
						World( const char* worldName, const TCHAR* modelFilePath );
	void				setupLight();
	void				handleCollision( Unit* heroUnit, Unit* opponentUnit );
	void				startTileDefinedDialogIfExist();
	void				proceedCurDialog();
	void				loadWorldModel();

	void				addCollisionMesh( ArnMesh* collisionMesh );

	ArnFileData*					m_modelArnFile;
	ArnSceneGraph*					m_modelSg;

	PictureMap						m_pic;
	Picture							m_picRhw;
	Picture							m_picSmiley;
	Picture							m_avatar;
	Sound*							m_sound;

	DialogList						m_scriptedDialog;
	IncidentList					m_incidents;


	float							m_sampleTeapotMeshRot;
	LPD3DXMESH						m_aTile;

	Character*						m_heroUnit;
	Character*						m_curEnemyUnit;
	
	UnitSet							m_unitSet;

	Dialog*							m_curDialog;

	std::string						m_worldName;
	std::tstring					m_modelFilePath;

	std::list<ArnMesh*>				m_collisionMeshes;
	
};

SCRIPT_FACTORY( World )