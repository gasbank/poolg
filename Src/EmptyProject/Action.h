#pragma once
#include "TileManager.h"

class World;
class Enemy;
class Unit;


enum ActionType { AT_BATTLE, AT_DIALOG, AT_SOUND, AT_HEAL, AT_UNITSPAWN, AT_SCRIPT, AT_FADE };

class Action
{
public:
	virtual ~Action(void);

	virtual void activate() = 0;
	virtual bool update( double dTime, float fElapsedTime ) { return false; }
	virtual void release() {}
	
	World* getCurWorld() const;

protected:
	Action(void);

private:
	virtual void onActionFinished() {}
};

SCRIPT_FACTORY( Action )

//////////////////////////////////////////////////////////////////////////

class BattleAction : public Action
{
public:
	BattleAction( const Unit* targetUnit, float dist );
	virtual ~BattleAction(void);

	virtual void activate();

private:
	const Unit* m_targetUnit;
	float m_dist;
};

//////////////////////////////////////////////////////////////////////////

class Dialog;

class DialogAction : public Action
{
public:
	DialogAction( const char* dialogName );
	virtual ~DialogAction(void);

	virtual void activate();
	virtual bool update( double dTime, float fElapsedTime );
	
private:
	virtual void onActionFinished();

	std::string m_dialogName;
	Dialog* m_dialog;
};

//////////////////////////////////////////////////////////////////////////

class SoundAction : public Action
{
public:
	SoundAction( const char* soundName );
	virtual ~SoundAction(void);

	virtual void activate();

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
	virtual bool update( double dTime, float fElapsedTime );

private:
	Unit* m_targetUnit;
	float m_activateElapsedTime;
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

private:
	std::string m_scriptCommand;
};

//////////////////////////////////////////////////////////////////////////

class FadeAction : public Action
{
public:
	FadeAction( int type, float duration ) { m_iType = type; m_fDuration = duration; }
	virtual ~FadeAction() {}

	virtual void activate();
	virtual bool update( double dTime, float fElapsedTime );
private:
	int m_iType;
	float m_fDuration;
};

//////////////////////////////////////////////////////////////////////////

class TeleportAction : public Action
{
public:
	TeleportAction( Unit* targetUnit, int x, int y );
	virtual ~TeleportAction();

	virtual void activate();

private:
	Unit* m_targetUnit;
	int m_tileX;
	int m_tileY;
};

//////////////////////////////////////////////////////////////////////////

class ArnCamera;

// type == 0 : Camera follow hero
// type == 1 : Externally defined camera

class CameraAction : public Action
{
public:
	CameraAction( int type, int duration, ArnCamera* arnCam );
	CameraAction( int type, int duration );
	virtual ~CameraAction() {}

	virtual void activate();
private:
	int m_type;
	int m_duration;
	ArnCamera* m_arnCam;
};

//////////////////////////////////////////////////////////////////////////

class ControllableAction : public Action
{
public:
	ControllableAction( Character* c, bool controllable );

	virtual void activate();
private:
	Character* m_c;
	bool m_bControllable;
};