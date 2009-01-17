#pragma once

enum GameState
{
	NULL_STATE = 0,

	GAME_TOP_STATE_INTRO = 1000,
	GAME_TOP_STATE_PLAY,
	GAME_TOP_STATE_CREDIT,
	GAME_TOP_STATE_MAIN,
	
	GAME_WORLD_STATE_FIELD = 3000,
	GAME_WORLD_STATE_BATTLE,
	GAME_WORLD_STATE_SCORE,
	GAME_WORLD_STATE_MENU,
};

class State;

typedef std::map<GameState, State*> StateMap;

/**
@brief State 인스턴스를 관리하는 클래스

다른 State로 바꾸거나 현재 State를 알고 싶을 때 사용합니다.
*/
class StateManager
{
public:
					StateManager(void);
	virtual			~StateManager(void);

	virtual void	init() = 0;
	virtual void	release();

	void			setNextState( GameState gs );
	void			setNextStateForced( GameState gs );
	GameState		curStateEnum();
	GameState		prevStateEnum();

	void			setNextStateAsPrevState();
	State*			getCurState() const;
	State*			getState( GameState gs ) const;
	void			transit( double dCurrentTime );

protected:
	StateMap		m_states;
	State*			m_prevStates;
	State*			m_curStates;
	State*			m_nextState;
};
