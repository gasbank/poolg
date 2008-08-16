#pragma once

enum GameState
{
	NULL_STATE = 0,

	GAME_TOP_STATE_INTRO = 1000,
	GAME_TOP_STATE_WORLD,
	GAME_TOP_STATE_CREDIT,
	
	GAME_WORLD_STATE_FIELD = 2000,
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

protected:
	StateMap m_states;
	State* m_prevStates;
	State* m_curStates;
	State* m_nextState;

private:
	virtual void init() = 0;
	virtual void release();
};
