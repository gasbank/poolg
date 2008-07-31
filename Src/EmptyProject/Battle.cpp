#include "EmptyProjectPCH.h"
#include "Battle.h"

void Battle::init (LPDIRECT3DDEVICE9& d3dDev, int scrWidth, int scrHeight)
{
	m_backGround.init(L"BattleUI\\BGcampus.bmp", d3dDev);
	m_backGround.setPosition (-(float)scrWidth/2, -(float)scrHeight/2, 20);
	m_backGround.setSize((float)scrWidth, (float)scrHeight);

	float statusBoxWidth = (float)scrWidth*0.2344f;
	float statusBoxHeight = (float)scrHeight*0.2083f;

	float statusBoxPlayersPositionX = -(float)scrWidth/2 + 10;
	float statusBoxPlayersPositionY = -(float)scrHeight/2 + 10;
	m_StatusBoxPlayer.init(L"BattleUI\\StatusBox.jpg", d3dDev);
	m_StatusBoxPlayer.setPosition (statusBoxPlayersPositionX, statusBoxPlayersPositionY, 7);
	m_StatusBoxPlayer.setSize(statusBoxWidth, statusBoxHeight);

	float statusBoxEnemysPositionX = (float)scrWidth/2 - statusBoxWidth - 10;
	float statusBoxEnemysPositionY = (float)scrHeight/2 - statusBoxHeight - 10;
	m_StatusBoxEnemy.init(L"BattleUI\\StatusBox.jpg", d3dDev);
	m_StatusBoxEnemy.setPosition (statusBoxEnemysPositionX, statusBoxEnemysPositionY, 6.9f);
	m_StatusBoxEnemy.setSize(statusBoxWidth, statusBoxHeight);

	m_SkillBox.init(L"BattleUI\\SkillBox.jpg", d3dDev);
	m_SkillBox.setPosition (statusBoxEnemysPositionX, statusBoxEnemysPositionY, 6.9f);
	m_SkillBox.setSize(statusBoxWidth, statusBoxHeight);

	m_SkillContentBox.init(L"BattleUI\\SkillContent.jpg", d3dDev);
	m_SkillContentBox.setPosition (statusBoxEnemysPositionX, statusBoxEnemysPositionY, 6.9f);
	m_SkillContentBox.setSize(statusBoxWidth, statusBoxHeight);

	m_DialogBox.init(L"BattleUI\\DialogBox.jpg", d3dDev);
	m_DialogBox.setPosition (statusBoxEnemysPositionX, statusBoxEnemysPositionY, 6.9f);
	m_DialogBox.setSize(statusBoxWidth, statusBoxHeight);


	float statusBarWidth = statusBoxWidth * 0.72f;
	float statusBarHeight = statusBarWidth * 0.1f;
	m_hpBgPlayer.init(L"BattleUI\\HPbg.jpg", d3dDev);
	m_hpBgPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 5);
	m_hpBgPlayer.setSize(statusBarWidth, statusBarHeight);
	m_mpBgPlayer.init(L"BattleUI\\MPbg.jpg", d3dDev);
	m_mpBgPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.65f, 5);
	m_mpBgPlayer.setSize(statusBarWidth, statusBarHeight);
	m_expBgPlayer.init(L"BattleUI\\EXPbg.jpg", d3dDev);
	m_expBgPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.48f, 5);
	m_expBgPlayer.setSize(statusBarWidth, statusBarHeight);

	m_hpBgEnemy.init(L"BattleUI\\HPbg.jpg", d3dDev);
	m_hpBgEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.82f, 5);
	m_hpBgEnemy.setSize(statusBarWidth, statusBarHeight);
	m_mpBgEnemy.init(L"BattleUI\\MPbg.jpg", d3dDev);
	m_mpBgEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.65f, 5);
	m_mpBgEnemy.setSize(statusBarWidth, statusBarHeight);

	m_hpBarPlayer.init(L"BattleUI\\HPbar.jpg", d3dDev);
	m_hpBarPlayer.initRate();
	m_hpBarPlayer.setSize(statusBarWidth, statusBarHeight);
	m_hpBarPlayer.changeRate(-20);
	m_hpBarPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.82f, 4.5f);
	
	m_mpBarPlayer.init(L"BattleUI\\MPbar.jpg", d3dDev);
	m_mpBarPlayer.initRate();
	m_mpBarPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.65f, 4.5f);
	m_mpBarPlayer.setSize(statusBarWidth, statusBarHeight);

	m_expBarPlayer.init(L"BattleUI\\EXPbar.jpg", d3dDev);
	m_expBarPlayer.initRate();
	m_expBarPlayer.setPosition (statusBoxPlayersPositionX + statusBoxWidth*0.23f, statusBoxPlayersPositionY + statusBoxHeight * 0.48f, 4.5f);
	m_expBarPlayer.setSize(statusBarWidth, statusBarHeight);
	m_expBarPlayer.changeRate(-73.3f);

	m_hpBarEnemy.init(L"BattleUI\\HPbar.jpg", d3dDev);
	m_hpBarEnemy.initRate();
	m_hpBarEnemy.changeRate(-10);
	m_hpBarEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.82f, 4.5f);
	m_hpBarEnemy.setSize(statusBarWidth, statusBarHeight);

	m_mpBarEnemy.init(L"BattleUI\\MPbar.jpg", d3dDev);
	m_mpBarEnemy.initRate();
	m_mpBarEnemy.changeRate(-3);
	m_mpBarEnemy.setPosition (statusBoxEnemysPositionX + statusBoxWidth*0.23f, statusBoxEnemysPositionY + statusBoxHeight * 0.65f, 4.5f);
	m_mpBarEnemy.setSize(statusBarWidth, statusBarHeight);

	//[À±¿í]
	m_menu.init(L"menu.png", d3dDev);
	m_menu.setPosition (8.5f, -2.0f, 4);
	m_menu.setSize(9.9f, 15.75f);

	m_selc.init(L"triangle.png", d3dDev);
	m_selc.setPosition (9.5f, 11.2f, 4);
	m_selc.setSize(0.75, 1.5);




	

}

void Battle::frameMove(float fElapsedTime)
{
	m_backGround.frameMove(fElapsedTime);
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

	//[À±¿í]
	m_menu.frameMove(fElapsedTime);
	m_selc.frameMove(fElapsedTime);
	
	
	
	//m_Player.frameMove(fElapsedTime);
	//m_Enemy.frameMove(fElapsedTime);
}

void Battle::draw()
{
	m_backGround.draw();
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
	
//	D3DXMATRIX mRot, mScale, mTrans, mWorld;
//	D3DXMatrixRotationZ(&mRot, D3DXToRadian(45));
//	D3DXMatrixScaling(&mScale, 280.0f, 220.0f, 1.0f);
//	D3DXMatrixTranslation(&mTrans, 100.0f, -100.0f, 0);
//	mWorld = mScale * mRot * mTrans;
//	m_StatusBoxPlayer.setLocalXform(&mWorld);
//	
	
	//


	//[À±¿í]
	//m_menu.draw();
	//m_selc.draw();
	
}

void Battle::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	m_hpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_mpBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_expBarPlayer.handleMessages(hWnd, uMsg, wParam, lParam);
	m_mpBarEnemy.handleMessages(hWnd, uMsg, wParam, lParam);
}

void Battle::release ()
{
	m_backGround.release();
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
	
	

	//[À±¿í]
	m_menu.release();
	m_selc.release();
}
