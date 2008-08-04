#pragma once

enum GameTopState {GAME_TOP_STATE_INTRO, GAME_TOP_STATE_WORLD};

class State;

class StateManager : public Singleton<StateManager>
{
public:
	StateManager(void);
	~StateManager(void);

	void init();
	void release();
	State* getCurState() const;
	void setNextState(GameTopState);
	void transit();

private:
	State** m_states;
	State* m_curStates;
	State* m_nextState;
};
