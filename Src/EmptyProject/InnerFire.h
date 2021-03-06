#pragma once
#include "Picture.h"
#include "Unit.h"

class InnerFire : public Unit
{
public:
	InnerFire();
	~InnerFire();

	void init (const TCHAR* imgFileName, float radius, float height, int angleNumber);
	void release ();
	HRESULT draw (bool textured = true);
	virtual bool frameMove ( double dTime, float fElapsedTime);
	virtual HRESULT onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext );
	virtual void onLostDevice();
protected:
	//ArnMatrix m_localXform;
	//LPDIRECT3DDEVICE9 m_d3dDev;
	//LPD3DXMESH m_d3dxMesh;
	//LPDIRECT3DTEXTURE9 m_d3dTex;
	//ArnVec3 m_vVelocity;
	//ArnVec3 m_vPos;
	
	ArnMatrix m_mWorld;
	float m_radius, m_height;
	int m_angleNumber;
	float m_angle;
	float m_angleVelocity;
	float m_angleAccelRate;

	// for RHW vertex
	//LPDIRECT3DVERTEXBUFFER9 m_d3dVB;

	float m_texWidth, m_texHeight;

	bool m_bInit;

private:
	LPDIRECT3DTEXTURE9 m_d3dTex;
	
	std::tstring m_imgFileName;
};


