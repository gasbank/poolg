#pragma once
#include "ProgressBar.h"

class HPBar : public ProgressBar
{
public:
	void frameMove( float fElapsedTime );
};