#pragma once

#include "UnitBase.h"
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

/** It is responsible for a basic rendered entity.
* Can have either a pointer of LPD3DXMESH or ArnMesh(but not both).
* And it has local transformation matrix which contains position, scaling factor, orientation.
* Also, there are 2D tile position and movement methods.
*/
class Unit : public UnitBase
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
	virtual const D3DXVECTOR3&		getPos() const { return *(D3DXVECTOR3*)&getPosRaw(); }

	virtual void					updateArnMesh();

	HRESULT							init();
	
	/** Clear the keyboard buffer on this unit.
	*/
	void							clearKey();

	/**
	@name Getter/Setter Methods
	Here are the list of public getter/setter methods.
	*/
	//@{

	void							setHeadDir( UnitInput ui );

	
	const D3DXMATRIX&				getLocalXform() const					{ return *(D3DXMATRIX*)&getLocalXformRaw(); }

	

	bool							getMovable() const						{ return m_bMovable; }
	void							setMovable( bool val )					{ m_bMovable = val; }

	World*							getCurWorld() const						{ return GetWorldManager().getCurWorld(); }
	void							setForcedMove( int i );

	void							setArnMeshName( const char* arnMeshName )	{ m_arnMeshName = arnMeshName; }
	const std::string&				getArnMeshName() const						{ return m_arnMeshName; }
	ArnMesh*						getArnMesh() const							{ return m_arnMesh; }

	void							setMesh( LPD3DXMESH d3dxMesh );
	LPD3DXMESH						getMesh() const { return m_d3dxMesh; }

	void							setColor( int r, int g, int b );
	
	bool							isForcedMove() const					{ return m_bForcedMove; }
	
	void							setViewAt( const D3DXVECTOR3* at );

	void							setVisible (bool choice);
	
	bool							getSoulAnimation() { return m_bSoulAnimation; }
	void							setNameVisible( bool b ) { m_bNameVisible = b; }
	void							setName( std::string str ) { m_name = str; }
	//@}

	static Unit*					createUnit( LPD3DXMESH mesh, int tileX = 0, int tileY = 0, float posZ = 0 );
	
	void							forcedMoveTest();
	
	
	void							startSoulAnimation( float duration, float height );
protected:
									Unit( UnitType type );
	virtual UnitInput				mapKey( UINT nKey ) const;

	
	HRESULT							rayTesting( UnitInput ui );

	

	BYTE							m_aKeys[UNIT_MAX_KEYS];		///< Keyboard input buffer
	UINT							m_cKeysDown;				///< Number of keys down
	D3DXVECTOR3						m_vKeyboardDirection;
	D3DXVECTOR3						m_vVelocity;
	
private:
	void							drawSoul( IDirect3DDevice9* pd3dDevice );
	void							updateSoulAnimation( float fElapsedTime );
	void							drawName( IDirect3DDevice9* pd3dDevice );
	void							updateLocalXform();
	
	D3DMATERIAL9					m_material;
	
	
	bool							m_bForcedMove;


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
