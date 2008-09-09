#include "EpCommonLibraryPCH.h"
#include "EpReplicaManagerConnection.h"


RakNet::Replica2* EpReplicaManagerConnection::Construct( RakNet::BitStream *replicaData, SystemAddress sender, RakNet::SerializationType type, RakNet::ReplicaManager2 *replicaManager, RakNetTime timestamp, NetworkID networkId, bool networkIDCollision )
{

	return 0;
}

void EpReplicaManagerConnection::DeserializeDownloadComplete( RakNet::BitStream *objectData, SystemAddress sender, RakNet::ReplicaManager2 *replicaManager, RakNetTime timestamp, RakNet::SerializationType serializationType )
{

}