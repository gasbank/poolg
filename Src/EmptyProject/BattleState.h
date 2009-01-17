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
class SkillSet;

enum SubBattleState { SBS_NULLSTATE, SBS_PLAYER_TURN, SBS_ENEMY_TURN, SBS_LEVEL_UP };
enum StatSelect { SS_HEALTH, SS_WILL, SS_CODING, SS_DEF, SS_SENSE, SS_IMMUNITY, SS_EXIT };

typedef std::list<Enemy*> EnemyList;



/**
@brief 전투 State
*/
class BattleState : public State
{
public:
							BattleState();
							~BattleState();

	virtual HRESULT			enter( double dStartTime );
	virtual HRESULT			leave();
	virtual HRESULT			frameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime );
	virtual HRESULT			frameMove( double dTime, float fElapsedTime );
	virtual HRESULT			handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual HRESULT			onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	virtual HRESULT			onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	virtual void			onLostDevice();
	virtual HRESULT			release();

	void					renderFixedText( int scrWidth, int scrHeight );

	void					passTurn();
	void					pushBattleLog( const char* log ) { m_battleLog.push_back( log ); }	
	void					statSelectMove( char choice );
	
	void					insertEnemy( Enemy* enemy ) { m_enemies.push_back( enemy ); }

	Enemy*					getFirstEnemy() { if ( m_enemies.empty() ) return 0; else return *m_enemies.begin(); }

private:
	enum InputEnum
	{
		BSI_SKILL_MOVE_UP		= 0,
		BSI_SKILL_MOVE_DOWN		= 1,
		BSI_SKILL_USE			= 2,
		BSI_SKILLCONTENT_SHOW	= 3,
		BSI_SKILLCONTENT_HIDE	= 4,

		BSI_LEVELUP_UP			= 5,
		BSI_LEVELUP_DOWN		= 6,
		BSI_LEVELUP_SET			= 7,

		BSI_MAX_KEYS,
		BSI_UNKNOWN				= 0xff
	};

	InputEnum				mapKey( UINT nKey ) const;

	void					frameMoveUserInterfaces( double dTime, float fElapsedTime );
	void					printBattleStateEnterDebugMessage();
	void					doEnemyAction();
	Character*				getHero();
	void					setupCamera();
	void					updateBarRate();
	void					printEasterEggMessage();

	void					handleLevelUpState();
	void					handlePlayerTurnState();
	void					handleEnemyTurnState();

	SubBattleState			m_curSubBattleState;
	SubBattleState			m_nextSubBattleState;

	const SkillSet*			m_heroSkillSet;
	UINT					m_curSelSkill;		// Currently selected skill slot index

	EnemyList				m_enemies;			// Enemy pool of this battle session. Should be cleared at BattleState::leave
	EnemyList				m_deadEnemies;

	BYTE					m_aKeys[BSI_MAX_KEYS];
	UINT					m_cKeysDown;

	/*이곳부터가 컴포넌트*/
	Sprite*					m_sprite;

	DrawRequest*			m_skillContentBox;
	DrawRequest*			m_statSelectBox;

	WindowMover*			m_skillContentBoxMover;
	WindowMover*			m_statSelectBoxMover;


	DrawRequest*			m_hpBarPlayer;
	DrawRequest*			m_csBarPlayer;
	DrawRequest*			m_expBarPlayer;

	DrawRequest*			m_hpBarEnemy;
	DrawRequest*			m_csBarEnemy;

	DrawRequest*			m_hpIllusionPlayer;
	DrawRequest*			m_csIllusionPlayer;
	DrawRequest*			m_expIllusionPlayer;

	DrawRequest*			m_hpIllusionEnemy;
	DrawRequest*			m_csIllusionEnemy;


	ProgressUi*				m_hpBarPlayerProg;
	ProgressUi*				m_csBarPlayerProg;
	ProgressUi*				m_expBarPlayerProg;
	ProgressUi*				m_hpBarEnemyProg;
	ProgressUi*				m_csBarEnemyProg;

	ProgressUi*				m_hpIllusionPlayerProg;
	ProgressUi*				m_csIllusionPlayerProg;
	ProgressUi*				m_expIllusionPlayerProg;
	ProgressUi*				m_hpIllusionEnemyProg;
	ProgressUi*				m_csIllusionEnemyProg;


	StringList				m_battleLog;
	
	InnerFire*				m_innerFire;

	int						m_noneSkillSelectedCount;
	bool					m_levelProgress;
	bool					m_levelUpFlag;

	//StatSelect 관련
	StatSelect				m_statSelect;
	int						m_statCount;

	float					m_desaturation; // Screen goes to grayscale when player defeated.
};
