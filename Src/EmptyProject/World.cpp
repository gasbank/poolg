#include "EmptyProjectPCH.h"
#include "World.h"
#include "WorldStateManager.h"
#include "TopStateManager.h"
#include "BattleState.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "ShaderWrapper.h"
#include "Enemy.h"
#include "Dialog.h"
#include "Incident.h"
#include "Sound.h"
#include "Utility.h"
#include "Hero.h"
#include "EpLight.h"
#include "SequentialIncident.h"
// Aran Includes
#include "ArnMesh.h"
#include "ArnCamera.h"


IMPLEMENT_SINGLETON( TileManager );
TileManager	tileManager;

extern std::wstring g_debugBuffer;

World::World( const char* worldName, const TCHAR* modelFilePath )
{
	m_worldName				= worldName;
	m_modelFilePath			= modelFilePath;
	m_modelArnFile			= 0;
	m_modelSg				= 0;
	m_sampleTeapotMeshRot	= 0;
	m_heroUnit				= 0;
	m_curDialog				= 0;
	m_sound					= 0;
	m_bNotEntered			= true;
}

World::~World(void)
{
}

HRESULT World::init()
{
	assert( GetG().m_dev );

	HRESULT hr = S_OK;

	loadWorldModel();

	char command[128];
	StringCchPrintfA( command, 128, "%s::init 0x%p", m_worldName.c_str(), this );
	GetScriptManager().execute( command );

	LPDIRECT3DDEVICE9& pd3dDevice =  GetG().m_dev;
	
	if ( !m_sound )
	{
		m_sound = new Sound;
		m_sound->init();
	}
	else
		throw std::runtime_error( "Sound should not be init twice or more" );

	
	setupLight();

	// 'enter' function implemented in the script file defines which characters are exist in this world
	char scriptCommand[128];
	StringCchPrintfA( scriptCommand, 128, "%s::enter", m_worldName.c_str() );
	GetScriptManager().execute( scriptCommand );
	
	// Hero and enemies are defined afterwards

	assert( m_heroUnit );

	m_curDialog = 0;
	return hr;
}


HRESULT World::frameRender(IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime)
{		
	EpCamera& camera = GetG().m_camera;
	
	HRESULT hr;

	UNREFERENCED_PARAMETER( hr );	

	//////////////////////////////////////////////////////////////////////////
	// Perspective Rendering Phase

	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	pd3dDevice->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());
	

	//////////////////////////////////////////////////////////////////////////
	// Aran lib rendering routine (CW)
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	pd3dDevice->SetFVF(ArnVertex::FVF);
	D3DXMATRIX transform;
	D3DXMatrixTranslation( &transform, m_heroUnit->getPos().x, m_heroUnit->getPos().y, m_heroUnit->getPos().z );
	GetG().m_videoMan.renderMeshesOnly(m_modelSg->getSceneRoot());
	
	//////////////////////////////////////////////////////////////////////////
	// EP rendering routine (CCW)

	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( !(*it)->getRemoveFlag() )
			(*it)->frameRender( dTime, fElapsedTime );
	}
	
	DialogList::iterator itDialog = m_scriptedDialog.begin();
	for ( ; itDialog != m_scriptedDialog.end(); ++itDialog )
	{
		(*itDialog)->frameRender(pd3dDevice, dTime, fElapsedTime);
	}

	WorldStateManager& wsm = WorldStateManager::getSingleton();
	wsm.getCurState()->frameRender(pd3dDevice, dTime, fElapsedTime);

	return S_OK;

}

HRESULT World::frameMove( double dTime, float fElapsedTime )
{
	EpCamera& camera = GetG().m_camera;

	HRESULT hr;

	UNREFERENCED_PARAMETER( hr );

	if ( m_bNotEntered )
	{
		enter();
		m_bNotEntered = false;
	}
	
	m_pic.frameMove(fElapsedTime);
	m_avatar.frameMove(fElapsedTime);
	//camera.frameMove(fElapsedTime);

	if ( m_sound )
		m_sound->UpdateAudio();
	
	DialogList::iterator itDialog = m_scriptedDialog.begin();
	for ( ; itDialog != m_scriptedDialog.end(); ++itDialog )
	{
		(*itDialog)->frameMove( dTime, fElapsedTime );
	}
	
	//GetG().m_camera.frameMove( fElapsedTime );
	
	m_sampleTeapotMeshRot += fElapsedTime * D3DXToRadian(35); // 35 degrees per second

	GetWorldStateManager().transit();
	GetWorldStateManager().getCurState()->frameMove(dTime, fElapsedTime);

	m_modelSg->getSceneRoot()->update(dTime, fElapsedTime);
	

	WCHAR msg[128];

	StringCchPrintf( msg, 128, L"Hero Pos : (%.2f, %.2f, %.2f)\n",
		m_heroUnit->getPos().x,
		m_heroUnit->getPos().y,
		m_heroUnit->getPos().z );
	g_debugBuffer.append( msg );

	StringCchPrintf( msg, 128, L"Hero TilePos : (%d, %d)\n",
		m_heroUnit->getTilePosX(),
		m_heroUnit->getTilePosY() );
	g_debugBuffer.append( msg );

	D3DXVECTOR3 diff = m_heroUnit->getPos() - *GetG().m_camera.GetEyePt();
	StringCchPrintf( msg, 128, L"Hero - Camera Pos : (%.4f, %.4f, %.4f)\n",
		diff.x, diff.y, diff.z );
	g_debugBuffer.append( msg );

	
	if ( D3DXVec2Length( (const D3DXVECTOR2*)&diff ) > 0.001f)
	{
		// Check for camera and character move inconsistency when field state
		//DebugBreak();
	}

	if ( ((Hero*)getHeroUnit())->isEncounterEnemy() )
		battleEventCheck();

	wannaTalkingEventCheck();

	// Incidents update
	IncidentList::iterator itInc = m_incidents.begin();
	for ( ; itInc != m_incidents.end(); ++itInc )
	{
		(*itInc)->update( dTime, fElapsedTime );
	}

	UnitSet::iterator it2 = m_unitSet.begin();
	for ( ; it2 != m_unitSet.end(); )
	{
		(*it2)->frameMove(fElapsedTime);
		/*if ( (*it2)->getRemoveFlag() )
		{
			it2 = removeUnit( *it2 );
		}
		else*/
			++it2;
	}


	return S_OK;
}

HRESULT World::release()
{
	// DO NOT RELEASE THIS POINTER: m_heroUnit

	EP_SAFE_RELEASE( m_sound );

	m_pic.release();
	m_picRhw.release();
	m_picSmiley.release();
	m_avatar.release();

	EpSafeReleaseAll( m_scriptedDialog );
	EpSafeReleaseAll( m_unitSet );
	EpSafeReleaseAll( m_incidents );

	unloadWorldModel();

	return S_OK;
}

HRESULT World::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_pic.handleMessages(hWnd, uMsg, wParam, lParam);

	if ( m_sound )
		m_sound->handleMessages(hWnd, uMsg, wParam, lParam);

	bool bTalking = false;
	
	DialogList::iterator itDialog = m_scriptedDialog.begin();
	for ( ; itDialog != m_scriptedDialog.end(); ++itDialog )
	{
		(*itDialog)->handleMessages( hWnd, uMsg, wParam, lParam );
		bTalking |= (*itDialog)->isTalking();
	}
	
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( (*it)->getType() == UT_HERO && GetWorldStateManager().curStateEnum() == GAME_WORLD_STATE_FIELD )
		{
			if ( !bTalking )
			{
				(*it)->handleMessages(hWnd, uMsg, wParam, lParam);
			}
			else
				getHero()->clearKey();
		}
	}

	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_ESCAPE)
		{
			if ( GetWorldStateManager().curStateEnum() != GAME_WORLD_STATE_BATTLE )
				GetWorldStateManager().setNextState(GAME_WORLD_STATE_MENU);
		}
		if (wParam == 'P')
		{
			printf("이녀석의 x 위치 : %d\n", m_heroUnit->getTilePosX());
			printf("이녀석의 y 위치 : %d\n", m_heroUnit->getTilePosY());
		}
	}

	if (uMsg == WM_KEYUP)
	{
		if (wParam == VK_RETURN)
		{
			if ( !m_curDialog )
				startTileDefinedDialogIfExist();

			if ( m_curDialog )
				proceedCurDialog();
		}
		if (wParam == VK_END)
		{
			forceCloseCurDialog();
		}
	}

	switch ( uMsg )
	{
	case WM_KEYDOWN:
		switch ( wParam )
		{
		case 'C':
			//////////////////////////////////////////////////////////////////////////
			// External camera test
			EpCamera& rCamera = GetG().m_camera;
			const D3DXVECTOR3& vHeroPos = getHeroPos();
			static bool bExtCam = true;
			if ( bExtCam )
			{
				rCamera.setExternalCamera( 
					static_cast<ArnCamera*>(m_modelSg->getSceneRoot()->getNodeByName( "Camera" )) );
				rCamera.begin( CAMERA_EXTERNAL );
			}
			else
			{
				rCamera.setAttachPos( &vHeroPos );
				rCamera.setSmoothCameraDuration( 1.0f );
				rCamera.begin( CAMERA_SMOOTH_ATTACH );

			}
			bExtCam = !bExtCam;
			//////////////////////////////////////////////////////////////////////////
			break;
		}
		break;
	case WM_KEYUP:
		break;
	}

	if ( GetWorldStateManager().getCurState() )
		GetWorldStateManager().getCurState()->handleMessages(hWnd, uMsg, wParam, lParam);

	return S_OK;
}

void World::setupLight() 
{
	D3DLIGHT9& light = GetG().m_light;
	LPDIRECT3DDEVICE9& pd3dDevice = GetG().m_dev;

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	D3DCOLORVALUE cv = { 0.0f, 0.0f, 0.0f, 1.0f };
	light.Ambient = cv;
	light.Diffuse = cv;
	light.Specular = cv;
	
	light.Attenuation0 = 0.5f;

	D3DXVECTOR3 dir(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &dir);

	D3DXVECTOR3 pos(0.0f, -30.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Position, &pos);

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Range = 1000.0f;

	pd3dDevice->SetLight(0, &light);
	pd3dDevice->LightEnable(0, TRUE);
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

UINT World::addUnit( Unit* u )
{
	m_unitSet.insert(u);
	if ( u->getType() == UT_HERO )
	{
		/*주인공 유닛 설정*/
		m_heroUnit = dynamic_cast<Character*>( u );
	}

	return m_unitSet.size();
}

const D3DXVECTOR3& World::getHeroPos()
{
	assert( m_heroUnit );
	return m_heroUnit->getPos();
}

bool World::isInEventArea( Character* heroPt, Character* enemyPt )
{
	// 적의 타일 좌표가 0, 0일 때 플레이어가 들어오면 전투가 시작되는 타일 좌표 범위.
	
	int fightTileArea[13][2] = { 
		{-2, 0}, 
		{-1, 1}, {-1, 0}, {-1, -1}, 
		{ 0, 2}, { 0, 1}, { 0,  0}, {0, -1}, {0, -2},
		{ 1, 1}, { 1, 0}, { 1, -1},
		{ 2, 0}
	};

	int range = static_cast<Enemy*>(enemyPt)->getFightRange();

	range++;

	// 적의 전투 범위 타일 안에 주인공이 있는지 판단한다.
	for ( int i = -range; i <= range; i++ )
	{
		for ( int j = -(range - abs( i )); j <= (range - abs( i )); j++ )
		{
			if ( (abs( i ) == range) || (abs( j ) == range) )
				continue;

			if ( (enemyPt->getTilePosX() + i ) == heroPt->getTilePosX()
				&& (enemyPt->getTilePosY() + j ) == heroPt->getTilePosY() )			
				return true;

		}
	}
	return false;

	
}

bool World::isCollide( const D3DXVECTOR3* vec0, const D3DXVECTOR3* vec1 )
{
	const float collideRange = 10.0f;
	float range = sqrt( (vec0->x - vec1->x) * (vec0->x - vec1->x) + (vec0->y - vec1->y) * (vec0->y - vec1->y) );
	if ( range <= collideRange )
				return true;

	return false;
}

Dialog* World::startDialog( const char* dialogName )
{
	if ( !m_curDialog )
	{
		getHeroUnit()->clearKey();
		m_curDialog = getDialogByName( dialogName );

		if ( m_curDialog->getRemoveFlag() )
			m_curDialog = 0;
		else
			proceedCurDialog();

		return m_curDialog;
	}
	else
		throw std::runtime_error( "Current dialog already exists! Cannot start a new dialog" );
}


// 유닛의 포인터를 받아서, UnitSet에서 해당 유닛을 찾아 지운다.
UnitSet::iterator World::removeUnit( Unit* pUnit )
{
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); it++ )
	{
		if ( *it == pUnit )
		{
			EP_SAFE_RELEASE( pUnit );
			return m_unitSet.erase( it );
		}
	}
	return m_unitSet.end();
}

/*void WorldState::detectBattleEvent()
{
	// Detect battle event.
	// If current selected unit isn't hero unit, and isn't talkable,
	// regard as enemy. And if hero is in the fight area of enemy, start battle.
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( (*it) != m_heroUnit )
		{
			Enemy* oppCharacter = dynamic_cast<Enemy*>( *it );
			if ( oppCharacter->isTalkable() == false )
			{
				if ( isInEventArea( m_heroUnit, oppCharacter ) == true )
				{
					// Set current enemy unit and enter into BattleState
					m_curEnemyUnit = oppCharacter;

					m_curEnemyUnit->setAttack (30);

					if ( GetWorldStateManager().curStateEnum() == GAME_WORLD_STATE_FIELD )
						GetWorldStateManager().setNextState( GAME_WORLD_STATE_BATTLE );
				}
			}
		}	
	}
} */

Unit* World::findUnitAtTile( UINT x, UINT y )
{
	Unit* ret = 0;
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( (*it)->getTilePosX() == x && (*it)->getTilePosY() == y && !(*it)->getRemoveFlag() )
		{
			ret = *it;
			break;
		}
	}
	return ret;
}

void World::startTileDefinedDialogIfExist()
{
	if( !getHeroUnit()->getMoving() && m_curDialog == 0 )
	{
		getHeroUnit()->clearKey();
		DialogList::iterator it = m_scriptedDialog.begin();
		for ( ; it != m_scriptedDialog.end(); ++it )
		{
			Dialog* dialog = (*it);

			if ( dialog->getRegion().isExist( getHeroUnit()->getTilePos() ) )
			{
				m_curDialog = dialog;
				break;
			}
		}
	}
}

void World::proceedCurDialog()
{
	if ( m_curDialog )
	{
		if ( m_curDialog->nextDialog() == false )
		{
			m_curDialog->setRemoveFlag( true );
			m_curDialog = 0;
		}
		else if ( !m_curDialog->isTalking() )
			m_curDialog = 0;
	}
}
void World::forceCloseCurDialog()
{
	while ( m_curDialog )
		proceedCurDialog();
}

Dialog* World::getDialogByName( const char* dialogName )
{
	DialogList::iterator it = m_scriptedDialog.begin();
	for ( ; it != m_scriptedDialog.end(); ++it )
	{
		if ( (*it)->getDialogName() == dialogName )
			return (*it);
	}

	// Dialog not found! Configure at runtime.
	OutputDebugString( _T( " - INFO: Requested dialog is not found on World. Trying to generate and register new one...\n" ) );
	Dialog* newDlg = Dialog::createDialogByScript( dialogName );
	newDlg->init();
	addDialog( newDlg );
	return getDialogByName( dialogName );
}

UINT World::addIncident( Incident* inc )
{
	m_incidents.push_back( inc );
	return m_incidents.size();
}

void World::loadWorldModel()
{
	if ( m_modelArnFile == 0 && m_modelSg == 0 )
	{
		// World model init and loading
		m_modelArnFile = new ArnFileData;
		load_arnfile( m_modelFilePath.c_str(), *m_modelArnFile );
		m_modelSg = new ArnSceneGraph( *m_modelArnFile );
	}
	else if ( m_modelArnFile && m_modelSg )
	{
		// Already loaded
	}
	else
	{
		// Partially loaded?!
		throw std::runtime_error( "Arn file loading corrupted!" );
	}
}
void World::unloadWorldModel()
{
	if ( m_modelArnFile )
		release_arnfile( *m_modelArnFile );
	SAFE_DELETE( m_modelArnFile );
	SAFE_DELETE( m_modelSg );
}
World* World::createWorld( const char* worldName, TCHAR* modelFilePath, bool preloadDlg )
{
	World* ret = new World( worldName, modelFilePath );
	if ( preloadDlg )
		ret->preloadDialogs();

	return ret;
}

void World::addCollisionMesh( ArnMesh* collisionMesh )
{
	m_collisionMeshes.push_back( collisionMesh );
}

ArnNode* World::getNode( const char* nodeName ) const
{
	return m_modelSg->getSceneRoot()->getNodeByName( nodeName );
}

void World::battleEventCheck()
{
	BattleState* bs = static_cast<BattleState*>( GetWorldStateManager().getState( GAME_WORLD_STATE_BATTLE ) );
	assert( bs );

	// Detect battle event.
	// If current selected unit isn't hero unit, and isn't talkable,
	// regard as enemy. And if hero is in the fight area of enemy, start battle.
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( (*it) != getHeroUnit() )
		{
			Enemy* oppCharacter = static_cast<Enemy*>( *it );
			assert( oppCharacter );
			if ( oppCharacter->getType() == UT_ENEMY && oppCharacter->isTalkable() == false && !oppCharacter->getRemoveFlag() )
			{
				if ( isInEventArea( getHeroUnit(), oppCharacter ) == true)
				{
					//////////////////////////////////////////////////////////////////////////
					// Check whether there is obstacle between hero and enemy.
					// Shoot ray from hero to enemy, and if distance to the third intersected
					// mesh is shorter than distance between hero and enemy, then there exists
					// at least one obstacle.
					D3DXVECTOR3 vStartPos( getHero()->getPos() );
					D3DXVECTOR3 vRayDir = oppCharacter->getPos() - getHero()->getPos();
					float fRayLength = D3DXVec3Length( &vRayDir );
					D3DXVec3Normalize( &vRayDir, &vRayDir );
					float f3rdDist = Utility::FullTraverseExhaustiveRayTesting(
						getArnSceneGraphPt()->getSceneRoot(),
						vStartPos,
						vRayDir,
						1 );

					//printf( "3rd dist, ray length %f, %f\n", f3rdDist, fRayLength );

					if ( f3rdDist < fRayLength )
						continue;
					//////////////////////////////////////////////////////////////////////////

					
					// No more move!
					getHeroUnit()->clearKey();
					oppCharacter->clearKey();
					getHeroUnit()->setControllable( false );

					// view each other
					getHero()->setViewAt( &oppCharacter->getPos() );
					oppCharacter->setViewAt( &getHero()->getPos() );

					// Insert to enemy pool at BattleState
					bs->insertEnemy( oppCharacter );
					
					if ( GetWorldStateManager().curStateEnum() == GAME_WORLD_STATE_FIELD )
						GetWorldStateManager().setNextState( GAME_WORLD_STATE_BATTLE );
				}
			}
		}	
	}
}

void World::wannaTalkingEventCheck()
{
	BattleState* bs = static_cast<BattleState*>( GetWorldStateManager().getState( GAME_WORLD_STATE_BATTLE ) );
	assert( bs );

	// Check if hero is in the talking enemy's fight area. If true, the enemy look at hero.
	// This work is almost same as battle event check.
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( (*it) != getHeroUnit() )
		{
			Enemy* unconfirmedEnemy = dynamic_cast<Enemy*>( *it );
			if ( unconfirmedEnemy != NULL 
				&& unconfirmedEnemy->isTalkable() == true
				&& !unconfirmedEnemy->getRemoveFlag() )
			{
				Enemy* talkingEnemy = unconfirmedEnemy;

				if ( isInEventArea( getHeroUnit() , talkingEnemy ) == true )
				{
					// no more move!
					talkingEnemy->clearKey();

					// View at hero
					talkingEnemy->setViewAt( &getHero()->getPos() );

					//bs->insertEnemy( talkingEnemy );
				}
				else
				{
					talkingEnemy->setRotZ( 0.0f );
				}
			}
		}	
	}
}



void World::printDebugInfo() const
{
	EpPrintDebugInfoAll( m_unitSet );
	EpPrintDebugInfoAll( m_scriptedDialog );
	EpPrintDebugInfoAll( m_incidents );
}

void World::printDebugInfoOfIncident( UINT index ) const
{
	const Incident* inc = getIncident( index );
	inc->printDebugInfoDetailed();
}

Incident* World::getIncident( UINT idx ) const
{
	assert( idx >= 0 && idx < m_incidents.size() );

	IncidentList::const_iterator it = m_incidents.begin();
	while ( idx )
	{
		++it;
		--idx;
	}
	return (*it);
}

// Delete hero pointer from UnitSet of this world and return hero's pointer.
Hero* World::pullOutHero()
{
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( *it == m_heroUnit )
		{
			m_unitSet.erase( it );
			break;
		}
	}
	assert( m_heroUnit );
	return static_cast<Hero*>(getHero());
}

VOID World::enter()
{
	GetG().m_camera.setAttachPos( &getHeroPos() );
	GetG().m_camera.begin( CAMERA_ATTACH );

	/*D3DXVECTOR3 eye( getHero()->getPos().x, getHero()->getPos().y, getHero()->getPos().z - 30.0f );
	D3DXVECTOR3 at( getHero()->getPos().x, getHero()->getPos().y, getHero()->getPos().z - 1.0f );
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f );
	GetG().m_camera.setViewParamsWithUp( &eye, &at, &up );*/

	//GetEpLight().setFadeDuration( 1.0f );
	GetEpLight().setBrightness( 0.0f );
	//GetEpLight().fadeInLightForcedDelayed( 2.0f );
}

UINT World::preloadDialogs()
{
	char scriptCommand[512];
	ConstCharList dialogNameList;
	StringCchPrintfA( scriptCommand, 512, "%s::dialogNameList", m_worldName );
	GetScriptManager().readCharPtrList( scriptCommand, dialogNameList );

	ConstCharList::iterator itDlg = dialogNameList.begin();
	for ( ; itDlg != dialogNameList.end(); ++itDlg )
	{
		StringCchPrintfA( scriptCommand, 512, "%s::%s", m_worldName, *itDlg );
		Dialog* newDlg = Dialog::createDialogByScript( scriptCommand );
		newDlg->init();
		addDialog( newDlg );
	}

	return dialogNameList.size();
}

HRESULT World::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
							  void* pUserContext )
{
	OutputDebugString( _T( " - INFO: World::onResetDevice called.\n" ) );
	loadWorldModel();
	return S_OK;
}

void World::onLostDevice()
{
	OutputDebugString( _T( " - INFO: World::onLostDevice called.\n" ) );
	unloadWorldModel();
}

HRESULT World::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	OutputDebugString( _T( " - INFO: World::onCreateDevice called.\n" ) );

	return S_OK;
}
//////////////////////////////////////////////////////////////////////////

Unit* EpGetHero()
{
	return GetWorldManager().getCurWorld()->getHero();

} SCRIPT_CALLABLE_PV( EpGetHero )

int EpHasHero()
{
	if ( GetWorldManager().getCurWorld()->getHero() == NULL )
		return 0;
	else
		return 1;
} SCRIPT_CALLABLE_I( EpHasHero )

int EpRegisterIncident( void* ptr )
{
	Incident* inc = reinterpret_cast<Incident*>( ptr );
	return GetWorldManager().getCurWorld()->addIncident( inc );

} SCRIPT_CALLABLE_I_PV( EpRegisterIncident )


ArnNode* EpGetNode( const char* nodeName )
{
	return GetWorldManager().getCurWorld()->getNode( nodeName );

} SCRIPT_CALLABLE_PV_PC( EpGetNode )

int EpMeshSetVisible( void* node, int b )
{
	ArnMesh* mesh = static_cast<ArnMesh*>( node );
	if ( mesh->getType() == NDT_RT_MESH )
	{
		mesh->setVisible( b?true:false );
	}
	else
		throw std::runtime_error( "First argument should be ArnMesh type" );

	return 0;

} SCRIPT_CALLABLE_I_PV_I( EpMeshSetVisible )

int EpMeshSetCollide( void* node, int b )
{
	ArnMesh* mesh = static_cast<ArnMesh*>( node );
	if ( mesh->getType() == NDT_RT_MESH )
	{
		mesh->setCollide( b?true:false );
	}
	else
		throw std::runtime_error( "First argument should be ArnMesh type" );

	return 0;

} SCRIPT_CALLABLE_I_PV_I( EpMeshSetCollide )

int EpSetDoAnim( void* ptr, int bDoAnim )
{
	if ( ptr )
	{
		ArnXformable* xformable = reinterpret_cast<ArnXformable*>( ptr );
		xformable->setDoAnim( bDoAnim?true:false );
		return 0;
	}
	else
		return -1;
} SCRIPT_CALLABLE_I_PV_I( EpSetDoAnim )


int EpSetAnimTime( void* ptr, double dTime )
{
	ArnXformable* xformable = reinterpret_cast<ArnXformable*>( ptr );
	xformable->setAnimCtrlTime( dTime );
	return 0;

} SCRIPT_CALLABLE_I_PV_D( EpSetAnimTime )


int EpCurWorldDebugInfo()
{
	GetWorldManager().getCurWorld()->printDebugInfo();
	return 0;
} SCRIPT_CALLABLE_I( EpCurWorldDebugInfo )

int EpCurWorldDebugInfoOfIncident( int idx )
{
	GetWorldManager().getCurWorld()->printDebugInfoOfIncident( (UINT)idx );
	return 0;
} SCRIPT_CALLABLE_I_I( EpCurWorldDebugInfoOfIncident )

int EpCameraSetShake( int b )
{
	GetG().m_camera.setShake( b?true:false );
	return 0;
} SCRIPT_CALLABLE_I_I( EpCameraSetShake )


START_SCRIPT_FACTORY( World )
	CREATE_OBJ_COMMAND( EpGetHero )
	CREATE_OBJ_COMMAND( EpHasHero )
	CREATE_OBJ_COMMAND( EpRegisterIncident )
	CREATE_OBJ_COMMAND( EpGetNode )
	CREATE_OBJ_COMMAND( EpSetDoAnim )
	CREATE_OBJ_COMMAND( EpSetAnimTime )
	CREATE_OBJ_COMMAND( EpCurWorldDebugInfo )
	CREATE_OBJ_COMMAND( EpCurWorldDebugInfoOfIncident )
	CREATE_OBJ_COMMAND( EpMeshSetVisible )
	CREATE_OBJ_COMMAND( EpMeshSetCollide )
	CREATE_OBJ_COMMAND( EpCameraSetShake )
END_SCRIPT_FACTORY( World )
