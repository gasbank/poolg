#pragma once
#include "Box.h"
#include "StatusBar.h"
#include "ProgressBar.h"
#include "StatusBar.h"
#include "State.h"

class BattleState : public State
{
public:
	HRESULT enter ();

	HRESULT frameRender (IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	HRESULT frameMove (double fTime, float fElapsedTime);

	HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT release ();

	void drawFixedText (int scrWidth, int scrHeight);
	
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

};