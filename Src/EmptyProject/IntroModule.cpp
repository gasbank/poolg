#include "EmptyProjectPCH.h"
#include "IntroModule.h"

IntroModule::IntroModule(void)
{
	for( int i = 0; i < NUM_OF_LINES; i++ )
	{
		m_pStrs[i] = NULL;
		m_pTextMeshes[i] = NULL;
	}

	m_pStrFont = L"Arial";
	m_bBold = true;
	m_bItalic = false;
	m_dwFontSize = 10;

	m_pStrs[0] = L"                        Ǯ���� �����";
	m_pStrs[1] = L"                       ~ ����� ���� ~";
	m_pStrs[3] = L"����� �ô�! ����� ���� ���翵�� ����� ����";
	m_pStrs[4] = L"Ȥ����������. �����ǰ� ��ȭ���� �̹� ��ǵǾ���,";
	m_pStrs[5] = L"������ �������. �ؽ��� ��� ���� �ӿ�, ���ε���";
	m_pStrs[6] = L"�Ϸ� 20�ð� �ڵ� �뵿�� �ô޷ȴ�. ������ ������";
	m_pStrs[7] = L"���Ҵ�. ��û�ҷ� ���� ������ ���� Ǯ��� ������";
	m_pStrs[8] = L"�����Ͽ���, ���� ������ �ڵ� ������ ���ƴ�. ����";
	m_pStrs[9] = L"Ǯ���� �賭�� ������ ���۵Ƿ� �ϴµ�...";
	m_pStrs[11] = L"�������� �ɱ�? ���� �� ������ ����� �� �� �ִ�";
	m_pStrs[12] = L"��� ���� ��� ����.";
	m_pStrs[14] = L"�̱۰Ÿ��� ����Ʈ�� �ǵ������� ������ ������";
	m_pStrs[15] = L"Antialiasing�� ���ؼ� �׷��°� ���� �ƴϴ�.";
	m_pStrs[17] = L"F4�� �����ÿ�";
}

IntroModule::~IntroModule(void)
{
	release();
}

HRESULT IntroModule::CreateTextMeshes( IDirect3DDevice9* pd3dDevice )
{
	for( int i = 0; i < NUM_OF_LINES; i++ )
	{
		if ( m_pStrs[i] == NULL )
		{
			continue;
		}

		if( FAILED( CreateD3DXTextMesh( pd3dDevice, m_pStrs[i], &(m_pTextMeshes[i]) ) ) )
			return E_FAIL;
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
HRESULT IntroModule::CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPCWSTR pStr, LPD3DXMESH* ppTextMesh )
{
    HRESULT hr;
    LPD3DXMESH pMeshNew = NULL;
    HDC hdc = CreateCompatibleDC( NULL );
    if( hdc == NULL )
        return E_OUTOFMEMORY;
    INT nHeight = -MulDiv( m_dwFontSize, GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
    HFONT hFont;
    HFONT hFontOld;

    hFont = CreateFont( nHeight, 0, 0, 0, m_bBold ? FW_BOLD : FW_NORMAL, m_bItalic, FALSE, FALSE, DEFAULT_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
						m_pStrFont );

    hFontOld = ( HFONT )SelectObject( hdc, hFont );

	hr = D3DXCreateText( pd3dDevice, hdc, pStr,
		0.01f, 0.1f, &pMeshNew, NULL, NULL );

    SelectObject( hdc, hFontOld );
    DeleteObject( hFont );
    DeleteDC( hdc );

    if( SUCCEEDED( hr ) )
        *ppTextMesh = pMeshNew;

    return hr;
}

void IntroModule::SetCameraAndLight( IDirect3DDevice9* pd3dDevice, 
									const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
									CFirstPersonCamera* pCamera )
{

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x80808080 );

    D3DLIGHT9 light;
    D3DXVECTOR3 vecLightDirUnnormalized( 10.0f, -10.0f, 10.0f );
    ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
    D3DXVec3Normalize( ( D3DXVECTOR3* )&light.Direction, &vecLightDirUnnormalized );
    light.Position.x = 10.0f;
    light.Position.y = -10.0f;
    light.Position.z = 10.0f;
    light.Range = 1000.0f;
    pd3dDevice->SetLight( 0, &light );
    pd3dDevice->LightEnable( 0, TRUE );

    // Set the transform matrices
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity( &matWorld );
    pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    // Setup the camera with view & projection matrix
    D3DXVECTOR3 vecEye( 0.0f, -30.0f, -20.0f );
    D3DXVECTOR3 vecAt( 0.0f, 0.0f, 0.0f );
    pCamera->SetViewParams( &vecEye, &vecAt );

	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    pCamera->SetProjParams( D3DX_PI / 4, fAspectRatio, 1.0f, 1000.0f );
}

void IntroModule::frameMove( double fTime )
{
    // Setup five rotation matrices (for rotating text strings)
    D3DXVECTOR3 vAxis1( 0.0f,2.0f,0.0f );

	//D3DXMATRIX mScale;
    //D3DXMatrixScaling( &mScale, 0.5f, 0.5f, 0.5f );
	//g_matObj2 *= mScale;

    // Add some translational values to the matrices
	for (int i = 0; i < NUM_OF_LINES; i++)
	{
		D3DXMatrixRotationAxis( &m_matObjs[i], &vAxis1, 0.0f );		
		m_matObjs[i]._41 = -10.0f;   
		m_matObjs[i]._42 = -16 - ( float) i * 2.0f + ( float ) fTime / 0.5f;
		m_matObjs[i]._43 = 0.0f;
	}
}

void IntroModule::draw( IDirect3DDevice9* pd3dDevice )
{
	D3DMATERIAL9 mtrl;

	pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );

	if( m_pTextMeshes[0] != NULL )
    {
        ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
        mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
        mtrl.Diffuse.g = mtrl.Ambient.g = 0.8f;
        mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
        mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
        pd3dDevice->SetMaterial( &mtrl );

		for (int i = 0; i < NUM_OF_LINES; i++)
		{
			if ( m_pTextMeshes[i] == NULL )
				continue;

			pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObjs[i] );
			m_pTextMeshes[i]->DrawSubset( 0 );
		}
	}

}

void IntroModule::release()
{
	for( int i = 0; i < NUM_OF_LINES; i++ )
	{		
		SAFE_RELEASE( m_pTextMeshes[i] );
	}
}