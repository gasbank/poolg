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
#include "Hero.h"
#include "Enemy.h"
#include "ArnMesh.h"
#include "SpriteManager.h"
#include "Sprite.h"

BattleState::BattleState()
{
	m_pDev = GetG().m_dev;
	assert( m_pDev );

	/*UI �ʱ�ȭ �κ��Դϴ�.*/
	float statusBoxWidth = 163;
	float statusBoxHeight = 124;

	int scrWidth = GetG().m_scrWidth;
	int scrHeight = GetG().m_scrHeight;
/*
	m_sprite = GetSpriteManager().registerSprite( "BattleUI", "Images/BattleUI.tga" );
	m_sprite->setCustomRendered( true );
	m_sprite->registerRect( "StatusBoxBg", 0, 150, 0+163, 150+128 );
	m_sprite->registerRect( "BattleLogBg", 0, 0, 0+503, 0+150 );
	m_sprite->registerRect( "SkillBg", 0, 272, 0+170, 272+161 );
	m_sprite->registerRect( "BarFg", 163, 150, 163+220, 150+22 );		// Progress bar foreground
	m_sprite->registerRect( "BarBg", 163, 172, 163+220, 172+22 );		// Progress bar background
	*/
	m_sprite = GetSpriteManager().registerSprite( "BattleUI", "Images/BattleUI/BattleUISet.tga" );
	m_sprite->setCustomRendered( true );
	m_sprite->registerRect( "StatusBoxBg", 600, 0, 763, 124 );
	m_sprite->registerRect( "BattleLogBg", 0, 0, 600, 124 );
	m_sprite->registerRect( "SkillBg", 0, 124, 128, 324 );
	m_sprite->registerRect( "BarFg", 0, 554, 110, 567 );		// Progress bar foreground
	m_sprite->registerRect( "BarBg", 0, 580, 110, 593 );	



	m_sprite->drawRequest( "StatusBoxBg", Sprite::RIGHT_TOP, 0xffffffff );
	m_sprite->drawRequest( "StatusBoxBg", Sprite::LEFT_BOTTOM, 0xffffffff );
	m_sprite->drawRequest( "BattleLogBg", Sprite::LEFT_TOP, D3DCOLOR_RGBA(255, 0, 0, 255) );
	m_sprite->drawRequest( "SkillBg", Sprite::RIGHT_BOTTOM, 0xffffffff );

	m_sprite->drawRequest( "BarBg", 0, 200, 200, 0, 0xffffffff );
	DrawRequest* dr = m_sprite->drawRequest( "BarFg", 0, 200, 200, 0, D3DCOLOR_RGBA(255, 0, 0, 255) );

	dr->srcRect.right -= 50;
	
	

	//////////////////////////////////////////////////////////////////////////

	float statusBoxPlayersPositionX = -(float)scrWidth/2 + 10;
	float statusBoxPlayersPositionY = -(float)scrHeight/2 + 10;

	float statusBoxEnemysPositionX = (float)scrWidth/2 - statusBoxWidth - 10;
	float statusBoxEnemysPositionY = (float)scrHeight/2 - statusBoxHeight - 10;

	int skillBoxHeight = 200;
	int skillBoxWidth = skillBoxHeight * 593 / 933;
	int skillBoxPositionX = scrWidth/2 -skillBoxWidth - 3;
	int skillBoxPositionY =  -scrHeight/2 + 3;

	int skillContentBoxHeight = 200;
	int skillContentBoxWidth = skillContentBoxHeight * 390 / 269;
	m_SkillContentBox.init(L"Images/BattleUI/SkillContentBox.png", m_pDev);
	m_SkillContentBox.setDistance(500);
	m_SkillContentBox.setOff();
	m_SkillContentBox.setOnPos((float)skillBoxPositionX - skillContentBoxWidth, (float)skillBoxPositionY, 7);
	m_SkillContentBox.setSize((float)skillContentBoxWidth, (float)skillContentBoxHeight);


	int StatSelectBoxHeight = 230;
	int StatSelectBoxWidth = skillBoxWidth + 40;
	m_StatSelectBox.init(L"Images/BattleUI/StatSelectBox.png", m_pDev);
	m_StatSelectBox.setDistance(500);
	m_StatSelectBox.setOff();
	m_StatSelectBox.setOnPos( 0 , (float)skillBoxPositionY + StatSelectBoxHeight, 7);
	m_StatSelectBox.setSize((float)StatSelectBoxWidth, (float)StatSelectBoxHeight);


	float dialogBoxWidth = (float)(int)(scrWidth - statusBoxWidth - 30);
	float dialogBoxHeight = 124;

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

	m_expIllusionPlayer.init(L"Images/BattleUI/EXPillusion.jpg", m_pDev);
	m_expIllusionPlayer.setSize(statusBarWidth, statusBarHeight);
	m_expIllusionPlayer.setPos (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.48f, 4.8f);


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
	D3DXCreateFont(m_pDev, 17, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("HYnamL"), &m_lblStatSelect);


	m_noneSkillSelectedCount = 0;

	m_statSelect = SS_HEALTH;
	m_statCount = 0;

	m_curTurnType = TT_NATURAL;
	m_nextTurnType = TT_NATURAL;
}

BattleState::~BattleState()
{
	release();
}

/* ���� ���� �� ���� �Լ��� ȣ���*/
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
	m_expBarPlayer.initRate( (float) ( (Hero*)getHero() )->getMaxExp() );
	m_expIllusionPlayer.initRate( (float) ( (Hero*)getHero() )->getMaxExp() );

	m_hpBarPlayer.setRate((float)getHero()->getCurHp());
	m_hpBarEnemy.setRate((float)getEnemy()->getCurHp());
	m_mpBarPlayer.setRate((float)getHero()->getCurCs());
	m_expBarPlayer.setRate ( (float) ( (Hero*)getHero() )->getCurExp() );
	m_hpIllusionPlayer.setRate((float)getHero()->getCurHp());
	m_hpIllusionEnemy.setRate((float)getEnemy()->getCurHp());
	m_mpIllusionPlayer.setRate((float)getHero()->getCurCs());
	m_expIllusionPlayer.setRate( (float) ( (Hero*)getHero() )->getCurExp() );


	/*��ų ��� ����*/
	SkillSet* skillSet = getHero()->getSkillSet();
	skillSet->setCharacter (getHero(), getEnemy());
	skillSet->setBattleState(this);
	m_noneSkillSelectedCount = 0;
	m_levelProgress = false;
	m_levelUpFlag = false;
	m_SkillContentBox.setOff();
	m_StatSelectBox.setOff();

	setupCamera();
	

	m_battleLog.push_back(std::string("���� ����~~~~~~~~~!!!"));
	m_battleWinner = PS_NOTSET;
	setNextTurnType( TT_PLAYER );
	passTurn();


	// TODO Anim test
	ArnNode* guardBallNode = getHero()->getArnMesh()->getNodeByName("Sphere.004");
	((ArnMesh*)guardBallNode)->setDoAnim( true );
	((ArnMesh*)guardBallNode)->setVisible( true );
	return S_OK;
}

HRESULT BattleState::leave()
{
	m_startTime = -1.0f;
	m_battleLog.clear();

	GetAudioState().leaveBattle();

	getHero()->setControllable( true );
	
	ArnMesh* guardBallNode = (ArnMesh*)(getHero()->getArnMesh()->getNodeByName("Sphere.004"));
	guardBallNode->setVisible( false );
	return S_OK;
}

HRESULT BattleState::frameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime)
{
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetTransform(D3DTS_VIEW, &GetG().g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &GetG().g_orthoProjMat);

	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	GetSpriteManager().frameRenderSpecificSprite( "BattleUI" );

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
	
	m_StatSelectBox.draw();
	m_SkillContentBox.draw();
	
	m_hpBgPlayer.draw();
	m_mpBgPlayer.draw();
	m_expBgPlayer.draw();
	m_hpBgEnemy.draw();
	m_mpBgEnemy.draw();

	m_hpIllusionPlayer.draw();
	m_hpIllusionEnemy.draw();
	m_mpIllusionPlayer.draw();
	m_expIllusionPlayer.draw();


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

	m_SkillContentBox.frameMove(fElapsedTime);
	m_StatSelectBox.frameMove( fElapsedTime );
	m_hpBgPlayer.frameMove(fElapsedTime);
	m_mpBgPlayer.frameMove(fElapsedTime);
	m_expBgPlayer.frameMove(fElapsedTime);
	m_hpBgEnemy.frameMove(fElapsedTime);
	m_mpBgEnemy.frameMove(fElapsedTime);

	m_hpIllusionPlayer.frameMove(fElapsedTime);
	m_hpIllusionEnemy.frameMove(fElapsedTime);
	m_mpIllusionPlayer.frameMove(fElapsedTime);
	m_expIllusionPlayer.frameMove( fElapsedTime );

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
	m_expBarPlayer.changeRate( (float) ( (Hero*)getHero() )->getCurExp() );
	m_expIllusionPlayer.changeRate( (float) ( (Hero*)getHero() )->getCurExp() );


	// TODO Anim test

	ArnNode* guardBallNode = getHero()->getArnMesh()->getNodeByName("Sphere.004");
	guardBallNode->update( fTime, fElapsedTime );
	if ( ((ArnXformable*)guardBallNode)->isAnimSeqEnded() )
	{
		((ArnXformable*)guardBallNode)->resetAnimSeqTime();
		((ArnXformable*)guardBallNode)->setDoAnim( true );
	}
	
	//// WorldState�� �����ϱ� ���� �ڵ�.
	//TopStateManager& tsm = TopStateManager::getSingleton();
	//WorldState* ws = static_cast<WorldState*>( tsm.getCurState() );
	//const D3DXVECTOR3& vEnemyPos = ws->getEnemyPos();
	//const D3DXVECTOR3& vHeroPos = ws->getHeroPos();

	// ���ΰ��� ���� �־����ų�(�浹 ���°� �ƴϰų�), ���ڰ� �����Ǹ� FieldState�� ���ư���.
	//if ( ws->isCollide( &vEnemyPos, &vHeroPos ) == false || m_battleWinner != PS_NOTSET )

	// ���ڰ� �����Ǹ� FieldState�� ���ư���.
	if ( m_battleWinner != PS_NOTSET )
		GetWorldStateManager().setNextState(GAME_WORLD_STATE_FIELD);

	return S_OK;
}

HRESULT BattleState::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//m_hpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	//m_mpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	//m_expBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	//m_mpBarEnemy.handleMessages(hWnd, uMsg, wParam, lParam);





	if (uMsg == WM_KEYUP)
	{
		if ( m_StatSelectBox.isOn() )
		{
			if (wParam == VK_UP)
			{
				statSelectMove('u');
			}
			if (wParam == VK_DOWN)
			{
				statSelectMove('d');
			}
			if (wParam == VK_RETURN)
			{
				Stat retStat = getHero()->getStat();
				switch (m_statSelect)
				{
				case SS_HEALTH:
					if (m_statCount != 0)
					{
						m_statCount--;
						retStat.health ++;
						getHero()->setStat(retStat.health, retStat.will, retStat.coding, retStat.def, retStat.sense, retStat.immunity);
					}
					break;
				case SS_WILL:
					if (m_statCount != 0)
					{
						m_statCount--;
						retStat.will ++;
						getHero()->setStat(retStat.health, retStat.will, retStat.coding, retStat.def, retStat.sense, retStat.immunity);
					}
					break;
				case SS_CODING:
					if (m_statCount != 0)
					{
						m_statCount--;
						retStat.coding ++;
						getHero()->setStat(retStat.health, retStat.will, retStat.coding, retStat.def, retStat.sense, retStat.immunity);
					}
					break;
				case SS_DEF:
					if (m_statCount != 0)
					{
						m_statCount--;
						retStat.def ++;
						getHero()->setStat(retStat.health, retStat.will, retStat.coding, retStat.def, retStat.sense, retStat.immunity);
					}
					break;
				case SS_SENSE:
					if (m_statCount != 0)
					{
						m_statCount--;
						retStat.sense ++;
						getHero()->setStat(retStat.health, retStat.will, retStat.coding, retStat.def, retStat.sense, retStat.immunity);
					}
					break;
				case SS_IMMUNITY:
					if (m_statCount != 0)
					{
						m_statCount--;
						retStat.immunity ++;
						getHero()->setStat(retStat.health, retStat.will, retStat.coding, retStat.def, retStat.sense, retStat.immunity);
					}
					break;
				case SS_EXIT:
					GetWorldStateManager().setNextState(GAME_WORLD_STATE_FIELD);
					getEnemy()->setRemoveFlag( true ); 
					return S_OK;
				}

			}

			return S_OK;
		}
		/*�׾��� �� enter Ű�� �Է��ϸ� ��� �ı�, �ƴϸ� �ٸ� Ű �� �ް� �޽��� �ڵ鸵 ����*/
		if ( getEnemy()->getCurHp() <= 0 && !getEnemy()->getSoulAnimation() )
		{
			if (wParam == VK_RETURN)
			{
				Hero* hero = ( Hero* )getHero();
				Enemy* enemy = ( Enemy* )getEnemy();

				if (m_levelUpFlag == true)
				{
					hero->levelUp();
					m_expBarPlayer.initRate( (float) ( (Hero*)getHero() )->getMaxExp() );
					m_expIllusionPlayer.initRate( (float) ( (Hero*)getHero() )->getMaxExp() );
					m_expIllusionPlayer.setRate( (float) ( (Hero*)getHero() )->getCurExp() );
					m_expBarPlayer.setRate ( (float) ( (Hero*)getHero() )->getCurExp() );

					m_statCount += 5;
				}


				
				int expReward = enemy->getExpReward();

				if (expReward == 0)
				{
					GetWorldStateManager().setNextState(GAME_WORLD_STATE_FIELD);
					getEnemy()->setRemoveFlag( true ); // Should be deleted before next frame update
				}

				int remainExp = hero->gainExp( expReward );
				printf("remainExp : %d\n" , remainExp);

				//�������� �ϰ� ����ġ�� ���� �ʾ��� ��
				if ( remainExp == -1 )
				{
					pushBattleLog("������!");
					enemy->setExpReward( 0 );
					m_levelUpFlag = true;
				}
				//�������� �ϰ� ����ġ�� ������ ��
				else if ( remainExp > 0 )
				{
					pushBattleLog("������!");
					enemy->setExpReward( remainExp );
					m_levelUpFlag = true;
				}
				else
				{
					enemy->setExpReward( 0 );
					if (m_levelUpFlag == true)
					{
						m_StatSelectBox.onBox();
						m_levelUpFlag = false;
						return S_OK;
					}

					return S_OK;

				}
			}
			return S_OK;
		}
		/*�ڽ��� �׾��� �� ��� Ű�ε� �������� �ʴ´�. �޷� */
		else if (getHero()->getCurHp() <= 0 && !getHero()->getSoulAnimation() )
		{
			GetTopStateManager().setNextState( GAME_TOP_STATE_CREDIT );
			return S_OK;
		}
		/*�ڽ��� ���ʰ� �ƴ� ������ �������� �ʴ´�.*/
		else if (m_curTurnType != TT_PLAYER)
		{
			return S_OK;
		}

		SkillSet* skillSet = this->getHero()->getSkillSet();
		skillSet->setCharacter( getHero(), getEnemy() );

		/*ȭ��ǥ�� ���� ��� �б�*/
		if (wParam == VK_UP)
		{
			skillSet->moveSkillLocation('u');
		}
		if (wParam == VK_DOWN)
		{
			skillSet->moveSkillLocation('d');
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


			if (!(skillSet->useSkill()))
			{
				/*�̺κп� ��ų�� �����ϴ� ��� ���̾�α׸� �߰��Ҽ���.*/
				switch (m_noneSkillSelectedCount)
				{
				case 0:
					m_battleLog.push_back(std::string("��ų�� �����ϴ�."));
					break;
				case 1:
					m_battleLog.push_back(std::string("��ų�� ���µ���?"));
					break;
				case 2:
					m_battleLog.push_back(std::string("��ų�� ���ٴϱ��!!?"));
					break;
				case 3:
					m_battleLog.push_back(std::string("���� ��ų �ڲ� ������ ���ǰ� �����̳�!"));
					break;
				case 4:
					m_battleLog.push_back(std::string("�o��???"));
					break;
				case 5:
					m_battleLog.push_back(std::string("..."));
					break;
				case 6:
					m_battleLog.push_back(std::string("..."));
					break;
				case 7:
					m_battleLog.push_back(std::string("��¿ �� ����. �� ���� ���� ��������."));
					break;
				case 8:
					m_battleLog.push_back(std::string("���� ������ ������ ��ī���� �����ϴ� �� �ϴ�."));
					break;
				case 9:
					m_battleLog.push_back(std::string("��ī���� �����Ϸ��� �ʿ� �����ϴ� ������ ������ Ǯ��� �Ѵ�."));
					break;
				case 10:
					m_battleLog.push_back(std::string("���� �������� ������ ���� ������ �ִ��� ���� ��ܳ����ʽÿ�."));
					break;
				case 11:
					m_battleLog.push_back(std::string("�ŵ� ���̳����� �ż��մϴ�. �������ֽ���."));
					break;
				case 12:
					m_battleLog.push_back(std::string("��������"));
					break;
				case 13:
					m_battleLog.push_back(std::string("������ ��� �Ź̸� �������ϴ� �� �ϴ�."));
					break;
				case 14:
					m_battleLog.push_back(std::string("����, ���� ��ũ��Ʈ�� �𸣱� ���� �̷� switch�� �밡�ٸ� �Ѱ� �ƴϾ�!"));
					break;
				case 15:
					m_battleLog.push_back(std::string("goto���� �����սô�."));
					break;
				case 16:
					m_battleLog.push_back(std::string("1�� 1�ڵ�."));
					break;
				case 17:
					m_battleLog.push_back(std::string("������ ���ݸ� �սô�."));
					break;
				case 18:
					m_battleLog.push_back(std::string("�ڵ忡 �ּ��� �ʼ�."));
					break;
				case 19:
					m_battleLog.push_back(std::string("���� ���� OOO���� ������Ʈ ���� ���� �־����ϴ�."));
					break;
				case 20:
					m_battleLog.push_back(std::string("��������"));
					break;
				case 21:
					m_battleLog.push_back(std::string("�Ϻ� ��ų���� ���ӱ�� �� �� �ֽ��ϴ�."));
					break;
				case 22:
					m_battleLog.push_back( std::string( "OOO���� ���� EP�� �����, DXUT�� ��Ÿ�� ���ϴ� ���� ���ظ� ���߽��ϴ�." ));
					break;
				default:
					m_battleLog.push_back(std::string("��, �� �̻��� �ڼ��� ���� �����Ѵ�."));;
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
	m_sprite->release();

	m_SkillContentBox.release();
	m_StatSelectBox.release();
	m_hpBgPlayer.release();
	m_mpBgPlayer.release();
	m_expBgPlayer.release();
	m_hpBgEnemy.release();
	m_mpBgEnemy.release();

	m_hpIllusionPlayer.release();
	m_hpIllusionEnemy.release();
	m_mpIllusionPlayer.release();
	m_expIllusionPlayer.release ();

	m_hpBarPlayer.release();
	m_mpBarPlayer.release();
	m_expBarPlayer.release();

	m_hpBarEnemy.release();
	m_mpBarEnemy.release();

	m_innerFire.release();

	
	SAFE_RELEASE( m_lblHYnamL );
	SAFE_RELEASE( m_lblREB );
	SAFE_RELEASE( m_lblSkill );
	SAFE_RELEASE( m_lblSkillDescription );
	SAFE_RELEASE( m_lblStatSelect );

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
	rc.top += (LONG)21.08;
	StringCchPrintf(textBuffer, 512, L"Level : %d", ( ( Hero* )getHero() )->getLevel() );
	m_lblREB->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );


	/*�� status �� �׸���*/
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

	SkillSet* skillSet = getHero()->getSkillSet();

	StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_FIRST).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_SECOND).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_THIRD).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_FOURTH).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	rc.top += skillLineInterval;
	StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_FIFTH).c_str());
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );

	/*��ų �� �׸���*/
	switch (skillSet->getSkillLocation())
	{
	case SL_FIRST:
		rc.top = scrHeight - 190;
		StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_FIRST).c_str());
		break;
	case SL_SECOND:
		rc.top = scrHeight - 190 + skillLineInterval;
		StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_SECOND).c_str());
		break;
	case SL_THIRD:
		rc.top = scrHeight - 190 + skillLineInterval*2;
		StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_THIRD).c_str());
		break;
	case SL_FOURTH:
		rc.top = scrHeight - 190 + skillLineInterval*3;
		StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_FOURTH).c_str());
		break;
	case SL_FIFTH:
		rc.top = scrHeight - 190 + skillLineInterval*4;
		StringCchPrintf(textBuffer, 512, skillSet->getSkillName(SL_FIFTH).c_str());
		break;
	}
	m_lblSkill->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 0.0f, 1.0f, 1.0f ) );


	rc.top = scrHeight - 190;
	rc.left = scrWidth - 410;

	switch (skillSet->getSkillLocation())
	{
	case SL_FIRST:
		StringCchPrintf(textBuffer, 512, skillSet->getDescription(SL_FIRST).c_str());
		break;
	case SL_SECOND:
		StringCchPrintf(textBuffer, 512, skillSet->getDescription(SL_SECOND).c_str());
		break;
	case SL_THIRD:
		StringCchPrintf(textBuffer, 512, skillSet->getDescription(SL_THIRD).c_str());
		break;
	case SL_FOURTH:
		StringCchPrintf(textBuffer, 512, skillSet->getDescription(SL_FOURTH).c_str());
		break;
	case SL_FIFTH:
		StringCchPrintf(textBuffer, 512, skillSet->getDescription(SL_FIFTH).c_str());
		break;
	}

	if (m_SkillContentBox.isOn())
		m_lblSkillDescription->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 0.8f, 0.8f, 1.0f, 1.0f ) );


	if ( m_StatSelectBox.isOn() )
	{
		int statSelectX = scrWidth/2 + 10;
		int statSelectY = scrHeight/2 - 150;
		int statSelectInterval = 25;
		rc.top = statSelectY;
		rc.left = statSelectX;


		StringCchPrintf(textBuffer, 512, L"HEALTH");
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"WILL");
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"CODING");
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"DEFENSE");
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"SENSE");
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"IMMUNITY");
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"exit");
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;



		rc.top = statSelectY;

		switch (m_statSelect)
		{
		case SS_HEALTH:
			StringCchPrintf(textBuffer, 512, L"HEALTH");
			break;
		case SS_WILL:
			rc.top += statSelectInterval * 1;
			StringCchPrintf(textBuffer, 512, L"WILL");
			break;
		case SS_CODING:
			rc.top += statSelectInterval * 2;
			StringCchPrintf(textBuffer, 512, L"CODING");
			break;
		case SS_DEF:
			rc.top += statSelectInterval * 3;
			StringCchPrintf(textBuffer, 512, L"DEFENSE");
			break;
		case SS_SENSE:
			rc.top += statSelectInterval * 4;
			StringCchPrintf(textBuffer, 512, L"SENSE");
			break;
		case SS_IMMUNITY:
			rc.top += statSelectInterval * 5;
			StringCchPrintf(textBuffer, 512, L"IMMUNITY");
			break;
		case SS_EXIT:
			rc.top += statSelectInterval * 6;
			StringCchPrintf(textBuffer, 512, L"exit");
			break;
		}

		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );



		rc.top = statSelectY;
		rc.left = statSelectX + 100;

		StringCchPrintf(textBuffer, 512, L"%d", getHero()->getStat().health);
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"%d", getHero()->getStat().will);
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"%d", getHero()->getStat().coding);
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"%d", getHero()->getStat().def);
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"%d", getHero()->getStat().sense);
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval;
		StringCchPrintf(textBuffer, 512, L"%d", getHero()->getStat().immunity);
		m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		rc.top += statSelectInterval * 2;

		StringCchPrintf( textBuffer, 512, L"%d", m_statCount );
		if (m_statCount != 0)
		{
			m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		}
		else
		{
			m_lblStatSelect->DrawTextW(0, textBuffer, -1, &rc, DT_NOCLIP | DT_LEFT, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
		}




	}

}

void BattleState::passTurn()
{
	m_curTurnType = m_nextTurnType;
	m_nextTurnType = TT_NATURAL;

	if (m_curTurnType == TT_PLAYER)
	{
		m_battleLog.push_back(std::string("����� ������ �����Դϴ�."));
		getHero()->recoverCs();
	}
	else if (m_curTurnType == TT_COMPUTER)
	{
		m_battleLog.push_back(std::string("��ǻŸ �����Դϴ�."));
		doComputerAction();
	}
}

void BattleState::doComputerAction()
{
	getEnemy()->doNormalAttack(0, getHero());
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
	// ó�� ���۽� ���ΰ��� ���� �Ʒ�, ���� ������ ���� ���̵��� ī�޶� �����δ�.
	//

	// WorldState�� �����ϱ� ���� �ڵ�.
	TopStateManager& tsm = TopStateManager::getSingleton();
	World* ws = GetWorldManager().getCurWorld();
	const D3DXVECTOR3& vEnemyPos = GetWorldManager().getCurWorld()->getEnemyPos();
	const D3DXVECTOR3& vHeroPos = ws->getHeroPos();

	GetG().m_camera.beginShoulderLookCamera( &vHeroPos, &vEnemyPos );
}

void BattleState::statSelectMove (char choice)
{
	if (choice == 'u')
	{
		switch (m_statSelect)
		{
		case SS_HEALTH :
			m_statSelect = SS_EXIT;
			break;
		case SS_WILL :
			m_statSelect = SS_HEALTH;
			break;
		case SS_CODING :
			m_statSelect = SS_WILL;
			break;
		case SS_DEF :
			m_statSelect = SS_CODING;
			break;
		case SS_SENSE :
			m_statSelect = SS_DEF;
			break;
		case SS_IMMUNITY :
			m_statSelect = SS_SENSE;
			break;
		case SS_EXIT :
			m_statSelect = SS_IMMUNITY;
			break;
		}
	}
	else if (choice == 'd')
	{
		switch (m_statSelect)
		{
		case SS_HEALTH :
			m_statSelect = SS_WILL;
			break;
		case SS_WILL :
			m_statSelect = SS_CODING;
			break;
		case SS_CODING :
			m_statSelect = SS_DEF;
			break;
		case SS_DEF :
			m_statSelect = SS_SENSE;
			break;
		case SS_SENSE :
			m_statSelect = SS_IMMUNITY;
			break;
		case SS_IMMUNITY :
			m_statSelect = SS_EXIT;
			break;
		case SS_EXIT :
			m_statSelect = SS_HEALTH;
			break;
		}
	}
	else if (choice == 'i')
	{
		m_statSelect = SS_EXIT;
	}

}
