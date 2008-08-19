#include "EmptyProjectPCH.h"
#include "BattleState.h"
#include "WorldStateManager.h"
#include "TopStateManager.h"
#include "World.h"
#include "Utility.h"
#include "Action.h"
#include "Skill.h"
#include "Sound.h"
#include "TileManager.h"

BattleState::BattleState()
{
	m_pDev = GetG().m_dev;
	assert( m_pDev );

	/*UI 초기화 부분입니다.*/
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
	m_SkillContentBox.setDistance(500);
	m_SkillContentBox.setOff();
	m_SkillContentBox.setOnPos((float)skillBoxPositionX - skillContentBoxWidth, (float)skillBoxPositionY, 7);
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

	m_mpIllusionPlayer.init(L"Images/BattleUI/DKillusion.jpg", m_pDev);
	m_mpIllusionPlayer.setSize(statusBarWidth, statusBarHeight);
	m_mpIllusionPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.65f, 4.8f);

	m_hpBarPlayer.init(L"Images/BattleUI/HPbar.jpg", m_pDev);
	m_hpBarPlayer.setSize(statusBarWidth, statusBarHeight);
	m_hpBarPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 4.5f);
	
	m_innerFire.init (L"Images/BattleUI/BGchecker.jpg", m_pDev, 0.8f, 3, 9);
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
	D3DXCreateFont(m_pDev, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblSkill);
	D3DXCreateFont(m_pDev, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblSkillDescription);


	/*스킬 초기화 부분*/
	
	m_skillSet = new SkillSet();
	m_skillSet->setBattleState(this);

	m_skillSet->setSkill (SL_FIRST, (Skill*) new NormalAttack());
	m_skillSet->setSkill (SL_SECOND, (Skill*) new Heal());
	m_skillSet->setSkill (SL_FIFTH, (Skill*) new Meditation());

	m_noneSkillSelectedCount = 0;

	m_curTurnType = TT_NATURAL;
	m_nextTurnType = TT_NATURAL;
}

BattleState::~BattleState()
{
	release();
}

/* 전투 시작 시 다음 함수가 호출됨*/
HRESULT BattleState::enter()
{
	getHero()->clearKey();

	GetAudioState().enterBattle();

	m_hpBarPlayer.initRate((float)getHero()->getMaxHp());
	m_hpIllusionPlayer.initRate((float)getHero()->getMaxHp());
	m_hpBarEnemy.initRate((float)getEnemy()->getMaxHp());
	m_hpIllusionEnemy.initRate((float)getEnemy()->getMaxHp());
	m_mpBarPlayer.initRate((float)getHero()->getMaxCs());
	m_mpIllusionPlayer.initRate((float)getHero()->getMaxCs());

	m_hpBarPlayer.setRate((float)getHero()->getCurHp());
	m_hpBarEnemy.setRate((float)getEnemy()->getCurHp());
	m_mpBarPlayer.setRate((float)getHero()->getCurCs());
	m_hpIllusionPlayer.setRate((float)getHero()->getCurHp());
	m_hpIllusionEnemy.setRate((float)getEnemy()->getCurHp());
	m_mpIllusionPlayer.setRate((float)getHero()->getCurCs());

	/*스킬 대상 설정*/
	m_skillSet->setCharacter (getHero(), getEnemy());
	m_noneSkillSelectedCount = 0;
	m_SkillContentBox.setOff();

	setupCamera();
	

	m_battleLog.push_back(std::string("전투 개시~~~~~~~~~!!!"));
	m_battleWinner = PS_NOTSET;
	setNextTurnType( TT_PLAYER );
	passTurn();
	return S_OK;
}

HRESULT BattleState::leave()
{
	m_startTime = -1.0f;
	m_battleLog.clear();

	GetAudioState().leaveBattle();

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
	m_mpIllusionPlayer.draw();


	m_hpBarPlayer.draw();
	m_mpBarPlayer.draw();
	m_expBarPlayer.draw();
	m_hpBarEnemy.draw();
	m_mpBarEnemy.draw();

	

	m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pd3dDevice->SetTransform(D3DTS_VIEW, GetG().m_camera.GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, GetG().m_camera.GetProjMatrix());

	/*if (this->m_curTurnType == TT_PLAYER)
		m_innerFire.draw();*/

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
	m_mpIllusionPlayer.frameMove(fElapsedTime);

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
	m_mpBarPlayer.changeRate((float)getHero()->getCurCs());
	m_mpIllusionPlayer.changeRate((float)getHero()->getCurCs());


	
	//// WorldState에 접근하기 위한 코드.
	//TopStateManager& tsm = TopStateManager::getSingleton();
	//WorldState* ws = static_cast<WorldState*>( tsm.getCurState() );
	//const D3DXVECTOR3& vEnemyPos = ws->getEnemyPos();
	//const D3DXVECTOR3& vHeroPos = ws->getHeroPos();

	// 주인공이 적과 멀어지거나(충돌 상태가 아니거나), 승자가 결정되면 FieldState로 돌아간다.
	//if ( ws->isCollide( &vEnemyPos, &vHeroPos ) == false || m_battleWinner != PS_NOTSET )

	// 승자가 결정되면 FieldState로 돌아간다.
	if ( m_battleWinner != PS_NOTSET )
		GetWorldStateManager().setNextState(GAME_WORLD_STATE_FIELD);

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
		/*죽었을 시 enter 키를 입력하면 대상 파괴, 아니면 다른 키 안 받고 메시지 핸들링 종료*/
		if ( getEnemy()->getCurHp() <= 0 )
		{
			if (wParam == VK_RETURN)
			{
				GetWorldStateManager().setNextState(GAME_WORLD_STATE_FIELD);
				getEnemy()->setRemoveFlag( true ); // Should be deleted before next frame update
				return S_OK;
			}
			return S_OK;
		}
		/*자신이 죽었을 시 어떠한 키로도 반응하지 않는다.*/
		else if (getHero()->getCurHp() <= 0)
		{
			return S_OK;
		}
		/*자신의 차례가 아닐 때에도 반응하지 않는다.*/
		else if (m_curTurnType != TT_PLAYER)
		{
			return S_OK;
		}

		/*화살표에 따라 기술 분기*/
		if (wParam == VK_UP)
		{
			m_skillSet->moveSkillLocation('u');
		}
		if (wParam == VK_DOWN)
		{
			m_skillSet->moveSkillLocation('d');
		}
		if (wParam == VK_LEFT)
		{
			m_SkillContentBox.onBox();
		}
		if (wParam == VK_RIGHT)
		{
			m_SkillContentBox.offBox();
		}
		if (wParam == VK_RETURN)
		{
			m_curTurnType = TT_NATURAL;
			if (!(this->m_skillSet->useSkill()))
			{
				/*이부분에 스킬이 없습니다 라는 다이얼로그를 추가할수도.*/
				switch (m_noneSkillSelectedCount)
				{
				case 0:
					m_battleLog.push_back(std::string("스킬이 없습니다."));
					break;
				case 1:
					m_battleLog.push_back(std::string("스킬이 없는데요?"));
					break;
				case 2:
					m_battleLog.push_back(std::string("스킬이 없다니까요!!?"));
					break;
				case 3:
					m_battleLog.push_back(std::string("없는 스킬 자꾸 누르는 저의가 무엇이냐!"));
					break;
				case 4:
					m_battleLog.push_back(std::string("뭥미???"));
					break;
				case 5:
					m_battleLog.push_back(std::string("..."));
					break;
				case 6:
					m_battleLog.push_back(std::string("..."));
					break;
				case 7:
					m_battleLog.push_back(std::string("어쩔 수 없군. 몇 가지 팁을 제공하죠."));
					break;
				case 8:
					m_battleLog.push_back(std::string("맵의 곳곳에 숨겨진 박카스가 존재하는 듯 하다."));
					break;
				case 9:
					m_battleLog.push_back(std::string("박카스를 쟁취하려면 맵에 존재하는 갖가지 퍼즐을 풀어야 한다."));
					break;
				case 10:
					m_battleLog.push_back(std::string("최종 보스와의 대전을 위해 물약을 최대한 많이 쟁겨놓으십시오."));
					break;
				case 11:
					m_battleLog.push_back(std::string("신도 세미나실은 신성합니다. 존중해주시죠."));
					break;
				case 12:
					m_battleLog.push_back(std::string("제육덮밥"));
					break;
				case 13:
					m_battleLog.push_back(std::string("보스는 사실 거미를 무서워하는 듯 하다."));
					break;
				case 14:
					m_battleLog.push_back(std::string("흐흥, 딱히 스크립트를 모르기 땜시 이런 switch문 노가다를 한건 아니야!"));
					break;
				case 15:
					m_battleLog.push_back(std::string("goto문은 지양합시다."));
					break;
				case 16:
					m_battleLog.push_back(std::string("1일 1코딩."));
					break;
				case 17:
					m_battleLog.push_back(std::string("게임은 조금만 합시다."));
					break;
				case 18:
					m_battleLog.push_back(std::string("코드에 주석은 필수."));
					break;
				case 19:
					m_battleLog.push_back(std::string("제작 동안 OOO군의 프로젝트 말살 음모가 있었습니다."));
					break;
				case 20:
					m_battleLog.push_back(std::string("맛의진미"));
					break;
				case 21:
					m_battleLog.push_back(std::string("일부 스킬들은 연속기로 쓸 수 있습니다."));
					break;
				default:
					m_battleLog.push_back(std::string("훗, 더 이상의 자세한 팁은 생략한다."));;
					break;
				}
				m_noneSkillSelectedCount++;
				m_curTurnType = TT_PLAYER;
			}
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
	m_mpIllusionPlayer.release();

	m_hpBarPlayer.release();
	m_mpBarPlayer.release();
	m_expBarPlayer.release();

	m_hpBarEnemy.release();
	m_mpBarEnemy.release();

	m_innerFire.release();

	//delete m_skillSet;
	
	SAFE_RELEASE( m_lblHYnamL );
	SAFE_RELEASE( m_lblREB );
	SAFE_RELEASE( m_lblSkill );
	SAFE_RELEASE( m_lblSkillDescription );

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

	/*주인공 status 라벨 그리기*/
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


	/*적 status 라벨 그리기*/
	rc.top = (LONG)18;
	rc.left = (LONG)(scrWidth - 167);
	StringCchPrintf(textBuffer, 512, L"HP");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += (LONG)21.08;
	StringCchPrintf(textBuffer, 512, L"CS");
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );


	int skillLineInterval = 39;

	rc.top = scrHeight - 190;
	rc.left = scrWidth - 110;

	StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_FIRST).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_SECOND).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_THIRD).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_FOURTH).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_FIFTH).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

	/*스킬 라벨 그리기*/
	switch (m_skillSet->getSkillLocation())
	{
	case SL_FIRST:
		rc.top = scrHeight - 190;
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_FIRST).c_str());
		break;
	case SL_SECOND:
		rc.top = scrHeight - 190 + skillLineInterval;
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_SECOND).c_str());
		break;
	case SL_THIRD:
		rc.top = scrHeight - 190 + skillLineInterval*2;
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_THIRD).c_str());
		break;
	case SL_FOURTH:
		rc.top = scrHeight - 190 + skillLineInterval*3;
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_FOURTH).c_str());
		break;
	case SL_FIFTH:
		rc.top = scrHeight - 190 + skillLineInterval*4;
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getSkillName(SL_FIFTH).c_str());
		break;
	}
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );


	rc.top = scrHeight - 190;
	rc.left = scrWidth - 410;

	switch (m_skillSet->getSkillLocation())
	{
	case SL_FIRST:
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getDescription(SL_FIRST).c_str());
		break;
	case SL_SECOND:
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getDescription(SL_SECOND).c_str());
		break;
	case SL_THIRD:
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getDescription(SL_THIRD).c_str());
		break;
	case SL_FOURTH:
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getDescription(SL_FOURTH).c_str());
		break;
	case SL_FIFTH:
		StringCchPrintf(textBuffer, 512, (STRSAFE_LPCWSTR)m_skillSet->getDescription(SL_FIFTH).c_str());
		break;
	}

	if (m_SkillContentBox.isOn())
		m_lblSkillDescription->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 0.8f, 0.8f, 1.0f, 1.0f ) );

}

void BattleState::passTurn()
{
	m_curTurnType = m_nextTurnType;
	m_nextTurnType = TT_NATURAL;

	if (m_curTurnType == TT_PLAYER)
	{
		m_battleLog.push_back(std::string("당신이 공격할 차례입니다."));
		getHero()->recoverCs();
	}
	else if (m_curTurnType == TT_COMPUTER)
	{
		m_battleLog.push_back(std::string("콤퓨타 차례입니다."));
		doComputerAction();
	}
}

void BattleState::doComputerAction()
{
	getEnemy()->attack(0, getHero());
}

Character* BattleState::getHero()
{
	return getCurWorld()->getHeroUnit();
}

Character* BattleState::getEnemy()
{
	return getCurWorld()->getCurEnemyUnit();
}

void BattleState::setupCamera()
{//
	// 처음 시작시 주인공이 왼쪽 아래, 적이 오른쪽 위에 보이도록 카메라를 움직인다.
	//

	// WorldState에 접근하기 위한 코드.
	TopStateManager& tsm = TopStateManager::getSingleton();
	World* ws = GetWorldManager().getCurWorld();
	const D3DXVECTOR3& vEnemyPos = GetWorldManager().getCurWorld()->getEnemyPos();
	const D3DXVECTOR3& vHeroPos = ws->getHeroPos();

	GetG().m_camera.beginShoulderLookCamera( &vHeroPos, &vEnemyPos );
}
