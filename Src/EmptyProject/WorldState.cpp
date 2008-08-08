#include "EmptyProjectPCH.h"
#include "WorldState.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "ScriptManager.h"
#include "Dialog.h"

WorldStateManager worldStateManager;

WorldState::WorldState(void)
{
	m_sampleTeapotMeshRot	= 0;
	m_aTile					= 0;
	m_pVertexShader			= 0;
	m_pConstantTable		= 0;
	m_pVertexDeclaration	= 0;
	m_startTime				= -1.0f;
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
}

HRESULT WorldState::enter()
{
	HRESULT hr;

	LPDIRECT3DDEVICE9& pd3dDevice =  GetG().m_dev;

	// Aran file init
	m_afd = new ArnFileData;
	load_arnfile(_T("gus2.arn"), *m_afd);
	m_sg = new ArnSceneGraph(*m_afd);
	
	// Load sample image (vertex and index buffer creation with texture)
	const UINT mapSegments = 32;
	const UINT mapSize = 32;
	m_pic.init(L"graytile.tga", pd3dDevice, mapSegments);
	m_pic.setSize((float)mapSize, (float)mapSize);
	m_pic.setPos(D3DXVECTOR3(m_pic.getPos()->x - mapSize/2, m_pic.getPos()->y - mapSize/2, m_pic.getPos()->z));
	
	m_picRhw.initRhw(L"smiley.png", pd3dDevice);
	m_picSmiley.init(L"smiley.png", pd3dDevice);
	//g_picSmiley.setSizeToTexture();

	m_avatar.init(L"smiley.png", pd3dDevice);
	m_avatar.setSize(1, 1);

	m_sound.init();
	m_dialog.init();

	abc.a.bottom = -5;
	abc.a.left = -5;
	abc.a.right = -3;
	abc.a.top = -3;
	abc.onetime = false;
	abc.endless = true;

	// Create vertex shader
	WCHAR strPath[512];
	LPD3DXBUFFER pCode;

	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	V_RETURN( pd3dDevice->CreateVertexDeclaration( decl, &m_pVertexDeclaration ) );

	V_RETURN(StringCchCopy( strPath, sizeof(strPath)/sizeof(TCHAR), L"HLSLwithoutEffects.vsh" ));

	DWORD dwShaderFlags = D3DXSHADER_DEBUG | D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
	V_RETURN( D3DXCompileShaderFromFile( strPath, NULL, NULL, "Ripple",
		"vs_2_0", dwShaderFlags, &pCode,
		NULL, &m_pConstantTable ) );

	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader( ( DWORD* )pCode->GetBufferPointer(),
		&m_pVertexShader );
	pCode->Release();
	
	if( FAILED( hr ) )
		return DXTRACE_ERR( TEXT( "CreateVertexShader" ), hr );

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

	// Setup main camera
	D3DXVECTOR3 vecEye( 0.0f, 0.0f, -30.0f );
	D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
	GetG().m_camera.SetViewParams( &vecEye, &vecAt );

	setupLight();

	WorldStateManager::getSingleton().init();

	GetScriptManager().execute("EpWorldState::enter");

	return S_OK;
}

HRESULT WorldState::leave()
{
	m_startTime = -1.0f;

	GetScriptManager().execute("EpWorldState::leave");

	return S_OK;
}

HRESULT WorldState::frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{		
	EpCamera& camera = GetG().m_camera;

	//////////////////////////////////////////////////////////////////////////
	// Perspective Rendering Phase

	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	pd3dDevice->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

	// Draw picture map with shader settings
	D3DXMATRIXA16 mWorldViewProj = *m_pic.getLocalXform() * *camera.GetViewMatrix() * *camera.GetProjMatrix();
	m_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mWorldViewProj", &mWorldViewProj );
	
	// Sample 3D model rendering
	//m_heroUnit->draw();

	//[재우] 캐릭터

	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		(*it)->draw();
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
	m_dialog.frameRender(pd3dDevice, fTime, fElapsedTime);

	return S_OK;
}

HRESULT WorldState::frameMove(double fTime, float fElapsedTime)
{
	//D3DLIGHT9& light = GetG().m_light;
	EpCamera& camera = GetG().m_camera;

	//double fStateTime = getStateTime(fTime);

	/*if (0.0f < fTime && fTime < 5.0f)
	{
		D3DXVECTOR3 vecEye( 
			0.0f, 
			(-30.0f * (5.0f - fTime) + 0.0f * fTime) / 5.0f, 
			(-20.0f * (5.0f - fTime) -20.0f * fTime) / 5.0f );
		D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
		camera.SetViewParams( &vecEye, &vecAt );

		D3DCOLORVALUE cv = { fTime / 5.0f , fTime / 5.0f, fTime / 5.0f, fTime / 5.0f };
		const float lightLimit = 0.2f;
		if (cv.r > lightLimit) cv.r = lightLimit;
		if (cv.g > lightLimit) cv.g = lightLimit;
		if (cv.b > lightLimit) cv.b = lightLimit;
		light.Ambient = cv;
	}*/
	
	m_pic.frameMove(fElapsedTime);
	m_avatar.frameMove(fElapsedTime);
	camera.FrameMove(fElapsedTime);
	m_sound.UpdateAudio();
	m_dialog.frameMove(fTime, fElapsedTime);
	//m_heroUnit->frameMove(fElapsedTime);

	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		(*it)->frameMove(fElapsedTime);
	}

	// Set up the vertex shader constants
	D3DXMATRIXA16 mViewProj = *camera.GetViewMatrix() * *camera.GetProjMatrix();
	m_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mViewProj", &mViewProj );
	m_pConstantTable->SetFloat( DXUTGetD3D9Device(), "fTime", ( float )fTime );

	m_sampleTeapotMeshRot += fElapsedTime * D3DXToRadian(35); // 35 degrees per second

	WorldStateManager::getSingleton().transit();
	WorldStateManager::getSingleton().getCurState()->frameMove(fTime, fElapsedTime);

	// Detect collision always.
	it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ( (*it) != m_heroUnit )
			if ( isCollide( &m_heroUnit->getPos(), &(*it)->getPos() ) == true )
				handleCollision( m_heroUnit, (*it) );
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
	m_dialog.release();
	
	if (m_afd)
		release_arnfile(*m_afd);
	SAFE_DELETE(m_afd);
	SAFE_DELETE(m_sg);
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pConstantTable );
	SAFE_RELEASE( m_pVertexDeclaration );
	SAFE_RELEASE(m_aTile);

	WorldStateManager::getSingleton().release();

	return S_OK;
}

HRESULT WorldState::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_pic.handleMessages(hWnd, uMsg, wParam, lParam);
	m_sound.handleMessages(hWnd, uMsg, wParam, lParam);
	m_dialog.handleMessages(hWnd, uMsg, wParam, lParam);
	//m_heroUnit->handleMessages(hWnd, uMsg, wParam, lParam);

	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
		if ((*it)->getControllable())
		{
			(*it)->handleMessages(hWnd, uMsg, wParam, lParam);
		}
	}

	if (uMsg == WM_KEYDOWN)
	{

		if (wParam == VK_F4)
		{

		}
		if (wParam == VK_F5)
		{
			WorldStateManager::getSingleton().setNextState(GAME_WORLD_STATE_MENU);
		}
	}
	if (uMsg == WM_KEYUP)
	{
		if (wParam == VK_RETURN)
		{
			if ( (m_heroUnit->getPos().x > (float)abc.a.left)&&(m_heroUnit->getPos().x < (float)abc.a.right) && (m_heroUnit->getPos().y > (float)abc.a.bottom)&&(m_heroUnit->getPos().y < (float)abc.a.top) )
			{
				if ( abc.onetime )
				{
					abc.onetime = false;
					m_dialog.startTalk = true;
					if ( !m_dialog.dlg_ON )
						m_dialog.Toggle(&m_dialog.dlg_ON);
				}
				if ( !abc.onetime && !m_dialog.endTalk)
				{
					m_dialog.startTalk = true;
					if ( !m_dialog.dlg_ON )
						m_dialog.Toggle(&m_dialog.dlg_ON);
				}
				if ( m_dialog.endTalk )
					m_dialog.Toggle(&m_dialog.endTalk);
			}
		}
	}


	WorldStateManager::getSingleton().getCurState()->handleMessages(hWnd, uMsg, wParam, lParam);

	return S_OK;
}

void WorldState::setupLight() 
{
	D3DLIGHT9& light = GetG().m_light;
	LPDIRECT3DDEVICE9& pd3dDevice = GetG().m_dev;

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	D3DCOLORVALUE cv = { 0.5f, 0.5f, 0.5f, 1.0f };
	light.Ambient = cv;
	light.Diffuse = cv;
	light.Specular = cv;
	
	light.Attenuation0 = 0.5f;

	D3DXVECTOR3 dir(10.0f, -10.0f, 10.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &dir);

	D3DXVECTOR3 pos(-10.0f, 10.0f, -10.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Position, &pos);

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Range = 1000.0f;

	pd3dDevice->SetLight(0, &light);
	pd3dDevice->LightEnable(0, TRUE);
}

UINT WorldState::addUnit( Unit* u )
{
	m_unitSet.insert(u);
	// TODO controllable means it is hero?
	if (u->getControllable())
		m_heroUnit = u;

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
	if ( -5.0 <= (vec0->x - vec1->x) && (vec0->x - vec1->x) <= 5.0 )
		if ( -5.0 <= (vec0->y - vec1->y) && (vec0->y - vec1->y) <= 5.0 )
				return true;

	return false;
}

void WorldState::handleCollision( Unit* heroUnit, Unit* enemyUnit )
{
	m_curEnemyUnit = enemyUnit;
	if ( WorldStateManager::getSingleton().curStateEnum() == GAME_WORLD_STATE_FIELD )
		WorldStateManager::getSingleton().setNextState(GAME_WORLD_STATE_BATTLE);
}