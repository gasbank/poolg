#pragma once

class State;

class StateManager
{
public:
	StateManager(void);
	~StateManager(void);

	void init();
	void release();
	State* getCurState() const;
	void transit();

private:
	State** m_states;
	State* m_curStates;
	State* m_nextState;
};
