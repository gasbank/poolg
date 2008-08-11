#include "EmptyProjectPCH.h"
#include "ShaderWrapper.h"

Shader::Shader(void)
{
	m_dev					= 0;
	m_pVertexShader			= 0;
	m_pConstantTable		= 0;
	m_pVertexDeclaration	= 0;
	m_effect				= 0;
}

Shader::~Shader(void)
{
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

	V_RETURN( D3DXCompileShaderFromFile( m_shaderFileName, NULL, NULL, functionName, "vs_2_0", dwShaderFlags, &pCode, NULL, &m_pConstantTable ) );

	// Create the vertex shader
	hr = m_dev->CreateVertexShader( ( DWORD* )pCode->GetBufferPointer(), &m_pVertexShader );
	pCode->Release();

	if( FAILED( hr ) )
		return DXTRACE_ERR( TEXT( "CreateVertexShader" ), hr );

	return S_OK;
}

void Shader::update( float fTime, float fElapsedTime )
{
	// Draw picture map with shader settings
	D3DXMATRIXA16 mWorldViewProj;
	D3DXMatrixIdentity( &mWorldViewProj );
	m_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mWorldViewProj", &mWorldViewProj );

	// Set up the vertex shader constants
	D3DXMATRIXA16 mViewProj; //= *camera.GetViewMatrix() * *camera.GetProjMatrix();
	D3DXMatrixIdentity( &mViewProj );
	m_pConstantTable->SetMatrix( DXUTGetD3D9Device(), "mViewProj", &mViewProj );
	m_pConstantTable->SetFloat( DXUTGetD3D9Device(), "fTime", ( float )fTime );

}

void Shader::release()
{
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pConstantTable );
	SAFE_RELEASE( m_pVertexDeclaration );
	SAFE_RELEASE( m_effect );
}

HRESULT Shader::initEffect( LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* shaderFileName, DWORD dwShaderFlags )
{
	HRESULT hr = S_OK;
	m_dev = pd3dDevice;

	// If this fails, there should be debug output as to 
	// they the .fx file failed to compile
	V_RETURN( D3DXCreateEffectFromFile( m_dev, shaderFileName, NULL, NULL, dwShaderFlags, NULL, &m_effect, NULL ) );
	return hr;
}

HRESULT Shader::onResetDevice()
{
	HRESULT hr = S_OK;
	if( m_effect )
		V_RETURN( m_effect->OnResetDevice() );

	return hr;
}


//////////////////////////////////////////////////////////////////////////


BombShader::BombShader()
{

}
BombShader::~BombShader()
{
	SAFE_RELEASE( m_fireTexture );
}


HRESULT CALLBACK BombShader::onResetDevice()
{
	HRESULT hr = S_OK;

	/*
	D3DXCOLOR colorWhite( 1.0f, 1.0f, 1.0f, 1.0f );
	D3DXCOLOR colorBlack( 0.0f, 0.0f, 0.0f, 1.0f );
	D3DXCOLOR colorAmbient( 0.35f, 0.35f, 0.35f, 0 );
	D3DXCOLOR colorSpecular( 0.5f, 0.5f, 0.5f, 1.0f );
	LPDIRECT3DTEXTURE9 texture = 0;
	V_RETURN( m_effect->SetVector( "MaterialAmbientColor", ( D3DXVECTOR4* )&colorAmbient ) );
	V_RETURN( m_effect->SetVector( "MaterialDiffuseColor", ( D3DXVECTOR4* )&colorWhite ) );
	V_RETURN( m_effect->SetTexture( "RenderTargetTexture", texture ) );

	V_RETURN( m_effect->SetFloat( "VelocityCapSq", 1.0f ) );
	V_RETURN( m_effect->SetFloat( "ConvertToNonHomogeneous", 0.1f ) );
	*/

	V_RETURN( D3DXCreateTextureFromFile( m_dev, L"Shaders/Textures/FireGrade.bmp", &m_fireTexture ) );
	V_RETURN( m_effect->SetTexture( "gGradeTex", m_fireTexture) );
	return hr;
}

HRESULT BombShader::setWorldViewProj( double fTime, float fElapsedTime, const D3DXMATRIX* mWorld, const D3DXMATRIX* mView, const D3DXMATRIX* mProj )
{
	HRESULT hr = S_OK;

	D3DXMATRIX wvp = *mWorld * *mView * *mProj;
	V_RETURN( m_effect->SetMatrix( "gWvpXf", &wvp) );
	V_RETURN( m_effect->SetFloat( "gTimer", (float)fTime) );
	

	return hr;
}

