#pragma once

class Utility
{
public:
	Utility(void);
	~Utility(void);

	static void rotateAboutAxis( D3DXVECTOR3* vOut, D3DXVECTOR3* axis, float rad );
};
