#include "EmptyProjectPCH.h"
#include "Battle.h"

void Battle::init (LPDIRECT3DDEVICE9& d3dDev)
{
	m_backGround.init(L"bg01.jpg", d3dDev);
	m_backGround.setPosition (-50.0f, -25.0f, 20);
	m_backGround.setSize(100, 50);

	
	m_hpBg.initRhw(L"BattleUI\\HPbg.jpg", d3dDev);
	m_hpBg.setSize(7.0f, 0.7f);
	m_hpBg.setPosition (0, 3, 5);

	
	m_hpBar.initRhw(L"BattleUI\\HPbar.jpg", d3dDev);
	m_hpBar.initRate();
	m_hpBar.changeRate(-30);
	m_hpBar.setSize(7.0f, 0.7f);
	m_hpBar.setPosition (0, 3, 5);

	m_mpBg.init(L"BattleUI\\MPbg.jpg", d3dDev);
	m_mpBg.setSize(7.0f, 0.7f);
	m_mpBg.setPosition (0, 0, 5);

	m_mpBar.init(L"BattleUI\\MPbar.jpg", d3dDev);
	m_mpBar.initRate();
	m_mpBar.setSize(7.0f, 0.7f);
	m_mpBar.setPosition (0, 0, 5);

	m_expBg.init(L"BattleUI\\EXPbg.jpg", d3dDev);
	m_expBg.setSize(7.0f, 0.7f);
	m_expBg.setPosition (-8, 0, 5);

	m_expBar.init(L"BattleUI\\EXPbar.jpg", d3dDev);
	m_expBar.initRate();
	m_expBar.setSize(7.0f, 0.7f);
	m_expBar.setPosition (-8, 0, 5);




	//m_StatusBoxPlayer.initRhw(L"BattleUI\\50x50.bmp", d3dDev, 50, 50);
	//m_StatusBoxPlayer.setPosition (11.0f, 7.0f, 7);
	//m_StatusBoxPlayer.setSize(0.5, 0.5);

	//m_StatusBoxEnemy.initRhw(L"BattleUI\\StatusBox.jpg", d3dDev);
	//m_StatusBoxEnemy.setPosition (-21.0f, -15.0f, 7);
	//m_StatusBoxEnemy.setSize(0.5, 0.5);

	
	//m_Player.initRhw(L"BattleUI\\StatusBox.bmp", d3dDev);
	//m_Player.setPosition (5.0f, -14.0f, 10);
	//m_Player.setSize(15.5f, 17.4f);

	//m_Enemy.initRhw(L"poolG_l.jpg", d3dDev);
	//m_Enemy.setPosition (-20.0f, 7.0f, 10);
	//m_Enemy.setSize(7.75f, 8.7f);
}

void Battle::frameMove(float fElapsedTime)
{
	m_backGround.frameMove(fElapsedTime);
	m_hpBg.frameMove(fElapsedTime);
	m_hpBar.frameMove(fElapsedTime);
	m_mpBg.frameMove(fElapsedTime);
	m_mpBar.frameMove(fElapsedTime);
	m_expBg.frameMove(fElapsedTime);
	m_expBar.frameMove(fElapsedTime);
	
	
	
	//m_StatusBoxPlayer.frameMove(fElapsedTime);
	//m_StatusBoxEnemy.frameMove(fElapsedTime);
	//m_Player.frameMove(fElapsedTime);
	//m_Enemy.frameMove(fElapsedTime);
}

void Battle::draw()
{
	m_backGround.draw();
	m_hpBg.draw();
	m_hpBar.draw();
	m_mpBg.draw();
	m_mpBar.draw();
	m_expBg.draw();
	m_expBar.draw();
	//m_StatusBoxPlayer.draw();
	//m_StatusBoxEnemy.draw();
	//m_Player.draw();
	//m_Enemy.draw();
	
}

void Battle::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	m_hpBar.handleMessages(hWnd, uMsg, wParam, lParam);
	m_mpBar.handleMessages(hWnd, uMsg, wParam, lParam);
	m_expBar.handleMessages(hWnd, uMsg, wParam, lParam);
}

void Battle::release ()
{
	m_backGround.release();
	m_hpBar.release();
	m_mpBar.release();
	m_expBar.release();
	m_hpBg.release();
	m_mpBg.release();
	m_expBg.release();
	//m_StatusBoxPlayer.release();
//	m_StatusBoxEnemy.release();
	//m_Player.release();
	//m_Enemy.release();
}
