#pragma once

#if DEBUG
static const DWORD				gs_shaderFlags = D3DXSHADER_DEBUG | D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#else
static const DWORD				gs_shaderFlags = 0;
#endif

class Shader
{
public:
	Shader(void);
	virtual ~Shader(void);

	virtual HRESULT					initEffect( LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* shaderFileName, DWORD dwShaderFlags = gs_shaderFlags );
	virtual HRESULT					initShader( LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* shaderFileName );
	HRESULT							compileShader( const char* functionName, const char* profile, DWORD dwShaderFlags = gs_shaderFlags );
	LPDIRECT3DVERTEXDECLARATION9	getVertexDeclaration() const { return m_pVertexDeclaration; }
	LPD3DXCONSTANTTABLE				getConstantTable() const { return m_pConstantTable; }
	LPDIRECT3DVERTEXSHADER9			getVertexShader() const { return m_pVertexShader; }
	virtual void					release();
	virtual void					update( float fTime, float fElapsedTime );
	virtual HRESULT CALLBACK		onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );

	D3DVERTEXELEMENT9* getDecl() const { return m_decl; }

protected:
	LPD3DXEFFECT					m_effect;
	LPDIRECT3DDEVICE9				m_dev;
	LPDIRECT3DVERTEXSHADER9         m_pVertexShader;
	
	LPD3DXCONSTANTTABLE             m_pConstantTable;
	
	LPDIRECT3DVERTEXDECLARATION9    m_pVertexDeclaration;
	D3DVERTEXELEMENT9*				m_decl;
	
	WCHAR m_shaderFileName[512];
	
};

//////////////////////////////////////////////////////////////////////////

class BombShader : public Shader
{
public:
	BombShader();
	~BombShader();
	virtual HRESULT CALLBACK		onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );

	void							initMainTechnique() { m_hDefaultTech = m_effect->GetTechniqueByName( "Main" ); }
	HRESULT							setMainTechnique() { return m_effect->SetTechnique( m_hDefaultTech ); }
	HRESULT							begin( UINT* passes, DWORD flags ) { return m_effect->Begin( passes, flags ); }
	HRESULT							beginPass( UINT pass ) { return m_effect->BeginPass( pass ); }
	HRESULT							endPass() { return m_effect->EndPass(); }
	HRESULT							end() { return m_effect->End(); }
	void							onLostDevice() { m_effect->OnLostDevice(); }
	HRESULT							setWorldViewProj( double fTime, float fElapsedTime, const D3DXMATRIX* mWorld, const D3DXMATRIX* mView, const D3DXMATRIX* mProj );
private:
	D3DXHANDLE						m_hDefaultTech;
	LPDIRECT3DTEXTURE9				m_fireTexture;
};

//////////////////////////////////////////////////////////////////////////

class PostSepiaShader : public Shader
{
public:
	PostSepiaShader();
	~PostSepiaShader();
	virtual HRESULT CALLBACK		onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );

	void							initMainTechnique();
	HRESULT							setMainTechnique() { return m_effect->SetTechnique( m_hDefaultTech ); }
	HRESULT							begin( UINT* passes, DWORD flags ) { return m_effect->Begin( passes, flags ); }
	HRESULT							beginPass( UINT pass ) { return m_effect->BeginPass( pass ); }
	HRESULT							endPass() { return m_effect->EndPass(); }
	HRESULT							end() { return m_effect->End(); }
	void							onLostDevice() { m_effect->OnLostDevice(); }
	HRESULT							setWorldViewProj( double fTime, float fElapsedTime, const D3DXMATRIX* mWorld, const D3DXMATRIX* mView, const D3DXMATRIX* mProj );
	HRESULT							setFullscreenTexture( LPDIRECT3DTEXTURE9 tex );
	void							setDesaturation( float desat );
private:
	D3DXHANDLE						m_hDefaultTech;
	D3DXHANDLE						m_hDesaturation;
	LPDIRECT3DTEXTURE9				m_tex;
};
//////////////////////////////////////////////////////////////////////////

class AlphaShader : public Shader
{
public:
	AlphaShader() {}
	~AlphaShader() {}

	virtual HRESULT					initShader( LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* shaderFileName );
	virtual HRESULT CALLBACK		onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );

};


//////////////////////////////////////////////////////////////////////////

struct SCREEN_VERTEX
{
	D3DXVECTOR4 pos;
	DWORD clr;
	D3DXVECTOR2 tex1;

	static const DWORD FVF;
};

class MotionBlurShader : public Shader
{
public:
	MotionBlurShader() {}
	~MotionBlurShader() {}

	virtual HRESULT					initShader( LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* shaderFileName );
	virtual HRESULT CALLBACK		onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );

	void							onCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	void							onLostDevice();
	void							onDestroyDevice();
	void							onFrameMove( double dTime, float fElapsedTime );
	void							onFrameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime );
private:
	
	LPD3DXSPRITE					m_textSprite;
	
	LPDIRECT3DTEXTURE9          g_pFullScreenRenderTarget;
	LPDIRECT3DSURFACE9          g_pFullScreenRenderTargetSurf;

	LPDIRECT3DTEXTURE9          g_pPixelVelocityTexture1;
	LPDIRECT3DSURFACE9          g_pPixelVelocitySurf1;
	LPDIRECT3DTEXTURE9          g_pPixelVelocityTexture2;
	LPDIRECT3DSURFACE9          g_pPixelVelocitySurf2;

	LPDIRECT3DTEXTURE9          g_pLastFrameVelocityTexture;
	LPDIRECT3DSURFACE9          g_pLastFrameVelocitySurf;
	LPDIRECT3DTEXTURE9          g_pCurFrameVelocityTexture;
	LPDIRECT3DSURFACE9          g_pCurFrameVelocitySurf;

	D3DXHANDLE                  g_hWorld;
	D3DXHANDLE                  g_hWorldLast;
	D3DXHANDLE                  g_hMeshTexture;
	D3DXHANDLE                  g_hWorldViewProjection;
	D3DXHANDLE                  g_hWorldViewProjectionLast;
	D3DXHANDLE                  g_hCurFrameVelocityTexture;
	D3DXHANDLE                  g_hLastFrameVelocityTexture;
	D3DXHANDLE                  g_hTechWorldWithVelocity;
	D3DXHANDLE                  g_hPostProcessMotionBlur;

	int                         g_nPasses;          // Number of passes required to render
	int                         g_nRtUsed;          // Number of render targets used by each pass
	D3DFORMAT                   g_VelocityTexFormat;    // Texture format for velocity textures

	struct CRenderTargetSet
	{
		IDirect3DSurface9* pRT[2][2];  // Two passes, two RTs
	};

	CRenderTargetSet g_aRTSet[2];            // Two sets of render targets
	CRenderTargetSet*           g_pCurFrameRTSet;      // Render target set for current frame
	CRenderTargetSet*           g_pLastFrameRTSet;     // Render target set for last frame
	SCREEN_VERTEX g_Vertex[4];

	float                       g_fPixelBlurConst;
	float                       g_fObjectSpeed;
	float                       g_fCameraSpeed;
	
	DWORD                       g_dwBackgroundColor;

	D3DXMATRIX                  g_mViewProjectionLast;

};