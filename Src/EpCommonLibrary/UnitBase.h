#pragma once

// Trick to exclude DirectX library files.
class EpUser;
struct IDirect3DDevice9;
struct _D3DSURFACE_DESC;
typedef struct _D3DSURFACE_DESC D3DSURFACE_DESC;
struct D3DXVECTOR3;

class CommonWorldInterface;
class World;

enum UnitType { UT_UNITBASE, UT_UNIT, UT_CHARACTER, UT_HERO, UT_ENEMY, UT_ATTACKOBJECT, UT_INNERFIRE, UT_STRUCTREOBJECT, UT_SKILLOBJECT };

class UnitBase : public RakNet::Replica2
{
public:
	friend class World;

	// Static Variables
	static DataStructures::List<UnitBase*>	soldiers;
	static UnitBase*						mySoldier;

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
	// Overriding so the client can send serialization changes for its own soldier. By default, only the server can send serialization changes.
	virtual bool							QueryIsSerializationAuthority(void) const;

	virtual HRESULT							frameRender( IDirect3DDevice9* pd3dDevice, double dTime, float fElapsedTime ) { return S_OK; }
	virtual LRESULT							handleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) { return S_OK;}
	virtual bool							frameMove( double dTime, float fElapsedTime ) { return true; }
	virtual HRESULT							onResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) { return S_OK; }
	virtual void							onLostDevice() {}
	virtual void							updateArnMesh() {}


	void									printDebugInfo() const;
	void									updateLocalXform();

	const Point3Float&						getPosRaw() const { return m_vPos; }
	const Mat4Float&						getLocalXformRaw() const				{ return m_localXform; }
	void									setLocalXformRaw( void* ptr ) 			{ memcpy( &m_localXform.m[0][0], ptr, sizeof(float)*16 ); }
	UnitType								getType() const							{ return m_type; }
	const char*								getTypeString() const;
	void									setRemoveFlag( bool flag )				{ m_removeFlag = flag; }
	bool									getRemoveFlag() const					{ return m_removeFlag; }

	void									setTilePosRpc( int tileX, int tileY ) { setTileBufferPos(tileX, tileY); }
	void									setTilePos( int tileX, int tileY );
	void									setTilePos( const Point2Uint& newPos );
	void									setTileBufferPos( int tileX, int tileY ) { m_tileBufferPos.x = tileX; m_tileBufferPos.y = tileY; }

	UINT									getTilePosX() const						{ return m_tilePos.x; }
	UINT									getTilePosY() const						{ return m_tilePos.y; }
	void									getTilePos( UINT& x, UINT& y ) const	{ x = m_tilePos.x; y = m_tilePos.y; }
	const Point2Uint&						getTilePos() const						{ return m_tilePos; }
	const Point2Uint&						getTileBufferPos() const				{ return m_tileBufferPos; }

	void									setLocalXformDirty( bool b = true )		{ m_bLocalXformDirty = b; }
	bool									isLocalXformDirty() const				{ return m_bLocalXformDirty; }

	void									setRotX( float rad ) { m_vRot.x = rad; m_bLocalXformDirty = true; }
	void									setRotY( float rad ) { m_vRot.y = rad; m_bLocalXformDirty = true; }
	void									setRotZ( float rad ) { m_vRot.z = rad; m_bLocalXformDirty = true; }

	float									getRotX() const { return m_vRot.x; }
	float									getRotY() const { return m_vRot.y; }
	float									getRotZ() const { return m_vRot.z; }

	void									setUniformScale( float val ) { m_vScale.x = m_vScale.y = m_vScale.z = val; m_bLocalXformDirty = true; }
	void									setScaleX( float val )		{ m_vScale.x = val; m_bLocalXformDirty = true; }
	void									setScaleY( float val )		{ m_vScale.y = val; m_bLocalXformDirty = true; }
	void									setScaleZ( float val )		{ m_vScale.z = val; m_bLocalXformDirty = true; }

	float									getScaleX() const		{ return m_vScale.x; }
	float									getScaleY() const		{ return m_vScale.y; }
	float									getScaleZ() const		{ return m_vScale.z; }

	void									setPos( float x, float y, float z )	{ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z; m_bLocalXformDirty = true; }
	//void									setPos( const D3DXVECTOR3& val )	{ m_vPos = val; m_bLocalXformDirty = true; }
	void									setPosX( float val )				{ m_vPos.x = val; m_bLocalXformDirty = true; }
	void									setPosY( float val )				{ m_vPos.y = val; m_bLocalXformDirty = true; }
	void									setPosZ( float val )				{ m_vPos.z = val; m_bLocalXformDirty = true; }

	const RakNet::RakString&				getRepName() const { return m_repName; }
	void									setRepName( const char* repName ) { m_repName = repName; }
	bool									isHidden() const { return m_bHidden; }
	void									setHidden( bool hidden ) { m_bHidden = hidden; }

	void									setOwner( EpUser* owner ) { m_owner = owner; }
	EpUser*									getOwner() const { return m_owner; }

	const char*								getPositionAsString() const;

	CommonWorldInterface*					getAttachedWorld() const { return m_attachedWorld; }
private:
	
	void									setAttachedWorld( CommonWorldInterface* val ) { m_attachedWorld = val; }

	UnitType								m_type;
	bool									m_removeFlag;

	Point2Uint								m_tilePos;
	Point2Uint								m_tileBufferPos;

	Point3Float								m_vRot;
	Point3Float								m_vPos;
	Point3Float								m_vScale;

	bool									m_bLocalXformDirty;
	Mat4Float								m_localXform;

	EpUser*									m_owner; // The user that owns this soldier

	bool									m_bHidden;
	RakNet::RakString						m_repName;

	CommonWorldInterface*					m_attachedWorld;
	
};
