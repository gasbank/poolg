#include "EmptyProjectPCH.h"
#include "Unit.h"
#include "ScriptManager.h"
#include "WorldState.h"
#define KEY_WAS_DOWN_MASK 0x80
#define KEY_IS_DOWN_MASK  0x01


//////////////////////////////////////////////////////////////////////////

Unit::Unit()
{
	m_d3dxMesh			= 0;
	m_pd3dDevice		= 0;
	m_d3dTex			= 0;

	m_vRot				= D3DXVECTOR3(0, 0, 0);
	m_vPos				= D3DXVECTOR3(0, 0, 0);
	m_vScale			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bLocalXformDirty	= true;

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

	return hr;
}

HRESULT Unit::frameRender()
{
	HRESULT hr = S_OK;

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &m_localXform);
	m_pd3dDevice->SetTexture(0, m_d3dTex);
	m_pd3dDevice->SetMaterial(&m_material);
	m_d3dxMesh->DrawSubset(0);
	return hr;
}

LRESULT Unit::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			// Map this key to a D3DUtil_CameraKeys enum and update the
			// state of m_aKeys[] by adding the KEY_WAS_DOWN_MASK|KEY_IS_DOWN_MASK mask
			// only if the key is not down
			UnitInput mappedKey = mapKey( ( UINT )wParam );
			if( mappedKey != UNIT_UNKNOWN )
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

bool Unit::frameMove( float fElapsedTime )
{	
	

	updateLocalXform();

	return true;
}

void Unit::updateLocalXform()
{
	if (m_bLocalXformDirty)
	{
		D3DXMATRIX mRotX, mRotY, mRotZ, mScale, mTrans, mWorld;
		D3DXMatrixRotationX(&mRotX, m_vRot.x);
		D3DXMatrixRotationY(&mRotY, m_vRot.y);
		D3DXMatrixRotationZ(&mRotZ, m_vRot.z);
		D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);
		D3DXMatrixTranslation(&mTrans, m_vPos.x, m_vPos.y, m_vPos.z);
		m_localXform = mRotX * mRotY * mRotZ * mScale * mTrans;

		m_bLocalXformDirty = false;
	}
}

UnitInput Unit::mapKey( UINT nKey )
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

Unit* Unit::createUnit( LPD3DXMESH mesh, int tileX, int tileY, float posZ, bool bControllable )
{
	Unit* u = new Unit();
	u->init( GetG().m_dev, mesh );
	u->setControllable( bControllable );
	return u;
}

//////////////////////////////////////////////////////////////////////////

Unit* EpCreateUnit( int tileX, int tileY, int controllable )
{
	LPD3DXMESH d3dxMesh;
	D3DXCreateTeapot( GetG().m_dev, &d3dxMesh, 0 );

	return Unit::createUnit( d3dxMesh, tileX, tileY, 0, controllable?true:false );

} SCRIPT_CALLABLE_PV_I_I_I( EpCreateUnit )

int EpReleaseUnit( void* pv )
{
	Unit* u = reinterpret_cast<Unit*>(pv);
	EP_SAFE_RELEASE( u );
	return 0;

} SCRIPT_CALLABLE_I_PV( EpReleaseUnit )

int EpRegisterToWorld( void* pv1, void* pv2 )
{
	WorldState* w = reinterpret_cast<WorldState*>(pv1);
	Unit* u = reinterpret_cast<Unit*>(pv2);
	return w->addUnit(u);;
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




START_SCRIPT_FACTORY(Unit)
	CREATE_OBJ_COMMAND( EpCreateUnit )
	CREATE_OBJ_COMMAND( EpReleaseUnit )
	CREATE_OBJ_COMMAND( EpRegisterToWorld )
	CREATE_OBJ_COMMAND( EpUnitSetRotX )
	CREATE_OBJ_COMMAND( EpUnitSetRotY )
	CREATE_OBJ_COMMAND( EpUnitSetRotZ )
	CREATE_OBJ_COMMAND( EpUnitSetPosZ )
	CREATE_OBJ_COMMAND( EpUnitGetUpperRightZ )
END_SCRIPT_FACTORY(Unit)
