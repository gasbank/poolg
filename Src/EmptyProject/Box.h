#pragma once
#include "Picture.h"

class Box : public Picture
{
public:

	void setPosition (float x, float y, float z)
	{m_vPos.x = x; m_vPos.y = y; m_vPos.z = z;}
};
