#include "EmptyProjectPCH.h"
#include "InnerFire.h"

InnerFire::InnerFire(void)
: Unit( UT_INNERFIRE )
{
	m_bInit = false;
	m_angle = 0;
	m_angleVelocity = 0;
	m_angleAccelRate = 0.1f;
}


//segment는 어떤 걸 말하는 걸까.
void InnerFire::init(const TCHAR* imgFileName, LPDIRECT3DDEVICE9 pd3dDevice, float radius, float height, int angleNumber)
{
	/*
	if ( m_bInit )
	{
		throw std::runtime_error( "Double init on the same instance! You must release before reinit this instance." );
	}
	*/

	m_radius = radius;
	m_height = height;

	//면
	UINT faces = angleNumber * 2;
	//점
	UINT vertices = (angleNumber + 1) * 2;


	if (FAILED(D3DXCreateMeshFVF(faces, vertices, D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_TEX1, pd3dDevice, &m_d3dxMesh)))
	{
		throw std::runtime_error("Mesh creation failed. Check your D3D device");
	}

	Vertex* v = 0;
	m_d3dxMesh->LockVertexBuffer(0, (void**)&v);

	int i, j;
	for (i = 0; i < 2; ++i)
	{
		for (j = 0; j < angleNumber+1; ++j)
		{
			float angle = D3DXToRadian (360.0f/angleNumber*j);

			Vertex* cv = &v[(angleNumber+1) * i + j];
			cv->x = cos (angle) * radius;
			cv->y = sin (angle) * radius;
			cv->z = i * height;
			cv->u = 1.0f/angleNumber * j;
			cv->v = (float)i;
		}
	}
	

	m_d3dxMesh->UnlockVertexBuffer();

	WORD* ind = 0;
	m_d3dxMesh->LockIndexBuffer(0, (void**)&ind);

	for (i = 0; i < angleNumber; ++i)
	{
		ind[6 * i + 0] = (WORD)(i);
		ind[6 * i + 1] = (WORD)(i + 1);
		ind[6 * i + 2] = (WORD)(i + angleNumber + 1);

		ind[6 * i + 3] = (WORD)(i + 1);
		ind[6 * i + 4] = (WORD)(i + 2 + angleNumber);
		ind[6 * i + 5] = (WORD)(i + 1 + angleNumber);
	}


	m_d3dxMesh->UnlockIndexBuffer();

	if (FAILED(D3DXCreateTextureFromFile(pd3dDevice, imgFileName, &m_d3dTex)))
	{
		// ! Programmatical debug breakpoint: You can continue the program by pressing F5
		DebugBreak();

		// ! Programmatical debug breakpoint: You can continue the program by pressing F5
		m_d3dTex = 0;
		OutputDebugStringA( "--- Texture creation failed; Maybe file name error?\n" );
	}
	else
	{
		D3DSURFACE_DESC texDesc;
		m_d3dTex->GetLevelDesc(0, &texDesc);
		m_texWidth = (float)texDesc.Width;
		m_texHeight = (float)texDesc.Height;
	}

	setPos( DX_CONSTS::D3DXVEC3_ZERO );

	m_bInit = true;
}

void InnerFire::release()
{
	SAFE_RELEASE(m_d3dxMesh);
	SAFE_RELEASE(m_d3dTex);
}

HRESULT InnerFire::draw(bool textured)
{
	HRESULT hr = S_OK;

	if (m_d3dxMesh)
	{
		// Non-rhw drawing
		GetG().m_dev->SetTransform(D3DTS_WORLD, &getLocalXform());
		if (textured)
			GetG().m_dev->SetTexture(0, m_d3dTex);
		else
			GetG().m_dev->SetTexture(0, 0);
		m_d3dxMesh->DrawSubset(0);
	}
	else
	{
		assert( !"Should not be called" );
	}
	return hr;
}

bool InnerFire::frameMove( float fElapsedTime )
{
	m_angleVelocity += m_angleAccelRate;
	m_angle += m_angleVelocity;

	if (m_angle > 360*100)
		m_angle = 0;

	if (m_angleVelocity > 10000)
		m_angleVelocity = 0;

	setRotZ( D3DXToRadian( m_angle ) );
	setScaleX( 1.0f );
	setScaleY( 1.0f );
	setScaleZ( 1.0f );

	return Unit::frameMove( fElapsedTime );
}

