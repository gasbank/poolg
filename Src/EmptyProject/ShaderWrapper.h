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
	virtual HRESULT CALLBACK		onResetDevice();

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
	virtual HRESULT CALLBACK		onResetDevice();

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

class AlphaShader : public Shader
{
public:
	AlphaShader() {}
	~AlphaShader() {}

	virtual HRESULT					initShader( LPDIRECT3DDEVICE9 pd3dDevice, const WCHAR* shaderFileName );
	virtual HRESULT CALLBACK		onResetDevice();

};