#pragma once


// One instance of Connection_RM2 is implicitly created per connection that uses ReplicaManager2. The most important function to implement is Construct() as this creates your game objects.
// It is designed this way so you can override per-connection behavior in your own game classes
class EpReplicaManagerConnection : public RakNet::Connection_RM2
{
public:
	// Callback used to create objects
	// See Connection_RM2::Construct in ReplicaManager2.h for a full explanation of each parameter
	RakNet::Replica2* Construct( RakNet::BitStream *replicaData, SystemAddress sender, RakNet::SerializationType type, RakNet::ReplicaManager2 *replicaManager, RakNetTime timestamp, NetworkID networkId, bool networkIDCollision );

	// Callback when we finish downloading all objects from a new connection
	// See Connection_RM2::DeserializeDownloadComplete in ReplicaManager2.h for a full explanation of each parameter
	virtual void DeserializeDownloadComplete( RakNet::BitStream *objectData, SystemAddress sender, RakNet::ReplicaManager2 *replicaManager, RakNetTime timestamp, RakNet::SerializationType serializationType );

};


// This is a required class factory, that creates and destroys instances of ReplicaManager2DemoConnection
class EpReplicaManagerConnectionFactory : public RakNet::Connection_RM2Factory
{
	virtual RakNet::Connection_RM2* AllocConnection( void ) const { return new EpReplicaManagerConnection; }
	virtual void DeallocConnection( RakNet::Connection_RM2* s ) const { delete s; }
};
