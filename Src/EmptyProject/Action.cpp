#include "EmptyProjectPCH.h"
#include "Action.h"
#include "WorldManager.h"
#include "World.h"
#include "WorldStateManager.h"
#include "Enemy.h"
#include "Unit.h"
#include "ScriptManager.h"
#include "Sound.h"
#include "Unit.h"
#include "Dialog.h"
#include "ArnSceneGraph.h"
#include "ArnCamera.h"
#include "Incident.h"

Action::Action(void)
{
}

Action::~Action(void)
{
}

World* Action::getCurWorld() const
{
	return GetWorldManager().getCurWorld();
}

//////////////////////////////////////////////////////////////////////////

BattleAction::BattleAction( const Unit* targetUnit, float dist )
: m_targetUnit( targetUnit ), m_dist( dist )
{

}
BattleAction::~BattleAction( void )
{

}
void BattleAction::activate()
{
	// Do one-time init of this action
}

//////////////////////////////////////////////////////////////////////////

DialogAction::DialogAction( const char* dialogName )
: m_dialogName( dialogName ), m_dialog( 0 )
{

}

DialogAction::~DialogAction( void )
{

}

void DialogAction::activate()
{
	assert ( GetWorldManager().getCurWorld()->getCurDialog() == 0 );
	m_dialog = GetWorldManager().getCurWorld()->startDialog( m_dialogName.c_str() );
}

bool DialogAction::update( double dTime, float fElapsedTime )
{
	if ( m_dialog && !m_dialog->isTalking() )
	{
		onActionFinished();
		return false;
	}
	return true;
}

void DialogAction::onActionFinished()
{
	OutputDebugString( _T( " - Dialog started by DialogAction has closed.\n" ) );
	m_dialog = 0;
}
Action* EpCreateDialogAction( const char* dialogName )
{
	return new DialogAction( dialogName );
} SCRIPT_CALLABLE_PV_PC( EpCreateDialogAction )

//////////////////////////////////////////////////////////////////////////

SoundAction::SoundAction( const char* soundName )
: m_soundName( soundName )
{
}

SoundAction::~SoundAction( void )
{
}

void SoundAction::activate()
{
	if ( m_soundName == "Start Battle" )
	{
		GetAudioState().bBGMFade = true;
		GetAudioState().bMusicFade = false;
		GetAudioState().pEngine->Stop( GetAudioState().iMusicCategory, 0 );
		GetAudioState().pSoundBank->Play( GetAudioState().iBattle[GetAudioState().nCurWorld], 0, 0, NULL );
	}
	else if ( m_soundName == "End Battle" )
	{
		GetAudioState().bBGMFade = false;
		GetAudioState().bMusicFade = true;
	}
}

Action* EpCreateSoundAction( const char* soundName )
{
	return new SoundAction( soundName );
}  SCRIPT_CALLABLE_PV_PC( EpCreateSoundAction )

//////////////////////////////////////////////////////////////////////////

HealAction::HealAction( Character* targetChar )
: m_targetChar ( targetChar )
{
}

HealAction::~HealAction()
{
}

void HealAction::activate()
{
	m_targetChar->heal( 9999 );
}

Action* EpCreateHealAction( void* targetChar )
{
	Character* c = reinterpret_cast<Character*>( targetChar );
	return new HealAction( c );
}  SCRIPT_CALLABLE_PV_PV( EpCreateHealAction )

//////////////////////////////////////////////////////////////////////////

UnitSpawnAction::UnitSpawnAction( Unit* createUnit )
: m_createUnit ( createUnit )
{
}

UnitSpawnAction::~UnitSpawnAction()
{
	EP_SAFE_RELEASE( m_createUnit );
}

void UnitSpawnAction::activate()
{
	getCurWorld()->addUnit( m_createUnit );
	m_createUnit = 0; // Unit instance ownership moved to the world!
}

Action* EpCreateUnitSpawnAction( void* createUnit )
{
	Unit* u = reinterpret_cast<Unit*>( createUnit );
	return new UnitSpawnAction( u );
}  SCRIPT_CALLABLE_PV_PV( EpCreateUnitSpawnAction )

//////////////////////////////////////////////////////////////////////////



Action* EpCreateAction( ActionType at, ... )
{
	Action* ret = 0;
	va_list params;
	int count = 0;
	
	switch (at)
	{
	case AT_BATTLE:
		count = 2;
		va_start( params, count );
		ret = new BattleAction(
			va_arg( params, Unit*),
			va_arg( params, float) );
		break;
	}
	return ret;
}


void ScriptAction::activate()
{
	GetScriptManager().execute( m_scriptCommand.c_str() );
}

Action* EpCreateScriptAction( const char* scriptCommand )
{
	ScriptAction* sa = new ScriptAction();
	sa->setScriptCommand( scriptCommand );
	return sa;
}  SCRIPT_CALLABLE_PV_PC( EpCreateScriptAction )

//////////////////////////////////////////////////////////////////////////

UnitMoveAction::UnitMoveAction( Unit* targetUnit, std::string input )
: m_targetUnit ( targetUnit ), m_input ( input ), m_activateElapsedTime( 0 )
{
}

UnitMoveAction::~UnitMoveAction()
{
}

void UnitMoveAction::activate()
{
	int i = 4;

	if ( m_input == "LEFT" )
		i = 2;
	else if ( m_input == "RIGHT" )
		i = 3;
	else if ( m_input == "UP" )
		i = 0;
	else if ( m_input == "DOWN" )
		i = 1;

	m_targetUnit->setForcedMove( i );
	m_activateElapsedTime = 0;
}

bool UnitMoveAction::update( double dTime, float fElapsedTime )
{
	m_activateElapsedTime += fElapsedTime;
	if ( m_activateElapsedTime > 2.0f )
		return false;
	else
		return true;
}
Action* EpCreateUnitMoveAction( void* targetUnit, const char* input )
{
	Unit* u = reinterpret_cast<Unit*>( targetUnit );
	return new UnitMoveAction( u , input );
} SCRIPT_CALLABLE_PV_PV_PC( EpCreateUnitMoveAction )

//////////////////////////////////////////////////////////////////////////

void FadeAction::activate()
{
	GetG().m_EpLight.setFadeDuration( m_fDuration );
	if ( m_iType == 0 )
		GetG().m_EpLight.fadeInLight();
	else if ( m_iType == 1 )
		GetG().m_EpLight.fadeOutLight();
}

bool FadeAction::update( double dTime, float fElapsedTime )
{
	return GetG().m_EpLight.isInFading();
}

Action* EpCreateFadeAction( const char* type, int durationMs )
{
	int typeNum = 0;
	if ( strcmp( type, "in") == 0 ) typeNum = 0;
	else if ( strcmp( type, "out") == 0 ) typeNum = 1;
	else throw std::runtime_error( "Check your script" );

	return new FadeAction( typeNum, (float)durationMs / 1000 );
} SCRIPT_CALLABLE_PV_PC_I( EpCreateFadeAction )

//////////////////////////////////////////////////////////////////////////

TeleportAction::TeleportAction( Unit* targetUnit, int x, int y )
: m_targetUnit ( targetUnit ), m_tileX ( x ) , m_tileY ( y )
{
}

TeleportAction::~TeleportAction()
{
}

void TeleportAction::activate()
{
	m_targetUnit->setTileBufferPos( m_tileX, m_tileY );
	m_targetUnit->setTilePos( m_tileX, m_tileY );
}

Action* EpCreateTeleportAction( void* targetUnit, int x, int y )
{
	Unit* u = reinterpret_cast<Unit*>( targetUnit );
	return new TeleportAction( u, x, y );
}  SCRIPT_CALLABLE_PV_PV_I_I( EpCreateTeleportAction )

//////////////////////////////////////////////////////////////////////////

CameraAction::CameraAction( int type, int duration, ArnCamera* arnCam )
{
	m_type = type; 
	m_arnCam = arnCam; 
	m_duration = duration;
}

CameraAction::CameraAction( int type, int duration )
{
	m_type = type; 
	m_arnCam = 0; 
	m_duration = duration;
}

void CameraAction::activate()
{
	GetG().m_camera.setSmoothCameraDuration( (float)m_duration / 1000.0f );

	switch ( m_type )
	{
	case 0:
		GetG().m_camera.setAttachPos( &GetWorldManager().getCurWorld()->getHeroPos() );
		GetG().m_camera.begin( CAMERA_SMOOTH_ATTACH );
		break;
	case 1:
		GetG().m_camera.setExternalCamera( m_arnCam );
		GetG().m_camera.begin( CAMERA_EXTERNAL );
		break;
	}
}

Action* EpCreateCameraAction( const char* type, const char* extCamName, int durationMs  )
{
	ArnSceneGraph* arnSceneGraph = GetWorldManager().getCurWorld()->getArnSceneGraphPt();
	ArnCamera* arnCam = static_cast<ArnCamera*>(arnSceneGraph->getSceneRoot()->getNodeByName( extCamName ));

	if ( strcmp( type, "attach" ) == 0)
		return new CameraAction( 0, durationMs );
	else if ( strcmp( type, "external" ) == 0 )
	{
		return new CameraAction( 1, durationMs, arnCam );
	}
	else
		throw std::runtime_error( "Type check failed" );
} SCRIPT_CALLABLE_PV_PC_PC_I( EpCreateCameraAction )

//////////////////////////////////////////////////////////////////////////

ControllableAction::ControllableAction( Character* c, bool controllable )
{
	m_c = c; 
	m_bControllable = controllable;
}

void ControllableAction::activate()
{
	m_c->setControllable( m_bControllable );
}

Action* EpCreateControllableAction( void* target, int controllable )
{
	Character* c = reinterpret_cast<Character*>( target );
	return new ControllableAction( c, controllable?true:false );
} SCRIPT_CALLABLE_PV_PV_I( EpCreateControllableAction )

//////////////////////////////////////////////////////////////////////////

DelayAction::DelayAction( int delayMs )
: m_delayMs( delayMs ), m_activateElapsedTime( 0 )
{}
void DelayAction::activate()
{}

bool DelayAction::update( double dTime, float fElapsedTime )
{
	m_activateElapsedTime += fElapsedTime;
	if ( m_activateElapsedTime > m_delayMs/1000.0f )
		return false;
	else
		return true;
}

Action* EpCreateDelayAction( int delayMs )
{
	return new DelayAction( delayMs );
} SCRIPT_CALLABLE_PV_I( EpCreateDelayAction )

//////////////////////////////////////////////////////////////////////////

StartIncidentAndWaitAction::StartIncidentAndWaitAction( Incident* incident )
: m_incident ( dynamic_cast<BlockingActionIncident*>( incident ) )
{
	if ( !m_incident )
		throw std::runtime_error( "StartIncidentAndWaitAction is only applicable to blocking incidents, i.e. BlockingActionIncident" );
}

bool StartIncidentAndWaitAction::update( double dTime, float fElapsedTime )
{
	//IncidentTrigger* trigger = new IncidentTrigger( m_incident );
	//return !trigger->check();

	return !m_incident->isFinished();
}

void StartIncidentAndWaitAction::activate()
{
	getCurWorld()->addIncident( m_incident );
}

Action* EpCreateStartIncidentAndWaitAction( void* inc )
{
	return new StartIncidentAndWaitAction( /*reinterpret_cast<Incident*>( inc )*/ (Incident*)inc );
} SCRIPT_CALLABLE_PV_PV( EpCreateStartIncidentAndWaitAction )

//////////////////////////////////////////////////////////////////////////

START_SCRIPT_FACTORY( Action )
	CREATE_OBJ_COMMAND( EpCreateDialogAction )
	CREATE_OBJ_COMMAND( EpCreateSoundAction )
	CREATE_OBJ_COMMAND( EpCreateHealAction )
	CREATE_OBJ_COMMAND( EpCreateUnitSpawnAction )
	CREATE_OBJ_COMMAND( EpCreateScriptAction )
	CREATE_OBJ_COMMAND( EpCreateUnitMoveAction )
	CREATE_OBJ_COMMAND( EpCreateFadeAction )
	CREATE_OBJ_COMMAND( EpCreateTeleportAction )
	CREATE_OBJ_COMMAND( EpCreateCameraAction )
	CREATE_OBJ_COMMAND( EpCreateControllableAction )
	CREATE_OBJ_COMMAND( EpCreateDelayAction )
	CREATE_OBJ_COMMAND( EpCreateStartIncidentAndWaitAction )
END_SCRIPT_FACTORY( Action )
