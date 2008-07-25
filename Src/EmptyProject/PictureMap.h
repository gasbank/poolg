#pragma once
#include "picture.h"

class PictureMap :
	public Picture
{
public:
	PictureMap(void);
	~PictureMap(void);

protected:
	virtual PictureInput mapKey( UINT nKey );
};
