#pragma once
#include "Box.h"
#include "StatusBar.h"
#include "ProgressBar.h"
#include "StatusBar.h"

class Battle
{
public:
	void init (LPDIRECT3DDEVICE9& d3dDev, int scrWidth, int scrHeight);
	void frameMove (float fElapsedTime);
	void draw (int scrWidth, int scrHeight);
	void drawFixedText (int scrWidth, int scrHeight);
	void handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void release ();

	
private:
	/*이곳부터가 컴포넌트*/
	Box				m_StatusBoxPlayer;
	Box				m_StatusBoxEnemy;

	Box				m_SkillBox;
	Box				m_SkillContentBox;
	Box				m_DialogBox;

	Box				m_hpBgPlayer;
	Box				m_mpBgPlayer;
	Box				m_expBgPlayer;

	Box				m_hpBgEnemy;
	Box				m_mpBgEnemy;

	HPBarPlayer		m_hpBarPlayer;
	MPBarPlayer		m_mpBarPlayer;
	EXPBarPlayer	m_expBarPlayer;
	
	HPBarEnemy		m_hpBarEnemy;
	MPBarEnemy		m_mpBarEnemy;

	LPD3DXFONT		m_lblHYnamL;
	LPD3DXFONT		m_lblREB;
	
	LPDIRECT3DDEVICE9	m_pDev;

	//[윤욱]
	Box m_menu;
	Box m_selc;
};