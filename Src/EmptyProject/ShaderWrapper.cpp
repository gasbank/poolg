#include "EmptyProjectPCH.h"
#include "ShaderWrapper.h"
#include "ArnMath.h"

Shader::Shader(void)
{
	m_dev					= 0;
	m_pVertexShader			= 0;
	m_pConstantTable		= 0;
	m_pVertexDeclaration	= 0;
	m_effect				= 0;
	m_decl					= 0;
	m_hDefaultTech			= 0;
}

Shader::~Shader(void)
{
	SAFE_DELETE_ARRAY( m_decl );
}

HRESULT Shader::initShader( LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* shaderFileName )
{
	HRESULT hr = S_OK;
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	m_dev = pd3dDevice;
	V_RETURN( m_dev->CreateVertexDeclaration( decl, &m_pVertexDeclaration ) );
	V_RETURN( StringCchCopy( m_shaderFileName, sizeof(m_shaderFileName)/sizeof(TCHAR), shaderFileName ) );
	return hr;
}

HRESULT Shader::compileShader( const char* functionName, const char* profile, DWORD dwShaderFlags )
{
	HRESULT hr;
	LPD3DXBUFFER pCode;

	V_RETURN( D3DXCompileShaderFromFile( m_shaderFileName, NULL, NULL, functionName, profile, dwShaderFlags, &pCode, NULL, &m_pConstantTable ) );

	// Create the vertex shader
	hr = m_dev->CreateVertexShader(reinterpret_cast<const DWORD *>(pCode->GetBufferPointer()), &m_pVertexShader );
	pCode->Release();

	if( FAILED( hr ) )
		return DXTRACE_ERR( TEXT( "CreateVertexShader" ), hr );

	return S_OK;
}

void Shader::update( float fTime, float fElapsedTime )
{
	// Draw picture map with shader settings
	ArnMatrix mWorldViewProj;
	ArnMatrixIdentity( &mWorldViewProj );
	m_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mWorldViewProj", (const D3DXMATRIX*)mWorldViewProj.m );

	// Set up the vertex shader constants
	ArnMatrix mViewProj; //= *camera.GetViewMatrix() * *camera.GetProjMatrix();
	ArnMatrixIdentity( &mViewProj );
	m_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mViewProj", (const D3DXMATRIX*)mViewProj.m );
	m_pConstantTable->SetFloat( DXUTGetD3D9Device(), "fTime", ( float )fTime );

}

void Shader::release()
{
	onDestroyDevice();
}

HRESULT Shader::initEffect( LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* shaderFileName, DWORD dwShaderFlags )
{
	HRESULT hr = S_OK;
	m_dev = pd3dDevice;

	// If this fails, there should be debug output as to 
	// they the .fx file failed to compile
	if (FAILED(hr = D3DXCreateEffectFromFile( m_dev, shaderFileName, NULL, NULL, dwShaderFlags, NULL, &m_effect, NULL )))
	{
		OutputDebugString(_T("--D3DXCreateEffectFromFile() failed. Shader will not activated.\n"));
		return hr;
	}
	
	m_hDefaultTech = m_effect->GetTechniqueByName( "Main" );
	hr = m_effect->ValidateTechnique(m_hDefaultTech);
	if (FAILED(hr))
	{
		OutputDebugString(_T("*** ID3DXEffect::ValidateTechnique() failed. Check out the debug output messages related to 'D3DXCreateEffectFromFile()' call.\n"));
		m_hDefaultTech = 0;
	}
	return hr;
}

HRESULT Shader::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	HRESULT hr = S_OK;
	if( m_effect )
	{
		V_RETURN( m_effect->OnResetDevice() );
	}
	return hr;
}

void CALLBACK Shader::onDestroyDevice()
{
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pConstantTable );
	SAFE_RELEASE( m_pVertexDeclaration );
	SAFE_RELEASE( m_effect );
}

void CALLBACK Shader::onLostDevice()
{
	if ( m_effect )
		m_effect->OnLostDevice();
}

HRESULT CALLBACK Shader::onCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	return S_OK;
}

HRESULT Shader::setMainTechnique()
{
	HRESULT hr;
	if (m_effect && m_hDefaultTech)
	{
		hr = m_effect->SetTechnique( m_hDefaultTech );
	}
	else
	{
		hr = E_FAIL;
	}
	return hr;
}
//////////////////////////////////////////////////////////////////////////


BombShader::BombShader()
: m_fireTexture( 0 )
{

}
BombShader::~BombShader()
{
	SAFE_RELEASE( m_fireTexture );
}


HRESULT CALLBACK BombShader::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	HRESULT hr = S_OK;

	V_RETURN( D3DXCreateTextureFromFile( m_dev, L"Shaders/Textures/FireGrade.bmp", &m_fireTexture ) );
	V_RETURN( m_effect->SetTexture( "gGradeTex", m_fireTexture) );

	Shader::onResetDevice( pd3dDevice, pBackBufferSurfaceDesc );
	return hr;
}

HRESULT BombShader::setWorldViewProj( double fTime, float fElapsedTime, const ArnMatrix* mWorld, const ArnMatrix* mView, const ArnMatrix* mProj )
{
	HRESULT hr = S_OK;

	ArnMatrix wvp = *mWorld * *mView * *mProj;
	V_RETURN( m_effect->SetMatrix( "gWvpXf", (const D3DXMATRIX*)wvp.m ) );
	V_RETURN( m_effect->SetFloat( "gTimer", (float)fTime) );
	

	return hr;
}

HRESULT CALLBACK BombShader::onCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	initEffect( pd3dDevice, L"Shaders/HLSL/vbomb.fx" );
	return S_OK;
}

void CALLBACK BombShader::onLostDevice()
{
	SAFE_RELEASE( m_fireTexture );

	Shader::onLostDevice();
}

//////////////////////////////////////////////////////////////////////////()

HRESULT CALLBACK AlphaShader::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT CALLBACK AlphaShader::onCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	HRESULT hr = S_OK;

	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};

	m_decl = new D3DVERTEXELEMENT9[ sizeof( decl ) / sizeof( D3DVERTEXELEMENT9 ) ];
	memcpy( m_decl, decl, sizeof( decl ) );

	m_dev = pd3dDevice;
	V_RETURN( m_dev->CreateVertexDeclaration( decl, &m_pVertexDeclaration ) );
	V_RETURN( StringCchCopy( m_shaderFileName, sizeof(m_shaderFileName)/sizeof(TCHAR), L"Shaders/Alpha.vsh" ) );


	return hr;


}
//////////////////////////////////////////////////////////////////////////

const DWORD                 SCREEN_VERTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;



HRESULT CALLBACK MotionBlurShader::onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	HRESULT hr;

	if( m_effect )
		V_RETURN( m_effect->OnResetDevice() );

	// Create a sprite to help batch calls when drawing many lines of text
	V_RETURN( D3DXCreateSprite( pd3dDevice, &m_textSprite ) );


	// Create a A8R8G8B8 render target texture.  This will be used to render 
	// the full screen and then rendered to the backbuffer using a quad.
	V_RETURN( D3DXCreateTexture( pd3dDevice, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &g_pFullScreenRenderTarget ) );

	// Create two floating-point render targets with at least 2 channels.  These will be used to store 
	// velocity of each pixel (one for the current frame, and one for last frame).
	V_RETURN( D3DXCreateTexture( pd3dDevice, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height,
		1, D3DUSAGE_RENDERTARGET, g_VelocityTexFormat,
		D3DPOOL_DEFAULT, &g_pPixelVelocityTexture1 ) );
	V_RETURN( D3DXCreateTexture( pd3dDevice, pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height,
		1, D3DUSAGE_RENDERTARGET, g_VelocityTexFormat,
		D3DPOOL_DEFAULT, &g_pPixelVelocityTexture2 ) );

	// Store pointers to surfaces so we can call SetRenderTarget() later
	V_RETURN( g_pFullScreenRenderTarget->GetSurfaceLevel( 0, &g_pFullScreenRenderTargetSurf ) );
	V_RETURN( g_pPixelVelocityTexture1->GetSurfaceLevel( 0, &g_pPixelVelocitySurf1 ) );
	V_RETURN( g_pPixelVelocityTexture2->GetSurfaceLevel( 0, &g_pPixelVelocitySurf2 ) );

	
	// Setup render target sets
	if( 1 == g_nPasses )
	{
		// Multiple RTs

		// First frame
		g_aRTSet[0].pRT[0][0] = g_pFullScreenRenderTargetSurf;
		g_aRTSet[0].pRT[0][1] = g_pPixelVelocitySurf1;
		g_aRTSet[0].pRT[1][0] = NULL;  // 2nd pass is not needed
		g_aRTSet[0].pRT[1][1] = NULL;  // 2nd pass is not needed

		// Second frame
		g_aRTSet[1].pRT[0][0] = g_pFullScreenRenderTargetSurf;
		g_aRTSet[1].pRT[0][1] = g_pPixelVelocitySurf2;
		g_aRTSet[1].pRT[1][0] = NULL;  // 2nd pass is not needed
		g_aRTSet[1].pRT[1][1] = NULL;  // 2nd pass is not needed
	}
	else
	{
		// Single RT, multiple passes

		// First frame
		g_aRTSet[0].pRT[0][0] = g_pFullScreenRenderTargetSurf;
		g_aRTSet[0].pRT[0][1] = NULL;  // 2nd RT is not needed
		g_aRTSet[0].pRT[1][0] = g_pPixelVelocitySurf1;
		g_aRTSet[0].pRT[1][1] = NULL;  // 2nd RT is not needed

		// Second frame
		g_aRTSet[1].pRT[0][0] = g_pFullScreenRenderTargetSurf;
		g_aRTSet[1].pRT[0][1] = NULL;  // 2nd RT is not needed
		g_aRTSet[1].pRT[1][0] = g_pPixelVelocitySurf2;
		g_aRTSet[1].pRT[1][1] = NULL;  // 2nd RT is not needed
	}

	// Setup the current & last pointers that are swapped every frame.
	g_pCurFrameVelocityTexture = g_pPixelVelocityTexture1;
	g_pLastFrameVelocityTexture = g_pPixelVelocityTexture2;
	g_pCurFrameVelocitySurf = g_pPixelVelocitySurf1;
	g_pLastFrameVelocitySurf = g_pPixelVelocitySurf2;
	g_pCurFrameRTSet = &g_aRTSet[0];
	g_pLastFrameRTSet = &g_aRTSet[1];


	D3DXCOLOR colorWhite( 1.0f, 1.0f, 1.0f, 1.0f );
	D3DXCOLOR colorBlack( 0.0f, 0.0f, 0.0f, 1.0f );
	D3DXCOLOR colorAmbient( 0.35f, 0.35f, 0.35f, 0 );
	D3DXCOLOR colorSpecular( 0.5f, 0.5f, 0.5f, 1.0f );
	V_RETURN( m_effect->SetVector( "MaterialAmbientColor", ( D3DXVECTOR4* )&colorAmbient ) );
	V_RETURN( m_effect->SetVector( "MaterialDiffuseColor", ( D3DXVECTOR4* )&colorWhite ) );
	V_RETURN( m_effect->SetTexture( "RenderTargetTexture", g_pFullScreenRenderTarget ) );

	D3DSURFACE_DESC desc;
	V_RETURN( g_pFullScreenRenderTargetSurf->GetDesc( &desc ) );
	V_RETURN( m_effect->SetFloat( "RenderTargetWidth", ( FLOAT )desc.Width ) );
	V_RETURN( m_effect->SetFloat( "RenderTargetHeight", ( FLOAT )desc.Height ) );

	// 12 is the number of samples in our post-process pass, so we don't want 
	// pixel velocity of more than 12 pixels or else we'll see artifacts
	float fVelocityCapInPixels = 3.0f;
	float fVelocityCapNonHomogeneous = fVelocityCapInPixels * 2 / pBackBufferSurfaceDesc->Width;
	float fVelocityCapSqNonHomogeneous = fVelocityCapNonHomogeneous * fVelocityCapNonHomogeneous;

	V_RETURN( m_effect->SetFloat( "VelocityCapSq", fVelocityCapSqNonHomogeneous ) );
	V_RETURN( m_effect->SetFloat( "ConvertToNonHomogeneous", 1.0f / pBackBufferSurfaceDesc->Width ) );

	// Determine the technique to use when rendering world based on # of passes.
	if( 1 == g_nPasses )
		g_hTechWorldWithVelocity = m_effect->GetTechniqueByName( "WorldWithVelocityMRT" );
	else
		g_hTechWorldWithVelocity = m_effect->GetTechniqueByName( "WorldWithVelocityTwoPasses" );

	g_hPostProcessMotionBlur = m_effect->GetTechniqueByName( "PostProcessMotionBlur" );
	g_hWorld = m_effect->GetParameterByName( NULL, "mWorld" );
	g_hWorldLast = m_effect->GetParameterByName( NULL, "mWorldLast" );
	g_hWorldViewProjection = m_effect->GetParameterByName( NULL, "mWorldViewProjection" );
	g_hWorldViewProjectionLast = m_effect->GetParameterByName( NULL, "mWorldViewProjectionLast" );
	g_hMeshTexture = m_effect->GetParameterByName( NULL, "MeshTexture" );
	g_hCurFrameVelocityTexture = m_effect->GetParameterByName( NULL, "CurFrameVelocityTexture" );
	g_hLastFrameVelocityTexture = m_effect->GetParameterByName( NULL, "LastFrameVelocityTexture" );

	// Turn off lighting since its done in the shaders 
	V_RETURN( pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ) );

	// Save a pointer to the orignal render target to restore it later
	LPDIRECT3DSURFACE9 pOriginalRenderTarget;
	V_RETURN( pd3dDevice->GetRenderTarget( 0, &pOriginalRenderTarget ) );

	// Clear each RT
	V_RETURN( pd3dDevice->SetRenderTarget( 0, g_pFullScreenRenderTargetSurf ) );
	V_RETURN( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0 ) );
	V_RETURN( pd3dDevice->SetRenderTarget( 0, g_pLastFrameVelocitySurf ) );
	V_RETURN( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0 ) );
	V_RETURN( pd3dDevice->SetRenderTarget( 0, g_pCurFrameVelocitySurf ) );
	V_RETURN( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0 ) );

	// Restore the orignal RT
	V_RETURN( pd3dDevice->SetRenderTarget( 0, pOriginalRenderTarget ) );
	SAFE_RELEASE( pOriginalRenderTarget );

	return S_OK;
}

HRESULT MotionBlurShader::onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{

	// Query multiple RT setting and set the num of passes required
	D3DCAPS9 Caps;
	pd3dDevice->GetDeviceCaps( &Caps );
	if( Caps.NumSimultaneousRTs < 2 )
	{
		g_nPasses = 2;
		g_nRtUsed = 1;
	}
	else
	{
		g_nPasses = 1;
		g_nRtUsed = 2;
	}
	// Determine which of D3DFMT_G16R16F or D3DFMT_A16B16G16R16F to use for velocity texture
	IDirect3D9* pD3D;
	pd3dDevice->GetDirect3D( &pD3D );
	D3DDISPLAYMODE DisplayMode;
	pd3dDevice->GetDisplayMode( 0, &DisplayMode );
	if( FAILED( pD3D->CheckDeviceFormat( Caps.AdapterOrdinal, Caps.DeviceType,
		DisplayMode.Format, D3DUSAGE_RENDERTARGET,
		D3DRTYPE_TEXTURE, D3DFMT_G16R16F ) ) )
		g_VelocityTexFormat = D3DFMT_A16B16G16R16F;
	else
		g_VelocityTexFormat = D3DFMT_G16R16F;

	return Shader::onCreateDevice( pd3dDevice, pBackBufferSurfaceDesc );

}

void MotionBlurShader::onLostDevice()
{
	SAFE_RELEASE( m_textSprite );
	SAFE_RELEASE( g_pFullScreenRenderTargetSurf );
	SAFE_RELEASE( g_pFullScreenRenderTarget );
	SAFE_RELEASE( g_pPixelVelocitySurf1 );
	SAFE_RELEASE( g_pPixelVelocityTexture1 );
	SAFE_RELEASE( g_pPixelVelocitySurf2 );
	SAFE_RELEASE( g_pPixelVelocityTexture2 );

	Shader::onLostDevice();
}

void MotionBlurShader::onDestroyDevice()
{
	SAFE_RELEASE( g_pFullScreenRenderTargetSurf );
	SAFE_RELEASE( g_pFullScreenRenderTarget );

	Shader::onDestroyDevice();
}

void MotionBlurShader::onFrameMove( double dTime, float fElapsedTime )
{

}

void MotionBlurShader::onFrameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime )
{

	HRESULT hr;
	LPDIRECT3DSURFACE9 apOriginalRenderTarget[2] = { NULL, NULL };
	ArnMatrix mProj;
	ArnMatrix mView;
	ArnMatrix mViewProjection;
	ArnMatrix mWorldViewProjection;
	ArnMatrix mWorldViewProjectionLast;
	ArnMatrix mWorld;
	D3DXVECTOR4 vEyePt;
	UINT iPass, cPasses;

	// Swap the current frame's per-pixel velocity texture with  
	// last frame's per-pixel velocity texture
	LPDIRECT3DTEXTURE9 pTempTex = g_pCurFrameVelocityTexture;
	g_pCurFrameVelocityTexture = g_pLastFrameVelocityTexture;
	g_pLastFrameVelocityTexture = pTempTex;

	LPDIRECT3DSURFACE9 pTempSurf = g_pCurFrameVelocitySurf;
	g_pCurFrameVelocitySurf = g_pLastFrameVelocitySurf;
	g_pLastFrameVelocitySurf = pTempSurf;

	CRenderTargetSet* pTempRTSet = g_pCurFrameRTSet;
	g_pCurFrameRTSet = g_pLastFrameRTSet;
	g_pLastFrameRTSet = pTempRTSet;

	// Save a pointer to the current render target in the swap chain
	V( pd3dDevice->GetRenderTarget( 0, &apOriginalRenderTarget[0] ) );

	V( m_effect->SetFloat( "PixelBlurConst", g_fPixelBlurConst ) );

	V( m_effect->SetTexture( g_hCurFrameVelocityTexture, g_pCurFrameVelocityTexture ) );
	V( m_effect->SetTexture( g_hLastFrameVelocityTexture, g_pLastFrameVelocityTexture ) );

	// Clear the velocity render target to 0
	V( pd3dDevice->SetRenderTarget( 0, g_pCurFrameVelocitySurf ) );
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0 ) );

	// Clear the full screen render target to the background color
	V( pd3dDevice->SetRenderTarget( 0, g_pFullScreenRenderTargetSurf ) );
	V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, g_dwBackgroundColor, 1.0f, 0 ) );

	// Turn on Z for this pass
	V( pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE ) );

	// For the first pass we'll draw the screen to the full screen render target
	// and to update the velocity render target with the velocity of each pixel
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		// Set world drawing technique
		V( m_effect->SetTechnique( g_hTechWorldWithVelocity ) );

		// Get the projection & view matrix from the camera class
		memcpy(mProj.m, GetG().m_camera.GetProjMatrix()->m, sizeof(float)*16);
		memcpy(mView.m, GetG().m_camera.GetViewMatrix()->m, sizeof(float)*16);

		mViewProjection = mView * mProj;
		const int NUM_WALLS = 10;
		for( int iObject = 0; iObject < NUM_WALLS; iObject++ )
		{
			ArnMatrix mWorld, mWorldLast;
			LPDIRECT3DTEXTURE9 texture = 0;
			LPD3DXMESH		mesh = 0;

			mWorldViewProjection = mWorld * mViewProjection;
			mWorldViewProjectionLast = mWorldLast * g_mViewProjectionLast;

			// Tell the effect where the camera is now
			V( m_effect->SetMatrix( g_hWorldViewProjection, (const D3DXMATRIX*)mWorldViewProjection.m ) );
			V( m_effect->SetMatrix( g_hWorld, (const D3DXMATRIX*)mWorld.m ) );

			// Tell the effect where the camera was last frame
			V( m_effect->SetMatrix( g_hWorldViewProjectionLast, (const D3DXMATRIX*)mWorldViewProjectionLast.m ) );

			// Tell the effect the current mesh's texture
			V( m_effect->SetTexture( g_hMeshTexture, texture ) );

			V( m_effect->Begin( &cPasses, 0 ) );
			for( iPass = 0; iPass < cPasses; iPass++ )
			{
				// Set the render targets here.  If multiple render targets are
				// supported, render target 1 is set to be the velocity surface.
				// If multiple render targets are not supported, the velocity
				// surface will be rendered in the 2nd pass.
				for( int rt = 0; rt < g_nRtUsed; ++rt )
					V( pd3dDevice->SetRenderTarget( rt, g_pCurFrameRTSet->pRT[iPass][rt] ) );

				V( m_effect->BeginPass( iPass ) );
				V( mesh->DrawSubset( 0 ) );
				V( m_effect->EndPass() );
			}
			V( m_effect->End() );
		}


		V( pd3dDevice->EndScene() );
	}

	// Remember the current view projection matrix for next frame
	g_mViewProjectionLast = mViewProjection;

	// Now that we have the scene rendered into g_pFullScreenRenderTargetSurf
	// and the pixel velocity rendered into g_pCurFrameVelocitySurf 
	// we can do a final pass to render this into the backbuffer and use
	// a pixel shader to blur the pixels based on the last frame's & current frame's 
	// per pixel velocity to achieve a motion blur effect
	for( int rt = 0; rt < g_nRtUsed; ++rt )
		V( pd3dDevice->SetRenderTarget( rt, apOriginalRenderTarget[rt] ) );
	SAFE_RELEASE( apOriginalRenderTarget[0] );
	V( pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE ) );

	// Clear the render target
	V( pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L ) );

	// Above we rendered to a fullscreen render target texture, and now we 
	// render that texture using a quad to apply a pixel shader on every pixel of the scene.
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		V( m_effect->SetTechnique( g_hPostProcessMotionBlur ) );

		V( m_effect->Begin( &cPasses, 0 ) );
		for( iPass = 0; iPass < cPasses; iPass++ )
		{
			V( m_effect->BeginPass( iPass ) );
			V( pd3dDevice->SetFVF( SCREEN_VERTEX::FVF ) );
			V( pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_Vertex, sizeof( SCREEN_VERTEX ) ) );
			V( m_effect->EndPass() );
		}
		V( m_effect->End() );

		//RenderText();

		V( pd3dDevice->EndScene() );
	}
}

//////////////////////////////////////////////////////////////////////////()

PostSepiaShader::PostSepiaShader()
{

}

PostSepiaShader::~PostSepiaShader()
{

}

HRESULT PostSepiaShader::setFullscreenTexture( LPDIRECT3DTEXTURE9 tex )
{
	HRESULT hr = S_OK;
	V_RETURN( m_effect->SetTexture( "gSceneTexture", tex ) );
	return hr;
}

void PostSepiaShader::setDesaturation( float desat )
{
	m_effect->SetFloat( "gDesat", desat );
}

HRESULT CALLBACK PostSepiaShader::onCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	initEffect( pd3dDevice, L"Shaders/HLSL/post_sepia.fx" );
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////


HRESULT PostRadialBlurShader::setFullscreenTexture( LPDIRECT3DTEXTURE9 tex )
{
	HRESULT hr = S_OK;
	V_RETURN( m_effect->SetTexture( "gSceneTexture", tex ) );
	return hr;
}

void PostRadialBlurShader::setBlurWidth( float blurWidth )
{
	m_effect->SetFloat( "gBlurWidth", blurWidth );
}

HRESULT CALLBACK PostRadialBlurShader::onCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	initEffect( pd3dDevice, L"Shaders/HLSL/post_radialBlur.fx" );
	return S_OK;
}