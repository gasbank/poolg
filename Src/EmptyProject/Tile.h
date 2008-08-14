#pragma once

class Tile
{
public:
	Tile(void);
	~Tile(void);

	bool b_movable;
	bool b_eventTalk;
	bool b_heal;
	bool b_createEnemy;
};
