#include "EpCommonLibraryPCH.h"
#include "EpReplicaManagerConnection.h"
#include "EpUser.h"
#include "UnitBase.h"

class WorldManager;

bool QueryIsNetworkServer();
void AddToCurrentWorld( UnitBase* unitBase );

RakNet::Replica2* EpReplicaManagerConnection::Construct( RakNet::BitStream *replicaData, SystemAddress sender, RakNet::SerializationType type, RakNet::ReplicaManager2 *replicaManager, RakNetTime timestamp, NetworkID networkId, bool networkIDCollision )
{
	char objectName[128];
	// In our two Serialize() functions above, we used StringTable to encode the name of the class. Here we read it out, and create the desired object.
	RakNet::StringTable::Instance()->DecodeString( objectName, 128, replicaData );
	if ( networkIDCollision )
		return 0;
	if ( strcmp( objectName, "EpUser" ) == 0 )
		return new EpUser;
	if ( strcmp( objectName, "UnitBase" ) == 0 ) {
		UnitBase* newUnit = new UnitBase(UT_UNITBASE);
		if ( QueryIsNetworkServer() )
		{
			// If the server, then we can assign the owner by the address of the guy sending to us
			// For the client, we read this in UnitBase::Deserialize
			
			newUnit->setOwner( EpUser::GetUserByAddress( sender ) );
			newUnit->getOwner()->setUnit(newUnit);
		}
		else
		{
			AddToCurrentWorld( newUnit );
		}
		return newUnit;
	};
	return 0;
}

void EpReplicaManagerConnection::DeserializeDownloadComplete( RakNet::BitStream *objectData, SystemAddress sender, RakNet::ReplicaManager2 *replicaManager, RakNetTime timestamp, RakNet::SerializationType serializationType )
{
	// Since the system is inherently peer to peer, download notifications go both ways, not just server to client.
	// Just ignore download notifications from clients in a client/server app
	if ( QueryIsNetworkServer() == false )
		printf("Download complete\n");
}