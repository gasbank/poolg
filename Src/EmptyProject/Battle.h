#pragma once
#include "Box.h"
#include "HPbar.h"
#include "ProgressBar.h"

class Battle
{
public:
	void init (LPDIRECT3DDEVICE9& d3dDev);
	void frameMove (float fElapsedTime);
	void draw ();
	void handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void release ();

	
private:
	/*이곳부터가 컴포넌트*/
	Box m_StatusBoxPlayer;
	Box m_StatusBoxEnemy;
	Box m_backGround;

	Box m_Player;
	Box m_Enemy;
	HPBar m_hpbar;
};