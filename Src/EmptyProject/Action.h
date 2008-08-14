#pragma once

class Action
{
public:
	Action(void);
	~Action(void);

	void dialogAction();
	void unitMoveAction();
	void battleAction();
	void screenAction();
	void soundAction();
};