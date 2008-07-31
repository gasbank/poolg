#include "EmptyProjectPCH.h"
#include "Unit.h"

Unit::Unit()
{
	m_d3dxMesh = 0;
	m_pd3dDevice = 0;
	m_d3dTex = 0;

	m_material.Ambient.r = m_material.Ambient.g = m_material.Ambient.b = 0.6f;
	m_material.Diffuse.r = m_material.Diffuse.g = m_material.Diffuse.b = 0.3f;
	m_material.Specular.r = m_material.Specular.g = m_material.Specular.b = 0.2f;
	m_material.Ambient.a = m_material.Diffuse.a = m_material.Specular.a = 1.0f;
	
	D3DXMatrixIdentity(&m_localXform);

	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_vPos = D3DXVECTOR3(0, 0, 0);
	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_bLocalXformDirty = true;
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
	return FALSE;
}

void Unit::frameMove( float fElapsedTime )
{
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