#include "EpCommonLibraryPCH.h"
#include "UnitBase.h"
#include "EpUser.h"

bool QueryIsNetworkServer();
NetworkIDManager& GetNetworkIdManager();

DataStructures::List<UnitBase*> UnitBase::soldiers;
UnitBase *UnitBase::mySoldier;


UnitBase::UnitBase( UnitType type )
{
	m_type = type;
	m_removeFlag				= false;

	soldiers.Insert( this );
	m_owner = 0;
	m_repName = "DefaultName";
	m_bHidden = false;

	m_syncX = 1.0f;
	m_syncY = 2.0f;
	m_syncZ = 3.0f;

	m_tilePos					= Point2Uint::ZERO;
	m_tileBufferPos				= m_tilePos;

	m_vRot						= Point3Float::ZERO;
	m_vPos						= Point3Float::ZERO;
	m_vScale					= Point3Float::ONE;
	m_bLocalXformDirty			= true;

	// Set matrix as identity;
	m_localXform.m[0][0] = m_localXform.m[1][1] = m_localXform.m[2][2] = m_localXform.m[3][3] = 1.0f;

}

UnitBase::~UnitBase(void)
{
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
		StringCompressor::Instance()->EncodeString( m_repName, m_repName.GetLength(), bitStream );
		bitStream->Write( m_syncX );
		bitStream->Write( m_syncY );
		bitStream->Write( m_syncZ );
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
		bitStream->Read( m_syncX );
		bitStream->Read( m_syncY );
		bitStream->Read( m_syncZ );
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

	printf("Soldier %s updated.\n", getRepName() );
}

const char* UnitBase::getPositionAsString() const
{
	// TODO not thread safe?
	static char ret[30];
	sprintf_s( ret, 30, "%.2f, %.2f, %.2f", m_syncX, m_syncY, m_syncZ );
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
