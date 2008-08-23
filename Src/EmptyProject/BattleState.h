#pragma once

#include "StatusBar.h"
#include "ProgressBar.h"
#include "StatusBar.h"
#include "State.h"
#include "InnerFire.h"
#include "SkillDescBox.h"

class SkillSet;
class World;
class Action;

enum TurnType { TT_NATURAL, TT_COMPUTER, TT_PLAYER };
enum PlayerSide { PS_NOTSET, PS_COMPUTER, PS_PLAYER };
enum StatSelect { SS_HEALTH, SS_WILL, SS_CODING, SS_DEF, SS_SENSE, SS_IMMUNITY, SS_EXIT };

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

	void setNextTurnType( TurnType tt ) { m_nextTurnType = tt; }
	void passTurn();
	void pushBattleLog( const char* log ) { m_battleLog.push_back( log ); }	
	void statSelectMove (char choice);
	

private:

	void doComputerAction();
	Character* getHero();
	Character* getEnemy();
	void setupCamera();

	/*이곳부터가 컴포넌트*/
	Picture				m_StatusBoxPlayer;
	Picture				m_StatusBoxEnemy;

	Picture				m_SkillBox;
	SkillDescBox		m_SkillContentBox;
	SkillDescBox		m_StatSelectBox;
	Picture				m_DialogBox;

	Picture				m_hpBgPlayer;
	Picture				m_mpBgPlayer;
	Picture				m_expBgPlayer;

	Picture				m_hpBgEnemy;
	Picture				m_mpBgEnemy;

	IllusionBar			m_hpIllusionPlayer;
	IllusionBar			m_hpIllusionEnemy;
	IllusionBar			m_mpIllusionPlayer;
	IllusionBar			m_expIllusionPlayer;

	ProgressBar			m_hpBarPlayer;
	ProgressBar			m_mpBarPlayer;
	ProgressBar			m_expBarPlayer;
	
	ProgressBar			m_hpBarEnemy;
	ProgressBar			m_mpBarEnemy;

	LPD3DXFONT			m_lblHYnamL;
	LPD3DXFONT			m_lblREB;
	LPD3DXFONT			m_lblSkill;
	LPD3DXFONT			m_lblSkillDescription;
	LPD3DXFONT			m_lblStatSelect;

	
	LPDIRECT3DDEVICE9	m_pDev;

	typedef std::list<std::string> StringList;
	StringList m_battleLog;
	
	TurnType		m_curTurnType;
	TurnType		m_nextTurnType;
	PlayerSide		m_battleWinner;


	InnerFire		m_innerFire;

	int				m_noneSkillSelectedCount;
	bool			m_levelProgress;
	bool			m_levelUpFlag;

	//StatSelect 관련
	StatSelect		m_statSelect;
	int				m_statCount;
};
