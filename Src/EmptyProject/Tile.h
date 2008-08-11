#pragma once

class Tile
{
public:
	Tile(void);
	~Tile(void);

	bool movable;
	bool talkable;
	bool talkonetime;
	bool heal;
};
