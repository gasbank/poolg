#pragma once

#include "Character.h"

class Npc : public Character
{
public:
	~Npc() {}
protected:
	Npc( UnitType type ) : Character( type ) {}
	

};
