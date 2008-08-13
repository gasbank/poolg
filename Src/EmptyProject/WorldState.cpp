#include "EmptyProjectPCH.h"
#include "WorldState.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "ArnMesh.h"

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
	// Room model
	m_afd = new ArnFileData;
	load_arnfile(_T("gus2.arn"), *m_afd);
	m_sg = new ArnSceneGraph(*m_afd);
	// Rat model
	m_afdRat = new ArnFileData;
	load_arnfile(_T("rat.arn"), *m_afdRat);
	m_sgRat = new ArnSceneGraph(*m_afdRat);
	ArnMesh* mainWallMesh = dynamic_cast<ArnMesh*>( m_sg->getSceneRoot()->getNodeByName("MainWall") );
	
	
	//////////////////////////////////////////////////////////////////////////
	// Room Model MainWall intersection test
	D3DXVECTOR3 rayStartPos( 0, 0, -2.0f );
	D3DXVECTOR3 rayDir( 1, 0, 0 );
	BOOL hit;
	DWORD hitFaceIndex;
	float hitU, hitV;
	float hitDist;
	LPD3DXBUFFER allHitsBuffer = 0;
	DWORD allHitSCount;
	V_RETURN( D3DXIntersect( mainWallMesh->getD3DXMesh(), &rayStartPos, &rayDir,
		&hit, &hitFaceIndex, &hitU, &hitV, &hitDist, &allHitsBuffer, &allHitSCount ) );
	
	D3DXINTERSECTINFO* intersectInfo = static_cast<D3DXINTERSECTINFO*>( allHitsBuffer->GetBufferPointer() );
	UNREFERENCED_PARAMETER( intersectInfo );
	printf("Ray Testing test. (FaceIndex : %ui, Dist : %f)\n", intersectInfo->FaceIndex, intersectInfo->Dist );
	SAFE_RELEASE( allHitsBuffer );
	//////////////////////////////////////////////////////////////////////////
	
	
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


	tileManager.getTile( 14, 14 )->movable = false;
	tileManager.getTile( 9, 9 )->talkable = true;
	tileManager.getTile( 17, 14 )->heal = true;


	// Create sample 3D model(!)
	/*LPD3DXMESH teapot;
	D3DXCreateTeapot(pd3dDevice, &teapot, 0);
	m_heroUnit = Unit::createUnit( teapot );
	m_heroUnit->setPosZ( -m_heroUnit->getUpperRight().z );
	m_heroUnit->setRotX( D3DXToRadian( -90 ) );
	m_heroUnit->setRotZ( D3DXToRadian(  90 ) );*/
	
	D3DXCreateBox(pd3dDevice, 1.0f, 1.0f, 1.0f, &m_aTile, 0);



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
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	pd3dDevice->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());


	//////////////////////////////////////////////////////////////////////////
	// Aran lib rendering routine (CW)
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	pd3dDevice->SetFVF(ArnVertex::FVF);
	D3DXMATRIX transform;
	D3DXMatrixTranslation( &transform, m_heroUnit->getPos().x, m_heroUnit->getPos().y, m_heroUnit->getPos().z );
	GetG().m_videoMan.renderMeshesOnly(m_sgRat->getSceneRoot(), transform);
	GetG().m_videoMan.renderMeshesOnly(m_sg->getSceneRoot());
	


	//////////////////////////////////////////////////////////////////////////
	// EP rendering routine (CCW)

	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); ++it )
	{
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

HRESULT WorldState::frameMove(double fTime, float fElapsedTime)
{
	EpCamera& camera = GetG().m_camera;
	D3DLIGHT9& light = GetG().m_light;

	// Fade in at starting.
	static float fadeTime = 0.0f;

	if ( fadeTime < 2.0f )
		fadeTime += fElapsedTime;
	else
		fadeTime = 2.0f;

	D3DCOLORVALUE cv = { 0.8f * fadeTime / 2.0f, 0.8f * fadeTime / 2.0f, 0.8f * fadeTime / 2.0f, 1.0f };
	light.Ambient = cv;
	light.Diffuse = cv;
	
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

	m_sg->getSceneRoot()->update(fTime, fElapsedTime);
	m_sgRat->getSceneRoot()->update(fTime, fElapsedTime);
	
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

	if (m_afdRat)
		release_arnfile(*m_afdRat);
	SAFE_DELETE(m_afdRat);
	SAFE_DELETE(m_sgRat);


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
		bTalking |= (*itDialog)->isTalking();
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
			else
				getHero()->clearKey();
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
			if( !getHeroUnit()->getMoving() )
			{
				DialogList::iterator it = m_scriptedDialog.begin();
				for ( ; it != m_scriptedDialog.end(); )
				{
					Dialog* dialog = (*it);
					
					if ( (m_heroUnit->getTilePosX() == dialog->getRegion().left)
						&& (m_heroUnit->getTilePosY() == dialog->getRegion().top) )
					{
						if ( dialog->nextDialog() == false )
						{
							EP_SAFE_RELEASE( *it );
							it = m_scriptedDialog.erase( it );
						}
						else
						{
							++it;
						}
					}
					else
					{
						++it;
					}
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
		/*주인공 유닛 설정*/
		m_heroUnit = dynamic_cast<Character*>( u );
		m_heroUnit->setAttack (1);
		m_heroUnit->setInt (20);
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
	assert( m_heroUnit );
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

void WorldState::startDialog( int index )
{
	DialogList::iterator it = m_scriptedDialog.begin();
	while ( index )
	{
		++it;
		--index;
	}

	Dialog* dialog = (*it);
	if ( dialog->nextDialog() == false )
	{
		EP_SAFE_RELEASE( *it );
		it = m_scriptedDialog.erase( it );
	}
}

void WorldState::handleCollision( Unit* heroUnit, Unit* opponentUnit )
{
	if ( !dynamic_cast<Character*>(opponentUnit)->isTalkable() )
	{

		m_curEnemyUnit = dynamic_cast<Character*>( opponentUnit );
		/*데미지를 처리*/
		m_curEnemyUnit->setAttack (30);

		if ( GetWorldStateManager().curStateEnum() == GAME_WORLD_STATE_FIELD )
			GetWorldStateManager().setNextState( GAME_WORLD_STATE_BATTLE );
	}
}

// 유닛의 포인터를 받아서, UnitSet에서 해당 유닛을 찾아 지운다.
void WorldState::removeUnit( Unit* pUnit )
{
	UnitSet::iterator it = m_unitSet.begin();
	for ( ; it != m_unitSet.end(); it++ )
	{
		if ( *it == pUnit )
		{
			EP_SAFE_RELEASE( pUnit );
			m_unitSet.erase( it );
			break;
		}
	}
}
