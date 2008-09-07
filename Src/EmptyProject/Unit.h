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

enum UnitType { UT_UNIT, UT_CHARACTER, UT_HERO, UT_ENEMY, UT_ATTACKOBJECT, UT_INNERFIRE, UT_STRUCTREOBJECT, UT_SKILLOBJECT };

class Unit
{
public:
	// Ctor and Dtor and create
									/* Unit class instantiation is prohibited */
	virtual							~Unit();

	// Virtual Methods
	virtual HRESULT					frameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime );
	virtual LRESULT					handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool					frameMove( double dTime, float fElapsedTime );
	virtual HRESULT					onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
													void* pUserContext );
	virtual void					onLostDevice();
	virtual const D3DXVECTOR3&		getPos() const { return m_vPos; }


	HRESULT							init();
	
	void							release();
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
	const char*						getTypeString() const;
	World*							getCurWorld() const						{ return GetWorldManager().getCurWorld(); }
	void							setForcedMove( int i );
	void							forcedMoveTest();
	
	void							setArnMeshName( const char* arnMeshName )	{ m_arnMeshName = arnMeshName; }
	const std::string&				getArnMeshName() const						{ return m_arnMeshName; }
	ArnMesh*						getArnMesh() const							{ return m_arnMesh; }

	void							setMesh( LPD3DXMESH d3dxMesh );
	LPD3DXMESH						getMesh() const { return m_d3dxMesh; }

	void							setColor( int r, int g, int b );
	
	bool							isForcedMove() const					{ return m_bForcedMove; }
	void							startSoulAnimation( float duration, float height );
	void							setViewAt( const D3DXVECTOR3* at );

	void							setVisible (bool choice);
	static Unit*					createUnit( LPD3DXMESH mesh, int tileX = 0, int tileY = 0, float posZ = 0 );


	bool							getSoulAnimation() { return m_bSoulAnimation; }
	void							setNameVisible( bool b ) { m_bNameVisible = b; }
	void							setName( std::string str ) { m_name = str; }

	void							printDebugInfo() const;
	void							updateArnMesh();

protected:
									Unit( UnitType type );
	virtual UnitInput				mapKey( UINT nKey ) const;

	void							setLocalXformDirty()					{ m_bLocalXformDirty = true; }
	HRESULT							rayTesting( UnitInput ui );

	

	BYTE							m_aKeys[UNIT_MAX_KEYS];
	UINT							m_cKeysDown;
	D3DXVECTOR3						m_vKeyboardDirection;
	D3DXVECTOR3						m_vVelocity;
	
private:
	void							drawSoul( IDirect3DDevice9* pd3dDevice );
	void							updateSoulAnimation( float fElapsedTime );
	void							drawName( IDirect3DDevice9* pd3dDevice );
	void							updateLocalXform();
	
	D3DXVECTOR3						m_vRot;
	D3DXVECTOR3						m_vPos;
	D3DXVECTOR3						m_vScale;

	bool							m_bLocalXformDirty;
	D3DXMATRIX						m_localXform;
	D3DMATERIAL9					m_material;
	
	bool							m_removeFlag;
	bool							m_bForcedMove;

	Point2Uint						m_tilePos;
	Point2Uint						m_tileBufferPos;

	UnitType						m_type;

	// Pointer to mesh object. If ArnMesh is set for this Unit instance,
	// you should set m_d3dxMesh to NULL. If ArnMesh is not set, you should
	// assign non-NULL value to m_d3dxMesh. In short, you must set only one of those pointer.
	ArnMesh*						m_arnMesh;
	LPD3DXMESH						m_d3dxMesh;

	bool							m_bMovable;

	// For soul animation
	float							m_fSoulAnimationTimer;
	float							m_fSoulAnimationDuration;
	float							m_fSoulAnimationHeight;
	D3DXMATRIX						m_localXformSoul;
	D3DMATERIAL9					m_materialSoul;
	bool							m_bSoulAnimation;
	D3DXMATRIX						m_prevLocalXform;
	D3DMATERIAL9					m_prevMaterial;

	// For name drawing
	std::string						m_name;
	bool							m_bNameVisible;

	std::string						m_arnMeshName;
};

SCRIPT_FACTORY( Unit )
