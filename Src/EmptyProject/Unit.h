#pragma once

#include "TileManager.h"

class Trigger;

enum UnitInput
{
	UNIT_MOVE_UP	= 0,
	UNIT_MOVE_DOWN	= 1,
	UNIT_MOVE_LEFT	= 2,
	UNIT_MOVE_RIGHT = 3,
	UNIT_MAX_KEYS,
	UNIT_UNKNOWN = 0xFF
};

static const POINT g_moveAmount[4] = {
	{  0,  1 },
	{  0, -1 },
	{ -1,  0 },
	{  1,  0 }
};

class WorldState;

class Unit
{
public:
	static Unit* createUnit( LPD3DXMESH mesh, int tileX = 0, int tileY = 0, float posZ = 0, bool bCtrl = false );
	virtual ~Unit();

	HRESULT init(LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXMESH mesh);
	const D3DXVECTOR3& getLowerLeft() const { return m_lowerLeft; }
	const D3DXVECTOR3& getUpperRight() const { return m_upperRight; }
	LPD3DXMESH getMesh() const { return m_d3dxMesh; }
	void release() { SAFE_RELEASE(m_d3dxMesh); }
	bool getControllable() const { return m_bControllable; }
	void clearKey();

	virtual HRESULT frameRender();
	virtual LRESULT handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool frameMove(float fElapsedTime);

	void setRotX(float rad) { m_vRot.x = rad; m_bLocalXformDirty = true; }
	void setRotY(float rad) { m_vRot.y = rad; m_bLocalXformDirty = true; }
	void setRotZ(float rad) { m_vRot.z = rad; m_bLocalXformDirty = true; }
	void setControllable(bool bCtrl) { m_bControllable = bCtrl; }

	void setUniformScale(float val) { m_vScale.x = m_vScale.y = m_vScale.z = val; m_bLocalXformDirty = true; }
	void setScaleX(float val) { m_vScale.x = val; m_bLocalXformDirty = true; }
	void setScaleY(float val) { m_vScale.y = val; m_bLocalXformDirty = true; }
	void setScaleZ(float val) { m_vScale.z = val; m_bLocalXformDirty = true; }

	void setPos(const D3DXVECTOR3& val) { m_vPos = val; m_bLocalXformDirty = true; }
	void setPosX(float val) { m_vPos.x = val; m_bLocalXformDirty = true; }
	void setPosY(float val) { m_vPos.y = val; m_bLocalXformDirty = true; }
	void setPosZ(float val) { m_vPos.z = val; m_bLocalXformDirty = true; }
	const D3DXVECTOR3& getPos() const { return m_vPos; }

	bool isControllable() const { return m_bControllable; }

	void setHeadDir( UnitInput );


	void setTilePos(int tileX, int tileY);
	void setTilePos( const Point2Uint& newPos );
	void setTileBufferPos(int tileX, int tileY) { m_tileBufferPos.x = tileX; m_tileBufferPos.y = tileY; }
	
	UINT getTilePosX() const { return m_tilePos.x; }
	UINT getTilePosY() const { return m_tilePos.y; }
	void getTilePos( UINT& x, UINT& y ) const { x = m_tilePos.x; y = m_tilePos.y; }
	
	const Point2Uint& getTilePos() const { return m_tilePos; }
	const Point2Uint& getTileBufferPos() const { return m_tileBufferPos; }

protected:
	WorldState* getWorldState() const;

	virtual UnitInput mapKey( UINT nKey );

//[재우]상속받은 객체가 접근하기 위해 protected로.
//private:
	Unit();

	bool IsKeyDown( BYTE key ) const { return( (key & KEY_IS_DOWN_MASK) == KEY_IS_DOWN_MASK ); }
	bool WasKeyDown( BYTE key ) const { return( (key & KEY_WAS_DOWN_MASK) == KEY_WAS_DOWN_MASK ); }

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
	BYTE					m_aKeys[UNIT_MAX_KEYS];
	UINT					m_cKeysDown;            // Number of camera keys that are down.
	D3DXVECTOR3				m_vKeyboardDirection;
	D3DXVECTOR3				m_vVelocity;
	bool					m_bControllable;
	
	
private:
	Point2Uint				m_tilePos;
	Point2Uint				m_tileBufferPos;
	
};

SCRIPT_FACTORY( Unit )
