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
	m_aTile					= 0;
	m_heroUnit				= 0;
	m_curEnemyUnit			= 0;
	m_curDialog				= 0;
	m_sound					= 0;
}

World::~World(void)
{
}

HRESULT World::init()
{
	HRESULT hr = S_OK;

	loadWorldModel();

	char command[128];
	StringCchPrintfA( command, 128, "%s::init 0x%p", m_worldName.c_str(), this );
	GetScriptManager().execute( command );

	LPDIRECT3DDEVICE9& pd3dDevice =  GetG().m_dev;
	
	// Load sample image (vertex and index buffer creation with texture)
	const UINT mapSegments = 32;
	const UINT mapSize = 32;
	m_pic.init(L"Images/graytile.tga", pd3dDevice, mapSegments);
	m_pic.setSize((float)mapSize, (float)mapSize);
	m_pic.setPos(D3DXVECTOR3(m_pic.getPos()->x - mapSize/2, m_pic.getPos()->y - mapSize/2, m_pic.getPos()->z));
	
	m_picRhw.initRhw(L"Images/smiley.png", pd3dDevice);
	m_picSmiley.init(L"Images/smiley.png", pd3dDevice);
	//g_picSmiley.setSizeToTexture();

	m_avatar.init(L"Images/smiley.png", pd3dDevice);
	m_avatar.setSize(1, 1);

	if ( !m_sound )
	{
		m_sound = new Sound;
		m_sound->init();
	}
	else
		throw std::runtime_error( "Sound should not be init twice or more" );


	
	D3DXCreateBox(pd3dDevice, 1.0f, 1.0f, 1.0f, &m_aTile, 0);
	
	setupLight();

	// 'enter' function implemented in the script file defines which characters are exist in this world
	char scriptCommand[128];
	StringCchPrintfA( scriptCommand, 128, "%s::enter", m_worldName.c_str() );
	GetScriptManager().execute( scriptCommand );
	
	// Hero and enemies are defined afterwards
	
	// Load dialogs from script
	ConstCharList dialogList;
	GetScriptManager().readCharPtrList( "EpDialogList", dialogList );
	ConstCharList::iterator itDialogList = dialogList.begin();
	for ( ; itDialogList != dialogList.end(); ++itDialogList )
	{
		Dialog* newDlg = Dialog::createDialogByScript( *itDialogList );
		newDlg->init();
		m_scriptedDialog.push_back( newDlg );
	}

	assert( m_heroUnit );

	// Incidents construction
	/*Trigger* trigger = new UnitPositionTrigger( m_heroUnit, TileRegion( 26, 80, 27, 82 ) );
	Action* action = new DialogAction( "EpDialog4" );
	Incident* inc = new Incident( trigger, action );
	m_incidents.push_back( inc );*/

	/*trigger = new CharHpTrigger( m_heroUnit, 1, 100, true );
	Incident* inc2 = new Incident( trigger, action );
	m_incidents.push_back( inc2 );*/

	m_curDialog = 0;

	GetG().m_camera.setAttachPos( &getHeroPos() );
	GetG().m_camera.begin( CAMERA_ATTACH );
	return hr;
}


HRESULT World::frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
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
			(*it)->frameRender();
	}
	
	DialogList::iterator itDialog = m_scriptedDialog.begin();
	for ( ; itDialog != m_scriptedDialog.end(); ++itDialog )
	{
		(*itDialog)->frameRender(pd3dDevice, fTime, fElapsedTime);
	}

	WorldStateManager& wsm = WorldStateManager::getSingleton();
	wsm.getCurState()->frameRender(pd3dDevice, fTime, fElapsedTime);

	return S_OK;

}

HRESULT World::frameMove(double fTime, float fElapsedTime)
{
	EpCamera& camera = GetG().m_camera;

	HRESULT hr;

	UNREFERENCED_PARAMETER( hr );
	
	m_pic.frameMove(fElapsedTime);
	m_avatar.frameMove(fElapsedTime);
	//camera.frameMove(fElapsedTime);

	if ( m_sound )
		m_sound->UpdateAudio();
	
	DialogList::iterator itDialog = m_scriptedDialog.begin();
	for ( ; itDialog != m_scriptedDialog.end(); ++itDialog )
	{
		(*itDialog)->frameMove( fTime, fElapsedTime );
	}
	
	//GetG().m_camera.frameMove( fElapsedTime );
	
	m_sampleTeapotMeshRot += fElapsedTime * D3DXToRadian(35); // 35 degrees per second

	GetWorldStateManager().transit();
	GetWorldStateManager().getCurState()->frameMove(fTime, fElapsedTime);

	m_modelSg->getSceneRoot()->update(fTime, fElapsedTime);
	

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

	battleEventCheck();

	// Incidents update
	IncidentList::iterator itInc = m_incidents.begin();
	for ( ; itInc != m_incidents.end(); ++itInc )
	{
		(*itInc)->update( fTime, fElapsedTime );
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

	if (m_modelArnFile)
		release_arnfile(*m_modelArnFile);
	SAFE_DELETE(m_modelArnFile);
	SAFE_DELETE(m_modelSg);

	SAFE_RELEASE(m_aTile);

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
		if (wParam == VK_F4)
		{

		}
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
				rCamera.setSmoothCameraDuration( 3.0f );
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
	// TODO controllable means it is hero?
	if ( u->getType() == UT_HERO )
	{
		/*주인공 유닛 설정*/
		m_heroUnit = dynamic_cast<Character*>( u );
		m_heroUnit->setAttack (10);
		m_heroUnit->setInt (150);

	}

	return m_unitSet.size();
}

const D3DXVECTOR3& World::getEnemyPos()
{
	if ( m_curEnemyUnit != NULL )
	{
		return m_curEnemyUnit->getPos();
	} else {
		UnitSet::iterator it = m_unitSet.begin();
		it++;
		return (*it)->getPos();
	}
}

const D3DXVECTOR3& World::getHeroPos()
{
	assert( m_heroUnit );
	return m_heroUnit->getPos();
}

bool World::isInFightArea( Character* heroPt, Character* enemyPt )
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

void World::handleCollision( Unit* heroUnit, Unit* opponentUnit )
{
	// 상대방이 대화 불가능하면 적으로 간주한다.
	if ( !dynamic_cast<Enemy*>(opponentUnit)->isTalkable() )
	{
		m_curEnemyUnit = dynamic_cast<Character*>(opponentUnit);
	}

	// 전투는 타일 진입시 일어나는 것으로 한다.
	// 상대방이 이야기 가능한 상대가 아니면 적으로 간주한다.
	//if ( !dynamic_cast<Character*>(opponentUnit)->isTalkable() )
	//{

	//	m_curEnemyUnit = dynamic_cast<Character*>( opponentUnit );
	//	m_curEnemyUnit->setAttack (30);

	//	if ( GetWorldStateManager().curStateEnum() == GAME_WORLD_STATE_FIELD )
	//		GetWorldStateManager().setNextState( GAME_WORLD_STATE_BATTLE );
	//}
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
				if ( isInFightArea( m_heroUnit, oppCharacter ) == true )
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
			/*m_scriptedDialog.remove( m_curDialog );
			EP_SAFE_RELEASE( m_curDialog );*/
		}
		else if ( !m_curDialog->isTalking() )
			m_curDialog = 0;
	}
}

Dialog* World::getDialogByName( const char* dialogName )
{
	DialogList::iterator it = m_scriptedDialog.begin();
	for ( ; it != m_scriptedDialog.end(); ++it )
	{
		if ( (*it)->getDialogName() == dialogName )
			return (*it);
	}
	return 0;
}

UINT World::addIncident( Incident* inc )
{
	m_incidents.push_back( inc );
	return m_incidents.size();
}

void World::loadWorldModel()
{
	// World model init and loading
	m_modelArnFile = new ArnFileData;
	load_arnfile( m_modelFilePath.c_str(), *m_modelArnFile );
	m_modelSg = new ArnSceneGraph( *m_modelArnFile );
}

World* World::createWorld( const char* worldName, TCHAR* modelFilePath )
{
	World* ret = new World( worldName, modelFilePath );

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
	// Detect battle event.
	// If current selected unit isn't hero unit, and isn't talkable,
	// regard as enemy. And if hero is in the fight area of enemy, start battle.
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( (*it) != getHeroUnit() )
		{
			Enemy* oppCharacter = dynamic_cast<Enemy*>( *it );
			if ( oppCharacter != NULL && oppCharacter->isTalkable() == false && !oppCharacter->getRemoveFlag() )
			{
				if ( isInFightArea( getHeroUnit() , oppCharacter ) == true )
				{
					oppCharacter->clearKey(); // No more move!
					setCurEnemy( oppCharacter );

					getCurEnemyUnit()->setAttack(30);

					getHero()->setViewAt( &getCurEnemyUnit()->getPos() );
					getCurEnemyUnit()->setViewAt( &getHero()->getPos() );
					getHeroUnit()->setControllable( false );
					getHeroUnit()->clearKey();

					if ( GetWorldStateManager().curStateEnum() == GAME_WORLD_STATE_FIELD )
						GetWorldStateManager().setNextState( GAME_WORLD_STATE_BATTLE );
				}
			}
		}	
	}

}
//////////////////////////////////////////////////////////////////////////

Unit* EpGetHero()
{
	return GetWorldManager().getCurWorld()->getHero();

} SCRIPT_CALLABLE_PV( EpGetHero )

int EpRegisterIncident( void* ptr )
{
	Incident* inc = reinterpret_cast<Incident*>( ptr );
	return GetWorldManager().getCurWorld()->addIncident( inc );

} SCRIPT_CALLABLE_I_PV( EpRegisterIncident )

ArnNode* EpGetNode( const char* nodeName )
{
	return GetWorldManager().getCurWorld()->getNode( nodeName );

} SCRIPT_CALLABLE_PV_PC( EpGetNode )

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


START_SCRIPT_FACTORY( World )
	CREATE_OBJ_COMMAND( EpGetHero )
	CREATE_OBJ_COMMAND( EpRegisterIncident )
	CREATE_OBJ_COMMAND( EpGetNode )
	CREATE_OBJ_COMMAND( EpSetDoAnim )
	CREATE_OBJ_COMMAND( EpSetAnimTime )
END_SCRIPT_FACTORY( World )
