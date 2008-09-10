#include "EpCommonLibraryPCH.h"
#include "UnitBase.h"
#include "EpUser.h"
#include "CommonWorldInterface.h"


bool QueryIsNetworkServer();
NetworkIDManager& GetNetworkIdManager();

DataStructures::List<UnitBase*> UnitBase::soldiers;
UnitBase *UnitBase::mySoldier;


UnitBase::UnitBase( UnitType type )
{
	m_type						= type;
	m_removeFlag				= false;
	
	m_owner						= 0;
	m_repName					= "DefaultName";
	m_bHidden					= false;

	m_tilePos					= Point2Uint::ZERO;
	m_tileBufferPos				= m_tilePos;

	m_vRot						= Point3Float::ZERO;
	m_vPos						= Point3Float::ZERO;
	m_vScale					= Point3Float::ONE;
	m_bLocalXformDirty			= true;

	m_attachedWorld				= 0;

	// Set matrix as identity;
	Mat4Float::MakeIdentity( &m_localXform );

	soldiers.Insert( this );
}

UnitBase::~UnitBase(void)
{
	if ( m_attachedWorld && getType() == UT_UNITBASE )
		m_attachedWorld->detachUnit( this );

	soldiers.RemoveAtIndex( soldiers.GetIndexOf( this ) );
	if ( UnitBase::mySoldier == this )
	{
		printf( "My soldier deleted.\n" );
		UnitBase::mySoldier = 0;
	}
	else
		printf( "Soldier named %s deleted.\n", getRepName().C_String() );

	if ( m_owner )
		m_owner->soldier = 0;
}

bool UnitBase::SerializeConstruction( RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext )
{
	RakNet::StringTable::Instance()->EncodeString( "UnitBase", 128, bitStream );
	return true;
}

bool UnitBase::Serialize( RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext )
{
	// Both the client and the server write the name.
	// The client writes it to send it to the server
	// The server writes the name when sending to systems other than the one that owns this soldier.
	if ( QueryIsNetworkServer() == false || serializationContext->recipientAddress != m_owner->systemAddress )
	{
		StringCompressor::Instance()->EncodeString( m_repName, m_repName.GetLength() + 1, bitStream );
		bitStream->Write( m_vPos.x );
		bitStream->Write( m_vPos.y );
		bitStream->Write( m_vPos.z );
		bitStream->Write( m_vRot.x );
		bitStream->Write( m_vRot.y );
		bitStream->Write( m_vRot.z );
	}

	// Only the owner client needs to write the status of cloak, since this variable is only used by the server
	if ( QueryIsNetworkServer() == false && UnitBase::mySoldier == this )
		bitStream->Write( isHidden() );

	// Only the server writes the owner
	if ( QueryIsNetworkServer() )
		bitStream->Write( m_owner->GetNetworkID() );

	return true;
}

void UnitBase::Deserialize( RakNet::BitStream *bitStream, RakNet::SerializationType serializationType, SystemAddress sender, RakNetTime timestamp )
{
	NetworkID ownerNetworkId;

	// Similar to Serialize(), only read the name if the server, or if we are not the owner
	if ( QueryIsNetworkServer() || UnitBase::mySoldier != this )
	{
		StringCompressor::Instance()->DecodeString( &m_repName, 128, bitStream );
		bitStream->Read( m_vPos.x );
		bitStream->Read( m_vPos.y );
		bitStream->Read( m_vPos.z );
		bitStream->Read( m_vRot.x );
		bitStream->Read( m_vRot.y );
		bitStream->Read( m_vRot.z );
	}

	// Only the server reads isCloaked
	if ( QueryIsNetworkServer() )
	{
		bool isHidden;
		bitStream->Read( isHidden );
		setHidden( isHidden );
	}

	// Only clients read the owner
	if ( QueryIsNetworkServer() == false )
	{
		bitStream->Read( ownerNetworkId );
		m_owner= (EpUser*) GetNetworkIdManager().GET_OBJECT_FROM_ID( ownerNetworkId );
		m_owner->soldier = this;
	}

	printf("Soldier %s updated.\n", getRepName().C_String() );

	setLocalXformDirty( true );
	updateLocalXform();
}

// Implemented member of Replica2: Should this object be visible to this connection? If not visible, Serialize() will not be sent to that system.
RakNet::BooleanQueryResult UnitBase::QueryVisibility( RakNet::Connection_RM2 *connection )
{
	if ( QueryIsNetworkServer() )
		return m_bHidden == false ? RakNet::BQR_YES : RakNet::BQR_NO;
	else
		return RakNet::BQR_ALWAYS; // You never cloak your own updates to the server
}
// Implemented member of Replica2: Called when our visibility changes. While not visible, we will not get updates from Serialize() for affected connection(s)
void UnitBase::DeserializeVisibility( RakNet::BitStream *bitStream, RakNet::SerializationType serializationType, SystemAddress sender, RakNetTime timestamp )
{
	if ( m_owner )
	{
		if ( RakNet::SerializationContext::IsVisible( serializationType ) )
			printf( "Soldier named %s owned by user at address %s has become visible.\n", getRepName().C_String(), m_owner->systemAddress.ToString() );
		else
			printf( "Soldier named %s owned by user at address %s no longer visible.\n", getRepName().C_String(), m_owner->systemAddress.ToString() );
	}
	else
	{
		// Visibility message comes in before deserialize, and it is deserialize that contains the owner.
		// However, construction comes before visibility.
		// We could have thus just written the owner in Soldier::SerializeConstruction() and read and assigned it in ReplicaManager2DemoConnection::Construct()
		if ( RakNet::SerializationContext::IsVisible( serializationType ) )
			printf( "Soldier named %s has become visible.\n", getRepName().C_String() );
		else
			printf( "Soldier named %s is no longer visible.\n", getRepName().C_String() );
	}
}

bool UnitBase::QueryIsDestructionAuthority(void) const
{
	// Since we allow the controlling client to locally destroy the soldier, and transmit this across the network, the client must be the destruction authority
	// Were this function not overridden, if the client deleted the soldier it would only be deleted locally.
	// If the server were the only system deleting this object, it would not be necessary to override this function
	return QueryIsNetworkServer() || UnitBase::mySoldier == this;
}

bool UnitBase::QueryIsVisibilityAuthority(void) const
{
	// Same as QueryIsDestructionAuthority, but for QueryVisibility.
	return QueryIsNetworkServer() || UnitBase::mySoldier==this;
}

bool UnitBase::QueryIsSerializationAuthority(void) const
{
	// Same as QueryIsDestructionAuthority, but for Serialize and AddAutoSerializeTimer.
	return QueryIsNetworkServer() || UnitBase::mySoldier==this;
}


const char* UnitBase::getPositionAsString() const
{
	// TODO not thread safe?
	static char ret[30];
	sprintf_s( ret, 30, "%.2f, %.2f, %.2f", m_vPos.x, m_vPos.y, m_vPos.z );
	return ret;
}


const char* UnitBase::getTypeString() const
{
#define CASE_ENUM_TO_STRING(x) case x: return #x
	switch ( getType() )
	{
		CASE_ENUM_TO_STRING( UT_UNITBASE );
		CASE_ENUM_TO_STRING( UT_UNIT );
		CASE_ENUM_TO_STRING( UT_CHARACTER );
		CASE_ENUM_TO_STRING( UT_HERO );
		CASE_ENUM_TO_STRING( UT_ENEMY );
		CASE_ENUM_TO_STRING( UT_STRUCTREOBJECT );
		CASE_ENUM_TO_STRING( UT_ATTACKOBJECT );
		CASE_ENUM_TO_STRING( UT_INNERFIRE );
	default:
		_ASSERTE( !"Type string is not found" );
	}
#undef CASE_ENUM_TO_STRING
	return 0;
}

void UnitBase::setTilePos( int tileX, int tileY )
{
	m_tilePos.x = tileX;
	m_tilePos.y = tileY;

	setPos( (float)(tileX - (s_xSize / 2)) * s_tileSize,
		(float)(tileY  - (s_ySize / 2)) * s_tileSize,
		0 );

	//printf( "%s: setTilePos called (%d, %d)\n", typeid(this).name(), tileX, tileY );
}

void UnitBase::setTilePos( const Point2Uint& newPos )
{
	setTilePos( newPos.x, newPos.y );
}

void UnitBase::printDebugInfo() const
{
	printf( "Unit: Type - %s ", getTypeString() );
	printf( "(%.2f, %.2f, %.2f)\n", getPosRaw().x, getPosRaw().y, getPosRaw().z );
}


void UnitBase::updateLocalXform()
{
	if ( isLocalXformDirty() )
	{
		/*
		D3DXMATRIX mRotX, mRotY, mRotZ, mScale, mTrans, mWorld;
		D3DXMatrixRotationX( &mRotX, getRotX() );
		D3DXMatrixRotationY( &mRotY, getRotY() );
		D3DXMatrixRotationZ( &mRotZ, getRotZ() );
		D3DXMatrixScaling( &mScale, getScaleX(), getScaleY(), getScaleZ() );
		D3DXMatrixTranslation( &mTrans, getPos().x, getPos().y, getPos().z );

		D3DXMATRIX localXform = mRotX * mRotY * mRotZ * mScale * mTrans;
		setLocalXformRaw( &localXform );
		*/


		// TODO Rotation and Scale transformation was omitted!
		Mat4Float mRotZ;
		Mat4Float::MakeRotationZ( &mRotZ, getRotZ() );
		Mat4Float::MakeTranslation( &m_localXform, getPosRaw().x, getPosRaw().y, getPosRaw().z );
		Mat4Float::MakeMultiplication( &m_localXform, &mRotZ, &m_localXform );
		setLocalXformDirty( false );
	}
}
