#include "EmptyProjectPCH.h"
#include "Character.h"
#include "ScriptManager.h"
#include "TileManager.h"
#include "TopStateManager.h"
#include "WorldStateManager.h"
#include "World.h"
#include "ArnMesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Trigger.h"
#include "StructureObject.h"
#include "SkillObject.h"
#include "DynamicMotion.h"
#include "Skill.h"
#include "SkillSet.h"
#include "Utility.h"
#include "Sound.h"
#include "PlayState.h"

extern TileManager tileManager;

//
//void Character::doNormalAttack( int type, Character* enemy )
//{
//	ArnVec3 fireDir = enemy->getPos() - getPos();
//	float dist = ArnVec3Length( &fireDir );
//
//	D3DXVec3Normalize( &fireDir, &fireDir );
//
//
//
//	LPD3DXMESH mesh;
//	D3DXCreateSphere( GetG().m_dev, 0.3f, 16, 16, &mesh, 0 );
//	Unit* missile = Unit::createUnit(mesh, 0, 0, 0);
//
//	missile->setDynamicMotion(DynamicMotion::createDMfireUniformly
//		(missile, this->getPos(), fireDir, dist, 10.0f ));
//
//	SkillObject* so = SkillObject::createSOnormalAttack(this, enemy, missile);
//	m_skillObjectList.push_back(so);
//
//}



void Character::doCsBurn ()
{
	/*LPD3DXMESH mesh;
	D3DXCreateBox(GetG().m_dev, 5, 5, 0.5f, &mesh, 0);
	Unit* effectObj = Unit::createUnit(mesh, 0, 0, 0);

	effectObj->setDynamicMotion(DynamicMotion::createDMspinAround
		(effectObj, this->getPos(), 5, 0.03f, 50));

	SkillObject* so = SkillObject::createSOcsBurn(this, effectObj);
	m_skillObjectList.push_back(so);*/
}






void Character::recoverCs()
{
	int recoverCsPoint = (int)(m_maxCs * 0.1f);
	if (m_curCs + recoverCsPoint > m_maxCs)
		m_curCs = m_maxCs;
	else
		m_curCs += recoverCsPoint;
}

void Character::heal (int point)
{
	if (m_curHp + point < m_maxHp)
		m_curHp += point;
	else if (m_curHp + point < 0)
		m_curHp = 0;
	else
		m_curHp = m_maxHp;
}

Unit* Character::createCharacter( LPD3DXMESH mesh, int tileX, int tileY, float posZ )
{
	Character* u = new Character( UT_CHARACTER );
	u->setTilePos( tileX, tileY );
	u->setTileBufferPos( tileX, tileY );
	return u;
}

Character::~Character()
{
	deleteAllSkills();
	deleteAllItems();
	delete m_skillSet;
}

bool Character::frameMove( double dTime, float fElapsedTime )
{

	if (m_bMoving == false)
	{
		m_fMovingTime = 0;


		UINT i;
		for ( i = 0; i < UNIT_MAX_KEYS; i++ )
		{
			if( IsKeyDown( m_aKeys[ (UnitInput)i ] ) )
			{
				setMovable( true );
				rayTesting( (UnitInput)i );
				boundaryTesting( (UnitInput)i );

				// if there is no obstacles
				if( getMovable() )
				{
					TileRegion entireRegion( 0, 0, s_xSize - 1, s_ySize - 1);

					Point2Uint nextTilePos(
						getTileBufferPos().x + g_moveAmount[ i ].x,
						getTileBufferPos().y + g_moveAmount[ i ].y );

//  					Tile* nextTile = tileManager.getTile( nextTilePos );
//  					assert( nextTile );
					if( !GetWorldManager().getCurWorld()->findUnitAtTile( nextTilePos.x, nextTilePos.y ) && entireRegion.isExist( nextTilePos ) )
 					{
						m_bMoving = true;
						m_vKeyboardDirection.x = (float) g_moveAmount[ i ].x * s_tileSize;
						m_vKeyboardDirection.y = (float) g_moveAmount[ i ].y * s_tileSize;
						m_vKeyboardDirection.z = 0;

						setTileBufferPos(
							getTileBufferPos().x + g_moveAmount[ i ].x,
							getTileBufferPos().y + g_moveAmount[ i ].y );
					}
				}
				// 가는 방향으로 머리를 돌린다.
				this->setHeadDir( (UnitInput)i );
				// 앞에 있는 유닛을 밀거나 획득한다.
				this->processUnitInFront( (UnitInput)i );

				break;
			}
		}
	}


	if (m_bMoving && m_fMovingTime <= m_moveDuration)
	{
		// Update velocity
		m_vVelocity = m_vKeyboardDirection / m_moveDuration;

		// Simple euler method to calculate position delta
		ArnVec3 vPosDelta = m_vVelocity * fElapsedTime;
		m_fMovingTime += fElapsedTime;
		ArnVec3 finalPos = getPos() + vPosDelta;
		setPos( finalPos.x, finalPos.y, finalPos.z );
	}
	else
	{
		m_bMoving = false;

		//타일에 맞도록 위치보정
		if( getTileBufferPos() != getTilePos() )
		{
			setTilePos( getTileBufferPos() );
		}
	}

	SkillObjectList::iterator it = m_skillObjects.begin();
	for ( ; it != m_skillObjects.end() ; )
	{
		bool frameMoveInProgress = (*it)->frameMove( dTime, fElapsedTime );
		if ( !frameMoveInProgress )
		{
			SAFE_DELETE(*it);
			it = m_skillObjects.erase( it );
		}
		else
			++it;
	}

	const ArnVec3& unitPos = Unit::getPos();
	m_curPos = unitPos + m_moveImpulse;
	if ( m_moveImpulse != ArnConsts::ARNVEC3_ZERO )
	{
		/*
		printf( "%s : m_moveImpulse ", getTypeString() ); Utility::printValue( m_moveImpulse ); printf( "\n" );
		printf( "%s : Unit::getPos() ", getTypeString() ); Utility::printValue( Unit::getPos() ); printf( "\n" );
		printf( "%s : getPos() ", getTypeString() ); Utility::printValue( getPos() ); printf( "\n" );
		*/

		const float decimate = 0.5f;
		m_moveImpulse *= decimate;
		if ( ArnVec3Length( &m_moveImpulse ) < 1e-4 )
			m_moveImpulse = ArnConsts::ARNVEC3_ZERO;

		//printf( "Char FM: %.2f %.2f %.2f\n", m_curPos.x, m_curPos.y, m_curPos.z );
		setLocalXformDirty();
	}

	return Unit::frameMove( dTime, fElapsedTime );
}

HRESULT Character::frameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime )
{
	SkillObjectList::iterator it = m_skillObjects.begin();
	for ( ; it != m_skillObjects.end(); ++it )
	{
		(*it)->frameRender( pd3dDevice, dTime, fElapsedTime );
	}


	
	return Unit::frameRender( pd3dDevice, dTime, fElapsedTime );
}

LRESULT Character::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			// Map this key to a D3DUtil_CameraKeys enum and update the
			// state of m_aKeys[] by adding the KEY_WAS_DOWN_MASK|KEY_IS_DOWN_MASK mask
			// only if the key is not down
			UnitInput mappedKey = mapKey( ( UINT )wParam );
			if( mappedKey != UNIT_UNKNOWN && m_bControllable )
			{
				if( FALSE == IsKeyDown( m_aKeys[mappedKey] ) )
				{
					m_aKeys[ mappedKey ] = KEY_WAS_DOWN_MASK | KEY_IS_DOWN_MASK;
					++m_cKeysDown;
				}
			}
			break;
		}

	case WM_KEYUP:
		{
			// Map this key to a D3DUtil_CameraKeys enum and update the
			// state of m_aKeys[] by removing the KEY_IS_DOWN_MASK mask.
			UnitInput mappedKey = mapKey( ( UINT )wParam );
			if( mappedKey != UNIT_UNKNOWN && ( DWORD )mappedKey < 8 )
			{
				m_aKeys[ mappedKey ] &= ~KEY_IS_DOWN_MASK;
				--m_cKeysDown;
			}
			break;
		}

	}

	return FALSE;
}


Character::Character( UnitType type )
: Unit( type ), m_moveImpulse( ArnConsts::ARNVEC3_ZERO )
{
	m_maxHp				= 5;
	m_curHp				= m_maxHp;
	m_bMoving			= false;
	m_fMovingTime		= 0;
	m_moveDuration		= 1.0f;
	m_bControllable		= true;
	m_boundaryTileRect.setStart( Point2Uint( 0, 0 ) );
	m_boundaryTileRect.setEnd( Point2Uint( s_xSize - 1, s_ySize - 1 ) );
	m_curPos			= ArnConsts::ARNVEC3_ZERO;
	ZeroMemory( &m_stat, sizeof(Stat) );
	
	// Initialize random number
	srand ( (unsigned)time(NULL) );
	m_skillSet = new SkillSet( this );
}

void Character::setCurHp( int curHp )
{
	if ( curHp == -1 )
		m_curHp = m_maxHp;
	else if ( curHp > m_maxHp )
		m_curHp = m_maxHp;
	else if ( curHp < 0)
		m_curHp = 0;
	else
		m_curHp = curHp;
}

void Character::setCurCs( int curCs )
{
	if ( curCs == -1 )
		m_curCs = m_maxCs;
	else if ( curCs > m_maxCs )
		m_curCs = m_maxCs;
	else if ( curCs < 0)
		m_curCs = 0;
	else
		m_curCs = curCs;
}


/*void Character::setMaxAndCurHp( int maxHp, int curHp )
{
	if ( maxHp < curHp || maxHp <= 0 )
		throw std::runtime_error( "Logically incorrect value entered" );
	maxHp = m_stat.health * m_stat.health / 4 + 50;
	if ( curHp == -1 )
		curHp = maxHp;

	m_maxHp = maxHp;
	m_curHp = curHp;
}

void Character::setMaxAndCurCs( int maxCs, int curCs )
{
	if ( maxCs < curCs || maxCs <= 0 )
		throw std::runtime_error( "Logically incorrect value entered" );
	maxCs = m_stat.will * m_stat.will / 10 + 20;
	if ( curCs == -1 )
		curCs = maxCs;

	m_maxCs = maxCs;
	m_curCs = curCs;
}*/

// 지정된 사각형 경계 위에 캐릭터가 있을 때 경계 바깥으로 나가려고 하면 움직일 수 없게 한다.
void Character::boundaryTesting( UnitInput mappedKey )
{
	Point2Uint nextPos( 
		getTilePos().x + g_moveAmount[ (UINT)mappedKey ].x, 
		getTilePos().y + g_moveAmount[ (UINT)mappedKey ].y );
	if ( !m_boundaryTileRect.isExist( nextPos ) )
		setMovable( false );
}

void Character::setBoundaryRect( LONG x0, LONG y0, LONG x1, LONG y1 )
{
	m_boundaryTileRect = TileRegion( x0, y0, x1, y1 );
}

void Character::damage( int point )
{
	m_curHp -= point;
	
	if ( getType() == UT_HERO )
	{
		GetG().m_screenFlash.screenFlashing( 1.3f, 1.0f, 0.0f, 0.0f );
		GetAudioState().pSoundBank->Play( GetAudioState().iHit, 0, 0, NULL );
	}

	if ( getType() == UT_HERO )
	{
		printf( "HERO 체력은? %d\n", m_curHp );
	}
	else if ( getType() == UT_ENEMY )
	{
		printf( "Enemy 체력은? %d\n", m_curHp );
	}
	else
	{
		_ASSERTE( !"Character type error or unexpected type" );
	}
}


void Character::setStat( int statHealth, int statWill, int statCoding, int statDef, int statSen, int statImmu )
{
	m_stat.health	= statHealth;
	m_stat.will		= statWill;
	m_stat.coding	= statCoding;
	m_stat.def		= statDef;
	m_stat.sense	= statSen;
	m_stat.immunity	= statImmu;

	m_maxHp = m_stat.health * m_stat.health / 4 + 50;
	m_maxCs = m_stat.will * m_stat.will / 10 + 20;


	// Hp, Mp update goes here...
}

void Character::processUnitInFront( UnitInput dir )
{
	if ( this->getType() == UT_HERO )
	{
		const UINT nextTileX = getTileBufferPos().x + g_moveAmount[ (int)dir ].x;
		const UINT nextTileY = getTileBufferPos().y + g_moveAmount[ (int)dir ].y;
		UnitBase* u = GetWorldManager().getCurWorld()->findUnitAtTile( nextTileX, nextTileY );

		if ( u )
		{
			StructureObject* s = dynamic_cast<StructureObject*>( u );
			if ( s )
			{
				if ( s->isPushable() )
				{
					s->setForcedMove( dir );
				}
				else if ( s->isPickable() )
				{
					s->getAttachedWorld()->detachUnit( s );
					pickItem( s );
				}
			}
		}
	}
}

void Character::setDead()
{
	throw std::runtime_error( "Do not call this method." );
	/*m_curHp = -1;
	startSoulAnimation( 1.0f, 10.0f );*/
}

void Character::addMoveImpulse( const ArnVec3& impulse )
{
	//printf( "addMoveImpulse called with impulse " ); Utility::printValue( impulse ); printf( "\n" );
	m_moveImpulse += impulse;
	//printf( "Now impulse is " ); Utility::printValue( m_moveImpulse ); printf( "\n" );
}

bool Character::deleteSkill( SkillLocation skillLoc )
{
	if ( m_skillSet->getSkill( skillLoc ) )
	{
		m_skillSet->deleteSkill( skillLoc );
		return true;
	}
	return false;
}

bool Character::memorizeSkill( const Skill* skill )
{
	return m_skillSet->memorizeSkill( skill );
}

bool Character::equipSkill( UINT slot, const Skill* skill )
{
	return m_skillSet->equipSkill( slot, skill );
}

void Character::pushSkillObjectList( const SkillObjectList soList )
{
	SkillObjectList::const_iterator cit = soList.begin();
	for ( ; cit != soList.end(); ++cit )
	{
		pushSkillObject( *cit );
	}
}

void Character::pickItem( StructureObject* structureObj )
{
	assert( structureObj->getAttachedWorld() == 0 );
	structureObj->setOwner( this );
	m_items.push_back( structureObj );

	PlayState* playState = dynamic_cast<PlayState*>( GetTopStateManager().getCurState() );
	if ( playState )
		playState->updateHeroItemListThumbnails();
}

void Character::deleteAllItems()
{
	SafeDeleteAll( m_items );
}

void Character::deleteAllSkills()
{
	SafeDeleteAll( m_skillObjects );
}

const std::string& Character::getItemThumbnailName( UINT idx ) const
{
	return m_items[idx]->getThumbnailName();
}
//////////////////////////////////////////////////////////////////////////

int EpCharacterSetCurHp( void* ptr, int curHp )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setCurHp( curHp );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpCharacterSetCurHp )

int EpCharacterSetCurCs( void* ptr, int curCs )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setCurCs( curCs );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpCharacterSetCurCs )

int EpCharacterSetMoveDuration( void* ptr, double val )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setMoveDuration( (float)val );
	return 0;
} SCRIPT_CALLABLE_I_PV_D( EpCharacterSetMoveDuration )


int EpCharacterSetBoundary( void* ptr, int left, int top, int right, int bottom )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setBoundaryRect( left, top, right, bottom );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I_I_I( EpCharacterSetBoundary )

int EpCharacterSetAutoBoundary( void* ptr, int range )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	UINT x = instance->getTilePosX();
	UINT y = instance->getTilePosY();
	instance->Character::setBoundaryRect( x - range, y + range, x + range, y - range );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpCharacterSetAutoBoundary )

int EpCharacterSetStat( void* ptr, int statHealth, int statWill, int statCoding, int statDef, int statSen, int statImmu )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setStat( statHealth, statWill, statCoding, statDef, statSen, statImmu );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I_I_I_I_I( EpCharacterSetStat )

int EpCharacterSetControllable( void* ptr, int controllable )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	instance->Character::setControllable( controllable?true:false );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpCharacterSetControllable )

int EpCharacterSetTilePos( void* ptr, Tcl_Obj* tilePos )
{
	Character* instance = reinterpret_cast<Character*>( ptr );
	int tileX = 0, tileY = 0;
	GetScriptManager().readTwoIntObj( tilePos, tileX, tileY );
	instance->Character::setTilePos( tileX, tileY );
	instance->Character::setTileBufferPos( tileX, tileY );
	return 0;
} SCRIPT_CALLABLE_I_PV_OBJ( EpCharacterSetTilePos )

START_SCRIPT_FACTORY(Character)
	//CREATE_OBJ_COMMAND( EpCreateCharacter )
	CREATE_OBJ_COMMAND( EpCharacterSetCurHp )
	CREATE_OBJ_COMMAND( EpCharacterSetCurCs )
	CREATE_OBJ_COMMAND( EpCharacterSetMoveDuration )
	CREATE_OBJ_COMMAND( EpCharacterSetBoundary )
	CREATE_OBJ_COMMAND( EpCharacterSetStat )
	CREATE_OBJ_COMMAND( EpCharacterSetControllable )
	CREATE_OBJ_COMMAND( EpCharacterSetTilePos )
	CREATE_OBJ_COMMAND( EpCharacterSetAutoBoundary )
END_SCRIPT_FACTORY(Character)
