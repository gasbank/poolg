#pragma once

class Unit
{
public:
	Unit();
	~Unit();

	HRESULT init(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXMESH mesh);
	const D3DXVECTOR3& getLowerLeft() const { return m_lowerLeft; }
	const D3DXVECTOR3& getUpperRight() const { return m_upperRight; }
	LPD3DXMESH getMesh() const { return m_d3dxMesh; }
	void release() { SAFE_RELEASE(m_d3dxMesh); }

	HRESULT draw();
	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void frameMove(float fElapsedTime);

	void setRotX(float rad) { m_vRot.x = rad; m_bLocalXformDirty = true; }
	void setRotY(float rad) { m_vRot.y = rad; m_bLocalXformDirty = true; }
	void setRotZ(float rad) { m_vRot.z = rad; m_bLocalXformDirty = true; }

	void setScaleX(float val) { m_vScale.x = val; m_bLocalXformDirty = true; }
	void setScaleY(float val) { m_vScale.y = val; m_bLocalXformDirty = true; }
	void setScaleZ(float val) { m_vScale.z = val; m_bLocalXformDirty = true; }

	void setPosX(float val) { m_vPos.x = val; m_bLocalXformDirty = true; }
	void setPosY(float val) { m_vPos.y = val; m_bLocalXformDirty = true; }
	void setPosZ(float val) { m_vPos.z = val; m_bLocalXformDirty = true; }

private:
	struct TeapotVertex
	{
		float x, y, z;
		float nx, ny, nz;
	};
	void updateLocalXform();
	LPD3DXMESH				m_d3dxMesh;
	D3DXVECTOR3				m_lowerLeft, m_upperRight; // bounding box params
	D3DXVECTOR3				m_vRot, m_vPos, m_vScale;
	bool					m_bLocalXformDirty;
	D3DXMATRIX				m_localXform;
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	D3DMATERIAL9			m_material;
	LPDIRECT3DTEXTURE9		m_d3dTex;
};
