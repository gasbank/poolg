#pragma once

// Trick to exclude DirectX library files.
class EpUser;
struct IDirect3DDevice9;
struct _D3DSURFACE_DESC;
typedef struct _D3DSURFACE_DESC D3DSURFACE_DESC;
struct D3DXVECTOR3;

class CommonWorldInterface;
class World;

enum UnitType
{
	UT_UNITBASE,
	UT_UNIT,
	UT_CHARACTER,
	UT_HERO,
	UT_ENEMY,
	UT_ATTACKOBJECT,
	UT_INNERFIRE,
	UT_STRUCTREOBJECT,
	UT_SKILLOBJECT,
	UT_ITEM,
};


/**
	@brief 서버와 클라이언트측에서 공동으로 사용하는 렌더링 엔티티 정보를 저장하는 클래스

	이 클래스에는 어떠한 렌더링 개체에 대한 요소나 Direct3D 혹은 DirectX 요소가 없습니다.
	순전히 어떤 화면상에 보이는 물체의 3차원 좌표값이나 크기값, 혹은 회전값을 가지고 있습니다.
	이러한 정보는 서버와 클라이언트에게 모두 필요한 것이며, 동기화를 맞추기 위해 필수적인
	것이므로 기존에 이러한 일을 담당하던 Unit 클래스에서 옮겨온 것입니다.

	어떤 하나의 렌더링되는 개체가 가질 수 있는 렌더링 외의 모든 정보는 이 클래스에서
	담당한다고 생각하시면 됩니다.

	본 클래스는 Raknet::Replica2를 상속받았습니다. 이는 본 클래스가 RakNet의 ReplicaManager2 클래스에
	의해 관리되도록 하기 위함이고, 이를 통해 많은 멀티플레이어 관련 요소를 배제하고 구현할
	수 있습니다. Raknet::Replica2는 주기적으로 본 클래스의 동기화가 필요한 요소를 알아내어
	시리얼라이즈 시키고, 또한 다른 컴퓨터에서 변경된 객체 정보를 받아서 디시리얼라이즈 시켜
	로컬 오브젝트의 값을 업데이트하기도 합니다. 이러한 것은 해당 virtual 메소드를 참조하십시오.

	@warning
	서버측에는 DirectX 라이브러리에 종속적이지 않게 설계하는 것이 목적이고, 이 클래스는
	서버 프로젝트에도 정적으로 링크되는 라이브러리이므로 본 클래스에서 DirectX와 관련된
	요소를 사용하는 것은 불가합니다.
*/
class UnitBase : public RakNet::Replica2
{
public:
	friend class World;

	// Static Variables
	static DataStructures::List<UnitBase*>	units;
	static UnitBase*						myUnit;

	// Ctor and Dtor
											UnitBase( UnitType type );
	virtual									~UnitBase(void);

	// Implemented member of Replica2: This function encodes the identifier for this class, so the class factory can create it
	virtual bool							SerializeConstruction( RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext );
	// Implemented member of Replica2: Write the data members of this class. ReplicaManager2 works with pointers as well as any other kind of data
	virtual bool							Serialize( RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext );
	// Implemented member of Replica2: Read what I wrote in Serialize() immediately above
	virtual void							Deserialize( RakNet::BitStream *bitStream, RakNet::SerializationType serializationType, SystemAddress sender, RakNetTime timestamp );
	// Implemented member of Replica2: Should this object be visible to this connection? If not visible, Serialize() will not be sent to that system.
	virtual RakNet::BooleanQueryResult		QueryVisibility( RakNet::Connection_RM2 *connection);
	// Implemented member of Replica2: Called when our visibility changes. While not visible, we will not get updates from Serialize() for affected connection(s)
	virtual void							DeserializeVisibility(RakNet::BitStream *bitStream, RakNet::SerializationType serializationType, SystemAddress sender, RakNetTime timestamp);
	// Overriding so the client can delete this object. By default, only the server could
	virtual bool							QueryIsDestructionAuthority(void) const;
	// Overriding so the client can control if this object is visible or not (for cloaking). By default, only the server control this
	// Objects that are not visible are not serialized automatically with ReplicaManager2::AutoSerialize()
	virtual bool							QueryIsVisibilityAuthority(void) const;
	// Overriding so the client can send serialization changes for its own unit. By default, only the server can send serialization changes.
	virtual bool							QueryIsSerializationAuthority(void) const;

	virtual HRESULT							frameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime ) { return S_OK; }
	virtual LRESULT							handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) { return S_OK;}
	virtual bool							frameMove( double dTime, float fElapsedTime ) { return true; }
	virtual HRESULT							onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) { return S_OK; }
	virtual void							onLostDevice() {}
	virtual void							updateArnMesh() {}

	void									printDebugInfo() const;
	
	
	
	/*! @name RTTI
	현재 유닛의 타입을 나타내는 RTTI 정보를 가져옵니다. 이 정보는 컴파일러에서 제공하는
	RTTI 값이 아니며, 클래스 생성시에 설정하게 되는 enum 값입니다.
	*/
	//@{
	UnitType								getType() const			{ return m_type; }
	const char*								getTypeString() const;
	//@}

	
	/*! @name 삭제 예정 플래그

	삭제 예정 플래그가 설정된 유닛은 렌더링 대상에서 제외됩니다.

	초기 디자인의 경우 유닛이 전투에서 사망하거나 더이상 렌더링될 필요가 없을 때 유닛을 즉시 해제하는
	방식을 사용했습니다. 그러나 Trigger/Action 클래스가 만들어지고 유닛 관련한 Incident가 구성되면서
	문제가 발생했습니다. 예를 들어, 유닛이 사망했을 경우를 체크하는 Trigger의 경우 당연히 해당 유닛에 대한
	포인터를 가지고 있습니다. 만일 그 유닛이 전투에서 사망했을 경우 이러한 Trigger가 걸려있는 것을 모르고
	즉시 할당 해제를 하게 된다면 Trigger가 그 다음에 유닛의 사망 여부를 확인할때에 access violation 오류가
	날 것입니다. 이러한 연유로 유닛이 필요없어질지라도 일단 지우지 않고 삭제 예정 플래그를 설정하는
	방법을 사용하고 있습니다.

	그러나 현재로는 한번 생성된 유닛이 실제로 해제되는 경우는 없고 매번 삭제 예정 플래그를 설정하는 것으로
	끝나도록 되어 있으므로 차후에 문제가 될 것으로 예상됩니다.
	*/
	//@{
	void									setRemoveFlag( bool flag )					{ m_removeFlag = flag; }
	bool									getRemoveFlag() const						{ return m_removeFlag; }
	//@}


	/*! @name 타일 위치
	타일의 위치를 설정하거나 가져올 수 있습니다. 타일 좌표계는 2차원적이며 타일 크기를 벗어나서 위치할 수는
	없습니다. 타일 시스템은 타일 단위로 이동할 수 있게 하는 기능만 구현되어 있으므로 앞으로 많이 바뀔 수
	있는 부분입니다.
	이동을 자연스럽게 시키기 위해 '버퍼 타일 위치' 개념이 도입되어 있습니다. 만일 유닛을 특정 타일로 이동시키고
	싶다면 setTilePos() 보다는 getTileBufferPos()를 이용해 주십시오.
	*/
	//@{
	UINT									getTilePosX() const							{ return m_tilePos.x; }
	UINT									getTilePosY() const							{ return m_tilePos.y; }
	void									getTilePos( UINT& x, UINT& y ) const		{ x = m_tilePos.x; y = m_tilePos.y; }
	const Point2Uint&						getTilePos() const							{ return m_tilePos; }
	const Point2Uint&						getTileBufferPos() const					{ return m_tileBufferPos; }
	void									setTilePosRpc( int tileX, int tileY )		{ setTileBufferPos(tileX, tileY); }
	void									setTilePos( int tileX, int tileY );
	void									setTilePos( const Point2Uint& newPos );
	void									setTileBufferPos( int tileX, int tileY )	{ m_tileBufferPos.x = tileX; m_tileBufferPos.y = tileY; }
	//@}

	

	/*! @name 지역 변환 행렬의 업데이트 유무 설정
	지역 변환 행렬 dirty 값을 설정하거나 가져옵니다.

	본 클래스에는 위치(translation), 크기(scaling), 회전(rotation) 값을 저장하는 변수가 있습니다.
	그렇지만 렌더링 엔진에게 이러한 정보를 전달할 때는 하나의 4x4 행렬로 만들어야 합니다.
	이러한 작업을 위해서는 적지않은 계산과정을 거쳐야하는데 매 프레임마다 이 지역 변환 행렬을 업데이트하는 것은
	낭비일 수 있으므로 모든 위치, 크기, 회전의 셋터(setter) 메소드에서 dirty 값을 설정하도록 해 두었습니다.
	이 값을 참고로하여 프레임마다 지역 변환 행렬을 업데이트할지 하지 않을지를 결정하게 됩니다.
	대개의 경우 이는 잘 작동하지만 반드시 셋터(setter)를 통해 위치, 크기, 회전 값이 바뀐다는 보장이
	없습니다. 그래서 어떤 경우에는 dirty 값을 강제로 설정해야하는 경우도 있습니다.
	*/
	//@{
	void									setLocalXformDirty( bool b = true )		{ m_bLocalXformDirty = b; }
	bool									isLocalXformDirty() const				{ return m_bLocalXformDirty; }
	void									updateLocalXform(); ///< 만일 dirty가 설정된 상태라면 지역 변환 행렬을 업데이트합니다.
	const Mat4Float&						getLocalXformRaw() const				{ return m_localXform; }
	void									setLocalXformRaw( void* ptr ) 			{ memcpy( &m_localXform.m[0][0], ptr, sizeof(float)*16 ); }

	//@}

	/*! @name 회전 정보
	X, Y, Z 축으로 객체를 몇 도나 회전시킬 것인지 설정하거나 가져옵니다.
	각도값의 단위는 radian입니다.
	*/
	//@{
	float									getRotX() const			{ return m_vRot.x; }
	float									getRotY() const			{ return m_vRot.y; }
	float									getRotZ() const			{ return m_vRot.z; }
	void									setRotX( float rad )	{ m_vRot.x = rad; m_bLocalXformDirty = true; }
	void									setRotY( float rad )	{ m_vRot.y = rad; m_bLocalXformDirty = true; }
	void									setRotZ( float rad )	{ m_vRot.z = rad; m_bLocalXformDirty = true; }
	//@}

	/*! @name 크기(스케일) 정보
	X, Y, Z 축으로 크기를 얼마나 변형시킬 것인지 설정하거나 가져옵니다.
	*/
	//@{
	float									getScaleX() const				{ return m_vScale.x; }
	float									getScaleY() const				{ return m_vScale.y; }
	float									getScaleZ() const				{ return m_vScale.z; }
	void									setUniformScale( float val )	{ m_vScale.x = m_vScale.y = m_vScale.z = val; m_bLocalXformDirty = true; }
	void									setScaleX( float val )			{ m_vScale.x = val; m_bLocalXformDirty = true; }
	void									setScaleY( float val )			{ m_vScale.y = val; m_bLocalXformDirty = true; }
	void									setScaleZ( float val )			{ m_vScale.z = val; m_bLocalXformDirty = true; }
	//@}
	

	/*! @name 위치 정보
	3차원 좌표계를 이용한 공간 위치 정보를 설정하거나 가져옵니다.
	*/
	//@{
	const Point3Float&						getPosRaw() const					{ return m_vPos; }
	const char*								getPositionAsString() const;
	void									setPos( float x, float y, float z )	{ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z; m_bLocalXformDirty = true; }
	void									setPosX( float val )				{ m_vPos.x = val; m_bLocalXformDirty = true; }
	void									setPosY( float val )				{ m_vPos.y = val; m_bLocalXformDirty = true; }
	void									setPosZ( float val )				{ m_vPos.z = val; m_bLocalXformDirty = true; }
	//@}


	/*! @name RakNet Replica 이름
	RakNet에 의해 관리되므로 디버그를 위해 이름을 설정할 수 있도록 하였습니다.
	게임 플레이에 영향을 주지는 않습니다.
	*/
	//@{
	const RakNet::RakString&				getRepName() const					{ return m_repName; }
	void									setRepName( const char* repName )	{ m_repName = repName; }
	//@}

	/*! @name RakNet 동기화 여부
	본 객체가 다른 컴퓨터에서 업데이트가 되지 않도록 설정할 수 있는 메소드입니다.
	만일 m_bHidden 값이 true라면 이 객체는 객체가 생성된 곳에서만 업데이트되고, 다른 클라이언트에서는
	업데이트되지 않습니다. 만일 시야를 매우 벗어난 유닛의 경우에 이 값을 true로 설정하여
	멀티플레이에 의한 오버헤드를 최소화시키도록 하십시오.
	*/
	//@{
	bool									isHidden() const			{ return m_bHidden; }
	void									setHidden( bool hidden )	{ m_bHidden = hidden; }
	//@}

	/*! @name 소유자
	본 객체의 소유자가 누구인지 설정하거나 가져올 수 있습니다.
	*/
	//@{
	void									setOwner( EpUser* owner )	{ m_owner = owner; }
	EpUser*									getOwner() const			{ return m_owner; }
	//@}

	CommonWorldInterface*					getAttachedWorld() const	{ return m_attachedWorld; }

private:

	void									setAttachedWorld( CommonWorldInterface* val )	{ m_attachedWorld = val; }

	UnitType								m_type;
	bool									m_removeFlag;

	Point2Uint								m_tilePos;
	Point2Uint								m_tileBufferPos;

	Point3Float								m_vRot;
	Point3Float								m_vPos;
	Point3Float								m_vScale;

	bool									m_bLocalXformDirty;
	Mat4Float								m_localXform;

	EpUser*									m_owner; // The user that owns this unit

	bool									m_bHidden;
	RakNet::RakString						m_repName;

	CommonWorldInterface*					m_attachedWorld;
	
};
