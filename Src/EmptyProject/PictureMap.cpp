#include "EmptyProjectPCH.h"
#include "PictureMap.h"

PictureMap::PictureMap(void)
{
}

PictureMap::~PictureMap(void)
{
}

PictureInput PictureMap::mapKey( UINT nKey )
{
	switch( nKey )
	{
	case L'J':	return PIC_MOVE_LEFT;
	case L'L':	return PIC_MOVE_RIGHT;
	case L'I':	return PIC_MOVE_UP;
	case L'K':	return PIC_MOVE_DOWN;
	}

	return PIC_UNKNOWN;
}