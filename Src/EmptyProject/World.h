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
#include "ShaderWrapper.h"
#include "Trigger.h"



class Sound;
class ArnMesh;
class Hero;



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
	
	
	const D3DXVECTOR3&				getHeroPos();
	bool							isCollide( const D3DXVECTOR3* vec0, const D3DXVECTOR3* vec1 );

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
	void							setupLight();
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
