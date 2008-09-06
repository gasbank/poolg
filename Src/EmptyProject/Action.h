#pragma once
#include "TileManager.h"
#include "Incident.h"

class World;
class Enemy;
class Unit;
class ArnXformable;

enum ActionType { AT_BATTLE, AT_DIALOG, AT_SOUND, AT_HEAL, AT_UNITSPAWN, AT_SCRIPT, AT_FADE };

class Action
{
public:
	virtual					~Action(void);

	virtual Action*			clone() const { throw std::runtime_error( "clone() is not implemented for this class." ); }

	// Action::update() returns the validity of update.
	// It returns the false when the action is completely done since this
	// means no need to do more update of the action.
	// Otherwise, returns true when the action is on the way or not started.
	virtual bool			update( double dTime, float fElapsedTime );
	virtual void			release() {}
	virtual void			printDebugInfo() const { printf( "Debug Info not implemented on Action class\n" ); }
	
	
	World*					getCurWorld() const;
	bool					isActivated() const { return m_bActivated; }
	
protected:
							Action(void);

	virtual void			activate();
	virtual void			deactivate();

private:
	virtual void			onActionFinished() {}

	bool					m_bActivated;
};

SCRIPT_FACTORY( Action )

//////////////////////////////////////////////////////////////////////////

class BattleAction : public Action
{
public:
							BattleAction( const Unit* targetUnit, float dist );
	virtual					~BattleAction(void);


private:
	const Unit*				m_targetUnit;
	float					m_dist;
};

//////////////////////////////////////////////////////////////////////////

class Dialog;

class DialogAction : public Action
{
public:
							DialogAction( const char* dialogName );
	virtual					~DialogAction(void);

	virtual void			activate();
	virtual bool			update( double dTime, float fElapsedTime );
	
private:
	virtual void			onActionFinished();

	std::string				m_dialogName;
	Dialog*					m_dialog;
};

//////////////////////////////////////////////////////////////////////////

class SoundAction : public Action
{
public:
							SoundAction( const char* soundName );
	virtual					~SoundAction(void);

	virtual void			activate();

private:
	std::string				m_soundName;
};

//////////////////////////////////////////////////////////////////////////

class HealAction : public Action
{
public:
							HealAction( Character* targetChar, int healAmount );
	virtual					~HealAction();
	
	virtual Action*			clone() const { return new HealAction( *this ); }
	virtual void			activate();

private:
	Character*				m_targetChar;
	int						m_healAmount;
};

//////////////////////////////////////////////////////////////////////////

class UnitSpawnAction : public Action
{
public:
							UnitSpawnAction( Unit* createUnit );
	virtual					~UnitSpawnAction();

	virtual void			activate();

private:
	Unit*					m_createUnit;
};


//////////////////////////////////////////////////////////////////////////

// Should be applied blocked type incident only
class DelayAction : public Action
{
public:
							DelayAction( int delayMs );
	virtual					~DelayAction() {}

	virtual bool			update( double dTime, float fElapsedTime );

private:
	float					m_activateElapsedTime;
	int						m_delayMs; // delay in milliseconds
};


//////////////////////////////////////////////////////////////////////////

class UnitMoveAction : public Action
{
public:
							UnitMoveAction( Unit* targetUnit, std::string input );
	virtual					~UnitMoveAction();

	virtual void			activate();
	virtual bool			update( double dTime, float fElapsedTime );

private:
	Unit*					m_targetUnit;
	float					m_activateElapsedTime;
	std::string				m_input;
};

//////////////////////////////////////////////////////////////////////////

class ScriptAction : public Action
{
public:
							ScriptAction() {}
	virtual					~ScriptAction() {}

	void					setScriptCommand( const char* command ) { m_scriptCommand = command; }

	virtual void			activate();

private:
	std::string				m_scriptCommand;
};

//////////////////////////////////////////////////////////////////////////

class FadeAction : public Action
{
public:
							FadeAction( int type, float duration ) { m_iType = type; m_fDuration = duration; }
	virtual					~FadeAction() {}

	virtual void			activate();
	virtual bool			update( double dTime, float fElapsedTime );
private:
	int						m_iType;
	float					m_fDuration;
};

//////////////////////////////////////////////////////////////////////////

class TeleportAction : public Action
{
public:
							TeleportAction( Unit* targetUnit, int x, int y );
	virtual					~TeleportAction();

	virtual void			activate();

private:
	Unit*					m_targetUnit;
	int						m_tileX;
	int						m_tileY;
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
	virtual					~CameraAction() {}

	virtual void			activate();
private:
	int						m_type;
	int						m_duration;
	ArnCamera*				m_arnCam;
};

//////////////////////////////////////////////////////////////////////////

class ControllableAction : public Action
{
public:
							ControllableAction( Character* c, bool controllable );

	virtual void			activate();
private:
	Character*				m_c;
	bool					m_bControllable;
};

//////////////////////////////////////////////////////////////////////////

class StartIncidentAction : public Action
{
public:
							StartIncidentAction( Incident* incident );

	virtual bool			update( double dTime, float fElapsedTime );
	virtual void			release() { Action::release(); EP_SAFE_RELEASE( m_incident ); }
private:
	Incident*				m_incident;
};

//////////////////////////////////////////////////////////////////////////

class FlickerAction : public Action
{
public:
							FlickerAction( float durationms, float fadeDuration, D3DXCOLOR& color );

	virtual bool			update( double dTime, float fElapsedTime );

protected:
	virtual	void			activate();
	virtual void			deactivate();

private:
	float					m_durationms;
	float					m_fadeDuration;
	D3DXCOLOR				m_color;
};


//////////////////////////////////////////////////////////////////////////

// Should be applied blocked type incident only
class StartAnimationAction : public Action
{
public:
							StartAnimationAction( ArnXformable* xformableNode );
	virtual					~StartAnimationAction() {}

	virtual bool			update( double dTime, float fElapsedTime );

protected:
	virtual	void			activate();

private:
	ArnXformable*			m_xformableNode;
};
