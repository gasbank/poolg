#pragma once

#include "StatusBar.h"
#include "ProgressBar.h"
#include "StatusBar.h"
#include "State.h"

class WorldState;

class BattleState : public State
{
public:
	BattleState();
	~BattleState();

	HRESULT enter();
	HRESULT leave();

	HRESULT frameRender (IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	HRESULT frameMove (double fTime, float fElapsedTime);

	HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HRESULT release ();

	void renderFixedText (int scrWidth, int scrHeight);

	void passTurn();
	
private:
	/*이곳부터가 컴포넌트*/
	Picture				m_StatusBoxPlayer;
	Picture				m_StatusBoxEnemy;

	Picture				m_SkillBox;
	Picture				m_SkillContentBox;
	Picture				m_DialogBox;

	Picture				m_hpBgPlayer;
	Picture				m_mpBgPlayer;
	Picture				m_expBgPlayer;

	Picture				m_hpBgEnemy;
	Picture				m_mpBgEnemy;

	ProgressBar			m_hpBarPlayer;
	ProgressBar			m_mpBarPlayer;
	ProgressBar			m_expBarPlayer;
	
	ProgressBar			m_hpBarEnemy;
	ProgressBar			m_mpBarEnemy;

	LPD3DXFONT		m_lblHYnamL;
	LPD3DXFONT		m_lblREB;
	
	LPDIRECT3DDEVICE9	m_pDev;
	D3DXVECTOR3		m_vWorldEye;
	D3DXVECTOR3		m_vWorldLookAt;

	typedef std::list<std::string> StringList;
	StringList m_battleLog;

	WorldState*		m_ws;
};