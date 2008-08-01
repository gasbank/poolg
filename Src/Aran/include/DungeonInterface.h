#pragma once

struct D3DXVECTOR3;

class DungeonInterface
{
public:
	DungeonInterface(void);
	~DungeonInterface(void);

	virtual void ScrollBy( D3DXVECTOR3* dScroll ) = 0;
};
