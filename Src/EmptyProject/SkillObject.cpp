#include "EmptyProjectPCH.h"
#include "SkillObject.h"
#include "WorldStateManager.h"
#include "BattleState.h"
#include "Character.h"
#include "Sound.h"

SkillObject::~SkillObject(void)
{
}

BattleState* SkillObject::getBattleState()
{
	return reinterpret_cast<BattleState*>( GetWorldStateManager().getCurState() );
}
