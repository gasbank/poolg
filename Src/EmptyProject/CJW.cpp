#include "EmptyProjectPCH.h"
#include "CJW.h"

void CJW::init (LPDIRECT3DDEVICE9& d3dDev)
{
	m_backGround.init(L"bg01.jpg", d3dDev);
	m_backGround.setPosition (-0.5f, -0.5f, 20);
	m_backGround.setSize(100, 50);

	m_hpbar.init(L"hpBar.bmp", d3dDev);
	m_hpbar.initRate();
	m_hpbar.setSize(7.0f, 0.7f);
	m_hpbar.setPosition (0, 10, 5);

	m_StatusBoxPlayer.init(L"statusWindow.bmp", d3dDev);
	m_StatusBoxPlayer.setPosition (1.0f, 0.8f, 7);
	m_StatusBoxPlayer.setSize(10, 8);

	m_StatusBoxEnemy.init(L"statusWindow.bmp", d3dDev);
	m_StatusBoxEnemy.setPosition (-2.0f, -1.8f, 7);
	m_StatusBoxEnemy.setSize(10, 8);

	m_Player.init(L"poolG_r.jpg", d3dDev);
	m_Player.setPosition (0.5f, -0.8f, 10);
	m_Player.setSize(15.5f, 17.4f);

	m_Enemy.init(L"poolG_l.jpg", d3dDev);
	m_Enemy.setPosition (-2.5f, -0.2f, 10);
	m_Enemy.setSize(7.75f, 8.7f);
}

void CJW::frameMove(float fElapsedTime)
{
	m_backGround.frameMove(fElapsedTime);
	m_hpbar.frameMove(fElapsedTime);
	m_StatusBoxPlayer.frameMove(fElapsedTime);
	m_StatusBoxEnemy.frameMove(fElapsedTime);
	m_Player.frameMove(fElapsedTime);
	m_Enemy.frameMove(fElapsedTime);
}

void CJW::draw()
{
	m_backGround.draw();
	m_hpbar.draw();
	m_StatusBoxPlayer.draw();
	m_StatusBoxEnemy.draw();
	m_Player.draw();
	m_Enemy.draw();
	
}

void CJW::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	m_hpbar.handleMessages(hWnd, uMsg, wParam, lParam);
}

void CJW::release ()
{
	m_backGround.release();
	m_hpbar.release();
	m_StatusBoxPlayer.release();
	m_StatusBoxEnemy.release();
	m_Player.release();
	m_Enemy.release();
}
