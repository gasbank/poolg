#pragma once
#include "TileManager.h"
#include "Incident.h"

class World;
class Enemy;
class Unit;
class ArnXformable;

enum ActionType { AT_BATTLE, AT_DIALOG, AT_SOUND, AT_HEAL, AT_UNITSPAWN, AT_SCRIPT, AT_FADE };


/**
@brief 게임의 단위 행동에 대해 정의하는 클래스

본 클래스는 게임에서 일어날 수 있는 단위 행동을 정의할 수 있도록 하는
베이스 클래스입니다. 이는 행동을 활성화하거나 비활성화 할 수 있도록 인터페이스를 제공합니다.

이러한 단위 행동은 독립적으로 쓰이지 않고 Incident 클래스를 통해서 사용하게 됩니다.
*/
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

/**
@brief Unit 과 관련된 Action
*/
class UnitAction : public Action
{
public:
	virtual					~UnitAction();

	Unit*					getUnit() const { return m_unit; }
	void					setUnit( Unit* val ) { m_unit = val; }

protected:
							UnitAction( Unit* unit ) : m_unit( unit ) {}

private:
	Unit*					m_unit;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief Character 와 관련된 Action

If there is an action which affects one character in the world,
you can create a class based on this CharacterAction class.
CharacterAction derived class can be used at Skill's onHitAction.
*/
class CharacterAction : public UnitAction
{
public:
	Character*				getCharacter() const;
	void					setCharacter( Character* val );

protected:
							CharacterAction( Character* character );
};


//////////////////////////////////////////////////////////////////////////

class Dialog;

/**
@brief 대화를 시작하는 Action

@remarks Dialog window will be shown. Player cannot abort dialog in middle.
*/
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

/**
@brief 소리를 재생하는 Action
*/
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


/**
@brief Character 의 HP를 조작하는 Action
*/
class HealAction : public CharacterAction
{
public:
							HealAction( Character* targetChar, int healAmount );
	virtual					~HealAction();
	
	virtual Action*			clone() const { return new HealAction( *this ); }
	virtual void			activate();

private:
	int						m_healAmount;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief Unit 을 등록하는 Action

Unit을 미리 생성해 두었다가 등록해야합니다. 본 액션이 유닛을 생성까지
하지는 않습니다.
*/
class UnitSpawnAction : public UnitAction
{
public:
							UnitSpawnAction( Unit* unit );
	virtual					~UnitSpawnAction();

	virtual void			activate();

};


//////////////////////////////////////////////////////////////////////////

/**
@brief 특정 밀리세컨트만큼 다음 Action 을 지연해주는 Action

@remarks Should be applied blocked type incident only
*/
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

/**
@brief Unit 을 움직이게 하는 Action
*/
class UnitMoveAction : public UnitAction
{
public:
							UnitMoveAction( Unit* unit, std::string input );
	virtual					~UnitMoveAction();

	virtual void			activate();
	virtual bool			update( double dTime, float fElapsedTime );

private:
	float					m_activateElapsedTime;
	std::string				m_input;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief 스크립트 명령어를 실행하는 Action
*/
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

/**
@brief 페이드 인, 아웃을 해주는 Action
*/
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

/**
@brief Unit 을 특정 위치로 순간이동 시켜주는 Action
*/
class TeleportAction : public UnitAction
{
public:
							TeleportAction( Unit* unit, int x, int y );
	virtual					~TeleportAction();

	virtual void			activate();

private:
	int						m_tileX;
	int						m_tileY;
};

//////////////////////////////////////////////////////////////////////////

class ArnCamera;

/**
@brief 카메라 모드를 바꿔주는 Action

type == 0 : Camera follow hero
type == 1 : Externally defined camera
*/
class CameraAction : public Action
{
public:
							CameraAction( int type, int duration, const char* arnCamNodeName );
							CameraAction( int type, int duration );
	virtual					~CameraAction() {}

	virtual void			activate();

private:
	int						m_type;
	int						m_duration;
	std::string				m_arnCamNodeName;
};

//////////////////////////////////////////////////////////////////////////

/**
@brief Character 를 조종가능한지 여부를 설정하는 Action

@remarks 일반적으로 Character 는 플레이어가 조작하는 Hero 가 될 것입니다.
*/
class ControllableAction : public CharacterAction
{
public:
							ControllableAction( Character* character, bool controllable );
							~ControllableAction();

	virtual void			activate();

private:
	bool					m_bControllable;
};

//////////////////////////////////////////////////////////////////////////


/**
@brief Incident 를 시작하는 Action
*/
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

/**
@brief 화면을 특정 색깔로 깜빡거리게 하는 Action
*/
class FlickerAction : public Action
{
public:
							FlickerAction( float durationms, float fadeDuration, D3DXCOLOR& color );

	virtual bool			update( double dTime, float fElapsedTime );
	virtual Action*			clone() const { return new FlickerAction( *this ); }

protected:
	virtual	void			activate();
	virtual void			deactivate();

private:
	float					m_durationms;
	float					m_fadeDuration;
	D3DXCOLOR				m_color;
};


//////////////////////////////////////////////////////////////////////////

/**
@brief 애니메이션 가능한 3D 메시의 애니메이션을 시작하게 하는 Action

@remarks Should be applied blocked type incident only
*/
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
