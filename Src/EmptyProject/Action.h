#pragma once
#include "TileManager.h"

class World;
class Enemy;
class Unit;


enum ActionType { AT_BATTLE, AT_DIALOG, AT_SOUND, AT_HEAL, AT_UNITSPAWN, AT_SCRIPT };

class Action
{
public:
	virtual ~Action(void);

	virtual void activate() = 0;
	virtual void update() = 0;
	virtual void release() {}

	World* getCurWorld() const;
protected:
	Action(void);
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

//////////////////////////////////////////////////////////////////////////

class UnitSpawnAction : public Action
{
public:
	UnitSpawnAction( Unit* createUnit );
	virtual ~UnitSpawnAction();

	virtual void activate();
	virtual void update();

private:
	Unit* m_createUnit;
};

//////////////////////////////////////////////////////////////////////////

class UnitMoveAction : public Action
{
public:
	UnitMoveAction( Unit* targetUnit, std::string input );
	virtual ~UnitMoveAction();

	virtual void activate();
	virtual void update();

private:
	Unit* m_targetUnit;
	std::string m_input;
};

//////////////////////////////////////////////////////////////////////////

class ScriptAction : public Action
{
public:
	ScriptAction() {}
	virtual ~ScriptAction() {}

	void setScriptCommand( const char* command ) { m_scriptCommand = command; }

	virtual void activate();
	virtual void update();

private:
	std::string m_scriptCommand;
};

