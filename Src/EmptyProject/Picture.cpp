#include "EmptyProjectPCH.h"
#include "Picture.h"

Picture::Picture(void)
{
	m_d3dxMesh = 0;
	m_d3dTex = 0;
	m_d3dVB = 0;
	m_width = m_height = 0;
	m_bInit = false;
}

Picture::~Picture(void)
{
	release();
}

void Picture::init(const TCHAR* imgFileName, LPDIRECT3DDEVICE9 d3dDev, UINT segments, float x, float y )
{
	if ( m_bInit )
	{
		throw std::runtime_error( "Double init on the same instance! You must release before reinit this instance." );
	}
	UINT faces = segments * segments * 2;
	UINT vertices = (segments+1) * (segments+1);

	m_d3dDev = d3dDev;

	D3DXMatrixIdentity(&m_localXform);

	if (FAILED(D3DXCreateMeshFVF(faces, vertices, D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_TEX1, d3dDev, &m_d3dxMesh)))
	{
		throw std::runtime_error("Mesh creation failed. Check your D3D device");
	}

	Vertex* v = 0;
	m_d3dxMesh->LockVertexBuffer(0, (void**)&v);

	UINT i, j;
	for (i = 0; i < segments+1; ++i)
	{
		for (j = 0; j < segments+1; ++j)
		{
			Vertex* cv = &v[(segments+1) * i + j];
			cv->x = 1.0f/segments * j;
			cv->y = 1.0f/segments * i;
			cv->z = 0;
			cv->u = 1.0f/segments * j;
			cv->v = abs(cv->y - 1);
		}
	}
	

	m_d3dxMesh->UnlockVertexBuffer();

	WORD* ind = 0;
	m_d3dxMesh->LockIndexBuffer(0, (void**)&ind);

	for (i = 0; i < segments; ++i)
	{
		for (j = 0; j < segments; ++j)
		{
			ind[6 * (segments * i + j) + 0] = (WORD)((segments + 1) * i + j);
			ind[6 * (segments * i + j) + 1] = (WORD)((segments + 1) * i + j + 1 + segments);
			ind[6 * (segments * i + j) + 2] = (WORD)((segments + 1) * i + j + 1);

			ind[6 * (segments * i + j) + 3] = (WORD)((segments + 1) * i + j + 1);
			ind[6 * (segments * i + j) + 4] = (WORD)((segments + 1) * i + j + 1 + segments);
			ind[6 * (segments * i + j) + 5] = (WORD)((segments + 1) * i + j + 1 + segments + 1);
		}
	}

	m_d3dxMesh->UnlockIndexBuffer();

	if (FAILED(D3DXCreateTextureFromFile(d3dDev, imgFileName, &m_d3dTex)))
	{
		throw std::runtime_error("Texture creation failed; Maybe file name error?");
	}

	D3DSURFACE_DESC texDesc;
	m_d3dTex->GetLevelDesc(0, &texDesc);

	m_vPos.x = x;
	m_vPos.y = y;
	m_vPos.z = 0;
	m_texWidth = (float)texDesc.Width;
	m_texHeight = (float)texDesc.Height;

	m_bInit = true;
}

HRESULT Picture::initRhw( const TCHAR* imgFileName, LPDIRECT3DDEVICE9 d3dDev, float x, float y )
{
	HRESULT hr = S_OK;

	m_d3dDev = d3dDev;

	D3DXMatrixIdentity(&m_localXform);

	V(D3DXCreateTextureFromFile(d3dDev, imgFileName, &m_d3dTex));

	V(d3dDev->CreateVertexBuffer(4 * sizeof(VertexRhw), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_TEX1, D3DPOOL_MANAGED, &m_d3dVB, 0));

	D3DSURFACE_DESC texDesc;
	m_d3dTex->GetLevelDesc(0, &texDesc);

	VertexRhw* v = 0;
	m_d3dVB->Lock(0, 0, (void**)&v, 0);
	
	v[0] = VertexRhw(x,                 y,                  0, 1, 0, 0);
	v[1] = VertexRhw(x + texDesc.Width, y,                  0, 1, 1, 0);
	v[2] = VertexRhw(x + texDesc.Width, y + texDesc.Height, 0, 1, 1, 1);
	v[3] = VertexRhw(x,                 y + texDesc.Height, 0, 1, 0, 1);

	m_d3dVB->Unlock();

	return hr;
	
}

void Picture::release()
{
	SAFE_RELEASE(m_d3dxMesh);
	SAFE_RELEASE(m_d3dTex);
	SAFE_RELEASE(m_d3dVB);
}

HRESULT Picture::draw(bool textured)
{
	HRESULT hr = S_OK;

	if (m_d3dxMesh)
	{
		// Non-rhw drawing
		m_d3dDev->SetTransform(D3DTS_WORLD, &m_localXform);
		if (textured)
			m_d3dDev->SetTexture(0, m_d3dTex);
		else
			m_d3dDev->SetTexture(0, 0);
		m_d3dxMesh->DrawSubset(0);
	}
	else if (m_d3dVB)
	{
		// RHW drawing
		m_d3dDev->SetTexture(0, m_d3dTex);
		m_d3dDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
		m_d3dDev->SetStreamSource(0, m_d3dVB, 0, sizeof(VertexRhw));
		m_d3dDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	}
	else
	{
		assert( !"Should not be called" );
	}
	return hr;
}


LRESULT Picture::handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( hWnd );
	UNREFERENCED_PARAMETER( lParam );

	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			// Map this key to a D3DUtil_CameraKeys enum and update the
			// state of m_aKeys[] by adding the KEY_WAS_DOWN_MASK|KEY_IS_DOWN_MASK mask
			// only if the key is not down
			PictureInput mappedKey = mapKey( ( UINT )wParam );
			if( mappedKey != PIC_UNKNOWN )
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
			PictureInput mappedKey = mapKey( ( UINT )wParam );
			if( mappedKey != PIC_UNKNOWN && ( DWORD )mappedKey < 8 )
			{
				m_aKeys[ mappedKey ] &= ~KEY_IS_DOWN_MASK;
				--m_cKeysDown;
			}
			break;
		}

	}

	return FALSE;
}

PictureInput Picture::mapKey( UINT nKey )
{
	switch( nKey )
	{
	case VK_LEFT:	return PIC_MOVE_LEFT;
	case VK_RIGHT:	return PIC_MOVE_RIGHT;
	case VK_UP:		return PIC_MOVE_UP;
	case VK_DOWN:	return PIC_MOVE_DOWN;
	}

	return PIC_UNKNOWN;
}

void Picture::frameMove( float fElapsedTime )
{
	m_vKeyboardDirection = D3DXVECTOR3( 0, 0, 0 );

	//// Update acceleration vector based on keyboard state
	
	//if( IsKeyDown( m_aKeys[PIC_MOVE_UP] ) )
	//	m_vKeyboardDirection.y += 0.5f;
	//if( IsKeyDown( m_aKeys[PIC_MOVE_DOWN] ) )
	//	m_vKeyboardDirection.y -= 0.5f;
	//if( IsKeyDown( m_aKeys[PIC_MOVE_RIGHT] ) )
	//	m_vKeyboardDirection.x += 0.5f;
	//if( IsKeyDown( m_aKeys[PIC_MOVE_LEFT] ) )
	//	m_vKeyboardDirection.x -= 0.5f;

	// Update velocity
	m_vVelocity = m_vKeyboardDirection;

	// Simple euler method to calculate position delta
	D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;
	m_vPos += vPosDelta;

	D3DXMATRIX mScaling, mTranslation;
	D3DXMatrixScaling(&mScaling, m_width, m_height, 1.0f);
	D3DXMatrixTranslation(&mTranslation, m_vPos.x, m_vPos.y, m_vPos.z);

	m_localXform = mScaling * mTranslation;
}

void Picture::setSizeToTexture()
{
	m_width = m_texWidth;
	m_height = m_texHeight;

	D3DXMATRIX rotMat, scaleMat, transMat;
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixScaling(&scaleMat, m_width, m_height, 1.0f);
	D3DXMatrixTranslation(&transMat, m_vPos.x, m_vPos.y, 10.0f);
	
	m_localXform = rotMat * scaleMat * transMat;
}