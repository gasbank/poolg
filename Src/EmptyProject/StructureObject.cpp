#include "EmptyProjectPCH.h"
#include "StructureObject.h"
#include "World.h"
#include "WorldManager.h"

StructureObject::StructureObject(void)
: Unit ( UT_STRUCTREOBJECT )
{
	m_bMoving			= false;
	m_fMovingTime		= 0;
	m_moveDuration		= 1.0f;
}

StructureObject::~StructureObject(void)
{
}

Unit* StructureObject::createStructureObject( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool pushable )
{
	StructureObject* u = new StructureObject();
	u->init();
	u->setTilePos( tileX, tileY );
	u->setTileBufferPos( tileX, tileY );
	u->setPushable( pushable );

	return u;
}

bool StructureObject::frameMove( double dTime, float fElapsedTime )
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

				// if there is no obstacles
				if( getMovable() )
				{
					TileRegion entireRegion( 0, 0, s_xSize - 1, s_ySize - 1);

					Point2Uint nextTilePos(
						getTileBufferPos().x + g_moveAmount[ i ].x,
						getTileBufferPos().y + g_moveAmount[ i ].y );

						/*Tile* nextTile = GetTileManager().getTile( nextTilePos );
						assert( nextTile );*/
						if( !GetWorldManager().getCurWorld()->findUnitAtTile( nextTilePos.x, nextTilePos.y ) && entireRegion.isExist( nextTilePos ) )
						{
							m_bMoving = true;
							m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
							m_vKeyboardDirection.x += (float) g_moveAmount[ i ].x * s_tileSize;
							m_vKeyboardDirection.y += (float) g_moveAmount[ i ].y * s_tileSize;

							setTileBufferPos(
								getTileBufferPos().x + g_moveAmount[ i ].x,
								getTileBufferPos().y + g_moveAmount[ i ].y );
						}
				}
				break;
			}
		}
	}


	if (m_bMoving && m_fMovingTime <= m_moveDuration)
	{
		// Update velocity
		m_vVelocity = m_vKeyboardDirection / m_moveDuration;

		// Simple euler method to calculate position delta
		D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;
		m_fMovingTime += fElapsedTime;
		setPos(getPos() + vPosDelta);
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

	return Unit::frameMove( dTime, fElapsedTime );
}

Unit* EpCreateStructureObject( int tileX, int tileY, int p )
{
	//LPD3DXMESH d3dxMesh;
	//D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
	bool pushable;
	if ( p == 0 )
		pushable = false;
	else
		pushable = true;
	return StructureObject::createStructureObject( 0, tileX, tileY, 0, pushable );
} SCRIPT_CALLABLE_PV_I_I_I( EpCreateStructureObject )

START_SCRIPT_FACTORY( StructureObject )
	CREATE_OBJ_COMMAND( EpCreateStructureObject )
END_SCRIPT_FACTORY( StructureObject )