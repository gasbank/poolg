#pragma once

#include "Character.h"

/**
@brief NPC 를 나타내는 클래스 (미구현)
*/
class Npc : public Character
{
public:
	~Npc() {}
protected:
	Npc( UnitType type ) : Character( type ) {}
	

};
