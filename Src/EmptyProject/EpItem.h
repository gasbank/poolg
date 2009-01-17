#pragma once

#include "ScriptManager.h"
#include "Unit.h"

/**
@brief 아이템을 나타내는 클래스
*/
class EpItem : public Unit
{
public:
	EpItem( BasicShapeType bst );
	EpItem( const char* arnMeshName );
	~EpItem(void);

private:
	int m_weight;
	int m_priceWeight;
	
};

SCRIPT_FACTORY( EpItem )
