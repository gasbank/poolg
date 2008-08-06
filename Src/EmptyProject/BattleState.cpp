#include "EmptyProjectPCH.h"
#include "BattleState.h"
#include "WorldStateManager.h"
#include "TopStateManager.h"
#include "WorldState.h"

extern D3DXMATRIX g_orthoProjMat;
extern D3DXMATRIX g_fixedViewMat;

BattleState::BattleState() : 
m_vecAtFrom(0.0f, 0.0f, 0.0f),
m_vecAtTo(0.0f, 0.0f, 0.0f),
m_vecEyeFrom(0.0f, 0.0f, 0.0f),
m_vecEyeTo(0.0f, 0.0f, 0.0f)
{
	m_pDev = GetG().m_dev;

	float statusBoxWidth = 163;
	float statusBoxHeight = 124;

	int scrWidth = GetG().m_scrWidth;
	int scrHeight = GetG().m_scrHeight;

	float statusBoxPlayersPositionX = -(float)scrWidth/2 + 10;
	float statusBoxPlayersPositionY = -(float)scrHeight/2 + 10;
	m_StatusBoxPlayer.init(L"BattleUI\\StatusBox.png", m_pDev);
	m_StatusBoxPlayer.setPosition (statusBoxPlayersPositionX, statusBoxPlayersPositionY, 7);
	m_StatusBoxPlayer.setSize(statusBoxWidth, statusBoxHeight);

	float statusBoxEnemysPositionX = (float)scrWidth/2 - statusBoxWidth - 10;
	float statusBoxEnemysPositionY = (float)scrHeight/2 - statusBoxHeight - 10;
	m_StatusBoxEnemy.init(L"BattleUI\\StatusBox.png", m_pDev);
	m_StatusBoxEnemy.setPosition (statusBoxEnemysPositionX, statusBoxEnemysPositionY, 6.9f);
	m_StatusBoxEnemy.setSize(statusBoxWidth, statusBoxHeight);

	int skillBoxHeight = 200;
	int skillBoxWidth = skillBoxHeight * 593 / 933;
	int skillBoxPositionX = scrWidth/2 -skillBoxWidth - 3;
	int skillBoxPositionY =  -scrHeight/2 + 3;
	m_SkillBox.init(L"BattleUI\\SkillBox.png", m_pDev);
	m_SkillBox.setPosition ((float)skillBoxPositionX, (float)skillBoxPositionY, 6.9f);
	m_SkillBox.setSize((float)skillBoxWidth, (float)skillBoxHeight);

	int skillContentBoxHeight = 200;
	int skillContentBoxWidth = skillContentBoxHeight * 390 / 269;
	m_SkillContentBox.init(L"BattleUI\\SkillContentBox.png", m_pDev);
	m_SkillContentBox.setPosition ((float)skillBoxPositionX - skillContentBoxWidth, (float)skillBoxPositionY, 6.9f);
	m_SkillContentBox.setSize((float)skillContentBoxWidth, (float)skillContentBoxHeight);

	float dialogBoxWidth = (float)(int)(scrWidth - statusBoxWidth - 30);
	float dialogBoxHeight = 124;
	m_DialogBox.init(L"BattleUI\\DialogBox.png", m_pDev);
	m_DialogBox.setPosition ((float)-(scrWidth)/2 + 10, (float)(scrHeight)/2 - dialogBoxHeight - 10, 6.9f);
	//m_DialogBox.setPosition (10, 10, 6.9f);
	m_DialogBox.setSize(dialogBoxWidth, dialogBoxHeight);

	float statusBarWidth = statusBoxWidth * 0.67f;
	float statusBarHeight = statusBarWidth * 0.1f;
	m_hpBgPlayer.init(L"BattleUI\\HPbg.jpg", m_pDev);
	m_hpBgPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 5);
	m_hpBgPlayer.setSize(statusBarWidth, statusBarHeight);
	m_mpBgPlayer.init(L"BattleUI\\MPbg.jpg", m_pDev);
	m_mpBgPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.65f, 5);
	m_mpBgPlayer.setSize(statusBarWidth, statusBarHeight);
	m_expBgPlayer.init(L"BattleUI\\EXPbg.jpg", m_pDev);
	m_expBgPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.48f, 5);
	m_expBgPlayer.setSize(statusBarWidth, statusBarHeight);

	m_hpBgEnemy.init(L"BattleUI\\HPbg.jpg", m_pDev);
	m_hpBgEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.82f, 5);
	m_hpBgEnemy.setSize(statusBarWidth, statusBarHeight);
	m_mpBgEnemy.init(L"BattleUI\\MPbg.jpg", m_pDev);
	m_mpBgEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.65f, 5);
	m_mpBgEnemy.setSize(statusBarWidth, statusBarHeight);

	m_hpBarPlayer.init(L"BattleUI\\HPbar.jpg", m_pDev);
	m_hpBarPlayer.initRate();
	m_hpBarPlayer.setSize(statusBarWidth, statusBarHeight);
	m_hpBarPlayer.changeRate(-20);
	m_hpBarPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 4.5f);

	m_mpBarPlayer.init(L"BattleUI\\MPbar.jpg", m_pDev);
	m_mpBarPlayer.initRate();
	m_mpBarPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.65f, 4.5f);
	m_mpBarPlayer.setSize(statusBarWidth, statusBarHeight);

	m_expBarPlayer.init(L"BattleUI\\EXPbar.jpg", m_pDev);
	m_expBarPlayer.initRate();
	m_expBarPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.48f, 4.5f);
	m_expBarPlayer.setSize(statusBarWidth, statusBarHeight);
	m_expBarPlayer.changeRate(-73.3f);

	m_hpBarEnemy.init(L"BattleUI\\HPbar.jpg", m_pDev);
	m_hpBarEnemy.initRate();
	m_hpBarEnemy.changeRate(-10);
	m_hpBarEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.82f, 4.5f);
	m_hpBarEnemy.setSize(statusBarWidth, statusBarHeight);

	m_mpBarEnemy.init(L"BattleUI\\MPbar.jpg", m_pDev);
	m_mpBarEnemy.initRate();
	m_mpBarEnemy.changeRate(-3);
	m_mpBarEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.65f, 4.5f);
	m_mpBarEnemy.setSize(statusBarWidth, statusBarHeight);

	D3DXCreateFont(m_pDev, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblHYnamL);
	D3DXCreateFont(m_pDev, 18, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Rockwell Extra Bold"), &m_lblREB);
}

BattleState::~BattleState()
{

}

HRESULT BattleState::enter()
{

	return S_OK;
}

HRESULT BattleState::leave()
{
	m_startTime = -1.0f;

	return S_OK;
}

HRESULT BattleState::frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, &g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &g_orthoProjMat);

	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DMATERIAL9 material;
	D3DCOLORVALUE cv, cv2;
	cv.a = 0.3f; cv.r = 0.1f; cv.g = 0.1f; cv.b = 0.1f;
	cv2.a = 0.3f; cv2.r = 1.0f; cv2.g = 0.1f; cv2.b = 0.3f;
	material.Ambient = cv;
	material.Diffuse = cv2;
	material.Emissive = cv;
	material.Power = 1.0f;
	material.Specular = cv2;
	m_pDev->SetMaterial(&material);

	m_StatusBoxPlayer.draw();
	m_StatusBoxEnemy.draw();
	m_SkillBox.draw();
	m_SkillContentBox.draw();
	m_DialogBox.draw();
	m_hpBgPlayer.draw();
	m_mpBgPlayer.draw();
	m_expBgPlayer.draw();
	m_hpBgEnemy.draw();
	m_mpBgEnemy.draw();
	m_hpBarPlayer.draw();
	m_mpBarPlayer.draw();
	m_expBarPlayer.draw();
	m_hpBarEnemy.draw();
	m_mpBarEnemy.draw();

	drawFixedText(GetG().m_scrWidth, GetG().m_scrHeight);
	
//	D3DXMATRIX mRot, mScale, mTrans, mWorld;
//	D3DXMatrixRotationZ(&mRot, D3DXToRadian(45));
//	D3DXMatrixScaling(&mScale, 280.0f, 220.0f, 1.0f);
//	D3DXMatrixTranslation(&mTrans, 100.0f, -100.0f, 0);
//	mWorld = mScale * mRot * mTrans;
//	m_StatusBoxPlayer.setLocalXform(&mWorld);
//	
	
	//	
	return S_OK;
}

HRESULT BattleState::frameMove(double fTime, float fElapsedTime)
{
	double fStateTime = getStateTime(fTime);

	m_StatusBoxPlayer.frameMove(fElapsedTime);
	m_StatusBoxEnemy.frameMove(fElapsedTime);
	m_SkillBox.frameMove(fElapsedTime);
	m_SkillContentBox.frameMove(fElapsedTime);
	m_DialogBox.frameMove(fElapsedTime);
	m_hpBgPlayer.frameMove(fElapsedTime);
	m_mpBgPlayer.frameMove(fElapsedTime);
	m_expBgPlayer.frameMove(fElapsedTime);
	m_hpBgEnemy.frameMove(fElapsedTime);
	m_mpBgEnemy.frameMove(fElapsedTime);

	m_hpBarPlayer.frameMove(fElapsedTime);
	m_mpBarPlayer.frameMove(fElapsedTime);
	m_expBarPlayer.frameMove(fElapsedTime);

	m_hpBarEnemy.frameMove(fElapsedTime);
	m_mpBarEnemy.frameMove(fElapsedTime);

	//m_Player.frameMove(fElapsedTime);
	//m_Enemy.frameMove(fElapsedTime);


	if (fStateTime < 1.0f)
	{
		//WorldState* worldState = (WorldState*) TopStateManager::getSingleton().getCurState();
		//const D3DXVECTOR3& battlePos = worldState->getBattlePos();		
		TopStateManager& tsm = TopStateManager::getSingleton();
		WorldState* ws = static_cast<WorldState*>( tsm.getCurState() );
		const D3DXVECTOR3& battlePos = ws->getBattlePos();

		EpCamera& refCamera = G::getSingleton().m_camera;
		

		D3DXVECTOR3 vecAt( battlePos );
		D3DXVECTOR3 vecEye( -10.0, -10.0f, -10.0f );

		refCamera.SetViewParams( &vecEye, &vecAt );
	}

	return S_OK;
}

HRESULT BattleState::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	m_hpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_mpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_expBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_mpBarEnemy.handleMessages(hWnd, uMsg, wParam, lParam);

	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_F4)
		{
			WorldStateManager::getSingleton().setNextState(GAME_WORLD_STATE_FIELD);
		}
	}

	return S_OK;
}

HRESULT BattleState::release ()
{
	m_StatusBoxPlayer.release();
	m_StatusBoxEnemy.release();
	m_SkillBox.release();
	m_SkillContentBox.release();
	m_DialogBox.release();
	m_hpBgPlayer.release();
	m_mpBgPlayer.release();
	m_expBgPlayer.release();
	m_hpBgEnemy.release();
	m_mpBgEnemy.release();

	m_hpBarPlayer.release();
	m_mpBarPlayer.release();
	m_expBarPlayer.release();

	m_hpBarEnemy.release();
	m_mpBarEnemy.release();
	
	
	SAFE_RELEASE( m_lblHYnamL );
	SAFE_RELEASE( m_lblREB);

	return S_OK;
}

void BattleState::drawFixedText(int scrWidth, int scrHeight)
{
	WCHAR textBuffer[512];
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = scrWidth;
	rc.bottom = scrHeight;
	StringCchPrintf(textBuffer, 512, L"TextTest!!!!");
	m_lblHYnamL->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(textBuffer, 512, L"HYnamL 글씨체의 묘미를 맘껏 느껴봅시다. W/A/S/D로 주전자 이동 가능!!!");
	m_lblHYnamL->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );


	float statusBoxPlayersPositionX = 10;
	float statusBoxPlayersPositionY = (float)(scrHeight - 10);
	rc.top = (LONG)(statusBoxPlayersPositionY - 102);
	rc.left = (LONG)(statusBoxPlayersPositionX + 5);
	StringCchPrintf(textBuffer, 512, L"HP");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

/*
	
	StringCchPrintf(debugBuffer, 512, L"カメラの位置: (%.2f, %.2f, %.2f)", g_camera.GetEyePt()->x, g_camera.GetEyePt()->y, g_camera.GetEyePt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"カメラの眺める場所: (%.2f, %.2f, %.2f)", g_camera.GetLookAtPt()->x, g_camera.GetLookAtPt()->y, g_camera.GetLookAtPt()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"タイルの位置: (%.2f, %.2f, %.2f)", g_pic.getPos()->x, g_pic.getPos()->y, g_pic.getPos()->z);
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - 상하좌우 화살표키를 이용해 카메라를 좌우/가까이멀리 이동 가능");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L"   (카메라는 항상 +Z 축을 바라보게 됨)");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - I/K/J/L키를 이용해 맵 이미지를 상하좌우로 이동 가능");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += 12;
	StringCchPrintf(debugBuffer, 512, L" - 스페이스키를 이용해 특수기능 작동 (누르고 있으면 필살공격)");
	g_pFont->DrawTextW(0, debugBuffer, -1, &rc, DT_NOCLIP | DT_RIGHT, D3DXCOLOR( 0.0f, 1.0f, 1.0f, 1.0f ) );
}

*/
}
