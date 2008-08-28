#pragma once

#include "State.h"

class SkillSet;
class World;
class Action;
class Sprite;
class Enemy;
class DrawRequest;
class ProgressUi;
class WindowMover;
class InnerFire;

enum TurnType { TT_NATURAL, TT_COMPUTER, TT_PLAYER };
enum PlayerSide { PS_NOTSET, PS_COMPUTER, PS_PLAYER };
enum StatSelect { SS_HEALTH, SS_WILL, SS_CODING, SS_DEF, SS_SENSE, SS_IMMUNITY, SS_EXIT };

typedef std::set<Enemy*> EnemyList;

class BattleState : public State
{
public:

	BattleState();
	~BattleState();

	virtual HRESULT enter();
	virtual HRESULT leave();

	virtual HRESULT frameRender (IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime);
	virtual HRESULT frameMove (double fTime, float fElapsedTime);

	virtual HRESULT handleMessages (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	virtual HRESULT onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	virtual HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	virtual void onLostDevice();

	virtual HRESULT release ();

	void renderFixedText (int scrWidth, int scrHeight);

	void setNextTurnType( TurnType tt ) { m_nextTurnType = tt; }
	void passTurn();
	void pushBattleLog( const char* log ) { m_battleLog.push_back( log ); }	
	void statSelectMove (char choice);
	
	void insertEnemy( Enemy* enemy ) { m_enemies.insert( enemy ); }
	Enemy* getFirstEnemy() { assert( m_enemies.size() ); return *m_enemies.begin(); } // TODO Only one enemy supported yet!

private:

	void doComputerAction();
	Character* getHero();
	void setupCamera();
	void updateBarRate();

	EnemyList			m_enemies;			// Enemy pool of this battle session. Should be cleared at BattleState::leave

	/*이곳부터가 컴포넌트*/
	Sprite*				m_sprite;

	DrawRequest*		m_skillContentBox;
	DrawRequest*		m_statSelectBox;

	WindowMover*		m_skillContentBoxMover;
	WindowMover*		m_statSelectBoxMover;


	DrawRequest*		m_hpBarPlayer;
	DrawRequest*		m_csBarPlayer;
	DrawRequest*		m_expBarPlayer;

	DrawRequest*		m_hpBarEnemy;
	DrawRequest*		m_csBarEnemy;

	DrawRequest*		m_hpIllusionPlayer;
	DrawRequest*		m_csIllusionPlayer;
	DrawRequest*		m_expIllusionPlayer;

	DrawRequest*		m_hpIllusionEnemy;
	DrawRequest*		m_csIllusionEnemy;


	ProgressUi*			m_hpBarPlayerProg;
	ProgressUi*			m_csBarPlayerProg;
	ProgressUi*			m_expBarPlayerProg;
	ProgressUi*			m_hpBarEnemyProg;
	ProgressUi*			m_csBarEnemyProg;

	ProgressUi*			m_hpIllusionPlayerProg;
	ProgressUi*			m_csIllusionPlayerProg;
	ProgressUi*			m_expIllusionPlayerProg;
	ProgressUi*			m_hpIllusionEnemyProg;
	ProgressUi*			m_csIllusionEnemyProg;



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


	InnerFire*		m_innerFire;

	int				m_noneSkillSelectedCount;
	bool			m_levelProgress;
	bool			m_levelUpFlag;

	//StatSelect 관련
	StatSelect		m_statSelect;
	int				m_statCount;

	float			m_desaturation; // Screen goes to grayscale when player defeated.
};
