#include "EmptyProjectPCH.h"
#include "Hero.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "World.h"
#include "TopStateManager.h"
#include "PlayState.h"
#include "ArnMesh.h"
#include "Skill.h"

Hero::Hero(void)
: Character( UT_HERO )
{
	m_skillSet = new SkillSet();
}

Hero::~Hero(void)
{
}

Unit* Hero::createHero( LPD3DXMESH mesh, int tileX, int tileY, float posZ )
{
	Hero* u = new Hero();
	u->init( GetG().m_dev, mesh );
	u->setTilePos( tileX, tileY );
	u->setTileBufferPos( tileX, tileY );

	// As default, hero's model is PoolG
	PlayState* ps = static_cast<PlayState*>(GetTopStateManager().getState( GAME_TOP_STATE_PLAY ));
	ArnSceneGraph* charSceneGraph = ps->getCharacterSceneGraph();
	ArnMesh* arnMesh = static_cast<ArnMesh*>(charSceneGraph->getSceneRoot()->getNodeByName( "PoolGModel" ));
	u->setArnMesh( arnMesh );


	SkillSet* skillSet = u->getSkillSet();

	skillSet->setSkill (SL_FIRST, (Skill*) new NormalAttack());
	skillSet->setSkill (SL_SECOND, (Skill*) new Heal());
	skillSet->setSkill (SL_THIRD, (Skill*) new Goto());
	skillSet->setSkill (SL_FOURTH, (Skill*) new MultiThread());
	skillSet->setSkill (SL_FIFTH, (Skill*) new Meditation());

	return u;
}

HRESULT Hero::frameRender()
{
	return Character::frameRender();
}

bool Hero::frameMove( float fElapsedTime )
{
	//printf( "Tile X, Y : %d, %d \n", getTilePosX(), getTilePosY() );
	return Character::frameMove( fElapsedTime );
}

LRESULT Hero::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return Character::handleMessages( hWnd, uMsg, wParam, lParam );
}

Unit* EpCreateHero( Tcl_Obj* tilePos )
{
	LPD3DXMESH d3dxMesh;
	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
	int tileX = 0, tileY = 0;
	GetScriptManager().readTwoIntObj( tilePos, tileX, tileY );
	return Hero::createHero( d3dxMesh, tileX, tileY, 0 );

} SCRIPT_CALLABLE_PV_OBJ( EpCreateHero )

START_SCRIPT_FACTORY(Hero)
	CREATE_OBJ_COMMAND( EpCreateHero )
END_SCRIPT_FACTORY(Hero)
