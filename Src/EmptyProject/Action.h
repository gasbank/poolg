#pragma once
#include "TileManager.h"
class WorldState;
class Enemy;
class Unit;

class Action
{
public:
	virtual ~Action(void);

	virtual void activate() = 0;
	virtual void update() = 0;

	void dialogAction( const char* dialogName );
	void moveUnitAction();
	void battleAction( Enemy* oppCharacter );
	void screenAction();
	void soundAction( std::string sz );
	void createUnitAction( int x, int y, bool random );

	WorldState* getWs() { return m_ws; }
protected:
	Action(void);
private:
	
	WorldState* m_ws;
};

SCRIPT_FACTORY( Action )

//////////////////////////////////////////////////////////////////////////

class BattleAction : public Action
{
public:
	BattleAction( const Unit* targetUnit, float dist );
	virtual ~BattleAction(void);

	virtual void activate();
	virtual void update();

private:
	const Unit* m_targetUnit;
	float m_dist;
};

//////////////////////////////////////////////////////////////////////////

class DialogAction : public Action
{
public:
	DialogAction( const char* dialogName );
	virtual ~DialogAction(void);

	virtual void activate();
	virtual void update();

private:
	std::string m_dialogName;
};

//////////////////////////////////////////////////////////////////////////

class SoundAction : public Action
{
public:
	SoundAction( const char* soundName );
	virtual ~SoundAction(void);

	virtual void activate();
	virtual void update();

private:
	std::string m_soundName;
};

//////////////////////////////////////////////////////////////////////////

class HealAction : public Action
{
public:
	HealAction( Character* targetChar );
	virtual ~HealAction();

	virtual void activate();
	virtual void update();

private:
	Character* m_targetChar;
};