#include "EmptyProjectPCH.h"
#include "IntroModule.h"
#include "Picture.h"

extern D3DXMATRIX						g_orthoProjMat;
extern D3DXMATRIX						g_fixedViewMat;
extern int						g_scrWidth;
extern int						g_scrHeight;

//The Great Adventure of PoolG"
//~ PoolG's Strikes Back ~"
//
//It is an age of darkness! Evil lord Choi 
//revealed his unholy desire. Republic's 
//Finance and Enviroment is already in his 
//hand. No freedom of speech, and no freedom 
//of gaming. People suffer from 20 hours 
//coding labor. There are heros on both sides.
//
//After 'Big Cleaning', PoolG lost home, food,
//dental cream, everything. For revenge, PoolG 
//trained coding very hard. He accepted the 
//dark side of coding spirit. He gained power 
//of infinite loop, memory leaking, and so on.
//Now PoolG's difficult journey is just 
//beginning.

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

	m_velocity = 1.2f;

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

	m_pStrs[0] = L"The Great Adventure of PoolG";
	m_pStrs[1] = L"~ PoolG's Strikes Back ~";
	m_pStrs[3] = L"It is an age of darkness! Evil lord Choi ";
	m_pStrs[4] = L"revealed his unholy desire. Republic's ";
	m_pStrs[5] = L"Finance and Enviroment is already in his ";
	m_pStrs[6] = L"hand. No freedom of speech, and no freedom ";
	m_pStrs[7] = L"of gaming. People suffer from 20 hours ";
	m_pStrs[8] = L"coding labor. There are heros on both sides.";
	m_pStrs[10] = L"After 'Big Cleaning', PoolG lost home, food,";
	m_pStrs[11] = L"dental cream, everything. For revenge, PoolG ";
	m_pStrs[12] = L"trained coding very hard. He accepted the ";
	m_pStrs[13] = L"dark side of coding spirit. He gained power ";
	m_pStrs[14] = L"of infinite loop, memory leaking, and so on.";	
	m_pStrs[15] = L"Now PoolG's difficult journey is just";
	m_pStrs[16] = L"beginning!";
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

	m_background.init(L"the Whirlpool Galaxy (M51) and Companion Galaxy.jpg", pd3dDevice);
	//m_background.init(L"ratatouille.jpg", pd3dDevice);
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
		0.01f, 0.2f, &pMeshNew, NULL, NULL );

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
	else
	{
		float newfTime = fTime - 6.0f;

		if ( newfTime < 5.0f )
			m_mtrlControl = newfTime / 5.0f;
		else if ( 35.0f < newfTime < 40.0f )
			m_mtrlControl = 1.0f - (newfTime - 25.0f)  / 5.0f;

		m_logoVisible = false;
		m_backgroundVisible = true;

		D3DXVECTOR3 vAxis( 0.0f, 0.0f, -1.0f );
		D3DXMatrixRotationAxis( &m_matBackground, &vAxis, D3DXToRadian( (FLOAT) newfTime ) );

		// Add some translational values to the matrices
		for (int i = 0; i < NUM_OF_LINES; i++)
		{
			D3DXMatrixTranslation( 
				&m_matObjs[i], 
				-12.0f, 
				-20 - ( float) i * 2.0f + ( float ) newfTime * m_velocity,
				0.0f );
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

	// Fixed camera
	pd3dDevice->SetTransform(D3DTS_VIEW, &g_fixedViewMat);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &g_orthoProjMat);

	// Background
	if ( m_backgroundVisible )
	{		

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

		m_background.draw();
	}

	// Logo
	if ( m_logoVisible )
	{
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

		m_pLogo.draw();
	}


	// Setup view and projection xforms
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	pd3dDevice->SetTexture(0, 0);

	pd3dDevice->SetTransform(D3DTS_VIEW, pCamera->GetViewMatrix());
	pd3dDevice->SetTransform(D3DTS_PROJECTION, pCamera->GetProjMatrix());

	if( m_pTextMeshes[0] != NULL )
    {
        ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
		D3DCOLORVALUE cv_diffuse = { 1.0f, 0.8f, 0.0f, 1.0f };
		mtrl.Diffuse  = cv_diffuse;
		D3DCOLORVALUE cv_ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
		mtrl.Ambient = cv_ambient;
		D3DCOLORVALUE cv_specular = { 1.0f, 0.8f, 0.0f, 1.0f };
		mtrl.Ambient = cv_specular;
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