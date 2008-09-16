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



/**
@brief UnitBase의 정보를 기반으로 실제로 화면에 3D 모델을 그려주는 일을 담당하는 클래스

이 클래스는 '렌더링 되는 개체 하나'를 나타냅니다.

멤버 변수로 LPD3DXMESH와 ArnMesh* 타입 변수를 가지고 있습니다. 그렇지만 반드시
둘 다 널이거나 둘 중 하나가 널이어야 합니다.

3D 모델을 렌더링하되 렌더링할때 적용하는 변환 매트릭스는 이 클래스의 상위 클래스인
UnitBase가 담당합니다.
*/
class Unit : public UnitBase
{
public:
	// Ctor and Dtor and create
	static Unit*					createUnit( LPD3DXMESH mesh, int tileX = 0, int tileY = 0, float posZ = 0 );
									/* Unit class instantiation is prohibited */
	virtual							~Unit();

	// Virtual Methods
	virtual HRESULT					frameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime );
	virtual LRESULT					handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool					frameMove( double dTime, float fElapsedTime );
	virtual HRESULT					onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	virtual void					onLostDevice();
	virtual const D3DXVECTOR3&		getPos() const { return *(D3DXVECTOR3*)&getPosRaw(); }
	virtual void					updateArnMesh();
	
	
	void							clearKey(); ///< 키보드 버퍼 값을 지웁니다.
	void							setHeadDir( UnitInput ui );
	const D3DXMATRIX&				getLocalXform() const					{ return *(D3DXMATRIX*)&getLocalXformRaw(); }
	World*							getCurWorld() const						{ return GetWorldManager().getCurWorld(); }
	
	void							setColor( int r, int g, int b );
	bool							isForcedMove() const					{ return m_bForcedMove; }
	void							setViewAt( const D3DXVECTOR3* at );
	void							forcedMoveTest();


	/** 유닛을 특정 방향으로 한 타일 이동시킴 */
	void							setForcedMove( UnitInput dir );

	/** @name 이동 가능성
	이 유닛이 이동 가능한지에 대해 설정하거나 가져올 수 있습니다.
	*/
	//@{
	bool							getMovable() const						{ return m_bMovable; }
	void							setMovable( bool val )					{ m_bMovable = val; }
	//@}

	/*! @name ArnMesh 노드 설정
	이 유닛을 렌더링할 때 사용할 ArnMesh에 대한 이름을 설정합니다.
	유닛이 생성될 당시에는 ArnMesh가 생성되어있지 않으므로 먼저 이름만 저장하고 있다가
	ArnMesh가 생성되면 그 때 지연 바인딩을 하게 됩니다.
	*/
	//@{
	void							setArnMeshName( const char* arnMeshName )	{ m_arnMeshName = arnMeshName; }
	const std::string&				getArnMeshName() const						{ return m_arnMeshName; }
	ArnMesh*						getArnMesh() const							{ return m_arnMesh; }
	//@}

	/*! @name LPD3DXMESH 설정
	이 유닛을 렌더링할 때 사용할 LPD3DXMESH를 설정하거나 가져올 수 있습니다.
	*/
	//@{
	void							setMesh( LPD3DXMESH d3dxMesh );
	LPD3DXMESH						getMesh() const { return m_d3dxMesh; }
	//@}

	/*! @name 가시성 및 유닛 이름
	현재 이 유닛 자체의 렌더링 여부, 혹은 유닛의 이름 렌더링 여부를 설정합니다.
	유닛 이름을 설정할 수도 있습니다. 이 이름은 화면상으로 보여지는 이름이기도 하고,
	디버그용으로도 사용할 수 있습니다.
	*/
	//@{
	void							setVisible (bool choice);
	void							setNameVisible( bool b ) { m_bNameVisible = b; }
	void							setName( std::string str ) { m_name = str; }
	//@}

	/*! @name SoulAnimation 기능 관련
	전투 중 유닛이 사망했을 경우 유닛 렌더링 모델과 동일한 모델을 위에 유령처럼 렌더링하는 기능과
	관련된 메소드입니다. SoulAnimation이 끝났다고 해서 Unit이 소멸되거나 하지는 않습니다.
	*/
	//@{
	void							startSoulAnimation( float duration, float height );		///< SoulAnimation을 시작합니다.
	bool							getSoulAnimation() { return m_bSoulAnimation; }			///< 현재 SoulAnimation 중인지를 알아봅니다.
	//@}

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
