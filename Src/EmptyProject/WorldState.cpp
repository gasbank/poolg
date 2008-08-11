#include "EmptyProjectPCH.h"
#include "WorldState.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "ScriptManager.h"
#include "TileManager.h"


TileManager	tileManager;


WorldState::WorldState(void)
{
	m_sampleTeapotMeshRot	= 0;
	m_aTile					= 0;
	m_afd					= 0;
	m_sg					= 0;
	m_heroUnit				= 0;
	m_curEnemyUnit			= 0;

	char command[128];
	StringCchPrintfA(command, 128, "EpWorldState::init 0x%p", this);
	GetScriptManager().execute(command);
}

WorldState::~WorldState(void)
{
	detachAllUnits();
	DialogList::iterator it = m_scriptedDialog.begin();
	for ( ; it != m_scriptedDialog.end(); ++it )
	{
		EP_SAFE_RELEASE( *it );
	}
}

HRESULT WorldState::enter()
{
	HRESULT hr = S_OK;

	LPDIRECT3DDEVICE9& pd3dDevice =  GetG().m_dev;

	// Aran file init
	m_afd = new ArnFileData;
	load_arnfile(_T("gus2.arn"), *m_afd);
	m_sg = new ArnSceneGraph(*m_afd);
	
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

	m_sound.init();


	tileManager.tile[14][14].movable = false;
	tileManager.tile[9][9].talkable = true;
	tileManager.tile[9][9].talkonetime = false;
	tileManager.tile[17][14].heal = true;


	// Create sample 3D model(!)
	/*LPD3DXMESH teapot;
	D3DXCreateTeapot(pd3dDevice, &teapot, 0);
	m_heroUnit = Unit::createUnit( teapot );
	m_heroUnit->setPosZ( -m_heroUnit->getUpperRight().z );
	m_heroUnit->setRotX( D3DXToRadian( -90 ) );
	m_heroUnit->setRotZ( D3DXToRadian(  90 ) );*/
	
	D3DXCreateBox(pd3dDevice, 1.0f, 1.0f, 1.0f, &m_aTile, 0);

	// [재우] 캐릭터 구현
	/*
	LPD3DXMESH d3dxMesh;
	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
	m_player.createUnit (d3dxMesh, 20, 20, 0, true);
	addUnit(&m_player);*/

	// Get position of hero.
	D3DXVECTOR3 heroPos;
	if ( m_heroUnit != 0 )
		heroPos = m_heroUnit->getPos();
	else
		heroPos.x = 0.0f; heroPos.y = 0.0f; heroPos.z = -30.0f;

	// Setup main camera.
	D3DXVECTOR3 vecEye( heroPos.x, heroPos.y, heroPos.z - 30.0f );
	D3DXVECTOR3 vecAt ( 0.0f, 0.0f, 0.0f );
	GetG().m_camera.SetViewParams( &vecEye, &vecAt );

	setupLight();

	GetScriptManager().execute("EpWorldState::enter");

	
	// Load dialogs from script
	std::list<const char*> dialogList;
	GetScriptManager().readCharPtrList( "EpDialogList", dialogList );
	std::list<const char*>::iterator itDialogList = dialogList.begin();
	for ( ; itDialogList != dialogList.end(); ++itDialogList )
	{
		m_scriptedDialog.push_back( Dialog::createDialogByScript( *itDialogList ) );
		(*m_scriptedDialog.rbegin())->init();
	}
	return hr;
}

HRESULT WorldState::leave()
{


	GetScriptManager().execute("EpWorldState::leave");

	return S_OK;
}

HRESULT WorldState::frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{		
	EpCamera& camera = GetG().m_camera;
	D3DLIGHT9& light = GetG().m_light;

	//////////////////////////////////////////////////////////////////////////
	// Perspective Rendering Phase

	pd3dDevice->SetLight(0, &light);

	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	pd3dDevice->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

	
	// Sample 3D model rendering
	//m_heroUnit->draw();

	//[재우] 캐릭터

	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		(*it)->frameRender();
	}

	// Draw floor gray tile (2D)
	//g_pic.draw();
	

	D3DXMATRIX transform;
	D3DXMatrixIdentity(&transform);
	pd3dDevice->SetTransform(D3DTS_WORLD, &transform);

	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	pd3dDevice->SetFVF(ArnVertex::FVF);
	GetG().m_videoMan.renderMeshesOnly(m_sg->getSceneRoot());
	m_sg->getSceneRoot()->update(fTime, fElapsedTime);

	WorldStateManager& wsm = WorldStateManager::getSingleton();
	wsm.getCurState()->frameRender(pd3dDevice, fTime, fElapsedTime);
	
	
	DialogList::iterator itDialog = m_scriptedDialog.begin();
	for ( ; itDialog != m_scriptedDialog.end(); ++itDialog )
	{
		(*itDialog)->frameRender(pd3dDevice, fTime, fElapsedTime);
	}
	

	return S_OK;
}

HRESULT WorldState::frameMove(double fTime, float fElapsedTime)
{
	EpCamera& camera = GetG().m_camera;
	D3DLIGHT9& light = GetG().m_light;

	double fStateTime = getStateTime(fTime);

	fStateTime -= 1.5f;

	// Fade in at starting.
	if ( 0.0f < fStateTime && fStateTime < 2.0f )
	{
		D3DCOLORVALUE cv = { (float)fStateTime / 2.0f * 0.7f, (float)fStateTime / 2.0f * 0.7f, (float)fStateTime / 2.0f * 0.7f, 1.0f };
		light.Ambient = cv;
		light.Diffuse = cv;
		//light.Specular = cv;
	}
	
	m_pic.frameMove(fElapsedTime);
	m_avatar.frameMove(fElapsedTime);
	camera.FrameMove(fElapsedTime);
	m_sound.UpdateAudio();
	
	DialogList::iterator itDialog = m_scriptedDialog.begin();
	for ( ; itDialog != m_scriptedDialog.end(); ++itDialog )
	{
		(*itDialog)->frameMove( fTime, fElapsedTime );
	}
	

	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		(*it)->frameMove(fElapsedTime);
	}

	
	m_sampleTeapotMeshRot += fElapsedTime * D3DXToRadian(35); // 35 degrees per second

	GetWorldStateManager().transit();
	GetWorldStateManager().getCurState()->frameMove(fTime, fElapsedTime);

	// Detect collision always.
	it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( (*it) != m_heroUnit )
			if ( isCollide( &m_heroUnit->getPos(), &(*it)->getPos() ) == true )
			{
				handleCollision( m_heroUnit, (*it) );
			}
	}
	
	return S_OK;
}

HRESULT WorldState::release()
{
	// DO NOT RELEASE THIS POINTER: m_heroUnit

	m_pic.release();
	m_picRhw.release();
	m_picSmiley.release();
	m_avatar.release();
	m_sound.release();
	//m_dialog.release();
	DialogList::iterator it = m_scriptedDialog.begin();
	for ( ; it != m_scriptedDialog.end(); ++it )
	{
		(*it)->release();
	}
	
	if (m_afd)
		release_arnfile(*m_afd);
	SAFE_DELETE(m_afd);
	SAFE_DELETE(m_sg);
	SAFE_RELEASE(m_aTile);

	return S_OK;
}

HRESULT WorldState::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_pic.handleMessages(hWnd, uMsg, wParam, lParam);
	m_sound.handleMessages(hWnd, uMsg, wParam, lParam);

	bool bTalking = false;
	
	DialogList::iterator itDialog = m_scriptedDialog.begin();
	for ( ; itDialog != m_scriptedDialog.end(); ++itDialog )
	{
		(*itDialog)->handleMessages( hWnd, uMsg, wParam, lParam );
		bTalking |= (*itDialog)->startTalk;
	}
	
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( (*it)->getControllable() && GetWorldStateManager().curStateEnum() == GAME_WORLD_STATE_FIELD )
		{
			if ( !bTalking )
			{
				(*it)->handleMessages(hWnd, uMsg, wParam, lParam);
			}
		}
	}

	if (uMsg == WM_KEYDOWN)
	{

		if (wParam == VK_F4)
		{

		}
		if (wParam == VK_F5)
		{
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
			DialogList::iterator it = m_scriptedDialog.begin();
			for ( ; it != m_scriptedDialog.end(); ++it )
			{
				Dialog* dialog = (*it);
				if ( (m_heroUnit->getTilePosX() == dialog->getRegion().left)
					&& (m_heroUnit->getTilePosY() == dialog->getRegion().top) )
				{
					if ( dialog->isOneTime() && !dialog->endTalk )
					{
						dialog->startTalk = true;
						if ( !dialog->dlg_ON )
							dialog->Toggle(&dialog->dlg_ON);
					}
					if ( !dialog->isOneTime() && !dialog->endTalk )
					{
						dialog->startTalk = true;
						if ( !dialog->dlg_ON )
							dialog->Toggle( &dialog->dlg_ON );
					}
					if ( dialog->endTalk && !dialog->isOneTime() )
						dialog->Toggle( &dialog->endTalk );
				}
			}
		}
	}

	if ( GetWorldStateManager().getCurState() )
		GetWorldStateManager().getCurState()->handleMessages(hWnd, uMsg, wParam, lParam);

	return S_OK;
}

void WorldState::setupLight() 
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

UINT WorldState::addUnit( Unit* u )
{
	m_unitSet.insert(u);
	// TODO controllable means it is hero?
	if (u->getControllable())
	{
		m_heroUnit = dynamic_cast<Character*>( u );
		m_heroUnit->setAttack (4);
	}

	return m_unitSet.size();
}

void WorldState::detachAllUnits()
{
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		EP_SAFE_RELEASE( *it );
	}
	m_unitSet.clear();
}
const D3DXVECTOR3& WorldState::getEnemyPos()
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

const D3DXVECTOR3& WorldState::getHeroPos()
{
	return m_heroUnit->getPos();
}

bool WorldState::isCollide( const D3DXVECTOR3* vec0, const D3DXVECTOR3* vec1 )
{
	const float collideRange = 10.0f;
	float range = sqrt( (vec0->x - vec1->x) * (vec0->x - vec1->x) + (vec0->y - vec1->y) * (vec0->y - vec1->y) );
	if ( range <= collideRange )
		if ( range <= collideRange )
				return true;

	return false;
}

void WorldState::handleCollision( Unit* heroUnit, Unit* enemyUnit )
{
	m_curEnemyUnit = dynamic_cast<Character*>( enemyUnit );
	/*데미지를 처리*/
	m_curEnemyUnit->setAttack (20);
	
	if ( !m_curEnemyUnit->isTalkable() )
	{
		if ( GetWorldStateManager().curStateEnum() == GAME_WORLD_STATE_FIELD )
			GetWorldStateManager().setNextState( GAME_WORLD_STATE_BATTLE );

		GetAudioState().bBGMFade = true;
		GetAudioState().bMusicFade = false;
		GetAudioState().pEngine->Stop( GetAudioState().iMusicCategory, 0 );
		GetAudioState().pSoundBank->Play( GetAudioState().iBattle, 0, 0, NULL );
	}
}

void WorldState::startDialog( int index )
{
	DialogList::iterator it = m_scriptedDialog.begin();
	while ( index )
	{
		++it;
		--index;
	}

	Dialog* dialog = (*it);
	if ( dialog->isOneTime() && !dialog->endTalk )
	{
		dialog->startTalk = true;
		if ( !dialog->dlg_ON )
			dialog->Toggle(&dialog->dlg_ON);
	}
	if ( !dialog->isOneTime() && !dialog->endTalk )
	{
		dialog->startTalk = true;
		if ( !dialog->dlg_ON )
			dialog->Toggle( &dialog->dlg_ON );
	}
	if ( dialog->endTalk && !dialog->isOneTime() )
		dialog->Toggle( &dialog->endTalk );
}

// 유닛의 포인터를 받아서, UnitSet에서 해당 유닛을 찾아 지운다.
void WorldState::removeUnit( Unit* pUnit )
{
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); it++ )
	{
		if ( *it == pUnit )
		{
			pUnit->release();
			m_unitSet.erase( it );
			break;
		}
	}
}
