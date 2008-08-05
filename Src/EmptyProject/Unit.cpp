#include "EmptyProjectPCH.h"
#include "Unit.h"
#include "ScriptManager.h"

#define KEY_WAS_DOWN_MASK 0x80
#define KEY_IS_DOWN_MASK  0x01


//////////////////////////////////////////////////////////////////////////

Unit::Unit()
{
	m_d3dxMesh = 0;
	m_pd3dDevice = 0;
	m_d3dTex = 0;

	ZeroMemory(&m_material, sizeof(D3DMATERIAL9));
	m_material.Ambient.r = m_material.Ambient.g = m_material.Ambient.b = 0.6f;
	m_material.Diffuse.r = m_material.Diffuse.g = m_material.Diffuse.b = 0.3f;
	m_material.Specular.r = m_material.Specular.g = m_material.Specular.b = 0.2f;
	m_material.Ambient.a = m_material.Diffuse.a = m_material.Specular.a = 1.0f;
	
	D3DXMatrixIdentity(&m_localXform);

	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_vPos = D3DXVECTOR3(0, 0, 0);
	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	m_bLocalXformDirty = true;
	m_bMoving = false;

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

HRESULT Unit::draw()
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

void Unit::frameMove( float fElapsedTime )
{	
	if (m_bMoving == false)
	{
		m_fMovingTime = 0;

		if( IsKeyDown( m_aKeys[UNIT_MOVE_UP] ) )
		{
			m_bMoving = true;
			m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
			m_vKeyboardDirection.y += 2.0f;
			ScriptManager::getSingleton().execute("EpUnitOnMove 0");
		}
		else if( IsKeyDown( m_aKeys[UNIT_MOVE_DOWN] ) )
		{
			m_bMoving = true;
			m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
			m_vKeyboardDirection.y -= 2.0f;
			ScriptManager::getSingleton().execute("EpUnitOnMove 1");
		}
		else if( IsKeyDown( m_aKeys[UNIT_MOVE_RIGHT] ) )
		{
			m_bMoving = true;
			m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
			m_vKeyboardDirection.x += 2.0f;
			ScriptManager::getSingleton().execute("EpUnitOnMove 2");
		}
		else if( IsKeyDown( m_aKeys[UNIT_MOVE_LEFT] ) )
		{
			m_bMoving = true;
			m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );
			m_vKeyboardDirection.x -= 2.0f;
			ScriptManager::getSingleton().execute("EpUnitOnMove 3");
		}
	}
	
	if (m_bMoving && m_fMovingTime <= 1.0f)
	{
		// Update velocity
		m_vVelocity = m_vKeyboardDirection;

		// Simple euler method to calculate position delta
		D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;
		m_fMovingTime += fElapsedTime;
		setPos(getPos() + vPosDelta);
	}
	else
	{
		m_bMoving = false;
	}

	updateLocalXform();
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

Unit* Unit::createUnit( LPD3DXMESH mesh )
{
	Unit* u = new Unit();
	u->init(G::getSingleton().m_dev, mesh);
	return u;
}
