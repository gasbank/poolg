#pragma once
#include "WorldManager.h"
#include "TileManager.h"

class DynamicMotion;
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

class World;
class ArnMesh;

enum UnitType { UT_UNIT, UT_CHARACTER, UT_HERO, UT_ENEMY, UT_ATTACKOBJECT, UT_INNERFIRE, UT_STRUCTREOBJECT };

class Unit
{
public:
	// Ctor and Dtor and create
									/* Unit class instantiation is prohibited */
	virtual							~Unit();

	// Virtual Methods
	virtual HRESULT					frameRender();
	virtual LRESULT					handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool					frameMove( float fElapsedTime );
	virtual const D3DXVECTOR3&		getPos() const			{ return m_vPos; }


	HRESULT							init( LPDIRECT3DDEVICE9 pd3dDevice, LPD3DXMESH mesh );
	const D3DXVECTOR3&				getLowerLeft() const	{ return m_lowerLeft; }
	const D3DXVECTOR3&				getUpperRight() const	{ return m_upperRight; }
	LPD3DXMESH						getMesh() const			{ return m_d3dxMesh; }
	void							release()				{ SAFE_RELEASE(m_d3dxMesh); }
	void							clearKey();

	void							setRotX( float rad ) { m_vRot.x = rad; m_bLocalXformDirty = true; }
	void							setRotY( float rad ) { m_vRot.y = rad; m_bLocalXformDirty = true; }
	void							setRotZ( float rad ) { m_vRot.z = rad; m_bLocalXformDirty = true; }

	void							setUniformScale( float val ) { m_vScale.x = m_vScale.y = m_vScale.z = val; m_bLocalXformDirty = true; }
	void							setScaleX( float val )		{ m_vScale.x = val; m_bLocalXformDirty = true; }
	void							setScaleY( float val )		{ m_vScale.y = val; m_bLocalXformDirty = true; }
	void							setScaleZ( float val )		{ m_vScale.z = val; m_bLocalXformDirty = true; }

	void							setPos( float x, float y, float z )	{ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z; m_bLocalXformDirty = true; }
	void							setPos( const D3DXVECTOR3& val )	{ m_vPos = val; m_bLocalXformDirty = true; }
	void							setPosX( float val )				{ m_vPos.x = val; m_bLocalXformDirty = true; }
	void							setPosY( float val )				{ m_vPos.y = val; m_bLocalXformDirty = true; }
	void							setPosZ( float val )				{ m_vPos.z = val; m_bLocalXformDirty = true; }
	

	void							setHeadDir( UnitInput ui );

	void							setTilePos( int tileX, int tileY );
	void							setTilePos( const Point2Uint& newPos );
	void							setTileBufferPos( int tileX, int tileY ) { m_tileBufferPos.x = tileX; m_tileBufferPos.y = tileY; }
	
	UINT							getTilePosX() const						{ return m_tilePos.x; }
	UINT							getTilePosY() const						{ return m_tilePos.y; }
	void							getTilePos( UINT& x, UINT& y ) const	{ x = m_tilePos.x; y = m_tilePos.y; }
	
	const D3DXMATRIX&				getLocalXform() const					{ return m_localXform; }

	const Point2Uint&				getTilePos() const						{ return m_tilePos; }
	const Point2Uint&				getTileBufferPos() const				{ return m_tileBufferPos; }
	
	void							setRemoveFlag( bool flag )				{ m_removeFlag = flag; }
	bool							getRemoveFlag() const					{ return m_removeFlag; }

	bool							getMovable() const						{ return m_bMovable; }
	void							setMovable( bool val )					{ m_bMovable = val; }

	UnitType						getType() const							{ return m_type; }
	World*							getCurWorld() const						{ return GetWorldManager().getCurWorld(); }
	void							setDynamicMotion( DynamicMotion* dm )	{ m_dm = dm; }
	void							setForcedMove( int i );
	void							forcedMoveTest();
	void							setArnMesh( ArnMesh* arnMesh )			{ assert( arnMesh ); m_arnMesh = arnMesh; }
	
	void							setColor( int r, int g, int b );
	
	bool							isForcedMove() const					{ return m_bForcedMove; }
	void							startSoulAnimation( float duration, float height );
	void							setViewAt( const D3DXVECTOR3* at );

	bool getSoulAnimation() { return m_bSoulAnimation; }

protected:
									Unit( UnitType type );
	virtual UnitInput				mapKey( UINT nKey ) const;

	void							setLocalXformDirty()					{ m_bLocalXformDirty = true; }
	HRESULT							rayTesting( UnitInput ui );

	BYTE							m_aKeys[UNIT_MAX_KEYS];
	UINT							m_cKeysDown;            // Number of camera keys that are down.
	D3DXVECTOR3						m_vKeyboardDirection;
	D3DXVECTOR3						m_vVelocity;
	LPD3DXMESH						m_d3dxMesh;
	LPDIRECT3DTEXTURE9				m_d3dTex;

	

private:
	static Unit*					createUnit( LPD3DXMESH mesh, int tileX = 0, int tileY = 0, float posZ = 0 );
	World*							getWorldState() const;
	void							drawSoul();
	void							updateSoulAnimation( float fElapsedTime );

	struct TeapotVertex
	{
		float x, y, z;
		float nx, ny, nz;
	};
	void							updateLocalXform();
	
	D3DXVECTOR3						m_lowerLeft, m_upperRight; // bounding box params
	D3DXVECTOR3						m_vRot, m_vPos, m_vScale;
	bool							m_bLocalXformDirty;
	D3DXMATRIX						m_localXform;
	LPDIRECT3DDEVICE9				m_pd3dDevice;
	D3DMATERIAL9					m_material;
	
	bool							m_removeFlag;
	bool							m_bForcedMove;
	
	DynamicMotion*					m_dm;
	
	

	Point2Uint						m_tilePos;
	Point2Uint						m_tileBufferPos;

	UnitType						m_type;

	ArnMesh*						m_arnMesh;
	bool							m_bMovable;

	float							m_fSoulAnimationTimer;
	float							m_fSoulAnimationDuration;
	float							m_fSoulAnimationHeight;
	D3DXMATRIX						m_localXformSoul;
	D3DMATERIAL9					m_materialSoul;
	bool							m_bSoulAnimation;
	D3DXMATRIX						m_prevLocalXform;
	D3DMATERIAL9					m_prevMaterial;
};

SCRIPT_FACTORY( Unit )