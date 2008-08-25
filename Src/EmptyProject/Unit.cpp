#include "EmptyProjectPCH.h"
#include "Unit.h"
#include "ScriptManager.h"
#include "World.h"
#include "TopStateManager.h"
#include "PlayState.h"
#include "TileManager.h"
#include "Trigger.h"
#include "DynamicMotion.h"
#include "ArnMesh.h"
#include "ArnSceneGraph.h"
#include "Utility.h"
#include "EpLight.h"

extern TileManager tileManager;

//////////////////////////////////////////////////////////////////////////

Unit::Unit( UnitType type )
: m_type( type )
{
	m_d3dxMesh			= 0;
	m_arnMesh			= 0;
	m_pd3dDevice		= 0;
	m_d3dTex			= 0;
	m_pd3dxFont			= 0;

	m_fSoulAnimationDuration	= 1.0f;
	m_fSoulAnimationHeight		= 10.0f;
	m_fSoulAnimationTimer		= m_fSoulAnimationDuration;
	m_bSoulAnimation			= false;

	m_name				= "Unconfirmed Object";
	m_bNameVisible				= false;

	m_tilePos			= Point2Uint::ZERO;
	m_tileBufferPos		= m_tilePos;

	m_vRot				= D3DXVECTOR3(0, 0, 0);
	m_vPos				= D3DXVECTOR3(0, 0, 0);
	m_vScale			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bLocalXformDirty	= true;
	m_removeFlag		= false;
	m_bForcedMove		= false;
	m_dm				= NULL;

	D3DXMatrixIdentity(&m_localXform);

	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));
	m_material.Ambient.r = m_material.Ambient.g = m_material.Ambient.b = 0.6f;
	m_material.Diffuse.r = m_material.Diffuse.g = m_material.Diffuse.b = 0.3f;
	m_material.Specular.r = m_material.Specular.g = m_material.Specular.b = 0.2f;
	m_material.Ambient.a = m_material.Diffuse.a = m_material.Specular.a = 1.0f;

	ZeroMemory(m_aKeys, sizeof(m_aKeys));
}
Unit::~Unit()
{
	SAFE_RELEASE(m_d3dxMesh);
}

HRESULT Unit::init( LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXMESH mesh )
{
	HRESULT hr = S_OK;

	m_d3dxMesh = mesh;
	m_pd3dDevice = pd3dDevice;

	DWORD teapotFVF = m_d3dxMesh->GetFVF();
	assert(teapotFVF == 18 && m_d3dxMesh->GetNumVertices() != 0);
	TeapotVertex* v = 0;
	m_d3dxMesh->LockVertexBuffer(0, (void**)&v);
	V(D3DXComputeBoundingBox((const D3DXVECTOR3*)v, m_d3dxMesh->GetNumVertices(), sizeof(float)*6, &m_lowerLeft, &m_upperRight));
	m_d3dxMesh->UnlockVertexBuffer();

	D3DXCreateFont( 
		GetG().m_dev, 26, 0, 
		FW_NORMAL, 1, 
		FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, _T( "Arial Black"), &m_pd3dxFont );

	return hr;
}

void Unit::setTilePos( int tileX, int tileY )
{
	m_tilePos.x = tileX;
	m_tilePos.y = tileY;

	setPos( D3DXVECTOR3( (float)(tileX - (s_xSize / 2)) * s_tileSize, (float)(tileY  - (s_ySize / 2)) * s_tileSize, 0 ) );

	//printf( "%s: setTilePos called (%d, %d)\n", typeid(this).name(), tileX, tileY );
}

void Unit::setTilePos( const Point2Uint& newPos )
{
	setTilePos( newPos.x, newPos.y );
}

HRESULT Unit::frameRender()
{
	HRESULT hr = S_OK;

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_localXform);
	m_pd3dDevice->SetTexture(0, m_d3dTex);
	m_pd3dDevice->SetMaterial(&m_material);

	if ( m_arnMesh )
	{
		m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
		GetG().m_videoMan.renderMeshesOnly( m_arnMesh, m_localXform );
		m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

		drawSoul();
	}
	else
		m_d3dxMesh->DrawSubset(0);

	drawName();

	return hr;
}

LRESULT Unit::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

bool Unit::frameMove( float fElapsedTime )
{	
	bool flag = true;
	if ( m_dm != NULL )
		flag = m_dm->frameMove( fElapsedTime );

	updateLocalXform();
	if ( m_bForcedMove )
		forcedMoveTest();

	updateSoulAnimation( fElapsedTime );

	return flag;
}

void Unit::updateLocalXform()
{
	if ( m_bLocalXformDirty )
	{
		D3DXMATRIX mRotX, mRotY, mRotZ, mScale, mTrans, mWorld;
		D3DXMatrixRotationX(&mRotX, m_vRot.x);
		D3DXMatrixRotationY(&mRotY, m_vRot.y);
		D3DXMatrixRotationZ(&mRotZ, m_vRot.z);
		D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);
		D3DXMatrixTranslation(&mTrans, getPos().x, getPos().y, getPos().z);
		m_localXform = mRotX * mRotY * mRotZ * mScale * mTrans;

		m_bLocalXformDirty = false;
	}
}

UnitInput Unit::mapKey( UINT nKey ) const
{
	switch( nKey )
	{
	case 'A':	return UNIT_MOVE_LEFT;
	case 'D':	return UNIT_MOVE_RIGHT;
	case 'W':	return UNIT_MOVE_UP;
	case 'S':	return UNIT_MOVE_DOWN;
	}

	return UNIT_UNKNOWN;
}

void Unit::clearKey()
{
	for( int i = 0; i < UNIT_MAX_KEYS; i++ )
		m_aKeys[i] = 0;
	m_cKeysDown = 0;
}

Unit* Unit::createUnit( LPD3DXMESH mesh, int tileX, int tileY, float posZ )
{
	Unit* u = new Unit( UT_UNIT );
	u->init( GetG().m_dev, mesh );
	return u;
}


void Unit::setColor( int r, int g, int b )
{
	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));

	m_material.Ambient.r = (float)r / 255.0f;
	m_material.Ambient.g = (float)g / 255.0f;
	m_material.Ambient.b = (float)b / 255.0f;

	m_material.Diffuse.r = (float)r / 255.0f;
	m_material.Diffuse.g = (float)g / 255.0f;
	m_material.Diffuse.b = (float)b / 255.0f;

	m_material.Specular.r = (float)r / 255.0f;
	m_material.Specular.g = (float)g / 255.0f;
	m_material.Specular.b = (float)b / 255.0f;

	m_material.Ambient.a = m_material.Diffuse.a = m_material.Specular.a = 1.0f;
}


// 가고자 하는 방향으로 유닛을 회전시킨다.
void Unit::setHeadDir( UnitInput unitInput )
{
	if ( !m_arnMesh )
	{
		switch ( unitInput )
		{
		case UNIT_MOVE_UP:
			setRotZ( D3DXToRadian( 90 + 0 ) );
			break;
		case UNIT_MOVE_DOWN:
			setRotZ( D3DXToRadian( 90 + 180 ) );
			break;
		case UNIT_MOVE_RIGHT:
			setRotZ( D3DXToRadian( 90 - 90 ) );
			break;
		case UNIT_MOVE_LEFT:
			setRotZ( D3DXToRadian( 90 + 90 ) );
			break;
		}
	}
	else
	{
		switch ( unitInput )
		{
			case UNIT_MOVE_UP:
				setRotZ( D3DXToRadian( 0 ) );
				break;
			case UNIT_MOVE_DOWN:
				setRotZ( D3DXToRadian( 180 ) );
				break;
			case UNIT_MOVE_RIGHT:
				setRotZ( D3DXToRadian( -90 ) );
				break;
			case UNIT_MOVE_LEFT:
				setRotZ( D3DXToRadian( 90 ) );
				break;
		}
	}
}

void Unit::setForcedMove( int i )
{
	UnitInput mappedKey = (UnitInput)i ;

	if( mappedKey != UNIT_UNKNOWN )
	{
		if( FALSE == IsKeyDown( m_aKeys[mappedKey] ) )
		{
			m_aKeys[ mappedKey ] = KEY_WAS_DOWN_MASK | KEY_IS_DOWN_MASK;
			++m_cKeysDown;
		}
	}

	m_bForcedMove = true;
	
}

void Unit::forcedMoveTest()
{
	if ( m_tilePos != m_tileBufferPos )
	{
		for ( int j = 0; j < UNIT_MAX_KEYS; j++ )
		{
			if( IsKeyDown( m_aKeys[ (UnitInput)j ] ) )
			{

				UnitInput mappedKey = (UnitInput)j;
				if( mappedKey != UNIT_UNKNOWN && ( DWORD )mappedKey < 8 )
				{
					m_aKeys[ mappedKey ] &= ~KEY_IS_DOWN_MASK;
					--m_cKeysDown;
				}
			}
		}
		m_bForcedMove = false;
	}
}

HRESULT Unit::rayTesting( UnitInput mappedKey )
{
	HRESULT hr = S_OK;

	World* ws = GetWorldManager().getCurWorld();

	D3DXVECTOR3 rayStartPos( getPos().x, getPos().y, getPos().z - 2.0f );
	float dirArray[4][3] = { { 0.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } };
	D3DXVECTOR3 rayDir( dirArray[mappedKey][0], dirArray[mappedKey][1], dirArray[mappedKey][2] );

	bool intersected = Utility::FullTraverseExhaustiveRayTesting( ws->getArnSceneGraphPt()->getSceneRoot(), rayStartPos, rayDir, (float)s_tileSize * 1.01f );
	m_bMovable = !intersected;

	return hr;
}

void Unit::drawSoul()
{	
	if ( m_bSoulAnimation )
	{
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_localXformSoul);
		m_pd3dDevice->SetMaterial( &m_materialSoul );

		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
		m_arnMesh->getD3DXMesh()->DrawSubset( 0 );
		m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

void Unit::startSoulAnimation( float duration, float height )
{
	m_fSoulAnimationTimer = 0.0f;
	m_fSoulAnimationDuration = duration;
	m_fSoulAnimationHeight = height;
	m_bSoulAnimation = true;

	m_prevLocalXform = m_localXform;
	m_prevMaterial = m_material;
}

void Unit::updateSoulAnimation( float fElapsedTime )
{
	if ( m_bSoulAnimation )
	{
		if ( m_fSoulAnimationTimer < m_fSoulAnimationDuration )
			m_fSoulAnimationTimer += fElapsedTime;
		else
			m_bSoulAnimation = false;

		float ratio = sin( D3DXToRadian( m_fSoulAnimationTimer / m_fSoulAnimationDuration * 90.0f ) );
		float height = ratio * m_fSoulAnimationHeight;
		
		D3DXMATRIX translation;
		D3DXMatrixTranslation( &translation, 0.0f, 0.0f, -height );
		m_localXformSoul = m_prevLocalXform * translation;

		m_materialSoul = m_prevMaterial;
		D3DCOLORVALUE cv;
		cv.r = cv.g = cv.b = 1.0f; cv.a = 1 - ratio;
		m_materialSoul.Ambient = m_materialSoul.Diffuse = m_materialSoul.Specular = cv;

		setScaleZ( 1 - ratio );
	}
}

void Unit::setViewAt( const D3DXVECTOR3* at )
{
	D3DXVECTOR3 positiveY( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 dir = *at - getPos();
	
	float rad = Utility::radBetweenVectors( &positiveY, &dir );

	if ( dir.x > 0.0f )
		rad = -rad;

	setRotZ( rad );
}

void Unit::setVisible (bool choice)
{
	if (choice == false)
	{
		this->setScaleX(0);
		this->setScaleY(0);
		this->setScaleZ(0);
	}

	updateLocalXform();
}

const char* Unit::getTypeString() const
{
#define CASE_ENUM_TO_STRING(x) case x: return #x
	switch ( getType() )
	{
		CASE_ENUM_TO_STRING( UT_UNIT );
		CASE_ENUM_TO_STRING( UT_CHARACTER );
		CASE_ENUM_TO_STRING( UT_HERO );
		CASE_ENUM_TO_STRING( UT_ENEMY );
		CASE_ENUM_TO_STRING( UT_STRUCTREOBJECT );
		CASE_ENUM_TO_STRING( UT_ATTACKOBJECT );
		CASE_ENUM_TO_STRING( UT_INNERFIRE );
	default:
		_ASSERTE( !"Type string is not found" );
	}
	return 0;
}

void Unit::setDynamicMotion( DynamicMotion* dm )
{
	if (m_dm != NULL)
		delete m_dm;
	m_dm = dm;
}

void Unit::printDebugInfo() const
{
	printf( "Unit: Type - %s ", getTypeString() );
	Utility::printValue( getPos() );
	printf( "\n" );
}

void Unit::drawName()
{
	if ( m_bNameVisible && !GetEpLight().isInFading() )
	{

		// 대상이 카메라 앞에 있는지, 뒤에 있는지 카메라의 방향 벡터와,
		// 카메라로부터 대상까지의 방향 벡터를 내적하여 판단한다.
		const D3DXVECTOR3* pvCamEye = GetG().m_camera.GetEyePt();
		const D3DXVECTOR3* pvCamAt  = GetG().m_camera.GetLookAtPt();
		D3DXVECTOR3  vCamDir  = *pvCamAt - *pvCamEye;
		D3DXVECTOR3  vUnitDir = getPos() - *pvCamEye;
		if ( D3DXVec3Dot( &vCamDir, &vUnitDir ) > 0 )
		{
			D3DXVECTOR3 vProj;
			D3DXMATRIX ident;
			D3DXMatrixIdentity( &ident );

			D3DXVec3Project( 
				&vProj, &getPos(),
				&GetG().m_dev->Viewport,
				GetG().m_camera.GetProjMatrix(),
				GetG().m_camera.GetViewMatrix(),
				&ident );

			/*std::string str( "An Old Rat..." );
			if ( m_name == str )
			printf ( "%f %f %f\n", vProj.x, vProj.y, vProj.z );*/

			TEXTMETRICA tm;
			m_pd3dxFont->GetTextMetricsA( &tm );

			D3DXCOLOR m_textColor = D3DXCOLOR( 1.0f, 0.8f, 0.0f, 1.0f );
			RECT rc;
			rc.top = (int)vProj.y + 30;
			rc.left = (int)vProj.x - (int)(m_name.size() / 2.0f * tm.tmAveCharWidth);
			rc.right = (int)vProj.x - (int)(m_name.size() / 2.0f * tm.tmAveCharWidth);
			rc.bottom = (int)vProj.y + 30;
			GetG().m_dev->SetRenderState( D3DRS_LIGHTING, FALSE );
			assert( m_pd3dxFont->DrawTextA( 0, m_name.c_str(), -1, &rc, DT_NOCLIP, m_textColor ) );
			GetG().m_dev->SetRenderState( D3DRS_LIGHTING, TRUE );
		}
	}
}

void Unit::release()
{
	SAFE_DELETE( m_dm );

	SAFE_RELEASE( m_d3dxMesh );
	SAFE_RELEASE( m_pd3dxFont );
}
//////////////////////////////////////////////////////////////////////////
//
//Unit* EpCreateUnit( int tileX, int tileY )
//{
//	LPD3DXMESH d3dxMesh;
//	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );
//
//	return Unit::createUnit( d3dxMesh, tileX, tileY, 0 );
//
//} SCRIPT_CALLABLE_PV_I_I( EpCreateUnit )

int EpReleaseUnit( void* pv )
{
	Unit* u = reinterpret_cast<Unit*>(pv);
	EP_SAFE_RELEASE( u );
	return 0;

} SCRIPT_CALLABLE_I_PV( EpReleaseUnit )

int EpRegisterToWorld( void* pv1, void* pv2 )
{
	World* w = reinterpret_cast<World*>(pv1);
	Unit* u = reinterpret_cast<Unit*>(pv2);
	return w->addUnit(u);
} SCRIPT_CALLABLE_I_PV_PV( EpRegisterToWorld )






int EpUnitSetRotX( void* ptr, double val )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	instance->Unit::setRotX( (float)val );
	return 0;
} SCRIPT_CALLABLE_I_PV_D( EpUnitSetRotX )

int EpUnitSetRotY( void* ptr, double val )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	instance->Unit::setRotY( (float)val );
	return 0;
} SCRIPT_CALLABLE_I_PV_D( EpUnitSetRotY )

int EpUnitSetRotZ( void* ptr, double val )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	instance->Unit::setRotZ( (float)val );
	return 0;
} SCRIPT_CALLABLE_I_PV_D( EpUnitSetRotZ )

int EpUnitSetPosZ( void* ptr, double val )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	instance->Unit::setPosZ( (float)val );
	return 0;
} SCRIPT_CALLABLE_I_PV_D( EpUnitSetPosZ )

double EpUnitGetUpperRightZ( void* ptr )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	return instance->Unit::getUpperRight().z;
} SCRIPT_CALLABLE_D_PV( EpUnitGetUpperRightZ )

Tcl_Obj* EpUnitGetPos( void* ptr )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	Tcl_Obj* retObjv[3];
	retObjv[0] = Tcl_NewDoubleObj( instance->getPos().x );
	retObjv[1] = Tcl_NewDoubleObj( instance->getPos().y );
	retObjv[2] = Tcl_NewDoubleObj( instance->getPos().z );
	Tcl_Obj* ret = Tcl_NewListObj( 3, retObjv );
	return ret;
} SCRIPT_CALLABLE_OBJ_PV( EpUnitGetPos )

int EpUnitPrintTilePos( void* ptr )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	assert( instance );
	printf( "(%d, %d)\n", instance->getTilePosX(), instance->getTilePosY() );
	return 0;
} SCRIPT_CALLABLE_I_PV( EpUnitPrintTilePos )

int EpUnitSetArnMesh( void* ptr1, const char* ptr2 )
{
	Unit* instance1 = reinterpret_cast<Unit*>( ptr1 );
	ArnMesh* arnMesh = 
		static_cast<ArnMesh*>(
		static_cast<PlayState*>(
		GetTopStateManager().getState( GAME_TOP_STATE_PLAY ))->getCharacterSceneGraph()->getSceneRoot()->getNodeByName( ptr2 ));
	instance1->setArnMesh( arnMesh );
	return 0;
} SCRIPT_CALLABLE_I_PV_PC( EpUnitSetArnMesh )

int EpUnitSetColor( void* ptr, int r, int g, int b )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	instance->Unit::setColor( r, g, b );
	return 0;
} SCRIPT_CALLABLE_I_PV_I_I_I( EpUnitSetColor )

int EpUnitSetName( void* ptr, const char* name )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	std::string str( name );
	instance->Unit::setName( str );
	return 0;
} SCRIPT_CALLABLE_I_PV_PC( EpUnitSetName )

int EpUnitSetNameVisible( void* ptr, int i )
{
	Unit* instance = reinterpret_cast<Unit*>( ptr );
	instance->Unit::setNameVisible( i?true:false );
	return 0;
} SCRIPT_CALLABLE_I_PV_I( EpUnitSetNameVisible )


START_SCRIPT_FACTORY( Unit )
	//CREATE_OBJ_COMMAND( EpCreateUnit )
	CREATE_OBJ_COMMAND( EpReleaseUnit )
	CREATE_OBJ_COMMAND( EpRegisterToWorld )
	CREATE_OBJ_COMMAND( EpUnitSetRotX )
	CREATE_OBJ_COMMAND( EpUnitSetRotY )
	CREATE_OBJ_COMMAND( EpUnitSetRotZ )
	CREATE_OBJ_COMMAND( EpUnitSetPosZ )
	CREATE_OBJ_COMMAND( EpUnitGetUpperRightZ )
	CREATE_OBJ_COMMAND( EpUnitPrintTilePos )
	CREATE_OBJ_COMMAND( EpUnitGetPos )
	CREATE_OBJ_COMMAND( EpUnitSetArnMesh )
	CREATE_OBJ_COMMAND( EpUnitSetColor )
	CREATE_OBJ_COMMAND( EpUnitSetName )
	CREATE_OBJ_COMMAND( EpUnitSetNameVisible )
END_SCRIPT_FACTORY( Unit )
