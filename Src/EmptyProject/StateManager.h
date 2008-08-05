#pragma once

enum GameState {
	GAME_WORLD_STATE_FIELD, GAME_WORLD_STATE_BATTLE, GAME_WORLD_STATE_SCORE, 
	GAME_TOP_STATE_INTRO, GAME_TOP_STATE_WORLD, GAME_TOP_STATE_CREDIT};

class State;

class StateManager
{
public:
	StateManager(void);
	virtual ~StateManager(void);

	virtual void init() = 0;
	virtual void release() = 0;
	virtual void setNextState(GameState) = 0;

	State* getCurState() const;
	void transit();

protected:
	State** m_states;
	State* m_curStates;
	State* m_nextState;
};
