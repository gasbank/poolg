#pragma once
#include "CommonWorldInterface.h"
#include "State.h"
#include "ArnFile.h"
#include "ArnSceneGraph.h"
#include "ArnNode.h"
#include "Picture.h"
#include "PictureMap.h"
#include "VideoMan.h"
#include "Character.h"
#include "Trigger.h"



class Sound;
class ArnMesh;
class Hero;



/**
@brief 게임의 신 그래프(scene graph) 및 지도를 관리하는 클래스

게임의 가상세계는 지도와 그 위의 유닛으로 구성되어 있습니다. 이를 통합하여
World라고 부릅니다. 이 World에는 유닛을 추가할 수도 있고 뺄 수도 있습니다.
또한 World의 특정 구역에 들어가면 다른 World로 이동할 수도 있습니다.

일반적으로 주인공이 하나의 world에서 돌아다니다가 특정 조건이 만족되어
다른 world로 이동하게 될 때에는 기존 world에서 주인공 유닛을 빼 내어 다른 world에다가
넣어주어야 합니다.
*/
class World : public CommonWorldInterface
{
public:
	static World*					createWorld( const char* worldName, TCHAR* modelFilePath, bool preloadDlg );
									~World(void);

	virtual bool					detachUnit( UnitBase* pUnit );

	HRESULT							init();
	HRESULT							frameRender(IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime);
	HRESULT							frameMove(double dTime, float fElapsedTime);
	HRESULT							handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT							release();
	HRESULT							onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
													void* pUserContext );
	HRESULT							onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
													void* pUserContext );
	void							onLostDevice();
	void							enter();

	const std::string&				getWorldName() { return m_worldName; }
	UINT							addUnit( UnitBase* u );
	UINT							addIncident( Incident* inc );
	UINT							addDialog( Dialog* dlg ) { m_scriptedDialog.push_back( dlg ); return m_scriptedDialog.size(); }

	Incident*						getIncident( UINT idx ) const;

	
	bool							detachAllUnitBaseType();
	UnitSet::iterator				removeUnit( UnitBase* pUnit );
	
	
	const ArnVec3&				getHeroPos();
	bool							isCollide( const ArnVec3* vec0, const ArnVec3* vec1 );

	Unit*							getHero() { return m_heroUnit; }
	Character*						getHeroUnit() { return m_heroUnit; }

	UnitSet*						getUnitSet() { return &m_unitSet; }
	
	bool							isInEventArea( Character* heroPt, Character* enemyPt );

	Dialog*							getDialogByName( const char* dialogName );	
	ArnSceneGraph*					getArnSceneGraphPt() { return m_modelSg; }

	Dialog*							startDialog( const char* dialogName );
	UnitBase*						findUnitAtTile( UINT x, UINT y );
	const Dialog*					getCurDialog() const { return m_curDialog; }



	void							setModelFilePath( const TCHAR* modelFilePath ) { m_modelFilePath = modelFilePath; }
	const std::tstring& 			getModelFilePath() const { return m_modelFilePath; }
	
	ArnNode*						getNode( const char* nodeName ) const;

	void							printDebugInfo() const;
	void							printDebugInfoOfIncident( UINT index ) const;
	
	Hero*							pullOutHero();

private:
									World( const char* worldName, const TCHAR* modelFilePath );
	void							startTileDefinedDialogIfExist();
	void							proceedCurDialog();
	void							forceCloseCurDialog();
	void							loadWorldModel();
	void							unloadWorldModel();
	void							battleEventCheck();
	void							wannaTalkingEventCheck();

	void							addCollisionMesh( ArnMesh* collisionMesh );
	UINT							preloadDialogs();

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

	Character*						m_heroUnit;
	
	UnitSet							m_unitSet;

	Dialog*							m_curDialog;

	std::string						m_worldName;
	std::tstring					m_modelFilePath;

	std::list<ArnMesh*>				m_collisionMeshes;

	bool							m_bNotEntered;
	
};

SCRIPT_FACTORY( World )


template<typename T> void EpPrintDebugInfoAll( T& obj ) {
	T::const_iterator it = obj.begin();
	for ( ; it != obj.end(); ++it )
	{
		(*it)->printDebugInfo();
	}
};
