#include "EmptyProjectPCH.h"
#include "BattleState.h"
#include "WorldStateManager.h"
#include "TopStateManager.h"
#include "WorldState.h"
#include "Utility.h"

BattleState::BattleState()
{
	m_pDev = GetG().m_dev;
	assert( m_pDev );
	m_ws = 0;

	/*UI �ʱ�ȭ �κ��Դϴ�.*/
	float statusBoxWidth = 163;
	float statusBoxHeight = 124;

	int scrWidth = GetG().m_scrWidth;
	int scrHeight = GetG().m_scrHeight;

	float statusBoxPlayersPositionX = -(float)scrWidth/2 + 10;
	float statusBoxPlayersPositionY = -(float)scrHeight/2 + 10;
	m_StatusBoxPlayer.init(L"Images/BattleUI/StatusBox.png", m_pDev);
	m_StatusBoxPlayer.setPos(D3DXVECTOR3(statusBoxPlayersPositionX, statusBoxPlayersPositionY, 7));
	m_StatusBoxPlayer.setSize(statusBoxWidth, statusBoxHeight);

	float statusBoxEnemysPositionX = (float)scrWidth/2 - statusBoxWidth - 10;
	float statusBoxEnemysPositionY = (float)scrHeight/2 - statusBoxHeight - 10;
	m_StatusBoxEnemy.init(L"Images/BattleUI/StatusBox.png", m_pDev);
	m_StatusBoxEnemy.setPos(D3DXVECTOR3(statusBoxEnemysPositionX, statusBoxEnemysPositionY, 6.9f));
	m_StatusBoxEnemy.setSize(statusBoxWidth, statusBoxHeight);

	int skillBoxHeight = 200;
	int skillBoxWidth = skillBoxHeight * 593 / 933;
	int skillBoxPositionX = scrWidth/2 -skillBoxWidth - 3;
	int skillBoxPositionY =  -scrHeight/2 + 3;
	m_SkillBox.init(L"Images/BattleUI/SkillBox.png", m_pDev);
	m_SkillBox.setPos (D3DXVECTOR3((float)skillBoxPositionX, (float)skillBoxPositionY, 6.9f));
	m_SkillBox.setSize((float)skillBoxWidth, (float)skillBoxHeight);

	int skillContentBoxHeight = 200;
	int skillContentBoxWidth = skillContentBoxHeight * 390 / 269;
	m_SkillContentBox.init(L"Images/BattleUI/SkillContentBox.png", m_pDev);
	m_SkillContentBox.setPos(D3DXVECTOR3((float)skillBoxPositionX - skillContentBoxWidth, (float)skillBoxPositionY, 6.9f));
	m_SkillContentBox.setSize((float)skillContentBoxWidth, (float)skillContentBoxHeight);

	float dialogBoxWidth = (float)(int)(scrWidth - statusBoxWidth - 30);
	float dialogBoxHeight = 124;
	m_DialogBox.init(L"Images/BattleUI/DialogBox.png", m_pDev);
	m_DialogBox.setPos(D3DXVECTOR3((float)-(scrWidth)/2 + 10, (float)(scrHeight)/2 - dialogBoxHeight - 10, 6.9f));
	//m_DialogBox.setPos (10, 10, 6.9f);
	m_DialogBox.setSize(dialogBoxWidth, dialogBoxHeight);

	float statusBarWidth = statusBoxWidth * 0.67f;
	float statusBarHeight = statusBarWidth * 0.1f;
	m_hpBgPlayer.init(L"Images/BattleUI/HPbg.jpg", m_pDev);
	m_hpBgPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 5);
	m_hpBgPlayer.setSize(statusBarWidth, statusBarHeight);
	m_mpBgPlayer.init(L"Images/BattleUI/MPbg.jpg", m_pDev);
	m_mpBgPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.65f, 5);
	m_mpBgPlayer.setSize(statusBarWidth, statusBarHeight);
	m_expBgPlayer.init(L"Images/BattleUI/EXPbg.jpg", m_pDev);
	m_expBgPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.48f, 5);
	m_expBgPlayer.setSize(statusBarWidth, statusBarHeight);

	m_hpBgEnemy.init(L"Images/BattleUI/HPbg.jpg", m_pDev);
	m_hpBgEnemy.setPos (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.82f, 5);
	m_hpBgEnemy.setSize(statusBarWidth, statusBarHeight);
	m_mpBgEnemy.init(L"Images/BattleUI/MPbg.jpg", m_pDev);
	m_mpBgEnemy.setPos (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.65f, 5);
	m_mpBgEnemy.setSize(statusBarWidth, statusBarHeight);

	m_hpIllusionPlayer.init(L"Images/BattleUI/HPillusion.jpg", m_pDev);
	m_hpIllusionPlayer.setSize(statusBarWidth, statusBarHeight);
	m_hpIllusionPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 4.8f);

	m_hpIllusionEnemy.init(L"Images/BattleUI/HPillusion.jpg", m_pDev);
	m_hpIllusionEnemy.setSize(statusBarWidth, statusBarHeight);
	m_hpIllusionEnemy.setPos (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.82f, 4.8f);


	m_hpBarPlayer.init(L"Images/BattleUI/HPbar.jpg", m_pDev);
	m_hpBarPlayer.setSize(statusBarWidth, statusBarHeight);
	m_hpBarPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 4.5f);
	
	m_innerFire.init (L"Images/BattleUI/ChaosAura.png", m_pDev, 0.8f, 3, 20);
	m_innerFire.setPos (0, 0, 0);


	m_mpBarPlayer.init(L"Images/BattleUI/DKbar.jpg", m_pDev);
	m_mpBarPlayer.initRate();
	m_mpBarPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.65f, 4.5f);
	m_mpBarPlayer.setSize(statusBarWidth, statusBarHeight);

	m_expBarPlayer.init(L"Images/BattleUI/EXPbar.jpg", m_pDev);
	m_expBarPlayer.initRate();
	m_expBarPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.48f, 4.5f);
	m_expBarPlayer.setSize(statusBarWidth, statusBarHeight);
	m_expBarPlayer.changeRate(73.3f);

	m_hpBarEnemy.init(L"Images/BattleUI/HPbar.jpg", m_pDev);
	m_hpBarEnemy.setPos (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.82f, 4.5f);
	m_hpBarEnemy.setSize(statusBarWidth, statusBarHeight);

	m_mpBarEnemy.init(L"Images/BattleUI/MPbar.jpg", m_pDev);
	m_mpBarEnemy.setSize(statusBarWidth, statusBarHeight);
	m_mpBarEnemy.setPos (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.65f, 4.5f);
	
	D3DXCreateFont(m_pDev, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblHYnamL);
	D3DXCreateFont(m_pDev, 18, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Rockwell Extra Bold"), &m_lblREB);
	D3DXCreateFont(m_pDev, 25, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblArt);



	m_curTurnType = TT_NATURAL;
	m_nextTurnType = TT_NATURAL;
	m_playerArt = PA_ART1;
}

BattleState::~BattleState()
{
	release();
}

/* ���� ���� �� ���� �Լ��� ȣ���*/
HRESULT BattleState::enter()
{
	m_ws = dynamic_cast<WorldState*>( GetTopStateManager().getCurState() );
	assert( m_ws );
	getHero()->clearKey();

	GetAudioState().bBGMFade = true;
	GetAudioState().bMusicFade = false;
	GetAudioState().pEngine->Stop( GetAudioState().iMusicCategory, 0 );
	GetAudioState().pSoundBank->Play( GetAudioState().iBattle, 0, 0, NULL );

	EpCamera& camera = G::getSingleton().m_camera;

	// ī�޶� ��ŷ�� ���� ������Ʈ ���� �� ī�޶� ���¸� ������ �д�.
	m_vPrevEye		= *camera.GetEyePt();
	m_vPrevLookAt	= *camera.GetLookAtPt();
	m_vPrevUp		= *camera.GetUpPt();

	m_hpBarPlayer.initRate((float)getHero()->getMaxHp());
	m_hpIllusionPlayer.initRate((float)getHero()->getMaxHp());
	m_hpBarEnemy.initRate((float)getEnemy()->getMaxHp());
	m_hpIllusionEnemy.initRate((float)getEnemy()->getMaxHp());

	m_hpBarPlayer.setRate((float)getHero()->getCurHp());
	m_hpBarEnemy.setRate((float)getEnemy()->getCurHp());
	m_hpIllusionPlayer.setRate((float)getHero()->getCurHp());
	m_hpIllusionEnemy.setRate((float)getEnemy()->getCurHp());
	

	m_battleLog.push_back(std::string("���� ����~~~~~~~~~!!!"));
	m_battleWinner = PS_NOTSET;
	setNextTurnType( TT_PLAYER );
	passTurn();
	return S_OK;
}

HRESULT BattleState::leave()
{
	m_startTime = -1.0f;
	m_battleLog.clear();

	GetAudioState().bBGMFade = false;
	GetAudioState().bMusicFade = true;

	return S_OK;
}

HRESULT BattleState::frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, &GetG().g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &GetG().g_orthoProjMat);

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

	m_pDev->SetRenderState(D3DRS_ZENABLE, FALSE);
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

	m_hpIllusionPlayer.draw();
	m_hpIllusionEnemy.draw();

	m_hpBarPlayer.draw();
	m_mpBarPlayer.draw();
	m_expBarPlayer.draw();
	m_hpBarEnemy.draw();
	m_mpBarEnemy.draw();

	

	m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pd3dDevice->SetTransform(D3DTS_VIEW, GetG().m_camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, GetG().m_camera.GetProjMatrix());

	if (this->m_curTurnType == TT_PLAYER)
		m_innerFire.draw();

	renderFixedText(GetG().m_scrWidth, GetG().m_scrHeight);
	
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

	m_hpIllusionPlayer.frameMove(fElapsedTime);
	m_hpIllusionEnemy.frameMove(fElapsedTime);

	m_hpBarPlayer.frameMove(fElapsedTime);
	m_mpBarPlayer.frameMove(fElapsedTime);
	m_expBarPlayer.frameMove(fElapsedTime);

	m_innerFire.frameMove(fElapsedTime);
	m_innerFire.setPos( getHero()->getPos().x, getHero()->getPos().y, -3);


	m_hpBarEnemy.frameMove(fElapsedTime);
	m_mpBarEnemy.frameMove(fElapsedTime);



	m_hpBarPlayer.changeRate((float)getHero()->getCurHp());
	m_hpBarEnemy.changeRate((float)getEnemy()->getCurHp());
	m_hpIllusionPlayer.changeRate((float)getHero()->getCurHp());
	m_hpIllusionEnemy.changeRate((float)getEnemy()->getCurHp());
	

	// WorldState�� �����ϱ� ���� �ڵ�.
	TopStateManager& tsm = TopStateManager::getSingleton();
	WorldState* ws = static_cast<WorldState*>( tsm.getCurState() );
	const D3DXVECTOR3& vEnemyPos = ws->getEnemyPos();
	const D3DXVECTOR3& vHeroPos = ws->getHeroPos();

	// ���ΰ��� ���� �־����ų�(�浹 ���°� �ƴϰų�), ���ڰ� �����Ǹ� FieldState�� ���ư���.
	if ( ws->isCollide( &vEnemyPos, &vHeroPos ) == false || m_battleWinner != PS_NOTSET )
		GetWorldStateManager().setNextState(GAME_WORLD_STATE_FIELD);

	//
	// ó�� ���۽� ���ΰ��� ���� �Ʒ�, ���� ������ ���� ���̵��� ī�޶� �����δ�.
	//

	EpCamera& camera = G::getSingleton().m_camera;

	// ������ �Ͼ�� ��ġ�� ���Ѵ�. ���� ��ġ�� ���ΰ� ��ġ�� �߰���.
	D3DXVECTOR3 vBattlePos;
	vBattlePos.x = (vEnemyPos.x + vHeroPos.x) / 2.0f;
	vBattlePos.y = (vEnemyPos.y + vHeroPos.y) / 2.0f;
	vBattlePos.z = (vEnemyPos.z + vHeroPos.z) / 2.0f;

	// ���� ī�޶� ����.
	D3DXVECTOR3 vDesEye( 0.0f, 0.0f, -10.0f );
	D3DXVECTOR3 vDesLookAt( vBattlePos );
	D3DXVECTOR3 vDesUp( 0.0f, 0.0f, -1.0f );

	// ���ΰ����κ��� ������ �̾����� ���� ���Ѵ�.
	D3DXVECTOR3 vBattleAxis;
	vBattleAxis.x = (vEnemyPos.x - vHeroPos.x);
	vBattleAxis.y = (vEnemyPos.y - vHeroPos.y);
	vBattleAxis.z = (vEnemyPos.z - vHeroPos.z);

	// z �࿡ ��� vBattleAxis�� 45�� ������.
	D3DXVECTOR3 zAxis( 0.0f, 0.0f, 1.0f );
	Utility::rotateAboutAxis( &vBattleAxis, &zAxis, D3DXToRadian( -45.0f ) );

	// vDesEye�� vBattleAxis�� ��� -45�� ��ŭ ������.
	Utility::rotateAboutAxis( &vDesEye, &vBattleAxis, D3DXToRadian( -45.0f ) );

	// vDesEye�� ���ΰ� ���� �ű��.
	vDesEye.x += vHeroPos.x;
	vDesEye.y += vHeroPos.y;

	// ī�޶� �����̴� �ð�
	float camDuration = 1.0f;

	if ( 0.0f < fStateTime && fStateTime < camDuration && GetWorldStateManager().prevStateEnum() == GAME_WORLD_STATE_FIELD)
	{
		// ���� ī�޶� ���� ������ ���� ����.
		D3DXVECTOR3 vCurEye;
		D3DXVECTOR3 vCurLookAt;
		D3DXVECTOR3 vCurUp;
		
		// ī�޶� ��� ����.
		D3DXVECTOR3 vEye0( m_vPrevEye );
		D3DXVECTOR3 vEye1( m_vPrevEye );		
		D3DXVECTOR3 vEye2( vDesEye );
		D3DXVECTOR3 vEye3( vHeroPos );

		// ���� ī�޶� ���¸� ����Ѵ�.
		//D3DXVec3CatmullRom( &vCurEye, &vEye0, &vEye1, &vEye2, &vEye3, (FLOAT)fStateTime );
		D3DXVec3Lerp( &vCurEye, &m_vPrevEye, &vDesEye, (float)fStateTime / camDuration );
		D3DXVec3Lerp( &vCurUp, &m_vPrevUp, &vDesUp, (float)fStateTime / camDuration );
		D3DXVec3Lerp( &vCurLookAt, &m_vPrevLookAt, &vDesLookAt, (float)fStateTime / camDuration );

		D3DXVec3Normalize( &vCurUp, &vCurUp );

		camera.SetViewParamsWithUp( &vCurEye, &vCurLookAt, vCurUp );
	}
	else
		camera.SetViewParamsWithUp( &vDesEye, &vDesLookAt, vDesUp );

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
		/*�׾��� �� enter Ű�� �Է��ϸ� ��� �ı�, �ƴϸ� �ٸ� Ű �� �ް� �޽��� �ڵ鸵 ����*/
		if ( getEnemy()->getCurHp() <= 0 )
		{
			if (wParam == VK_RETURN)
			{
				TopStateManager& tsm = TopStateManager::getSingleton();
				WorldState* ws = static_cast<WorldState*>( tsm.getCurState() );
				GetWorldStateManager().setNextState(GAME_WORLD_STATE_FIELD);
				ws->removeUnit( getEnemy() );
				return S_OK;
			}
			return S_OK;
		}
		/*�ڽ��� �׾��� �� ��� Ű�ε� �������� �ʴ´�.*/
		else if (getHero()->getCurHp() <= 0)
		{
			return S_OK;
		}
		/*�ڽ��� ���ʰ� �ƴ� ������ �������� �ʴ´�.*/
		else if (m_curTurnType != TT_PLAYER)
		{
			return S_OK;
		}

		/*ȭ��ǥ�� ���� ��� �б�*/
		if (wParam == VK_UP)
		{
			setArt('u');
		}
		if (wParam == VK_DOWN)
		{
			setArt('d');
		}
		if (wParam == VK_RETURN)
		{
			spellArt ();
		}

		if ( wParam == VK_F4 )
		{
			GetWorldStateManager().setNextState(GAME_WORLD_STATE_FIELD);
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

	m_hpIllusionPlayer.release();
	m_hpIllusionEnemy.release();

	m_hpBarPlayer.release();
	m_mpBarPlayer.release();
	m_expBarPlayer.release();

	m_hpBarEnemy.release();
	m_mpBarEnemy.release();

	m_innerFire.release();
	
	
	SAFE_RELEASE( m_lblHYnamL );
	SAFE_RELEASE( m_lblREB );
	SAFE_RELEASE( m_lblArt );

	return S_OK;
}

void BattleState::renderFixedText(int scrWidth, int scrHeight)
{
	// Print battle log
	const UINT maxDrawLogCount = 7;
	const UINT lineHeight = 15;
	RECT rc;
	rc.top		= 110;
	rc.left		= 25;
	rc.right	= scrWidth;
	rc.bottom	= scrHeight;

	if ( m_battleLog.size() < maxDrawLogCount )
	{
		rc.top -= (maxDrawLogCount - m_battleLog.size()) * lineHeight;
	}
	
	StringList::const_reverse_iterator it = m_battleLog.rbegin();
	UINT drawLogCount = 0;
	for ( ; it != m_battleLog.rend(); ++it )
	{
		m_lblHYnamL->DrawTextA(0, (*it).c_str(), -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.9f, 0.9f, 0.9f, 1.0f ) );
		rc.top -= lineHeight;
		drawLogCount++;
		if ( drawLogCount >= maxDrawLogCount )
			break;
	}

	/*���ΰ� status �� �׸���*/
	WCHAR textBuffer[512];
	float statusBoxPlayersPositionX = 10;
	float statusBoxPlayersPositionY = (float)(scrHeight - 10);
	rc.top = (LONG)(statusBoxPlayersPositionY - 115);
	rc.left = (LONG)(statusBoxPlayersPositionX + 5);
	StringCchPrintf(textBuffer, 512, L"HP");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += (LONG)21.08;
	StringCchPrintf(textBuffer, 512, L"CS");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += (LONG)21.08;
	StringCchPrintf(textBuffer, 512, L"EX");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );


	/*�� status �׸���*/
	rc.top = (LONG)18;
	rc.left = (LONG)(scrWidth - 167);
	StringCchPrintf(textBuffer, 512, L"HP");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += (LONG)21.08;
	StringCchPrintf(textBuffer, 512, L"CS");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );


	int skillLineInterval = 30;

	rc.top = scrHeight - 190;
	rc.left = scrWidth - 110;
	StringCchPrintf(textBuffer, 512, L"Attack");
	m_lblArt->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, L"Heal");
	m_lblArt->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, L"~_~");
	m_lblArt->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, L"~_~");
	m_lblArt->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, L"~_~");
	m_lblArt->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

	switch (m_playerArt)
	{
	case PA_ART1:
		rc.top = scrHeight - 190;
		StringCchPrintf(textBuffer, 512, L"Attack");
		break;
	case PA_ART2:
		rc.top = scrHeight - 160;
		StringCchPrintf(textBuffer, 512, L"Heal");
		break;
	case PA_ART3:
		rc.top = scrHeight - 130;
		StringCchPrintf(textBuffer, 512, L"~_~");
		break;
	case PA_ART4:
		rc.top = scrHeight - 100;
		StringCchPrintf(textBuffer, 512, L"~_~");
		break;
	case PA_ART5:
		rc.top = scrHeight - 70;
		StringCchPrintf(textBuffer, 512, L"~_~");
		break;
	}
	m_lblArt->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );

}

void BattleState::passTurn()
{
	m_curTurnType = m_nextTurnType;
	m_nextTurnType = TT_NATURAL;

	if (m_curTurnType == TT_PLAYER)
	{
		m_battleLog.push_back(std::string("����� ������ �����Դϴ�."));		
	}
	else if (m_curTurnType == TT_COMPUTER)
	{
		m_battleLog.push_back(std::string("��ǻŸ �����Դϴ�."));
		doComputerAction();
	}
}

void BattleState::doComputerAction()
{
	getEnemy()->attack(0, getHero());
}

Character* BattleState::getHero()
{
	return static_cast<Character*>( m_ws->getHero() );
}

Character* BattleState::getEnemy()
{
	return static_cast<Character*>( m_ws->getCurEnemy() );
}

void BattleState::setArt(char direction)
{
	if (direction == 'u')
	{
		switch (m_playerArt)
		{
		case PA_ART1:
			m_playerArt = PA_ART5;
			break;
		case PA_ART2:
			m_playerArt = PA_ART1;
			break;
		case PA_ART3:
			m_playerArt = PA_ART2;
			break;
		case PA_ART4:
			m_playerArt = PA_ART3;
			break;
		case PA_ART5:
			m_playerArt = PA_ART4;
			break;
		}
	}
	else if (direction == 'd')
	{
		switch (m_playerArt)
		{
		case PA_ART1:
			m_playerArt = PA_ART2;
			break;
		case PA_ART2:
			m_playerArt = PA_ART3;
			break;
		case PA_ART3:
			m_playerArt = PA_ART4;
			break;
		case PA_ART4:
			m_playerArt = PA_ART5;
			break;
		case PA_ART5:
			m_playerArt = PA_ART1;
			break;
		}
	}
}

void BattleState::spellArt()
{
	/*attack*/
	if (m_playerArt == PA_ART1)
	{	
		m_curTurnType = TT_NATURAL;
		m_battleLog.push_back(std::string("�Ϲ� ������ ����Ͽ����ϴ�."));
		getHero()->attack( 0, getEnemy() );
	}
	/*heal*/
	else if (m_playerArt == PA_ART2)
	{
		m_curTurnType = TT_NATURAL;
		m_battleLog.push_back(std::string("������ ����Ͽ����ϴ�."));	
		int healPoint = getHero()->getInt();

		char stringBuffer[20];
		_itoa_s (healPoint, stringBuffer, 10);
		std::string resultLog = stringBuffer;
		resultLog += "����Ʈ HP�� ȸ���Ͽ����ϴ�.";
		m_battleLog.push_back (resultLog.c_str());
		getHero()->heal (healPoint);
		setNextTurnType(TT_COMPUTER);
		passTurn();
	}
	/*blank*/
	else if (m_playerArt == PA_ART3)
	{

	}
	/*blank*/
	else if (m_playerArt == PA_ART4)
	{

	}
	/*�����*/
	else if (m_playerArt == PA_ART5)
	{

	}
}
