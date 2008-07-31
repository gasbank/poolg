#include "EmptyProjectPCH.h"
#include "IntroModule.h"
#include "Picture.h"

extern D3DXMATRIX						g_orthoProjMat;
extern D3DXMATRIX						g_fixedViewMat;
extern int						g_scrWidth;
extern int						g_scrHeight;

IntroModule::IntroModule(void)
{
	for( int i = 0; i < NUM_OF_LINES; i++ )
	{
		m_pStrs[i] = NULL;
		m_pTextMeshes[i] = NULL;
	}

	m_pStrFont = L"Georgia";
	m_bBold = true;
	m_bItalic = false;
	m_dwFontSize = 10;

	m_backgroundVisible = false;
	m_logoVisible = false;

	m_velocity = 1.0f;

	/*m_pStrs[0] = L"                        Ǯ���� �����";
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
	m_pStrs[17] = L"F4�� �����ÿ�";*/

	m_pStrs[0] = L"              The Great Adventure of PoolG";
	m_pStrs[1] = L"                ~ PoolG's Strikes Back ~";
	m_pStrs[3] = L"It is an age of darkness! Evil lord Choi revealed";
	m_pStrs[4] = L"his unholy desire.";
	
	/*
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
	m_pStrs[17] = L"F4�� �����ÿ�";*/
	
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

	//m_background.init(L"the Whirlpool Galaxy (M51) and Companion Galaxy.jpg", pd3dDevice);
	m_background.init(L"ratatouille.jpg", pd3dDevice);
	m_pLogo.init( L"poolc.png", pd3dDevice );
	m_pBlack.init( L"the Whirlpool Galaxy (M51) and Companion Galaxy.jpg", pd3dDevice);

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
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
 //   pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
 //   pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    //pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	
    D3DLIGHT9 light;
    D3DXVECTOR3 vecLightDirUnnormalized( 10.0f, -10.0f, 10.0f );
    ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;
	light.Ambient.r = 1.0f;
	light.Ambient.g = 1.0f;
	light.Ambient.b = 1.0f;
	D3DXVec3Normalize( ( D3DXVECTOR3* )&light.Direction, &vecLightDirUnnormalized );
    light.Position.x = 5.0f;
    light.Position.y = 5.0f;
    light.Position.z = -5.0f;
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
	if ( fTime < 3.0f )
	{		
		m_logoVisible = true;
		m_logoFading = fTime / 3.0f;
	}
	else if ( fTime < 6.0f )
	{
		double newfTime = fTime - 3.0f;
		m_logoFading = 1 - newfTime / 3.0f;
	}
	else if ( 7.0f < fTime )

	{
		double newfTime = fTime - 6.0f;

		if ( m_mtrlControl < 1.0f )
			m_mtrlControl = newfTime / 5.0f;

		m_logoVisible = false;
		m_backgroundVisible = true;

		D3DXVECTOR3 vAxis( 0.0f, 0.0f, -1.0f );
		D3DXMatrixRotationAxis( &m_matBackground, &vAxis, D3DXToRadian( (FLOAT) newfTime ) );

		// Setup five rotation matrices (for rotating text strings)
		D3DXVECTOR3 vAxis1( 0.0f,2.0f,0.0f );

		// Add some translational values to the matrices
		for (int i = 0; i < NUM_OF_LINES; i++)
		{
			D3DXMatrixRotationAxis( &m_matObjs[i], &vAxis1, 0.0f );		
			m_matObjs[i]._41 = -10.0f;   
			m_matObjs[i]._42 = -20 - ( float) i * 2.0f + ( float ) newfTime * m_velocity;
			m_matObjs[i]._43 = 0.0f;
		}
	}
}

void IntroModule::draw( IDirect3DDevice9* pd3dDevice, CFirstPersonCamera* pCamera )
{
	D3DMATERIAL9 mtrl;

	//pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );

	//// Black
	//pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	//pd3dDevice->SetTransform(D3DTS_VIEW, &g_fixedViewMat);
	//pd3dDevice->SetTransform(D3DTS_PROJECTION, &g_orthoProjMat);

	//D3DXMATRIX mScaleb, mTransb, mWorldb;
	//D3DXMatrixScaling(&mScaleb, (FLOAT) g_scrWidth * 2.0, (FLOAT) g_scrHeight * 2.0 , 1.0f);
	//D3DXMatrixTranslation(&mTransb, (FLOAT) -g_scrWidth, (FLOAT) -g_scrHeight, 50.0f);

	//mWorldb = mScaleb * mTransb;

	//m_pBlack.setLocalXform(&mWorldb);

	//m_pBlack.draw();

	// Background
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	pd3dDevice->SetTransform(D3DTS_VIEW, &g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &g_orthoProjMat);

	D3DXMATRIX mScale, mTrans, mWorld;
	D3DXMatrixScaling(&mScale, (FLOAT) g_scrWidth * 2.0, (FLOAT) g_scrHeight * 2.0 , 1.0f);
	D3DXMatrixTranslation(&mTrans, (FLOAT) -g_scrWidth, (FLOAT) -g_scrHeight, 49.0f);

	mWorld = mScale * mTrans * m_matBackground;

	ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
	mtrl.Diffuse.r = mtrl.Ambient.r = mtrl.Specular.r = m_mtrlControl;
    mtrl.Diffuse.g = mtrl.Ambient.g = mtrl.Specular.g = m_mtrlControl;
	mtrl.Diffuse.b = mtrl.Ambient.b = mtrl.Specular.b = m_mtrlControl;
    mtrl.Diffuse.a = mtrl.Ambient.a = mtrl.Specular.a = m_mtrlControl;
    pd3dDevice->SetMaterial( &mtrl );

	m_background.setLocalXform(&mWorld);

	if ( m_backgroundVisible )
		m_background.draw();


	// Logo
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	pd3dDevice->SetTransform(D3DTS_VIEW, &g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &g_orthoProjMat);

	D3DXMATRIX mScale2, mTrans2, mWorld2;
	//D3DXMatrixScaling(&mScale2, (FLOAT) g_scrWidth * 0.5f, (FLOAT) g_scrHeight * 0.5f , 1.0f);
	//D3DXMatrixTranslation(&mTrans2, (FLOAT) -g_scrWidth / 4.0f, (FLOAT) -g_scrHeight / 4.0f, 0.0f);

	D3DXMatrixScaling(&mScale2, (FLOAT) 109.0, (FLOAT) 49.0, 1.0f);
	D3DXMatrixTranslation(&mTrans2, (FLOAT) -109.0 / 2.0f, (FLOAT) -49.0 / 2.0f, 0.0f);

	mWorld2 = mScale2 * mTrans2;

	D3DMATERIAL9 mtrlLogo;

	ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
	mtrl.Diffuse.r = mtrl.Ambient.r = mtrl.Specular.r = m_logoFading;
    mtrl.Diffuse.g = mtrl.Ambient.g = mtrl.Specular.g = m_logoFading;
	mtrl.Diffuse.b = mtrl.Ambient.b = mtrl.Specular.b = m_logoFading;
    mtrl.Diffuse.a = mtrl.Ambient.a = mtrl.Specular.a = 0.1f;
    pd3dDevice->SetMaterial( &mtrl );

	m_pLogo.setLocalXform(&mWorld2);

	if ( m_logoVisible )
		m_pLogo.draw();



	// Setup view and projection xforms
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	pd3dDevice->SetTexture(0, 0);

	pd3dDevice->SetTransform(D3DTS_VIEW, pCamera->GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, pCamera->GetProjMatrix());

	if( m_pTextMeshes[0] != NULL )
    {
        ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
		mtrl.Diffuse.r = mtrl.Ambient.r = mtrl.Specular.r = 1.0f * m_mtrlControl;
		mtrl.Diffuse.g = mtrl.Ambient.g = mtrl.Specular.g = 0.8f * m_mtrlControl;
		mtrl.Diffuse.b = mtrl.Ambient.b = mtrl.Specular.b = 0.0f * m_mtrlControl;
		mtrl.Diffuse.a = mtrl.Ambient.a = mtrl.Specular.a = 1.0f * m_mtrlControl ;
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

	m_background.release();
	m_pLogo.release();
	m_pBlack.release();
}