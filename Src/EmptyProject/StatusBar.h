#pragma once
#include "ProgressBar.h"

class HPBarPlayer : public ProgressBar
{
public:
	void frameMove( float fElapsedTime );
};

class MPBarPlayer : public ProgressBar
{
public:
	void frameMove( float fElapsedTime );
};

class EXPBarPlayer : public ProgressBar
{
public:
	void frameMove( float fElapsedTime );
};

class HPBarEnemy : public ProgressBar
{
public:

};

class MPBarEnemy : public ProgressBar
{
public:
	void frameMove( float fElapsedTime );
};

