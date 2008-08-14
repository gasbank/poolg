#include "Picture.h"

class InnerFire
{
public:
	InnerFire();
	void init (const TCHAR* imgFileName, LPDIRECT3DDEVICE9 d3dDev, float radius, float height, int angleNumber);
	void release ();
	HRESULT draw (bool textured = true);
	void frameMove (float fElapsedTime);
	void setPos(float x, float y, float z) { m_vPos.x = x; m_vPos.y = y; m_vPos.z = z;}


protected:
	D3DXMATRIX m_localXform;
	LPDIRECT3DDEVICE9 m_d3dDev;
	LPD3DXMESH m_d3dxMesh;
	LPDIRECT3DTEXTURE9 m_d3dTex;
	D3DXVECTOR3 m_vVelocity;
	D3DXVECTOR3 m_vPos;
	
	D3DXMATRIX m_mWorld;
	float m_radius, m_height;
	int m_angleNumber;

	// for RHW vertex
	LPDIRECT3DVERTEXBUFFER9 m_d3dVB;

	float m_texWidth, m_texHeight;

	bool m_bInit;
};

