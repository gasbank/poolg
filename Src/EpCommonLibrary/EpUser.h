#pragma once

class UnitBase;

// User represents a human behind a computer. One instance is created on the server per new connection.
class EpUser : public RakNet::Replica2
{
public:
	// Holds a pointer to my soldier.
	UnitBase *soldier;
	// All users that have been created
	static DataStructures::List<EpUser*> users;
	// Does this user represent me? Used on the client only.
	static EpUser *myUser;
	// System address of this user
	SystemAddress systemAddress;

	// User starts out with no soldier. Also, track this pointer.
	EpUser();
	// Remove myself from the static users list
	virtual ~EpUser();

	// Helper function to free memory when someone disconnections.
	static void DeleteUserByAddress(SystemAddress systemAddress);

	static void DeleteAllUsers();

	// Helper function to lookup the user in users by systemAddress
	static EpUser* GetUserByAddress(SystemAddress systemAddress);

	// Each user can have a soldier. Create my soldier if I don't have one already
	UnitBase* CreateSoldier(void);
	// Return my soldier
	UnitBase* GetMySoldier(void) const;

	// Implemented member of Replica2: This function encodes the identifier for this class, so the class factory can create it
	virtual bool SerializeConstruction(RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext);
	// Implemented member of Replica2: Write the data members of this class. ReplicaManager2 works with pointers as well as any other kind of data
	virtual bool Serialize(RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext);
	// Implemented member of Replica2: Read what I wrote in Serialize() immediately above
	virtual void Deserialize(RakNet::BitStream *bitStream, RakNet::SerializationType serializationType, SystemAddress sender, RakNetTime timestamp);
};
