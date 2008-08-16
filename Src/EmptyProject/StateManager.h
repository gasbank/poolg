#pragma once

enum GameState
{
	NULL_STATE = 0,

	GAME_TOP_STATE_INTRO = 1000,
	GAME_TOP_STATE_PLAY,
	GAME_TOP_STATE_CREDIT,
	
	GAME_WORLD_STATE_FIELD = 3000,
	GAME_WORLD_STATE_BATTLE,
	GAME_WORLD_STATE_SCORE,
	GAME_WORLD_STATE_MENU,
};

class State;

typedef std::map<GameState, State*> StateMap;

class StateManager
{
public:
	StateManager(void);
	virtual ~StateManager(void);

	virtual void setNextState( GameState gs );
	virtual GameState curStateEnum();
	virtual GameState prevStateEnum();

	void setNextStateAsPrevState();
	State* getCurState() const;
	State* getState( GameState gs );
	void transit();

	virtual void init() = 0;
	virtual void release();

protected:
	StateMap m_states;
	State* m_prevStates;
	State* m_curStates;
	State* m_nextState;
};
