#pragma once
class WorldState;
class Enemy;

class Action
{
public:
	Action(void);
	~Action(void);

	void dialogAction( int index );
	void unitMoveAction();
	void battleAction( Enemy* oppCharacter );
	void screenAction();
	void soundAction( std::string sz );
private:
	WorldState* m_ws;
};